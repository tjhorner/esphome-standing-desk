#pragma once

#include <cstdint>
#include "esphome/core/log.h"

namespace esphome {
namespace standing_desk_height {

enum DecoderVariant : uint8_t {
  // Keep this first
  DECODER_VARIANT_UNKNOWN,

  DECODER_VARIANT_JARVIS,
  DECODER_VARIANT_UPLIFT,
  DECODER_VARIANT_OMNIDESK,
  DECODER_VARIANT_ALZA,

  // Keep this last
  DECODER_VARIANT_COUNT
};

const LogString *decoder_variant_to_string(DecoderVariant variant);

}
}