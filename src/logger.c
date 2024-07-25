#include "logger.h"
#include <stdio.h>
#include <stdarg.h>
#include <time.h>

static const char *LOG_LEVELS[] = {
    "DEBUG",
    "INFO",
    "WARNING",
    "ERROR"
};

static const char *LOG_COLORS[] = {
    "\033[0;32m", // Green
    "\033[0;37m", // White
    "\033[0;33m", // Yellow
    "\033[0;31m" // Red
};

#define LOG_RESET_COLOR "\033[0m"
#define LOG_BOLD "\033[1m"


static unsigned char log_level = LOG_DEBUG;

// set log file as stdout
static FILE *log_file = NULL;

void set_log_file(FILE *file) {
    log_file = file;
}

void set_log_level(unsigned char level) {
    log_level = level;
}

void vprint(unsigned char level, const char *fmt, va_list args) {
    // Print time
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    FILE *_log_file = log_file ? log_file : stdout;
    fprintf(_log_file, "[%02d:%02d:%02d] ", t->tm_hour, t->tm_min, t->tm_sec);

    // Print log level in bold with color
    fprintf(_log_file, "%s%s%s: %s", LOG_BOLD, LOG_COLORS[level], LOG_LEVELS[level], LOG_RESET_COLOR);

    // Print log message with color
    fprintf(_log_file, "%s", LOG_COLORS[level]);
    vfprintf(_log_file, fmt, args);
    fprintf(_log_file, "%s\n", LOG_RESET_COLOR);
}

void print(unsigned char level, const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprint(level, fmt, args);
    va_end(args);
}

void debug(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprint(LOG_DEBUG, fmt, args);
    va_end(args);
}

void info(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprint(LOG_INFO, fmt, args);
    va_end(args);
}

void warn(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprint(LOG_WARN, fmt, args);
    va_end(args);
}

void error(const char *fmt, ...) {
    va_list args;
    va_start(args, fmt);
    vprint(LOG_ERROR, fmt, args);
    va_end(args);
}