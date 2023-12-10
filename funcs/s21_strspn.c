#include "s21_string.h"

s21_size_t s21_strspn(const char *dest, const char *src) {
  s21_size_t len = 0, i, z;
  int flag = 0;
  for (i = 0; dest[i] != '\0'; i++) {
    for (z = 0; src[z] != '\0'; z++) {
      if (dest[i] == src[z]) {
        len++;
        flag = 1;
        break;
      }
    }
    if (flag == 0) break;
    flag = 0;
  }
  return len;
}