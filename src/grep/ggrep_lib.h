#ifndef C_GREP_LIB
#define C_GREP_LIB
#define _GNU_SOURCE
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Flags {
  int pattern;
  int ignoresCase;
  int invertPattern;

  int sumMatchingLines;
  int matchingFiles;

  int printNumberLines;

  int noFilename;
  int noMassagesFails;

  int patternFromFiles;
  int printOnlyMatching;

  char *patternses;

  int sizeLine;
  int memLine;
} Flags;

void pattern_a_lot(Flags *keys, char *pattern);

void pattern_from_files(Flags *keys, char *filename);

Flags parsing_arguments(int argc, char *argv[]);

void print_line(char *line, int size);

void print_flag_match(Flags keys, char *filename, int lineCount);

void print_match(regex_t *re, char *line, char *filename, Flags keys,
                 int lineCount);

int process_file(Flags keys, char *filename, regex_t *reg);

void print_func(Flags keys, int argc, char *argv[]);

void print_no_arg();
#endif