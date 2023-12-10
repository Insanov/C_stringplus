#include "s21_string.h"

char *s21_strcat(char *dest, const char *src) {
  s21_size_t add = s21_strlen(dest);
  for (; *src; src++, add++) *(dest + add) = *src;
  *(dest + add) = '\0';

  return dest;
}