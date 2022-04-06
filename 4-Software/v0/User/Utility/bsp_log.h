#ifndef __BSP_LOG_H
#define __BSP_LOG_H

#include "stm32f10x.h"

#if CONFIG_LOG_COLORS
#define LOG_COLOR_BLACK   "30"
#define LOG_COLOR_RED     "31"
#define LOG_COLOR_GREEN   "32"
#define LOG_COLOR_BROWN   "33"
#define LOG_COLOR_BLUE    "34"
#define LOG_COLOR_PURPLE  "35"
#define LOG_COLOR_CYAN    "36"
#define LOG_COLOR(COLOR)  "\033[0;" COLOR "m"
#define LOG_BOLD(COLOR)   "\033[1;" COLOR "m"
#define LOG_RESET_COLOR   "\033[0m"
#define LOG_COLOR_E       LOG_COLOR(LOG_COLOR_RED)
#define LOG_COLOR_W       LOG_COLOR(LOG_COLOR_BROWN)
#define LOG_COLOR_I       LOG_COLOR(LOG_COLOR_GREEN)
#define LOG_COLOR_D
#define LOG_COLOR_V
#else //CONFIG_LOG_COLORS
#define LOG_COLOR_E
#define LOG_COLOR_W
#define LOG_COLOR_I
#define LOG_COLOR_D
#define LOG_COLOR_V
#define LOG_RESET_COLOR
#endif //CONFIG_LOG_COLORS


typedef enum {
    STM_LOG_NONE,       /*!< No log output */
    STM_LOG_ERROR,      /*!< Critical errors, software module can not recover on its own */
    STM_LOG_WARN,       /*!< Error conditions from which recovery measures have been taken */
    STM_LOG_INFO,       /*!< Information messages which describe normal flow of events */
    STM_LOG_DEBUG,      /*!< Extra information which is not necessary for normal use (values, pointers, sizes, etc). */
    STM_LOG_VERBOSE     /*!< Bigger chunks of debugging information, or frequent messages which can potentially flood the output. */
} stm_log_level_t;

uint32_t stm_log_timestamp(void);
void stm_log_write(stm_log_level_t level, const char *tag, const char *format, ...)__attribute__ ((format(printf, 3, 4)));;

#define LOG_FORMAT(letter, format)  LOG_COLOR_ ## letter #letter " (%d) %s: " format LOG_RESET_COLOR "\n"

#define STM_LOGE(tag, format, ...) {stm_log_write(STM_LOG_ERROR, 	tag, LOG_FORMAT(E, format), stm_log_timestamp(), tag, ##__VA_ARGS__);}

#define STM_LOGW(tag, format, ...) {stm_log_write(STM_LOG_WARN, 	tag, LOG_FORMAT(W, format), stm_log_timestamp(), tag, ##__VA_ARGS__);}

#define STM_LOGI(tag, format, ...) {stm_log_write(STM_LOG_INFO, 	tag, LOG_FORMAT(I, format), stm_log_timestamp(), tag, ##__VA_ARGS__);}

#define STM_LOGD(tag, format, ...) {stm_log_write(STM_LOG_DEBUG, 	tag, LOG_FORMAT(D, format), stm_log_timestamp(), tag, ##__VA_ARGS__);}


#endif 
