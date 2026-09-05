#include "ccat_lib.h"

int pars_arg_long(char *str, Flags *keys) {
  int error = 0;
  if ((strcmp(str, "number-nonblank")) == 0) {
    keys->numberNonBlank = 1;
  } else if ((strcmp(str, "number")) == 0) {
    keys->number = 1;
  } else if ((strcmp(str, "squeeze-blank")) == 0) {
    keys->squeezeBlank = 1;
  } else {
    printf(
        "ccat: invalid option -- '%s'\nTry 'ccat --help' for more "
        "information.\n",
        str);
    error = -1;
  }
  return error;
}

int pars_arg_short(char *str, Flags *keys) {
  int error = 0;
  for (int i = 0; str[i] != '\0'; i++) {
    if (error == 0) {
      switch (str[i]) {
        case 'b':
          keys->numberNonBlank = 1;
          break;
        case 'e':
          keys->showSymSinc = 1;
          keys->symbolsEndLine = 1;
          break;
        case 'n':
          keys->number = 1;
          break;
        case 's':
          keys->squeezeBlank = 1;
          break;
        case 't':
          keys->showSymSinc = 1;
          keys->showTabs = 1;
          break;
        case 'E':
          keys->symbolsEndLine = 1;
          break;
        case 'T':
          keys->showTabs = 1;
          break;
        case 'v':
          keys->showSymSinc = 1;
          break;
        default:
          printf(
              "ccat invalid option -- '%c'\nTry 'ccat --help' for more "
              "information.\n",
              str[i]);
          error = 0;
      }
    }
  }
  return error;
}

int parsing_arguments(Flags *keys, char *buff) {
  int error = 0;
  char *str = ++buff;
  if (str[0] == '-') {
    ++str;
    error = pars_arg_long(str, keys);
  } else {
    error = pars_arg_short(str, keys);
  }
  return error;
}

void copy_line_from_oldbuffer_in_newbuffer(char *newBuffer, char *buffer,
                                           size_t *indexNewBuffer,
                                           size_t lineStart, size_t i,
                                           size_t size) {
  memcpy(newBuffer + *indexNewBuffer, buffer + lineStart, i - lineStart);
  *indexNewBuffer += i - lineStart;
  if (i < size) {
    newBuffer[*indexNewBuffer] = '\n';
    (*indexNewBuffer)++;
  }
}

int is_line_empty_b_s(size_t lineStart, size_t i, char *buffer) {
  int isLineEmpty = 1;
  int breakFor = 1;
  for (size_t j = lineStart; j < i && breakFor == 1; j++) {
    if (buffer[j] != '\n') {
      isLineEmpty = 0;
      breakFor = 0;
    }
  }
  return isLineEmpty;
}

size_t copy_newbuffer_in_oldbuffer(char **buffer, char *newBuffer,
                                   size_t indexNewBuffer) {
  *buffer = (char *)realloc(*buffer, indexNewBuffer + 1 * sizeof(char));
  if (buffer == NULL) {
    perror("Ошибка выделения памятиasdfa12431");
    indexNewBuffer = -1;
  } else {
    memcpy(*buffer, newBuffer, indexNewBuffer);
    (*buffer)[indexNewBuffer] = '\0';
    free(newBuffer);
  }
  return indexNewBuffer;
}

size_t number_non_blank_b(char **buffer, size_t size) {
  size_t error = 0;
  char *newBuffer = (char *)calloc(size * 10, sizeof(char));
  if (newBuffer == NULL) {
    perror("Ошибка выделения памяти");
    error = -1;
  } else {
    int numberLine = 1;
    size_t indexNewBuffer = 0;
    size_t lineStart = 0;
    for (size_t i = 0; i <= size; i++) {
      if (i == size || (*buffer)[i] == '\n') {
        int isLineEmpty = is_line_empty_b_s(lineStart, i, *buffer);

        if (isLineEmpty == 0) {
          indexNewBuffer += snprintf(newBuffer + indexNewBuffer, size * 2,
                                     "%6d\t", numberLine);
          copy_line_from_oldbuffer_in_newbuffer(
              newBuffer, *buffer, &indexNewBuffer, lineStart, i, size);
          numberLine++;
        } else {
          copy_line_from_oldbuffer_in_newbuffer(
              newBuffer, *buffer, &indexNewBuffer, lineStart, i, size);
        }
        lineStart = i + 1;
      }
    }
    error = copy_newbuffer_in_oldbuffer(buffer, newBuffer, indexNewBuffer);
  }
  return error;
}

size_t number_n_creater(char *buffer, char *newBuffer, size_t size) {
  int numberLine = 1;
  size_t indexNewBuffer = 0;
  size_t lineStart = 0;
  if ((buffer)[size - 1] == '\n') {
    for (size_t i = 0; i <= size - 1; i++) {
      {
        if (i == size || (buffer)[i] == '\n') {
          indexNewBuffer += snprintf(newBuffer + indexNewBuffer, size * 2,
                                     "%6d\t", numberLine);
          copy_line_from_oldbuffer_in_newbuffer(
              newBuffer, buffer, &indexNewBuffer, lineStart, i, size);
          numberLine++;
          lineStart = i + 1;
        }
      }
    }
  } else {
    for (size_t i = 0; i <= size; i++) {
      {
        if (i == size || (buffer)[i] == '\n') {
          indexNewBuffer += snprintf(newBuffer + indexNewBuffer, size * 2,
                                     "%6d\t", numberLine);
          copy_line_from_oldbuffer_in_newbuffer(
              newBuffer, buffer, &indexNewBuffer, lineStart, i, size);
          numberLine++;
          lineStart = i + 1;
        }
      }
    }
  }
  return indexNewBuffer;
}

size_t number_n(char **buffer, size_t size) {
  size_t error = 0;
  if (size == 0) {
    error = -1;
  } else {
    char *newBuffer = (char *)calloc(size * 10, sizeof(char));
    if (newBuffer == NULL) {
      perror("Ошибка выделения памяти");
      error = 1;
    } else {
      size_t indexNewBuffer = number_n_creater(*buffer, newBuffer, size);
      error = copy_newbuffer_in_oldbuffer(buffer, newBuffer, indexNewBuffer);
    }
  }
  return error;
}

size_t symbols_end_line_e(char **buffer, size_t size) {
  size_t error = 0;
  char *newBuffer = (char *)calloc(size * 2, sizeof(char));
  if (newBuffer == NULL) {
    perror("Ошибка выделения памяти12");
    error = -1;
  } else {
    size_t indexNewBuffer = 0;

    for (size_t i = 0; i < size; i++) {
      if ((*buffer)[i] == '\r' && (*buffer)[i + 1] == '\n') {
        newBuffer[indexNewBuffer++] = '^';
        newBuffer[indexNewBuffer++] = 'M';
      } else {
        if ((*buffer)[i] == '\n') {
          newBuffer[indexNewBuffer++] = '$';
        }
        newBuffer[indexNewBuffer++] = (*buffer)[i];
      }
    }
    error = copy_newbuffer_in_oldbuffer(buffer, newBuffer, indexNewBuffer);
  }

  return error;
}

size_t squeeze_blank_s(char **buffer, size_t size) {
  size_t error = 0;
  char *newBuffer = (char *)calloc(size, sizeof(char));
  if (newBuffer == NULL) {
    perror("Ошибка выделения памяти");
    error = -1;
  } else {
    size_t indexNewBuffer = 0;
    size_t lineStart = 0;
    int isPreviousLineEmpty = 0;

    for (size_t i = 0; i <= size; i++) {
      if (i == size || (*buffer)[i] == '\n') {
        int isLineEmpty = is_line_empty_b_s(lineStart, i, *buffer);

        if (isPreviousLineEmpty == 0 || isLineEmpty == 0) {
          copy_line_from_oldbuffer_in_newbuffer(
              newBuffer, *buffer, &indexNewBuffer, lineStart, i, size);
        }
        lineStart = i + 1;
        isPreviousLineEmpty = isLineEmpty;
      }
    }
    error = copy_newbuffer_in_oldbuffer(buffer, newBuffer, indexNewBuffer);
  }
  return error;
}

size_t shouws_tabs_t(char **buffer, size_t size) {
  size_t error = 0;
  size_t indexNewBuffer = 0;
  char *newBuffer = (char *)calloc(size * 2, sizeof(char));
  if (newBuffer == NULL) {
    perror("Ошибка выделения памяти");
    error = -1;
  } else {
    for (size_t i = 0; i < size; i++) {
      if ((*buffer)[i] == '\t') {
        newBuffer[indexNewBuffer++] = '^';
        newBuffer[indexNewBuffer++] = 'I';
      } else {
        newBuffer[indexNewBuffer++] = (*buffer)[i];
      }
    }
    error = copy_newbuffer_in_oldbuffer(buffer, newBuffer, indexNewBuffer);
  }

  return error;
}

void ascii_chars_low(char *newBuffer, char *buffer, size_t *indexNewBuffer,
                     size_t i) {
  if ((buffer)[i] == '\r') {
    newBuffer[(*indexNewBuffer)++] = '^';
    newBuffer[(*indexNewBuffer)++] = 'M';
  } else if ((buffer)[i] == 127) {
    newBuffer[(*indexNewBuffer)++] = '^';
    newBuffer[(*indexNewBuffer)++] = '?';
  } else {
    newBuffer[(*indexNewBuffer)++] = '^';
    newBuffer[(*indexNewBuffer)++] = (buffer)[i] + 64;
  }
}
// Продолжение АААААААААААА.
void ascii_chars_hight(char *newBuffer, char *buffer, size_t *indexNewBuffer,
                       size_t i) {
  newBuffer[(*indexNewBuffer)++] = 'M';
  newBuffer[(*indexNewBuffer)++] = '-';
  if ((unsigned char)(buffer)[i] < 160) {
    newBuffer[(*indexNewBuffer)++] = '^';
    newBuffer[(*indexNewBuffer)++] = (buffer)[i] - 64;
  } else {
    if ((unsigned char)(buffer)[i] < 255) {
      newBuffer[(*indexNewBuffer)++] = (buffer)[i] - 128;
    } else {
      newBuffer[(*indexNewBuffer)++] = '^';
      newBuffer[(*indexNewBuffer)++] = '?';
    }
  }
}

size_t show_sym_sinc_v(char **buffer, size_t size) {
  size_t error = 0;
  char *newBuffer = (char *)calloc(size * 4, sizeof(char));
  if (newBuffer == NULL) {
    perror("Ошибка выделения памяти");
    error = -1;
  } else {
    size_t indexNewBuffer = 0;

    for (size_t i = 0; i < size; i++) {
      if ((iscntrl((*buffer)[i]) || (*buffer)[i] == '\r') &&
          ((*buffer)[i] != '\t' && (*buffer)[i] != '\n')) {
        ascii_chars_low(newBuffer, *buffer, &indexNewBuffer, i);

      } else if ((unsigned char)(*buffer)[i] >= 128) {
        ascii_chars_hight(newBuffer, *buffer, &indexNewBuffer, i);

      } else {
        newBuffer[indexNewBuffer++] = (*buffer)[i];
      }
    }
    error = copy_newbuffer_in_oldbuffer(buffer, newBuffer, indexNewBuffer);
  }

  return error;
}

size_t edition_file(Flags keys, char **buff, size_t count) {
  size_t sizeModifiedBuffer = count;
  if (keys.squeezeBlank == 1) {
    sizeModifiedBuffer = squeeze_blank_s(buff, sizeModifiedBuffer);
  }
  if (keys.showTabs == 1) {
    sizeModifiedBuffer = shouws_tabs_t(buff, sizeModifiedBuffer);
  }
  if (keys.showSymSinc == 1) {
    sizeModifiedBuffer = show_sym_sinc_v(buff, sizeModifiedBuffer);
  }
  if (keys.numberNonBlank == 1) {
    sizeModifiedBuffer = number_non_blank_b(buff, sizeModifiedBuffer);
  }
  if (keys.number == 1 && keys.numberNonBlank == 0) {
    sizeModifiedBuffer = number_n(buff, sizeModifiedBuffer);
  }
  if (keys.symbolsEndLine == 1) {
    sizeModifiedBuffer = symbols_end_line_e(buff, sizeModifiedBuffer);
  }

  return sizeModifiedBuffer;
}
size_t get_file_size(FILE *file) {
  fseek(file, 0, SEEK_END);
  size_t size = ftell(file);
  fseek(file, 0, SEEK_SET);
  return size;
}

size_t buffer_created(char *buff, char **buffer, size_t *bufferTotall) {
  int error = 0;
  size_t sizeFile = 0;
  size_t byteRead = 0;

  FILE *file = fopen(buff, "rb");
  if (file == NULL) {
    perror(buff);
    error = -1;
  } else {
    sizeFile = get_file_size(file);
    *buffer = (char *)realloc(*buffer, *bufferTotall + sizeFile + 2);
    if (buffer == NULL) {
      perror("Ошибка выделения памяти");
      fclose(file);
      error = -1;
    } else {
      byteRead = fread(*buffer + *bufferTotall, 1, sizeFile, file);
      if (byteRead != sizeFile) {
        perror("Ошибка считывания файла\n");
        fclose(file);
        error = -1;
      } else {
        *bufferTotall += byteRead;
        (*buffer)[*bufferTotall] = '\0';
        fclose(file);
      }
    }
  }
  return error;
}

void print_func(int a, char *buff[]) {
  Flags keys = {0, 0, 0, 0, 0, 0};
  char *buffer = NULL;
  size_t bufferTotall = 0;
  int breakWhile = 1;
  int i = 1;
  while (i < a && breakWhile == 1) {
    if (*buff[i] == '-') {
      if ((parsing_arguments(&keys, buff[i])) == 0) {
        ++i;
        continue;
      } else {
        breakWhile = 0;
      }
    } else {
      if ((buffer_created(buff[i], &buffer, &bufferTotall)) == 0) {
        ++i;
        continue;
      }
    }
    ++i;
  }
  if ((keys.number + keys.numberNonBlank + keys.showSymSinc + keys.showTabs +
       keys.squeezeBlank + keys.symbolsEndLine) > 0) {
    bufferTotall = edition_file(keys, &buffer, bufferTotall);
  }
  if (buffer != NULL) {
    fwrite(buffer, 1, bufferTotall, stdout);
    free(buffer);
  }
}

void print_no_arg() {
  char buff[1000];
  while (1) {
    fgets(buff, sizeof(buff), stdin);
    printf("%s", buff);
  }
}