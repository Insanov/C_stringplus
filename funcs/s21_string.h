#ifndef SRC_S21_STRING_H_
#define SRC_S21_STRING_H_

#include <limits.h>
#include <math.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#define SIZE 2000
#define s21_NULL ((void *)0)

typedef long unsigned s21_size_t;

typedef struct {
  char flag_zero;
  char flag_plus;
  char flag_minus;
  char flag_space;
  char flag_grid;
  int width;
  int presicion;
  char length;
  char specifier;
  int is_little;
  int is_o;
} s_spec;

typedef struct {
  double lfnum;
  long double Lfnum;
} s_float_family;

typedef struct {
  char s[SIZE];
  int is_not_num;
  int is_float;
  int need_space;
  int cum;
} s_type;

int s21_sprintf(char *str, const char *format, ...);
void specs_handle(const char *format, s_spec *par_list, s21_size_t *sym_count,
                  va_list ap);
void type_handle(s_type *type, s_spec spec, va_list ap);
void short_handle(s_type *type, s_spec spec, va_list ap);
void string_filling(char *str, s_spec spec, s_type type, s21_size_t *count,
                    int i);
void ntoa(char str[SIZE], long long int, s_spec spec);
void untoa(char str[SIZE], unsigned long long int num, int presicion,
           s_spec spec);
void ftoa(char str[SIZE], s_spec spec, s_float_family float_family,
          int presicion);
void xtoa(char str[SIZE], s_spec spec, unsigned long long int num);
void etoa(char str[SIZE], long double num, s_spec *spec);
void gtoa(char str[SIZE], long double num, s_spec *spec);
void ptoa(char str[SIZE], char *pointer, s_spec spec);

void *s21_memchr(const void *str, int c, s21_size_t n);
int s21_memcmp(const void *str1, const void *str2, s21_size_t n);
void *s21_memcpy(void *dest, const void *src, s21_size_t n);
void *s21_memmove(void *dest, const void *src, s21_size_t n);
void *s21_memset(void *str, int c, s21_size_t n);
s21_size_t s21_strlen(const char *str);
char *s21_strcat(char *dest, const char *src);
char *s21_strncat(char *dest, const char *src, s21_size_t n);
char *s21_strchr(const char *str, int c);
int s21_strcmp(const char *str1, const char *str2);
int s21_strncmp(const char *str1, const char *str2, s21_size_t n);
char *s21_strcpy(char *dest, const char *src);
char *s21_strncpy(char *dest, const char *src, s21_size_t n);
s21_size_t s21_strcspn(const char *str1, const char *str2);
char *s21_strpbrk(const char *str1, const char *str2);
char *s21_strrchr(const char *str, int c);
s21_size_t s21_strspn(const char *str1, const char *str2);
char *s21_strstr(const char *haystack, const char *needle);
char *s21_strtok(char *str, const char *delim);
void *s21_to_upper(const char *str);
void *s21_to_lower(const char *str);
void *s21_insert(const char *src, const char *str, s21_size_t start_index);
void *s21_trim(const char *src, const char *trim_chars);
char *s21_strerror(int errcode);

struct st {
  char flag[10];
  int width;
  int accuracy;
  char type;
};

int s21_sscanf(char *str, const char *format, ...);
int BetweenPercentAndSpecifier(const char *format, int i, struct st *spec);
int ParseType(char *str, char sym, struct st *spec, va_list params, int *j,
              int *resultation);
int ForDI(char *str, va_list params, int *j, int width, char type,
          int *resultation);
int ForS(char *str, va_list params, int *j, int width, int *resultation);
int ForC(char *str, va_list params, int *j, int *resultation);
int ForF(char *str, va_list params, int *j, int width, char type,
         int *resultation);
int ForU(char *str, va_list params, int *j, int width, char type,
         int *resultation);
int ForPercent(char *str, int *j, int *resultation);

#endif  // SRC_S21_STRING_H_
