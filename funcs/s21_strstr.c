#include "s21_string.h"

char *s21_strstr(const char *dest, const char *src) {
  int i, j, flag = 0;
  s21_size_t count = 0;
  char *res = s21_NULL;
  if (s21_strlen(dest) == 0 && s21_strlen(src) == 0) {
    res = ((char *)(""));
  } else if (s21_strlen(src) == 0) {
    res = ((char *)(dest));
  } else {
    for (i = 0; src[i] != '\0'; i++) {
      for (j = 0; dest[j] != '\0'; j++) {
        if (dest[j] == src[i]) {
          count++;
          if (count == s21_strlen(src) && flag == 0) {
            res = ((char *)(dest + j - count + 1));
          }
          break;
        }
      }
    }
  }
  return res;
}