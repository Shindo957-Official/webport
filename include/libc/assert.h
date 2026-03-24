#ifndef ASSERT_H
#define ASSERT_H

/* Minimal assert for N64 freestanding build */
#ifdef NDEBUG
#define assert(x) ((void)0)
#else
extern void __assert_fail(const char *expr, const char *file, int line);
#define assert(x) ((x) ? (void)0 : __assert_fail(#x, __FILE__, __LINE__))
#endif

#endif /* ASSERT_H */
