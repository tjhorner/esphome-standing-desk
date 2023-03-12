#include "standing_desk_height.h"

namespace esphome {
namespace standing_desk_height {

static const char *const TAG = "standing_desk_height";

void StandingDeskHeightSensor::set_decoder_variant(DecoderVariant decoder_variant) {
  if (this->decoder != nullptr) {
    delete this->decoder;
    this->decoder = nullptr;
  }

  this->decoder_variant = decoder_variant;
  switch (decoder_variant) {
    case DECODER_VARIANT_JARVIS:
      this->decoder = new JarvisDecoder();
      break;
    case DECODER_VARIANT_UPLIFT:
      this->decoder = new UpliftDecoder();
      break;
    case DECODER_VARIANT_OMNIDESK:
      this->decoder = new OmnideskDecoder();
      break;
    default:
      ESP_LOGE(TAG, "Unknown decoder variant %d", (uint8_t) decoder_variant);
      this->decoder = nullptr;
      return;
  }
}

void StandingDeskHeightSensor::start_decoder_detection() {
  ESP_LOGI(TAG, "Starting decoder detection");

  this->decoder_variant = DECODER_VARIANT_UNKNOWN;
  this->try_next_decoder();
}

void StandingDeskHeightSensor::try_next_decoder() {
  if (this->decoder_variant == DECODER_VARIANT_COUNT - 1) {
    ESP_LOGW(TAG, "No valid decoder found. Please make sure your desk is reporting the height and you can see it on the keypad");

    delete this->decoder;
    this->decoder = nullptr;

    this->decoder_variant = DECODER_VARIANT_UNKNOWN;
    this->is_detecting = false;
    return;
  }

  this->set_decoder_variant((DecoderVariant) (this->decoder_variant + 1));

  const LogString *variant_s = decoder_variant_to_string(this->decoder_variant);
  ESP_LOGD(TAG, "Attempting next decoder variant: %s", LOG_STR_ARG(variant_s));

  this->last_read = -1;
  this->started_detecting_at = millis();
  this->is_detecting = true;
}

void StandingDeskHeightSensor::setup() {
  this->decoder_pref_ = global_preferences->make_preference<DecoderVariant>(this->get_object_id_hash());

  if (this->decoder_variant == DECODER_VARIANT_UNKNOWN) {
    ESP_LOGD(TAG, "Loading decoder variant from flash");

    DecoderVariant saved_variant;
    if (!this->decoder_pref_.load(&saved_variant)) {
      ESP_LOGD(TAG, "Could not load decoder variant from flash; starting detection process");
      this->start_decoder_detection();
    } else {
      const LogString *variant_s = decoder_variant_to_string(saved_variant);
      ESP_LOGD(TAG, "Decoder variant from restore: %s", LOG_STR_ARG(saved_variant));
      this->set_decoder_variant(saved_variant);
    }
  } else {
    ESP_LOGD(TAG, "Decoder variant was not loaded from flash; using hardcoded variant");
  }
}

void StandingDeskHeightSensor::loop() {
  while (this->available() > 0)
  {
    uint8_t byte;
    this->read_byte(&byte);

    ESP_LOGVV(TAG, "Reading byte: %d", byte);

    if (this->decoder != nullptr && this->decoder->put(byte)) {
      float height = this->decoder->decode();
      ESP_LOGVV(TAG, "Got desk height: %f", height);
      this->last_read = height;
    }
  }

  if (this->is_detecting) {
    if (this->last_read != -1) {
      this->is_detecting = false;

      const LogString *variant_s = decoder_variant_to_string(this->decoder_variant);
      ESP_LOGI(TAG, "Decoder detection complete. Correct decoder variant: %s", LOG_STR_ARG(variant_s));
      ESP_LOGI(TAG, "This decoder variant will be saved to flash and restored on boot.");
      ESP_LOGI(TAG, "If you want to make this change permanent, add the following to this sensor's configuration:");
      ESP_LOGI(TAG, "  variant: %s", LOG_STR_ARG(variant_s));

      this->decoder_pref_.save(&this->decoder_variant);
    } else if (millis() - this->started_detecting_at > 1000) {
      const LogString *variant_s = decoder_variant_to_string(this->decoder_variant);
      ESP_LOGD(TAG, "Decoder %s does not appear to work; trying next decoder", LOG_STR_ARG(variant_s));
      this->try_next_decoder();
    }
  }
}

void StandingDeskHeightSensor::update() {
  if (this->last_read > 0 && this->last_read != this->last_published) {
    publish_state(this->last_read);
    this->last_published = this->last_read;
  }
}

float StandingDeskHeightSensor::get_last_read() {
  return this->last_read;
}

}
}
