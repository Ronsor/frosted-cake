#include "console.h"
#include "parser.h"
#include "visit.h"
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#ifndef TEST
static void print_report(struct report *report) {
  printf("\n");
  printf(" %d files in %.2f seconds\n", report->no_files,
         report->cpu_time_used_sec);

  printf(" %d" LIGHTRED " errors " RESET, report->error_count);
  printf("%d" LIGHTMAGENTA " warnings " RESET, report->warnings_count);
  printf("%d" LIGHTCYAN " notes " RESET, report->info_count);
  printf("\n");
}

int main(int argc, char **argv) {
  enable_vt_mode();

  printf(WHITE "Cake " CAKE_VERSION RESET "\n\n");

  if (argc < 2) {
    print_help();
    return 1;
  }

  struct report report = {0};
  compile(argc, (const char **)argv, &report);
  print_report(&report);

  return report.error_count > 0;
}

#else
#include "unit_test.c"

int main(int argc, char **argv) {

  enable_vt_mode();

  test_main();
  printf("%d tests failed, %d tests passed\n", g_unit_test_error_count,
         g_unit_test_success_count);
}
#endif
