#include "s21_string.h"

int s21_memcmp(const void *str1, const void *str2, s21_size_t n) {
  s21_size_t i = 0;
  unsigned char *s1 = (unsigned char *)str1, *s2 = (unsigned char *)str2;
  while (i < n) {
    if (s1[i] != s2[i]) return ((int)s1[i] - s2[i]);
    i++;
  }
  return 0;
}