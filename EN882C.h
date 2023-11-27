
#define DATA_SIZE 6  // Size of the data packet
int chk;
int add_nibbles(uint8_t const message[], unsigned num_bytes) {
  int result = 0;
  for (unsigned i = 0; i < num_bytes; ++i) {
    result += (message[i] >> 4) + (message[i] & 0x0f);
  }
  return result;
}
uint8_t* assemble_bits(int device_id, int channel, int battery_low, float temperature_C, int humidity) {
  static uint8_t bitstream[DATA_SIZE];
  int temperature_raw = (int)(temperature_C * 10.0);

  // Assemble the bits according to the decoding logic
  bitstream[0] = device_id;
  bitstream[1] = ((channel - 1) << 4) | (temperature_raw >> 8) & 0xFF;
  bitstream[2] = temperature_raw & 0xFF;  
  bitstream[3] = (humidity << 1) & 0xFE;
  bitstream[4] = ((battery_low ? 1 : 0) << 4);  
  int nibbles[8];

  chk = add_nibbles(bitstream, 4) + (bitstream[4] >> 4);

  bitstream[4] |= (chk & 0x3C) >> 2;   // Upper bits of the 6-bit checksum
  bitstream[5] = ((chk & 0x03) );  // Lower bits of the 6-bit checksum

  return bitstream;
}

