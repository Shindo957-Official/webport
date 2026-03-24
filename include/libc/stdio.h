#ifndef STDIO_H
#define STDIO_H

/* Minimal stdio for N64 freestanding build */
typedef struct FILE FILE;

extern int sprintf(char *s, const char *fmt, ...);
extern int snprintf(char *s, __SIZE_TYPE__ n, const char *fmt, ...);

#endif
