#include <stdio.h>
#include <stdarg.h>
#include "bsp_log.h"

extern uint32_t Sys_Clock;

void stm_log_write(stm_log_level_t level, 
									const char* tag, 
									const char* format, ...)
{
  va_list args;

  va_start (args, format);
  vprintf (format, args);
  va_end (args);
}

uint32_t stm_log_timestamp(void)
{
	return Sys_Clock;
}