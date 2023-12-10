#include "s21_string.h"

s21_size_t s21_strcspn(const char *dest, const char *src) {
  int len = 0;
  if ((dest == s21_NULL) || (src == s21_NULL)) return len;
  while (*dest) {
    if (s21_strchr(src, *dest)) {
      return len;
    } else {
      dest++;
      len++;
    }
  }
  return len;
}