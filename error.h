#pragma once

#ifndef __CAKE__

#define try
#define catch                                                                  \
  if (0)                                                                       \
  catch_label:
#define throw goto catch_label

#endif

const char *get_posix_error_message(int error);
int windows_error_to_posix(int i);
