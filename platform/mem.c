//
// Created by najamhaq on 2026-01-18.
//
#include <stddef.h>
#include <stdint.h>

void *memset(void *dst, int c, size_t n) {
  uint8_t *p = (uint8_t *) dst;
  while (n--) {
    *p++ = (uint8_t) c;
  }
  return dst;
}
