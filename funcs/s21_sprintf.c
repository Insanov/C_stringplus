#include "s21_string.h"

char *s22_strncpy(char *dest, const char *src, s21_size_t n);
s21_size_t s22_strlen(const char *str);
char *s22_strcat(char *dest, const char *src);
char *s22_strchr(const char *str, int c);

int s21_sprintf(char *str, const char *format, ...) {
  int res = 0;
  s21_size_t count = 0, sym_count = 0, i = 0;
  va_list ap;
  va_start(ap, format);

  for (; (i + sym_count) < s22_strlen(format) && (i + sym_count) < 2000; i++) {
    if (format[i + sym_count] == '%') {
      s_spec spec = {0};
      spec.presicion = -1;
      specs_handle(&(format[i + sym_count]), &spec, &sym_count, ap);
      s_type type = {{0}, 0, 0, 0, 0};
      type_handle(&type, spec, ap);
      if (type.cum) break;
      string_filling(str, spec, type, &count, i);
    }
    if ((i + sym_count) < s22_strlen(format))
      str[i + count] = format[i + sym_count];
    str[i + count + 1] = 0;
  }
  va_end(ap);
  res = s22_strlen(str);

  return res;
}

void string_filling(char *str, s_spec spec, s_type type, s21_size_t *count,
                    int i) {
  char tmp[SIZE] = {0};
  char tmp1[SIZE] = {0};
  int size = 0;
  int need_zero = (type.is_float && spec.presicion != -1);
  char space = (!spec.flag_minus && spec.flag_zero &&
                (spec.presicion == -1 || need_zero))
                   ? '0'
                   : ' ';
  if (type.cum) return;

  s22_strncpy(tmp, type.s, SIZE);
  int is_sign = (tmp[0] == '+' || tmp[0] == '-') ? 1 : 0;
  if (spec.flag_space && type.need_space && !is_sign) {
    int len = s22_strlen(str);
    str[len] = ' ';
    str[len + 1] = 0;
    size++;
    if (spec.width > (int)s22_strlen(tmp)) spec.width--;
  }

  spec.width =
      (!spec.width || (spec.width && spec.width < (int)s22_strlen(tmp)))
          ? (int)s22_strlen(tmp)
          : spec.width;

  if (spec.flag_minus) {
    for (int i = 0; i < spec.width && i < 2000; i++)
      tmp1[i] = tmp[i] ? tmp[i] : space;
  } else {
    int flag = 0;
    if (spec.flag_zero && spec.width > (int)s22_strlen(tmp) && is_sign &&
        (spec.presicion == -1 || need_zero)) {
      flag = tmp[0];
      tmp[0] = '0';
    }
    for (int i = 0, j = 0; i < spec.width && i < 2000; i++) {
      tmp1[i] = tmp[spec.width - i - 1] ? tmp[j++] : space;
      if (flag) {
        tmp1[i] = flag;
        flag = 0;
      }
    }
  }
  *count += spec.width;
  *count += size;
  if (!i) {
    s21_size_t j = 0;
    for (; j < s22_strlen(tmp1) && j < 2000; j++) str[j] = tmp1[j];
    str[j] = 0;
  } else {
    s22_strcat(str, tmp1);
  }
}

void type_handle(s_type *type, s_spec spec, va_list ap) {
  int d = 0, s_len = 0;
  long int ld = 0;
  long long int Ld = 0;
  int i = 0;
  long int li = 0;
  long long int Li = 0;
  unsigned int u = 0;
  unsigned long int lu = 0;
  unsigned long long int Lu = 0;
  char c = 0;
  char *s = s21_NULL;
  s_float_family float_family = {0};
  int presicion = (spec.presicion == -1) ? 6 : spec.presicion;

  switch (spec.specifier) {
    case 'd':
      switch (spec.length) {
        case 'h':
          type->need_space = 1;
          short_handle(type, spec, ap);
          break;
        case 'l':
          ld = va_arg(ap, long int);
          type->need_space = 1;
          ntoa(type->s, ld, spec);
          break;
        case 'L':
          Ld = va_arg(ap, long long int);
          type->need_space = 1;
          ntoa(type->s, Ld, spec);
          break;
        default:
          d = va_arg(ap, int);
          type->need_space = 1;
          ntoa(type->s, d, spec);
      }
      break;
    case 'i':
      switch (spec.length) {
        case 'h':
          type->need_space = 1;
          short_handle(type, spec, ap);
          break;
        case 'l':
          li = va_arg(ap, long int);
          type->need_space = 1;
          ntoa(type->s, li, spec);
          break;
        case 'L':
          Li = va_arg(ap, long long int);
          type->need_space = 1;
          ntoa(type->s, Li, spec);
          break;
        default:
          i = va_arg(ap, int);
          type->need_space = 1;
          ntoa(type->s, i, spec);
      }
      break;
    case 'u':
      switch (spec.length) {
        case 'h':
          short_handle(type, spec, ap);
          break;
        case 'l':
          lu = va_arg(ap, unsigned long int);
          untoa(type->s, lu, spec.presicion, spec);
          break;
        case 'L':
          Lu = va_arg(ap, unsigned long long int);
          untoa(type->s, Lu, spec.presicion, spec);
          break;
        default:
          u = va_arg(ap, unsigned int);
          untoa(type->s, u, spec.presicion, spec);
      }
      break;
    case 'X':
      switch (spec.length) {
        case 'h':
          short_handle(type, spec, ap);
          break;
        case 'l':
          lu = va_arg(ap, unsigned long int);
          xtoa(type->s, spec, lu);
          break;
        case 'L':
          Lu = va_arg(ap, unsigned long long int);
          xtoa(type->s, spec, Lu);
          break;
        default:
          u = va_arg(ap, unsigned int);
          xtoa(type->s, spec, u);
      }
      break;
    case 'f':
      switch (spec.length) {
        case 'l':
          float_family.lfnum = va_arg(ap, double);
          type->need_space = 1;
          type->is_float = 1;
          ftoa(type->s, spec, float_family, presicion);
          break;
        case 'L':
          float_family.Lfnum = va_arg(ap, long double);
          type->need_space = 1;
          type->is_float = 1;
          ftoa(type->s, spec, float_family, presicion);
          break;
        default:
          type->need_space = 1;
          type->is_float = 1;
          short_handle(type, spec, ap);
          break;
      }
      break;
    case 'E':
      switch (spec.length) {
        case 'l':
          float_family.lfnum = va_arg(ap, double);
          type->is_float = 1;
          type->need_space = 1;
          etoa(type->s, float_family.lfnum, &spec);
          break;
        case 'L':
          float_family.Lfnum = va_arg(ap, long double);
          type->is_float = 1;
          type->need_space = 1;
          etoa(type->s, float_family.Lfnum, &spec);
          break;
        default:
          type->is_float = 1;
          type->need_space = 1;
          short_handle(type, spec, ap);
          break;
      }
      break;
    case 'G':
      switch (spec.length) {
        case 'l':
          float_family.lfnum = va_arg(ap, double);
          type->is_float = 1;
          gtoa(type->s, float_family.lfnum, &spec);
          break;
        case 'L':
          float_family.Lfnum = va_arg(ap, long double);
          type->is_float = 1;
          gtoa(type->s, float_family.Lfnum, &spec);
          break;
        default:
          type->is_float = 1;
          short_handle(type, spec, ap);
          break;
      }
      break;
    case 'c':
      type->is_not_num = 1;
      c = va_arg(ap, int);
      s22_strncpy(type->s, &c, 1);
      if (c) {
        s22_strncpy(type->s, &c, 1);
      } else {
        type->s[s22_strlen(type->s)] = 0;
        type->cum = 1;
      }
      break;
    case 's':
      type->is_not_num = 1;
      s = va_arg(ap, char *);
      s_len =
          (spec.presicion == -1 || (s && spec.presicion > (int)s22_strlen(s)))
              ? SIZE
              : spec.presicion;
      if (!s && spec.presicion)
        s22_strncpy(type->s, "(null)", SIZE);
      else if (s)
        s22_strncpy(type->s, s, s_len);
      break;
    case 'p':
      type->is_not_num = 1;
      s = va_arg(ap, char *);
      ptoa(type->s, s, spec);
      break;
    case '%':
      c = '%';
      type->is_not_num = 1;
      s22_strncpy(type->s, &c, 1);
      break;
    case 'n':
      break;
    default:
      break;
  }
}

void short_handle(s_type *type, s_spec spec, va_list ap) {
  int16_t hd;
  int16_t hi;
  uint16_t hu;
  s_float_family float_family = {0};
  int presicion = (spec.presicion == -1) ? 6 : spec.presicion;

  switch (spec.specifier) {
    case 'd':
      hd = va_arg(ap, int);
      ntoa(type->s, hd, spec);
      break;
    case 'i':
      hi = va_arg(ap, int);
      ntoa(type->s, hi, spec);
      break;
    case 'u':
      hu = va_arg(ap, unsigned int);
      untoa(type->s, hu, spec.presicion, spec);
      break;
    case 'X':
      hu = va_arg(ap, unsigned int);
      xtoa(type->s, spec, hu);
      break;
    case 'f':
      float_family.lfnum = va_arg(ap, double);
      ftoa(type->s, spec, float_family, presicion);
      break;
    case 'E':
      float_family.lfnum = va_arg(ap, double);
      etoa(type->s, float_family.lfnum, &spec);
      break;
    case 'G':
      float_family.lfnum = va_arg(ap, double);
      gtoa(type->s, float_family.lfnum, &spec);
      break;
  }
}

void specs_handle(const char *format, s_spec *spec, s21_size_t *sym_count,
                  va_list ap) {
  int s_i = 0, new_width = 0, new_presicion = 0;
  char flags[] = "-+ #0";
  char specs[] = "cdieEfgGosuxXpn%";
  char little_specs[] = "egx";
  char lens[] = "hlL";

  s_i++;
  while (s22_strchr(flags, format[s_i])) {
    switch (format[s_i]) {
      case '-':
        spec->flag_minus = format[s_i++];
        break;
      case '+':
        spec->flag_plus = format[s_i++];
        break;
      case ' ':
        spec->flag_space = format[s_i++];
        break;
      case '#':
        spec->flag_grid = format[s_i++];
        break;
      case '0':
        spec->flag_zero = format[s_i++];
        break;
      default:
        break;
    }
  }
  if ((format[s_i] > 47 && format[s_i] < 58) || format[s_i] == '*') {
    if (format[s_i] == '*') {
      new_width = va_arg(ap, int);
      spec->width = new_width;
      s_i++;
    } else {
      char num[100] = {0};
      int index = 0;
      while (format[s_i] > 47 && format[s_i] < 58) num[index++] = format[s_i++];
      spec->width = atoi(num);
    }
  }
  if (format[s_i] == '.') {
    s_i++;
    if (format[s_i] == '*') {
      new_presicion = va_arg(ap, int);
      spec->presicion = (new_presicion > 308) ? 308 : new_presicion;
      s_i++;
    } else {
      char num[100] = {0};
      int index = 0;
      while (format[s_i] > 47 && format[s_i] < 58) num[index++] = format[s_i++];
      // spec->presicion = (atoi(num) > 308) ? 308 : atoi(num);
      spec->presicion = atoi(num);
    }
  }
  if (s22_strchr(lens, format[s_i])) spec->length = format[s_i++];
  if (s22_strchr(specs, format[s_i])) {
    if (s22_strchr(little_specs, format[s_i])) {
      spec->is_little = 1;
      spec->specifier = format[s_i++] - 32;
    } else if (format[s_i] == 'o') {
      spec->is_o = 1;
      spec->specifier = 'X';
      s_i++;
    } else {
      spec->specifier = format[s_i++];
    }
  }
  *sym_count += s_i;
}

void ftoa(char str[SIZE], s_spec spec, s_float_family float_family,
          int presicion) {
  char mantissa[SIZE] = {0};
  long double left = 0;
  long double right = 0;
  long double num = 0;

  switch (spec.length) {
    case 'h':
      break;
    case 'l':
      num = float_family.lfnum;
      break;
    case 'L':
      num = float_family.Lfnum;
      break;
    default:
      num = float_family.lfnum;
      break;
  }

  long double tmp = num;
  if (num < 0) {
    num = -num;
    str[0] = '-';
  } else if (spec.flag_plus) {
    str[0] = '+';
  }
  right = modfl(num, &left);
  long double check_pres_right = 0, check_pres_left = 0;
  check_pres_right = modfl(right * pow(10, presicion), &check_pres_left);
  check_pres_left /= pow(10, presicion - 1);
  int need_increase = (check_pres_left >= 9 && check_pres_right > 0.5) ? 1 : 0;
  int last_number = (unsigned long long int)left % 10;
  if ((!presicion || (need_increase && (last_number || !left))) &&
      right >= 0.5) {
    left++;
    right = 0;
  }
  // if (!presicion || need_increase) {
  //     int last_number = (unsigned long long int)left % 10;
  //     left = (right >= 0.5 && (last_number || !left)) ? left + 1 : left;
  //     right = 0;
  // }

  unsigned long long int left_copy = left;
  int count = 0;

  for (; left_copy && count < 2000; left_copy /= 10, count++)
    ;

  long double right_copy = right;
  int zero_count = 0;
  for (; right_copy && right_copy < 1 && zero_count < 2000;
       right_copy *= 10, zero_count++)
    ;

  right *= (unsigned long long int)pow(10, presicion);
  right = roundl(right);
  right = (unsigned long long int)right;
  zero_count = ((right - (unsigned long long int)pow(10, presicion - 1) == 0) &&
                zero_count > 0)
                   ? zero_count - 1
                   : zero_count;
  char super_tmp[SIZE] = {0};
  if (tmp < 0 || (spec.flag_plus && tmp > 0)) {
    untoa(super_tmp, left, -1, spec);
    for (s21_size_t i = 1; i < s22_strlen(super_tmp) + 1 && i < 2000; i++)
      str[i] = super_tmp[i - 1];
  } else {
    untoa(str, left, -1, spec);
  }
  if (presicion) str[s22_strlen(str)] = '.';

  if (zero_count) {
    if (presicion > 0 && zero_count > presicion)
      zero_count = (zero_count - presicion > 1) ? presicion - 1 : presicion;
    s21_size_t len = s22_strlen(str);
    for (int i = 0; i < zero_count - 1 && i < 2000 - zero_count - 1; i++)
      str[len + i] = '0';
    str[s22_strlen(str)] = 0;
  }

  if (right) {
    if (spec.flag_plus) spec.flag_plus = 0;
    untoa(mantissa, (unsigned long long int)right, -1, spec);
    s22_strcat(str, mantissa);
  } else {
    int len = (int)s22_strlen(str);
    for (int i = 0; i < presicion && i < 2000 - presicion - 1; i++)
      str[len + i] = '0';
    str[s22_strlen(str)] = 0;
  }
}

void untoa(char str[SIZE], unsigned long long int num, int presicion,
           s_spec spec) {  // +
  char arr[SIZE] = {0};
  unsigned long long int tmp = num;
  int count = (!num) ? 1 : 0;
  if (spec.specifier == 'u' && !presicion && !num) return;
  for (; tmp && count < 2000; tmp /= 10, count++)
    ;

  for (int arr_index = 0; arr_index < count && arr_index < 2000; arr_index++)
    arr[arr_index] =
        num / (long long int)pow(10, count - arr_index - 1) % 10 + 48;
  if (!num)
    for (int j = 0; j < presicion - 1 && j < 2000; j++) str[j] = '0';
  else
    for (int j = 0; (presicion != -1 &&
                     j < (presicion - (int)s22_strlen(arr)) && j < 2000);
         j++)
      str[j] = '0';
  s22_strcat(str, arr);
}

void ntoa(char str[SIZE], long long int num, s_spec spec) {  // +
  char arr[SIZE] = {0};
  long long int tmp = num;
  int count = (!num && spec.presicion) ? 1 : 0;

  for (; tmp && count < 2000; tmp /= 10, count++)
    ;

  s21_size_t str_index = 0;
  if (num < 0 || spec.flag_plus)
    str[str_index++] = (spec.flag_plus && num >= 0) ? '+' : '-';

  for (int arr_index = 0; arr_index < count && arr_index < 2000; arr_index++)
    arr[arr_index] =
        llabs(num) / (unsigned long long int)pow(10, count - arr_index - 1) %
            10 +
        48;

  for (int j = 0; (spec.presicion != -1 &&
                   j < spec.presicion - (int)s22_strlen(arr) && j < 2000);
       j++, str_index++)
    str[str_index] = '0';
  s22_strcat(str, arr);
}

void xtoa(char str[SIZE], s_spec spec, unsigned long long int num) {
  unsigned long long int tmp = num;
  char arr[SIZE] = {0};
  char str_tmp[SIZE] = {0};
  char arrX[] = "0123456789ABCDEF";
  char arrx[] = "0123456789abcdef";
  int base = (spec.is_o) ? 8 : 16;
  int bal = 0;
  if (spec.flag_grid && num) {
    if (spec.is_little) {
      s22_strcat(str, "0x");
    } else {
      if (spec.is_o) {
        s22_strcat(str, "0");
        bal = 1;
      } else {
        s22_strcat(str, "0X");
      }
    }
  }
  if (spec.presicion == -1 && !num) arr[0] = '0';
  for (s21_size_t i = 0; tmp && i < 2000; tmp /= base, i++)
    arr[i] = (spec.is_little) ? arrx[tmp % base] : arrX[tmp % base];

  int zero_amount = 0;
  for (; spec.presicion != -1 &&
         zero_amount < spec.presicion - (int)s22_strlen(arr) - bal &&
         zero_amount < 2000;
       zero_amount++)
    str_tmp[zero_amount] = '0';

  for (int j = (int)s22_strlen(arr) - 1; j != -1 && j > -2; j--)
    str_tmp[s22_strlen(arr) - j - 1 + zero_amount] = arr[j];
  s22_strcat(str, str_tmp);
}

void etoa(char str[SIZE], long double num, s_spec *spec) {
  char str_right[SIZE] = {0};
  char str_e[SIZE] = {0};
  long double tmp = (num < 0) ? -num : num;
  long double right = 0, left = 0;
  int count = 0;
  if (spec->specifier == 'G' && !spec->presicion) spec->presicion = 1;
  int presicion = (spec->presicion > -1) ? spec->presicion : 6;
  char e = (spec->is_little) ? 'e' : 'E';

  if (num < 0 || spec->flag_plus) str[0] = (num < 0) ? '-' : '+';

  if (tmp > 1)
    for (; tmp >= 10 && count < 2000; tmp /= 10, count++)
      ;
  else
    for (; tmp < 1 && tmp && count < 2000; tmp *= 10, count++)
      ;

  right = modfl(tmp, &left);
  if (right > 0.5 && (!presicion || presicion == 1) && spec->is_o != 2)
    left += 1;
  if (spec->specifier == 'G' && !spec->is_o && spec->presicion == 2)
    presicion = 1;
  right = right * (unsigned long long int)pow(10, presicion);
  right = roundl(right);
  untoa(str, (unsigned long long int)left, -1, *spec);
  presicion = (presicion == 1 && spec->specifier == 'G' && !spec->is_o &&
               spec->presicion != 2)
                  ? -1
                  : presicion;
  if ((presicion > 0) && !spec->is_o) {
    str[s22_strlen(str)] = '.';
    if (!num)
      untoa(str_right, right, presicion, *spec);
    else
      untoa(str_right, right, -1, *spec);
    s22_strcat(str, str_right);
  }

  str_e[0] = e;
  str_e[1] = (fabsl(num) < 1 && num) ? '-' : '+';
  str_e[2] = (count > 9) ? count / 10 + 48 : '0';
  str_e[3] = (count > 9) ? count % 10 + 48 : count + 48;

  s22_strcat(str, str_e);
}

void gtoa(char str[SIZE], long double num, s_spec *spec) {
  char arr[SIZE] = {0};
  long double left = 0, right = 0;
  // long double tmp = num;
  int presicion = (spec->presicion == -1) ? 6 : spec->presicion;

  if (num < 0) {
    num = -num;
    str[0] = '-';
  } else if (spec->flag_plus) {
    str[0] = '+';
  }

  right = modfl(num, &left);
  left = (unsigned long long int)left;
  untoa(arr, left, -1, *spec);
  if ((int)s22_strlen(arr) > presicion) {
    if (right > 0.5) num += 1;
    num /= (unsigned long long int)pow(10, s22_strlen(arr));
    num = (num > 0.9) ? roundl(num) : num;
    if (num > 0.9) spec->is_o = 2;
    num *= (unsigned long long int)pow(10, s22_strlen(arr));
    num = roundl(num);
    spec->presicion =
        (spec->presicion > 2) ? spec->presicion - 1 : spec->presicion;
    etoa(str, num, spec);
  } else if ((int)s22_strlen(arr) == presicion) {
    if (right > 0.5) {
      char arr1[SIZE];
      untoa(arr1, left + 1, -1, *spec);
      s22_strcat(str, arr1);
    } else {
      s22_strcat(str, arr);
    }
  } else {
    s22_strcat(str, arr);
    str[s22_strlen(str)] = '.';
    right *= (unsigned long long int)pow(10, presicion - s22_strlen(arr));
    long double right_tmp = 0;
    right_tmp = modfl(right, &right);
    if (right_tmp > 0.5) right += 1;
    int div = (unsigned long long int)right % 10;
    s21_size_t i = 0;
    for (; div == 0 && right && i < 2000; right /= 10, i++)
      div = (unsigned long long int)right % 10;
    if (i) right = (unsigned long long int)right * 10 + div;
    char tmp[SIZE] = {0};
    untoa(tmp, (unsigned long long int)right, -1, *spec);
    s22_strcat(str, tmp);
  }
}

void ptoa(char str[SIZE], char *pointer, s_spec spec) {
  if (!pointer) {
    if (spec.presicion) s22_strcat(str, "0");
  } else {
    for (int i = s22_strlen(str); pointer != 0 && i < 2000;
         pointer = ((void *)(((s21_size_t)pointer) >> 4)), i += 1) {
      unsigned int sym = ((s21_size_t)pointer) % 16;
      sym < 10 ? (str[i] = ('0' + sym)) : (str[i] = ('a' + (sym - 10)));
    }
  }
  int add = (str) ? s22_strlen(str) : 0;
  char arr[SIZE] = {0};
  for (int j = 0; j < spec.presicion - add; j++) arr[j] = '0';
  s22_strcat(str, arr);
  s22_strcat(str, "x0");

  char tmp[SIZE] = {0};
  for (s21_size_t i = 0; i < s22_strlen(str) && i < 2000; i++)
    tmp[s22_strlen(str) - i - 1] = str[i];

  s22_strncpy(str, tmp, SIZE);
}

char *s22_strncpy(char *dest, const char *src, s21_size_t n) {
  s21_size_t i = 0;
  while (src[i] != '\0' && i < n && i < 2000) {
    dest[i] = src[i];
    i++;
  }
  while (i < n && i < 2000) {
    dest[i] = '\0';
    i++;
  }
  return (dest);
}

s21_size_t s22_strlen(const char *str) {
  int i = 0;
  s21_size_t lenght = 0;
  while (str[i] != '\0' && i < 2000) {
    lenght++;
    i++;
  }

  return lenght;
}

char *s22_strcat(char *dest, const char *src) {
  s21_size_t add = s22_strlen(dest);
  s21_size_t i = 0;
  // for (; src[i] && i < 2000; add++, i++) *(dest + add) = src[i];
  for (; *src && i < 2000; src++, add++, i++) *(dest + add) = *src;
  *(dest + add) = '\0';

  return dest;
}

char *s22_strchr(const char *str, int c) {
  s21_size_t i;
  char *res = s21_NULL;
  if (c == '\0') {
    res = "";
  } else {
    for (i = 0; i < s22_strlen(str) && res == s21_NULL && i < 2000; i++) {
      if (c == str[i]) {
        res = ((char *)(str + i));
        break;
      }
    }
  }
  return res;
}