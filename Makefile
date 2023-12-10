CC = gcc
CFLAGS = -Wall -Wextra -Werror -g
LD_S21_STRING_FLAGS = -L. -ls21_string
LD_CHECK_FLAGS = -lcheck -lpthread -lm
GCOV_FLAGS = 

SOURCES = s21_sscanf.c s21_memset.c s21_strcat.c s21_strncat.c s21_strchr.c s21_memcmp.c s21_memcpy.c s21_strlen.c s21_memmove.c s21_strcspn.c s21_strpbrk.c s21_strcmp.c s21_strncmp.c s21_strcpy.c s21_strncpy.c  s21_strrchr.c s21_strspn.c s21_strtok.c s21_trim.c s21_to_lower.c s21_to_upper.c s21_memchr.c s21_insert.c s21_strstr.c s21_strerror.c s21_sprintf.c
OBJECTS = $(SOURCES:%.c=%.o)

FOR_TESTS = 0

UNAME := $(shell uname -s)
ifeq ($(UNAME), Linux)
	LD_CHECK_FLAGS +=  -lsubunit
endif

.PHONY: all gcov_report test s21_string.a clean my_gcov_report

all: gcov_report


gcov_report: GCOV_FLAGS += -fprofile-arcs -ftest-coverage
gcov_report: test
	./test > /dev/null
	lcov -t "report" -o report.info -c -d .
	genhtml -o "html_report" report.info
	open html_report/index.html

test: total_test.o s21_string.a
	$(CC) $< $(LD_S21_STRING_FLAGS) $(LD_CHECK_FLAGS) $(GCOV_FLAGS) -o $@
	./test

s21_string.a: $(OBJECTS)
	ar rc s21_string.a $^
	ranlib s21_string.a
	cp s21_string.a libs21_string.a

%.o: %.c s21_string.h
	$(CC) -c $^ $(GCOV_FLAGS) $(CFLAGS)


total_test.o: total_test.c
	$(CC) -c $^

clean:
	rm -rf *.a t *.o *.gcno o *.gcda report.info html_report test

