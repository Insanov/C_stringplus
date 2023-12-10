#include "s21_string.h"

void *s21_memcpy(void *dest, const void *src, s21_size_t n) {
  s21_size_t i = 0;
  unsigned char *dest1 = (unsigned char *)dest, *src1 = (unsigned char *)src;
  while (i < n) {
    dest1[i] = src1[i];
    i++;
  }
  return (dest);
}