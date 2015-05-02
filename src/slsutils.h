
/**
 * //
 * // Created by Steven on 4/25/15.
 * //
 * 
 * @file
 * @brief general purpose C macros.
 * @detail error checking macros inspired by
 * Zed's awsome debug macros
 * http://c.learncodethehardway.org/book/ex20.html
 */




#ifndef DANGERENGINE_SLSUTILS_H
#define DANGERENGINE_SLSUTILS_H

#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <stdbool.h>


#ifdef WIN32
#undef __func__
#define __func__ __FUNCTION__  
#endif


#ifdef SLS_USE_COLORS
/**
 * @brief ascii color code literals
 */
#define SLS_COLOR_DEFAULT  "\x1B[0m"
#define SLS_COLOR_RED       "\x1B[31m"
#define SLS_COLOR_GRN       "\x1B[32m"
#define SLS_COLOR_YEL       "\x1B[33m"
#define SLS_COLOR_BLU       "\x1B[34m"
#define SLS_COLOR_MAG       "\x1B[35m"
#define SLS_COLOR_CYN       "\x1B[36m"
#define SLS_COLOR_WHT       "\x1B[37m"

#else

#define SLS_COLOR_DEFAULT   ""
#define SLS_COLOR_RED       ""
#define SLS_COLOR_GRN       ""
#define SLS_COLOR_YEL       ""
#define SLS_COLOR_BLU       ""
#define SLS_COLOR_MAG       ""
#define SLS_COLOR_CYN       ""
#define SLS_COLOR_WHT       ""

#endif


/**
 * @brief prints a debug log to given file
 * @details formats the message and prints function name and
 * line number for debugging purposes
 * 
 * @param file file pointer
 * @param msg1 header message
 * @param fmt printf-style format
 * @param ... format arguments
 */
#define sls_debug(file, msg1, fmt, ...) do { \
    fprintf((file), "%s (%s %d):\n\t", (msg1), __func__, __LINE__);  \
    fprintf((file), fmt, ##__VA_ARGS__);    \
    fprintf((file), "\n");  \
} while(0)


#define sls_log_info(fmt, ...) sls_debug(stderr, "LOG", fmt, ##__VA_ARGS__)
#define sls_log_warn(fmt, ...) sls_debug(stderr, "WARNING", fmt, ##__VA_ARGS__)
#define sls_log_err(fmt, ...) sls_debug(stderr, "ERROR", fmt, ##__VA_ARGS__)

#define sls_log_fatal(fmt, ...) do{ \
    sls_debug(stderr, "FATAL ERROR", fmt, ##__VA_ARGS__);    \
    exit(EXIT_FAILURE);  \
} while (0)


#define sls_check(cond, msg, ...) do { \
    if (!(cond)) { \
        sls_log_err(msg, ##__VA_ARGS__);    \
        goto error;                         \
    }   \
} while (0)

#define sls_checkmem(pointer) sls_check((pointer), "memory error! %s", #pointer)

#define sls_fail() sls_check(0, "reached fail location %s %d", __FILE__, __LINE__);

#define sls_msg(obj, method, ...) obj->method(obj, ##__VA_ARGS__)

typedef enum slsBool {
    SLS_TRUE=true,
    SLS_FALSE=false
} slsBool;

/**
 * @brief allocates 'size' bytes on the heap, and copies contents to
 * pointer
 * @details essentially a heap allocated memcpy, this facilitates
 * struct object construction
 * 
 * @param prototype [description]
 * @param size [description]
 * 
 * @return [description]
 */
void *sls_objalloc(void const *prototype, size_t size);

void sls_sleep(clock_t ticks);

#endif //DANGERENGINE_SLSUTILS_H
