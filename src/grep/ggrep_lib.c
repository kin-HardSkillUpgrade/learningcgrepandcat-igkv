#include "ggrep_lib.h"

void pattern_a_lot(Flags *keys, char *pattern) {
  int size = strlen(pattern);
  if (keys->sizeLine == 0) {
    keys->patternses = malloc(1024 * sizeof(char));
    if (keys->patternses == NULL) {
      perror("Ошибка выделения памяти.");
      exit(1);
    }
    keys->patternses[0] = '\0';
    keys->memLine = 1024;
  }
  if (keys->memLine < keys->sizeLine + size) {
    keys->patternses = realloc(keys->patternses, keys->memLine * 2);
    if (keys->patternses == NULL) {
      perror("Ошибка перевыделения памяти.");
      exit(1);
    }
    keys->memLine *= 2;
  }
  if (keys->sizeLine != 0) {
    strcat(keys->patternses + keys->sizeLine, "|");
    keys->sizeLine += 1;
  }
  keys->sizeLine += sprintf(keys->patternses + keys->sizeLine, "(%s)", pattern);
}

void pattern_from_files(Flags *keys, char *filename) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    if (keys->noMassagesFails == 0) {
      perror(filename);
      exit(1);
    }
  }
  char *line = NULL;
  size_t size = 0;
  int bytesRead = 0;

  bytesRead = getline(&line, &size, file);

  while (bytesRead != -1) {
    if (line[bytesRead - 1] == '\n') {
      line[bytesRead - 1] = '\0';
    }
    pattern_a_lot(keys, line);
    bytesRead = getline(&line, &size, file);
  }
  free(line);
  fclose(file);
}

Flags parsing_arguments(int argc, char *argv[]) {
  Flags keys = {0};
  int opt;
  while ((opt = getopt(argc, argv, "e:ivclnhsf:o")) != -1) {
    switch (opt) {
      case 'e':
        keys.pattern = 1;
        pattern_a_lot(&keys, optarg);
        break;
      case 'i':
        keys.ignoresCase = REG_ICASE;
        break;
      case 'v':
        keys.invertPattern = 1;
        break;
      case 'c':
        keys.sumMatchingLines = 1;
        break;
      case 'l':
        keys.matchingFiles = 1;
        break;
      case 'n':
        keys.printNumberLines = 1;
        break;
      case 'h':
        keys.noFilename = 1;
        break;
      case 's':
        keys.noMassagesFails = 1;
        break;
      case 'f':
        keys.patternFromFiles = 1;
        pattern_from_files(&keys, optarg);
        break;
      case 'o':
        keys.printOnlyMatching = 1;
        break;
    }
  }
  if (keys.patternses ==
      NULL)  // это мы проверяем, если у нас нету флага, то мы некст значение
             // закидываем в наш массив регулярок.
  {
    pattern_a_lot(&keys, argv[optind]);
    optind++;  // optind - переменная хранящая индекс следующего элемента,
               // который обработан в argv
  }
  if (argc - optind == 1)  // позволяет нам активировать флаг -h, если у нас
                           // один файл на поток направлен.
  {
    keys.noFilename = 1;
  }
  return keys;
}
// тут мы рисуем наши результаты
void print_line(char *line, int size) {
  for (int i = 0; i < size; i++) {
    putchar(line[i]);
    //
  }
  if (line[size - 1] != '\n') {
    putchar('\n');
  }
}

void print_flag_match(Flags keys, char *filename, int lineCount) {
  if (keys.noFilename == 0) {
    printf("%s:", filename);
  }
  if (keys.printNumberLines == 1) {
    printf("%d:", lineCount);
  }
}

void print_match(regex_t *re, char *line, char *filename, Flags keys,
                 int lineCount) {
  regmatch_t math;
  int offset = 0;
  while (1) {
    int res = regexec(re, line + offset, 1, &math, 0);

    if (res != 0) {
      break;
    }
    print_flag_match(keys, filename, lineCount);
    for (int i = math.rm_so; i < math.rm_eo; i++) {
      putchar(line[offset + i]);
    }
    putchar('\n');

    offset += math.rm_eo;
  }
}

int process_file(Flags keys, char *filename, regex_t *reg) {
  FILE *file = fopen(filename, "r");
  if (file == NULL) {
    if (keys.noMassagesFails == 0) {
      perror(filename);
    }
  } else {
    char *line = NULL;
    size_t size = 0;
    int bytesRead = 0;
    int lineCount = 1;
    int countMatching = 0;
    bytesRead = getline(&line, &size, file);

    while (bytesRead != -1) {
      int result = regexec(reg, line, 0, NULL, 0);
      if ((result == 0 && keys.invertPattern == 0) ||
          (keys.invertPattern == 1 && result != 0)) {
        if (keys.sumMatchingLines == 0 && keys.matchingFiles == 0) {
          if (keys.printOnlyMatching == 1) {
            print_match(reg, line, filename, keys, lineCount);
          } else {
            if (keys.noFilename == 0) {
              printf("%s:", filename);
            }
            if (keys.printNumberLines == 1) {
              printf("%d:", lineCount);
            }

            print_line(line, bytesRead);
          }
        }
        countMatching++;
      }

      lineCount++;
      bytesRead = getline(&line, &size, file);
    }
    free(line);
    if (keys.sumMatchingLines == 1 && keys.matchingFiles == 0) {
      if (keys.noFilename == 0) {
        printf("%s:", filename);
      }
      printf("%d\n", countMatching);
    }
    if (keys.matchingFiles == 1 && countMatching > 0) {
      printf("%s\n", filename);
    }

    fclose(file);
  }
  return 0;
}

void print_func(Flags keys, int argc, char *argv[]) {
  regex_t re;
  int error = regcomp(&re, keys.patternses, REG_EXTENDED | keys.ignoresCase);
  if (error != 0) {
    perror("Error");
  } else {
    for (int i = optind; i < argc; i++) {
      if (process_file(keys, argv[i], &re) != 0) {
        continue;
      }
    }
    regfree(&re);
  }
}

void print_no_arg() {
  char buff[1000];
  while (1) {
    fgets(buff, sizeof(buff), stdin);
    printf("%s", buff);
  }
}
