#ifndef DEBUG_H
#define DEBUG_H

#define DEBUG_PRINT 1

#if DEBUG_PRINT
#define debug_print(fmt, ...) printf(fmt, ##__VA_ARGS__)
#else
#define debug_print(fmt, ...) ((void)0)
#endif

#endif // DEBUG_H