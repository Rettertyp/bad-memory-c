#ifndef DEBUG_H
#define DEBUG_H

#ifndef DEBUG_PRINT
#define DEBUG_PRINT 0
#endif

#include <stdio.h>

#if DEBUG_PRINT
#define debug_print(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define debug_print(fmt, ...) ((void)0)
#endif

#endif // DEBUG_H