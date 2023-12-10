#include "s21_string.h"

char *s21_strpbrk(const char *dest, const char *src) {
  s21_size_t i, j;
  char *ach = s21_NULL;
  int flag = 0;
  for (i = 0; i < s21_strlen((char *)dest); i++) {
    for (j = 0; j < s21_strlen((char *)src); j++) {
      if (dest[i] == src[j]) {
        ach = (char *)dest + i;
        flag = 1;
        break;
      }
    }
    if (flag == 1) break;
  }
  return (char *)ach;
}