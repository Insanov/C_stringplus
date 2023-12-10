#include "s21_string.h"

int s21_sscanf(char *str, const char *format, ...) {
  int resultation = 0;
  va_list params;
  va_start(params, format);
  int j = 0;
  for (int i = 0; format[i] != '\0'; i++) {
    if (format[i] == '%') {
      struct st spec = {"xxx", 0, INT_MIN, 0};
      i = BetweenPercentAndSpecifier(format, i, &spec);
      resultation = ParseType(str, format[i], &spec, params, &j, &resultation);
    }
  }
  va_end(params);
  return resultation;
}

int BetweenPercentAndSpecifier(const char *format, int i, struct st *spec) {
  // spec->star = *(s21_strchr(format, '*'));
  for (i += 1; s21_strchr("0123456789", format[i]); i += 1)
    spec->width = (spec->width * 10) + (format[i] - 48);
  for (; s21_strchr("hlL", format[i]); i += 1) spec->type = format[i];
  return i;
}

int ParseType(char *str, char sym, struct st *spec, va_list params, int *j,
              int *resultation) {
  if (s21_strchr("di", sym)) {
    *resultation = ForDI(str, params, j, spec->width, spec->type, resultation);
  } else if (sym == 's') {
    *resultation = ForS(str, params, j, spec->width, resultation);
  } else if (sym == 'c') {
    *resultation = ForC(str, params, j, resultation);
  } else if (sym == 'f' || sym == 'e' || sym == 'E' || sym == 'g' ||
             sym == 'G') {
    *resultation = ForF(str, params, j, spec->width, spec->type, resultation);
  } else if (sym == 'u' || sym == 'x' || sym == 'X' || sym == 'o') {
    *resultation = ForU(str, params, j, spec->width, spec->type, resultation);
  }
  return *resultation;
}

int ForDI(char *str, va_list params, int *j, int width, char type,
          int *resultation) {
  int number = 0, minus = 0, iter = *j, flag = 0;
  if (str[iter] == ' ') {
    while (str[iter] == ' ') iter++;
  } else if (str[*j] == '0') {
    while (str[*j] == '0') iter++;
  }
  if (width != 0) {
    for (; str[iter] != '\0' && str[iter] != ' ' && str[iter] != '\n' &&
           str[iter] != '\t' && width > 0;
         iter++) {
      if (str[iter] == '-') {
        minus = -1;
      } else if (str[iter] > 47 && str[iter] < 58 && width > 0) {
        flag = 1;
        number = (number * 10) + (str[iter] - 48);
      }
      width--;
    }
  } else {
    for (; str[iter] != '\0' && str[iter] != ' ' && str[iter] != '\n' &&
           str[iter] != '\t';
         iter++) {
      if (str[iter] == '-') {
        minus = -1;
      } else if (str[iter] > 47 && str[iter] < 58) {
        flag = 1;
        number = (number * 10) + (str[iter] - 48);
      }
    }
  }
  if (minus == -1) number = number * (-1);
  // if (!spec.star)
  if (type == 'l') {
    *(long int *)va_arg(params, void *) = number;
    if (flag == 1) *resultation += 1;
  } else if (type == 'h') {
    *(short int *)va_arg(params, void *) = number;
    if (flag == 1) *resultation += 1;
  } else {
    *(int *)va_arg(params, void *) = number;
    if (flag == 1) *resultation += 1;
  }
  *j = iter;
  return *resultation;
}

int ForS(char *str, va_list params, int *j, int width, int *resultation) {
  int count = 0, i = 0, inter = *j;
  char res[1000];
  char *chPtr = va_arg(params, char *);
  if (str[inter] == ' ') {
    while (str[inter] == ' ') inter++;
  }
  if (width != 0) {
    for (; str[inter] != '\0' && str[inter] != ' ' && str[inter] != '\n' &&
           str[inter] != '\t' && width > 0;
         inter++) {
      res[count++] = str[inter];
      width--;
    }
  } else {
    for (; str[inter] != '\0' && str[inter] != ' ' && str[inter] != '\n' &&
           str[inter] != '\t';
         inter++)
      res[count++] = str[inter];
  }

  res[count] = '\0';
  while (res[i] != '\0') {
    *(chPtr) = res[i];
    chPtr++;
    i++;
  }
  *chPtr = '\0';
  *resultation += 1;
  *j = inter;
  return *resultation;
}

int ForC(char *str, va_list params, int *j, int *resultation) {
  // if(str[*j] == ' '){
  //     while(str[*j] == ' ')
  //         j ++;
  // } else if(str[*j] == '0'){
  //      while(str[*j] == '0')
  //         j ++;
  // }
  *(char *)va_arg(params, char *) = str[*j];
  *resultation += 1;
  (*j)++;
  return *resultation;
}

int ForF(char *str, va_list params, int *j, int width, char type,
         int *resultation) {
  int minus = 0, flag_point = 0, count_point = 0, flag_exp = 0, sign = 0,
      iter = *j;
  long double ceil = 0, after_point = 0, exp = 0;
  if (str[iter] == ' ') {
    while (str[iter] == ' ') iter++;
  }
  if (width != 0) {
    for (; str[iter] != '\0' && str[iter] != ' ' && str[iter] != '\n' &&
           str[iter] != '\t' && width > 0;
         iter++) {
      if (str[iter] == '-' && flag_exp == 0) {
        minus = -1;
      } else if (flag_point == 0 && flag_exp == 0 && str[iter] > 47 &&
                 str[iter] < 58) {
        ceil = (ceil * 10) + (str[iter] - 48);
      } else if (str[iter] == '.') {
        flag_point = 1;
      } else if (flag_point == 1 && str[iter] > 47 && str[iter] < 58 &&
                 flag_exp == 0) {
        count_point++;
        after_point = (after_point * 10) + (str[iter] - 48);
      } else if (str[iter] == 'e' || str[iter] == 'E') {
        flag_exp = 1;
      } else if (flag_exp == 1) {
        if (str[iter] == '-') {
          sign = -1;
        } else if (str[iter] > 47 && str[iter] < 58) {
          exp = (exp * 10) + (str[iter] - 48);
        }
      }
      width--;
    }
  } else {
    for (; str[iter] != '\0' && str[iter] != ' ' && str[iter] != '\n' &&
           str[iter] != '\t';
         iter++) {
      if (str[iter] == '-' && flag_exp == 0) {
        minus = -1;
      } else if (flag_point == 0 && str[iter] > 47 && str[iter] < 58) {
        ceil = (ceil * 10) + (str[iter] - 48);
      } else if (str[iter] == '.') {
        flag_point = 1;
      } else if (flag_point == 1 && str[iter] > 47 && str[iter] < 58 &&
                 flag_exp == 0) {
        count_point++;
        after_point = (after_point * 10) + (str[iter] - 48);
      } else if (str[iter] == 'e' || str[iter] == 'E') {
        flag_exp = 1;
      } else if (flag_exp == 1) {
        if (str[iter] == '-') {
          sign = -1;
        } else if (str[iter] > 47 && str[iter] < 58) {
          exp = (exp * 10) + (str[iter] - 48);
        }
      }
    }
  }
  after_point = after_point / (pow(10, count_point));
  ceil = ceil + after_point;
  if (flag_exp == 1) {
    if (sign == 0) {
      ceil *= pow(10, exp);
    } else {
      ceil /= pow(10, exp);
    }
  }
  if (minus == -1) ceil = ceil * (-1);
  if (type == 'l') {
    *(double *)va_arg(params, void *) = ceil;
    *resultation += 1;
  } else {
    *(float *)va_arg(params, void *) = ceil;
    *resultation += 1;
  }
  *j = iter;
  return *resultation;
}

int ForU(char *str, va_list params, int *j, int width, char type,
         int *resultation) {
  int number = 0, iter = *j;
  if (str[iter] == ' ') {
    while (str[iter] == ' ') iter++;
  } else if (str[iter] == '0') {
    while (str[iter] == '0') iter++;
  }
  if (width != 0) {
    for (; str[iter] != '\0' && str[iter] != ' ' && str[iter] != '\n' &&
           str[iter] != '\t' && width > 0;
         iter++) {
      if (width > 0) {
        if (str[iter] > 47 && str[iter] < 58) {
          number = (number * 10) + (str[iter] - 48);
        } else {
          break;
        }
      }
      width--;
    }
  } else {
    for (; str[iter] != '\0' && str[iter] != ' ' && str[iter] != '\n' &&
           str[iter] != '\t';
         iter++) {
      if (str[iter] > 47 && str[iter] < 58) {
        number = (number * 10) + (str[iter] - 48);
      } else {
        break;
      }
    }
  }
  if (type == 'l') {
    *(long unsigned *)va_arg(params, void *) = number;
    *resultation += 1;
  } else if (type == 'h') {
    *(short unsigned *)va_arg(params, void *) = number;
    *resultation += 1;
  } else {
    *(unsigned *)va_arg(params, void *) = number;
    *resultation += 1;
  }
  *j = iter;
  return *resultation;
}
