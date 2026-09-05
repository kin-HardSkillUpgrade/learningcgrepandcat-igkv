#include "ggrep_lib.h"

int main(int argc, char *argv[]) {
  Flags keys = parsing_arguments(argc, argv);
  if (argc == 2 || (argc == 3 && keys.patternses == argv[2])) {
    print_no_arg();
  } else {
    print_func(keys, argc, argv);
  }
  if (keys.patternses != NULL) {
    free(keys.patternses);
  }
  return 0;
}