#ifndef __SB_PRINTF_H__
#define __SB_PRINTF_H__

#include <stdarg.h>

#include "types.h"

int sb_printf(const char *fmt, ...);
int vsb_printf(const char *fmt, va_list args);

#define INFO_LOG(fmt, ...) \
	sb_printf("[INFO   ]: "fmt"\n", ##__VA_ARGS__)
#define WARN_LOG(fmt, ...) \
	sb_printf("[WARNING]: "fmt"\n", ##__VA_ARGS__)
#define ERROR_LOG(fmt, ...) \
	sb_printf("[!!ERROR]: "fmt"\n", ##__VA_ARGS__)

#endif /* __SB_PRINTF_H__ */
