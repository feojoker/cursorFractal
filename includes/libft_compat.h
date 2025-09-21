#ifndef LIBFT_COMPAT_H
#define LIBFT_COMPAT_H

#include <string.h>
#include <stdlib.h>
#include <stdio.h>

// Simple implementations of needed libft functions
static inline void ft_bzero(void *s, size_t n) {
    memset(s, 0, n);
}

static inline char *ft_strncpy(char *dst, const char *src, size_t len) {
    return strncpy(dst, src, len);
}

static inline void ft_itoa_simple(int num, char *str) {
    sprintf(str, "%d", num);
}

// Alias for compatibility
#define ft_itoa ft_itoa_simple

#endif // LIBFT_COMPAT_H
