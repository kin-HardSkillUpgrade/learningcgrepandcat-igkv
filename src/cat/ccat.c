#include "ccat_lib.h"

int main(int argc, char *argv[]) {
  if (argc > 1) {
    print_func(argc, argv);
  } else {
    print_no_arg();
  }
  return 0;
}