#include "sb_printf.h"
#include "sb_uart.h"
#include "vsprintf.h"

/* Ideas stolen from Wii's mini... Thanks guys! */
/* Need to split into sendstringbuffer and sendbuffer */
static int sb_sendbuffer(const void *buffer, u32 length)
{
    u32   left = length;
    char *ptr  = (char *)buffer;

    while (left > 0 && *ptr) 
    {
        sb_putc(*ptr);
        if (*ptr == '\n')
        {
#ifdef SB_CRLF
            sb_putc('\r');
#endif
        }
        ptr++;
        left--;
    }
    return (length - left);
}

int sb_printf(const char *fmt, ...)
{
    va_list args;
    int ret;

    va_start(args, fmt);
    ret = vsb_printf(fmt, args);
    va_end(args);

    return ret;
}

#define SB_PRINTF_BUFFER_LEN 4096

int vsb_printf(const char *fmt, va_list args)
{
    char buffer[SB_PRINTF_BUFFER_LEN];
    int i;

    buffer[0] = '\0';

    i = vsnprintf(buffer, (size_t)sizeof(buffer), fmt, args);

    return sb_sendbuffer(buffer, i);
}

