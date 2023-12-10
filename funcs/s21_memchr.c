#include "s21_string.h"

void *s21_memchr(const void *str, int c, s21_size_t n) {
  s21_size_t i = 0;
  unsigned char *str1;
  unsigned char c1;
  str1 = (unsigned char *)str;
  c1 = (unsigned char)c;
  while (i < n) {
    if (str1[i] == c1) return ((void *)str + i);
    i++;
  }
  return (s21_NULL);
}