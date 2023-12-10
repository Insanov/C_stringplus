#include "s21_string.h"

void* s21_insert(const char* src, const char* str, s21_size_t start_index) {
  char* res = s21_NULL;
  if (src && str && s21_strlen(src) >= start_index) {
    res = malloc((s21_strlen(src) + s21_strlen(str) + 1) * sizeof(char));
    if (res) {
      s21_strncpy(res, src, start_index);
      *(res + start_index) = '\0';
      s21_strcat(res, str);
      s21_strcat(res, src + start_index);
    }
  }
  return res;
}
