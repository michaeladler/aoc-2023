/**
 * Copyright (c) 2017 rxi
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the MIT license. See `log.c` for details.
 */

#ifndef LOG_H
#define LOG_H

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdarg.h>

#define LOG_VERSION "0.2.0"

typedef void (*log_LockFn)(void *udata, int lock);

enum { LOG_TRACE, LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL };

#define STRIPPATH(s)\
    (sizeof(s) > 2 && (s)[sizeof(s)-2] == '/' ? (s) + sizeof(s) - 1 : \
    sizeof(s) > 3 && (s)[sizeof(s)-3] == '/' ? (s) + sizeof(s) - 2 : \
    sizeof(s) > 4 && (s)[sizeof(s)-4] == '/' ? (s) + sizeof(s) - 3 : \
    sizeof(s) > 5 && (s)[sizeof(s)-5] == '/' ? (s) + sizeof(s) - 4 : \
    sizeof(s) > 6 && (s)[sizeof(s)-6] == '/' ? (s) + sizeof(s) - 5 : \
    sizeof(s) > 7 && (s)[sizeof(s)-7] == '/' ? (s) + sizeof(s) - 6 : \
    sizeof(s) > 8 && (s)[sizeof(s)-8] == '/' ? (s) + sizeof(s) - 7 : \
    sizeof(s) > 9 && (s)[sizeof(s)-9] == '/' ? (s) + sizeof(s) - 8 : \
    sizeof(s) > 10 && (s)[sizeof(s)-10] == '/' ? (s) + sizeof(s) - 9 : \
    sizeof(s) > 11 && (s)[sizeof(s)-11] == '/' ? (s) + sizeof(s) - 10 : (s))

#define __JUSTFILE__ STRIPPATH(__FILE__)

#if defined(NDEBUG) || defined(RELEASE)
#define log_trace(...) do { } while(0)
#define log_debug(...) do { } while(0)
#else
#define log_trace(...) log_log(LOG_TRACE, 0, __JUSTFILE__, __LINE__, __VA_ARGS__)
#define log_debug(...) log_log(LOG_DEBUG, 0, __JUSTFILE__, __LINE__, __VA_ARGS__)
#endif
#define log_info(...)  log_log(LOG_INFO,  0, __JUSTFILE__, __LINE__, __VA_ARGS__)
#define log_warn(...)  log_log(LOG_WARN,  0, __JUSTFILE__, __LINE__, __VA_ARGS__)
#define log_error(...) log_log(LOG_ERROR, 0, __JUSTFILE__, __LINE__, __VA_ARGS__)
#define log_perror(...) log_log(LOG_ERROR, 1, __JUSTFILE__, __LINE__, __VA_ARGS__)
#define log_fatal(...) log_log(LOG_FATAL, 0, __JUSTFILE__, __LINE__, __VA_ARGS__)

void log_set_udata(void *udata);
void log_set_lock(log_LockFn fn);
void log_set_fp(FILE *fp);
void log_set_level(int level);
void log_set_file_level(int level);
void log_set_quiet(int enable);

void log_log(int level, const int do_perror, const char *file, int line, const char *fmt, ...);

#define LOGT log_trace
#define LOGD log_debug
#define LOGI log_info
#define LOGW log_warn
#define LOGE log_error
#define LOGP log_perror
#define LOGF log_fatal

#ifdef __cplusplus
}
#endif

#endif
