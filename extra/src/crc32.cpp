#include "crc32_support.hpp"
#include <iostream>

using namespace std;

/**
 * @brief simplest crc32 c++ implementation
 * @author tongjunhui@live.cn
 * @ref https://gist.github.com/timepp
 *
 */

void crc32::generate_table(uint32_t (&table)[256]) {
  uint32_t polynomial = 0xEDB88320;
  for (uint32_t i = 0; i < 256; i++) {
    uint32_t c = i;
    for (size_t j = 0; j < 8; j++) {
      if (c & 1) {
        c = polynomial ^ (c >> 1);
      } else {
        c >>= 1;
      }
    }
    table[i] = c;
  }
}

uint32_t crc32::update(uint32_t (&table)[256], uint32_t initial,
                       const void *buf, size_t len) {
  uint32_t c = initial ^ 0xFFFFFFFF;
  const uint8_t *u = static_cast<const uint8_t *>(buf);
  for (size_t i = 0; i < len; ++i) {
    c = table[(c ^ u[i]) & 0xFF] ^ (c >> 8);
  }
  return c ^ 0xFFFFFFFF;
}

uint32_t crc32::table[256];
uint32_t crc32::initial;
crc32 default_crc32;

crc32::crc32(const char *randomString) {
  crc32::generate_table(crc32::table);
  crc32::initial = crc32::update(table, 0, randomString);
}

uint32_t crc32::update(uint32_t (&table)[256], uint32_t initial,
                       const std::string &str) {
  return update(table, initial, str.c_str(), str.length());
}

uint32_t crc32::update(const std::string &str) {
  return update(table, crc32::initial, str.c_str(), str.length());
}
