#include "s21_string.h"

void *s21_to_lower(const char *str) {  // +
  char *res = calloc(sizeof(char), s21_strlen(str) + 2);
  if (res) {
    for (s21_size_t i = 0; str[i]; i++)
      res[i] = (str[i] > 64 && str[i] < 91) ? str[i] + 32 : str[i];
  }
  return (void *)res;
}