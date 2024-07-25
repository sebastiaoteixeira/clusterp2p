#ifndef LOGGER_H
#define LOGGER_H

#include <stdio.h>

// Log levels
#define LOG_DEBUG 0
#define LOG_INFO 1
#define LOG_WARN 2
#define LOG_ERROR 3

void vprint(unsigned char level, const char *fmt, va_list args);
void print(unsigned char level, const char *fmt, ...);

void debug(const char *fmt, ...);
void info(const char *fmt, ...);
void warn(const char *fmt, ...);
void error(const char *fmt, ...);

void set_log_level(unsigned char level);

void set_log_file(FILE *file);

#endif // LOGGER_H