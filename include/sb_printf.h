#ifndef __SB_PRINTF_H__
#define __SB_PRINTF_H__

#include <stdarg.h>

#include "types.h"

int sb_printf(const char *fmt, ...);
int vsb_printf(const char *fmt, va_list args);

#define INFO_LOG(...) \
        sb_printf("[INFO   ]:"); \
        sb_printf(__VA_ARGS__); \
        sb_printf("\n")

#define WARN_LOG(...) \
        sb_printf("[WARNING]:"); \
	sb_printf(__VA_ARGS__); \
        sb_printf("\n")

#define ERROR_LOG(...) \
        sb_printf("[!!ERROR]:"); \
	sb_printf(__VA_ARGS__); \
        sb_printf("\n")

#endif /* __SB_PRINTF_H__ */
