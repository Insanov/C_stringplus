#include "s21_string.h"

char *s21_strtok(char *str, const char *delim) {
  static char *buffer;
  char *token = s21_NULL;
  if (str) {
    buffer = str;
    while (*buffer && s21_strchr(delim, *buffer)) {
      *buffer++ = '\0';
    }
  }
  if (buffer && *buffer) {
    str = buffer;
    while (*buffer && !s21_strchr(delim, *buffer)) {
      ++buffer;
    }
    while (*buffer && s21_strchr(delim, *buffer)) {
      *buffer++ = '\0';
    }
    token = str;
  }
  return token;
}
