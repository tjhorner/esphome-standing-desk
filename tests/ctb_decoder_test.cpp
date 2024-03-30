#include <cassert>
#include "../components/standing_desk_height/ctb_decoder.h"

using esphome::standing_desk_height::CTBDecoder;

bool _put_packets(CTBDecoder &decoder, uint8_t *packets, size_t size) {
  bool last_return = false;
  for (size_t i = 0; i < size; i++) {
    last_return = decoder.put(packets[i]);
  }
  return last_return;
}

bool _floats_equal(float a, float b)
{
    return fabs(a - b) < std::numeric_limits<float>::epsilon();
}

void decode_well_formed_height_packet_with_decimal() {
  CTBDecoder decoder;

  uint8_t packets[] = {0xaa,0xff,0x06,0x13,0x04,0x3c,0xdb};
  bool should_decode = _put_packets(decoder, packets, sizeof(packets));
  assert(should_decode == true);

  float decoded_height = decoder.decode();
  assert(_floats_equal(decoded_height, 49.6));
}

void decode_well_formed_height_packet_without_decimal() {
  CTBDecoder decoder;

  uint8_t packets[] = {0xaa,0xff,0x06,0x02,0x01,0xe2,0xd6};
  bool should_decode = _put_packets(decoder, packets, sizeof(packets));
  assert(should_decode == true);

  float decoded_height = decoder.decode();
  assert(_floats_equal(decoded_height, 126.0));
}

void ignore_unrelated_packets() {
  CTBDecoder decoder;

  uint8_t packets[] = {0x00,0x00,0x06,0x02,0x01,0xe2,0xd6};
  bool should_decode = _put_packets(decoder, packets, sizeof(packets));

  assert(should_decode == false);
}

void handle_valid_packet_after_ignored_packet() {
  CTBDecoder decoder;

  uint8_t packets[] = {0x00,0x00,0x06,0x02,0x01,0xe2,0xd6};
  _put_packets(decoder, packets, sizeof(packets));

  uint8_t packets2[] = {0xaa,0xff,0x06,0x02,0x01,0xe2,0xd6};
  bool should_decode = _put_packets(decoder, packets2, sizeof(packets2));

  assert(should_decode == true);
}

void ignore_bad_crc() {
  CTBDecoder decoder;

  uint8_t packets[] = {0xaa,0xff,0x06,0x02,0x01,0x0F,0x0F};
  bool should_decode = _put_packets(decoder, packets, sizeof(packets));

  assert(should_decode == false);
}

int main() {
  decode_well_formed_height_packet_with_decimal();
  decode_well_formed_height_packet_without_decimal();
  ignore_unrelated_packets();
  handle_valid_packet_after_ignored_packet();
  ignore_bad_crc();
  return 0;
}