#include "s21_string.h"

char *s21_strchr(const char *str, int c) {
  s21_size_t i;
  char *res = s21_NULL;
  if (c == '\0') {
    res = "";
  } else {
    for (i = 0; i < s21_strlen(str) && res == s21_NULL; i++) {
      if (c == str[i]) {
        res = ((char *)(str + i));
        break;
      }
    }
  }
  return res;
}