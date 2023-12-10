#include "s21_string.h"

int s21_strncmp(const char *dest, const char *src, s21_size_t n) {
  s21_size_t i = 0;
  while ((dest[i] != '\0' || src[i] != '\0') && i < n) {
    if (dest[i] != src[i]) {
      return (dest[i] - src[i]);
    }
    i++;
  }
  return 0;
}