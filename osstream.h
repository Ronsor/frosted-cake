#pragma once

#include "ownership.h"
#include <stdarg.h>

struct osstream {
  char *owner c_str;
  int size;
  int capacity;
};

void ss_close(implicit struct osstream *obj_owner stream);

int ss_vafprintf(struct osstream *stream, const char *fmt, va_list args);
int ss_fprintf(struct osstream *stream, const char *fmt, ...);
int ss_putc(char ch, struct osstream *stream);
void ss_clear(struct osstream *stream);
void ss_swap(struct osstream *a, struct osstream *b);
