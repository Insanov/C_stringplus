#include "s21_string.h"

void *s21_memmove(void *dest, const void *src, s21_size_t n) {
  char *ach = (char *)malloc(sizeof(char) * n);
  if (ach) {
    s21_memcpy(ach, ((char *)src), n);
    s21_memcpy(((char *)dest), ach, n);
    free(ach);
  }
  return (dest);
}