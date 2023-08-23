#include "options.h"
#include "console.h"
#include <assert.h>
#include <stdio.h>
#include <string.h>

static struct w {
  enum warning w;
  const char *name;
} s_warnings[] = {{W_UNUSED_VARIABLE, "unused-variable"},
                  {W_DEPRECATED, "deprecated"},
                  {W_ENUN_COMPARE, "enum-compare"},
                  {W_NON_NULL, "nonnull"},
                  {W_ADDRESS, "address"},
                  {W_UNUSED_PARAMETER, "unused-parameter"},
                  {W_DECLARATOR_HIDE, "hide-declarator"},
                  {W_TYPEOF_ARRAY_PARAMETER, "typeof-parameter"},
                  {W_ATTRIBUTES, "attributes"},
                  {W_UNUSED_VALUE, "unused-value"},
                  {W_STYLE, "style"},
                  {W_DISCARDED_QUALIFIERS, "discarded-qualifiers"},
                  {W_UNINITIALZED, "uninitialized"},
                  {W_NON_OWNER_ASSIGN, "owner-assign"},
                  {W_EXPLICIT_MOVE, "explicit-move"},
                  {W_RETURN_LOCAL_ADDR, "return-local-addr"}};

enum warning get_warning_flag(const char *wname) {

  for (int j = 0; j < sizeof(s_warnings) / sizeof(s_warnings[0]); j++) {
    if (strncmp(s_warnings[j].name, wname, strlen(s_warnings[j].name)) == 0) {
      return s_warnings[j].w;
    }
  }
  return 0;
}

const char *get_warning_name(enum warning w) {
  int lower_index = 0;
  int upper_index = sizeof(s_warnings) / sizeof(s_warnings[0]) - 1;

  while (lower_index <= upper_index) {
    const int mid = (lower_index + upper_index) / 2;
    const int cmp = w - s_warnings[mid].w;

    if (cmp == 0) {
      return s_warnings[mid].name;
    } else if (cmp < 0) {
      upper_index = mid - 1;
    } else {
      lower_index = mid + 1;
    }
  }

  assert(false);
  return "";
}

int fill_options(struct options *options, int argc, const char **argv) {

  /*
     default at this moment is same as -Wall
  */
  options->enabled_warnings_stack[0] = ~0;
  options->enabled_warnings_stack[0] &= ~W_STYLE; // default is OFF

#ifdef __EMSCRIPTEN__
  options->flow_analysis = true;
#endif

  /*first loop used to collect options*/
  for (int i = 1; i < argc; i++) {
    if (argv[i][0] != '-')
      continue;

    if (argv[i][1] == 'I' || argv[i][1] == 'D' || argv[i][1] == 'i') {
      /*
        Valid, but handled with preprocessor
      */
      continue;
    }

    if (strcmp(argv[i], "-no-output") == 0) {
      options->no_output = true;
      continue;
    }

    if (strcmp(argv[i], "-o") == 0) {
      if (i + 1 < argc) {
        strcpy(options->output, argv[i + 1]);
        i++;
      } else {
        // ops
      }
      continue;
    }

    if (strcmp(argv[i], "-E") == 0) {
      options->preprocess_only = true;
      continue;
    }

    if (strcmp(argv[i], "-sarif") == 0) {
      options->sarif_output = true;
      continue;
    }

    if (strcmp(argv[i], "-flow-analysis") == 0) {
      options->flow_analysis = true;
      continue;
    }

    if (strcmp(argv[i], "-remove-comments") == 0) {
      options->remove_comments = true;
      continue;
    }

    if (strcmp(argv[i], "-direct-compilation") == 0 ||
        strcmp(argv[i], "-rm") == 0) {
      options->direct_compilation = true;
      continue;
    }

    if (strcmp(argv[i], "-fi") == 0) {
      options->format_input = true;
      continue;
    }

    if (strcmp(argv[i], "-fo") == 0) {
      options->format_ouput = true;
      continue;
    }

    if (strcmp(argv[i], "-no-discard") == 0) {
      options->nodiscard_is_default = true;
      continue;
    }

    //
    if (strcmp(argv[i], "-style=cake") == 0) {
      options->style = STYLE_CAKE;
      continue;
    }

    if (strcmp(argv[i], "-style=gnu") == 0) {
      options->style = STYLE_GNU;
      continue;
    }

    if (strcmp(argv[i], "-style=microsoft") == 0) {
      options->style = STYLE_GNU;
      continue;
    }

    //
    if (strcmp(argv[i], "-target=c89") == 0) {
      options->target = LANGUAGE_C89;
      continue;
    }

    if (strcmp(argv[i], "-target=c99") == 0) {
      options->target = LANGUAGE_C99;
      continue;
    }
    if (strcmp(argv[i], "-target=c11") == 0) {
      options->target = LANGUAGE_C11;
      continue;
    }
    if (strcmp(argv[i], "-target=c2x") == 0) {
      options->target = LANGUAGE_C2X;
      continue;
    }
    if (strcmp(argv[i], "-target=cxx") == 0) {
      options->target = LANGUAGE_CXX;
      continue;
    }

    //
    if (strcmp(argv[i], "-std=c99") == 0) {
      options->input = LANGUAGE_C99;
      continue;
    }
    if (strcmp(argv[i], "-std=c11") == 0) {
      options->input = LANGUAGE_C11;
      continue;
    }
    if (strcmp(argv[i], "-std=c2x") == 0) {
      options->input = LANGUAGE_C2X;
      continue;
    }
    if (strcmp(argv[i], "-std=cxx") == 0) {
      options->input = LANGUAGE_CXX;
      continue;
    }

    // warnings
    if (argv[i][1] == 'W') {
      if (strcmp(argv[i], "-Wall") == 0) {
        options->enabled_warnings_stack[0] = ~0;
        continue;
      }
      const bool disable_warning = (argv[i][2] == 'n' && argv[i][3] == 'o');

      enum warning w = 0;

      if (disable_warning)
        w = get_warning_flag(argv[i] + 5);
      else
        w = get_warning_flag(argv[i] + 2);

      if (w == 0) {
        printf("unknown warning '%s'", argv[i]);
        return 1;
      }

      if (disable_warning) {
        options->enabled_warnings_stack[0] &= ~w;
      } else {
        options->enabled_warnings_stack[0] |= w;
      }
      continue;
    }

    printf("unknown option '%s'", argv[i]);
    return 1;
  }
  return 0;
}

void print_help() {
  const char *options =
      "cake [options] source1.c source2.c ...\n"
      "\n" WHITE "SAMPLES\n" RESET "\n" WHITE "    cake source.c\n" RESET
      "    Compile source.c and output to out/source.c\n"
      "\n" WHITE "    cake -target=c11 source.c\n" RESET
      "    Compile source.c and output C11 code to out/source.c\n"
      "\n" WHITE "OPTIONS\n" RESET
      "\n" WHITE "  -Idir                 " RESET
      "Add directory to include search path\n"
      "\n" WHITE "  -ifile                " RESET
      "Automatically include file before preprocessing\n"
      "\n" WHITE "  -no-output            " RESET
      "Do not generate output\n"
      "\n" WHITE "  -Dsymbol              " RESET
      "Define a macro\n"
      "\n" WHITE "  -E                    " RESET
      "Preprocess and dump to stdout\n"
      "\n" WHITE "  -o name.c             " RESET
      "Set name of output file\n"
      "\n" WHITE "  -remove-comments      " RESET
      "Strip comments from output\n"
      "\n" WHITE "  -direct-compilation   " RESET
      "Output without macros or preprocessor directives\n"
      "\n" WHITE "  -target=standard      " RESET
      "Target C standard (c89, c99, c11, c2x, cxx)\n"
      "                        C99 is the default and C89 (ANSI C) is the "
      "minimum target\n"
      "\n" WHITE "  -std=standard         " RESET
      "Expect input to comply with specified C standard (not yet implemented)\n"
      "\n" WHITE "  -fi                   " RESET
      "Reformat input before processing\n"
      "\n" WHITE "  -fo                   " RESET
      "Reformat output after processing\n"
      "\n" WHITE "  -no-discard           " RESET
      "Make [[nodiscard]] the default\n"
      "\n" WHITE "  -Wname -Wno-name      " RESET "Enable or disable a specified warning\n"
      "\n" WHITE "  -sarif                " RESET "Generate sarif files\n"
      "\n"
      "More details at http://thradams.com/cake/manual.html\n";

  printf("%s", options);
}
