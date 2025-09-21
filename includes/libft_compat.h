#ifndef LIBFT_COMPAT_H
#define LIBFT_COMPAT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

// Simple implementations of needed libft functions
static inline void ft_bzero(void *s, size_t n) { memset(s, 0, n); }

static inline char *ft_strncpy(char *dst, const char *src, size_t len) {
  return strncpy(dst, src, len);
}

static inline void ft_itoa_simple(int num, char *str) {
  sprintf(str, "%d", num);
}

// Alias for compatibility
#define ft_itoa ft_itoa_simple

// Simple get_next_line implementation
static inline int get_next_line(int fd, char **line) {
  static char buffer[1024];
  static int buffer_pos = 0;
  static int bytes_read = 0;
  char *result = NULL;
  int result_len = 0;
  int capacity = 256;

  if (!line)
    return -1;

  result = malloc(capacity);
  if (!result)
    return -1;

  while (1) {
    if (buffer_pos >= bytes_read) {
      bytes_read = read(fd, buffer, sizeof(buffer));
      if (bytes_read <= 0) {
        if (result_len == 0) {
          free(result);
          return 0;
        }
        break;
      }
      buffer_pos = 0;
    }

    char c = buffer[buffer_pos++];
    if (c == '\n')
      break;

    if (result_len >= capacity - 1) {
      capacity *= 2;
      result = realloc(result, capacity);
      if (!result)
        return -1;
    }

    result[result_len++] = c;
  }

  result[result_len] = '\0';
  *line = result;
  return 1;
}

#endif // LIBFT_COMPAT_H
