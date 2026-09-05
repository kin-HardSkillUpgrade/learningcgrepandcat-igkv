#ifndef C_CAT_LIB
#define C_CAT_LIB
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int numberNonBlank;
  int symbolsEndLine;
  int number;
  int squeezeBlank;
  int showTabs;
  int showSymSinc;

} Flags;

int pars_arg_long(char *str, Flags *keys);

int pars_arg_short(char *str, Flags *keys);

int parsing_arguments(Flags *keys, char *buff);

void copy_line_from_oldbuffer_in_newbuffer(char *newBuffer, char *buffer,
                                           size_t *indexNewBuffer,
                                           size_t lineStart, size_t i,
                                           size_t size);

int is_line_empty_b_s(size_t lineStart, size_t i, char *buffer);

size_t copy_newbuffer_in_oldbuffer(char **buffer, char *newBuffer,
                                   size_t indexNewBuffer);

size_t number_non_blank_b(char **buffer, size_t size);

size_t number_n_creater(char *buffer, char *newBuffer, size_t size);

size_t number_n(char **buffer, size_t size);

size_t symbols_end_line_e(char **buffer, size_t size);
size_t squeeze_blank_s(char **buffer, size_t size);

size_t shouws_tabs_t(char **buffer, size_t size);

void ascii_chars_low(char *newBuffer, char *buffer, size_t *indexNewBuffer,
                     size_t i);

void ascii_chars_hight(char *newBuffer, char *buffer, size_t *indexNewBuffer,
                       size_t i);

size_t show_sym_sinc_v(char **buffer, size_t size);

size_t edition_file(Flags keys, char **buff, size_t count);

size_t get_file_size(FILE *file);

size_t buffer_created(char *buff, char **buffer, size_t *bufferTotall);

void print_func(int a, char *buff[]);

void print_no_arg();

#endif