#include "s21_string.h"

void *s21_memset(void *str, int c, s21_size_t n) {
  unsigned char *str1;
  str1 = str;
  while (n >= 1) {
    str1[n - 1] = c;
    n--;
  }
  return (str1);
}