/**
 * @file ${FILE}
 *
 * @brief General purpose C macros.
 * @detail Error checking macros inspired by
 * Zed's awsome debug macros
 * http://c.learncodethehardway.org/book/ex20.html
 * @license ${LICENSE}
 * Copyright (c) 1/6/16, Steven
 * 
 **/
#ifndef DANGERENGINE_SLSMACROS_H
#define DANGERENGINE_SLSMACROS_H

#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <time.h>
#include <assert.h>
#include <signal.h>

/**
 * @brief token concentrator macro
 */
#define SLS_CONCAT(x, y) x ## y


/**
 * @brief attribute wrappers for GCC & clang
 */
#ifndef _MSC_VER
/**
 * @brief Tags a function as non-null
 * @param (param, ...) _indices of non-null parameters. For some reason,
 * gcc's nonnull extension uses 1-indexing, so that's that.
 */
#   define SLS_NONNULL(param, ...) __attribute__((nonnull(param, ##__VA_ARGS__)))
#   define SLS_DEPRECIATED __attribute__((deprecated))
#   define SLS_PURE __attribute__((pure))
#   define SLS_CONSTFN __attribute__((const))
#   define SLS_CLEANUP(cleanup_fn) __attribute__((cleanup_fn))

/**
 * @brief flags that GNU attribute extensions are availible, i.e.
 * source can rely on SLS_NONNULL for nullability checks
 **/
#   define SLS_GNU_EXT 1

#else
#   define SLS_NONNULL(param, ...)
#   define SLS_DEPRECIATED
#   define SLS_PURE
#   define SLS_CONSTFN
#   define SLS_CLEANUP(cleanup_fn)
#endif //!_MSC_VER

#define SLS_USE_COLORS 1
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

/**
 * @brief prints a log message to stderr using the given printf format
 * @detail, log, warn, and err macros provide different levels of urgency
 */
#define sls_log_info(fmt, ...)  \
  sls_debug(stderr, SLS_COLOR_GRN "LOG" SLS_COLOR_DEFAULT, fmt, ##__VA_ARGS__)
#define sls_log_warn(fmt, ...)  \
   sls_debug(stderr, SLS_COLOR_YEL "WARNING" SLS_COLOR_DEFAULT, fmt, ##__VA_ARGS__)
#define sls_log_err(fmt, ...)   \
  sls_debug(stderr, SLS_COLOR_RED "ERROR" SLS_COLOR_DEFAULT, fmt, ##__VA_ARGS__)


#if defined(__cplusplus)
#   define SLS_STATIC_ASSERT(cond, msg) static_assert(cond, msg)
#elif !(defined(__cplusplus)) && defined(__GNUC__)
#   define SLS_STATIC_ASSERT(cond, msg) _Static_assert(cond, msg)
#else
#   define SLS_STATIC_ASSERT(cond, msg)  \
      typedef char __sls_c_assert_##msg[(cond)?1:-1]
#endif



/**
 * @brief Logs a message, throws an assertion failure, and exits with a failure
 * @detail Used to specify a fatal error
 */
#define sls_log_fatal(fmt, ...) do{ \
  sls_debug(stderr, "FATAL ERROR", fmt, ##__VA_ARGS__);    \
  assert(0);  \
  exit(EXIT_FAILURE);  \
} while (0)

/**
 * @brief A pseudo exception check macro for handling runtime errors
 * @detail takes a condition, if the condition fails, it calls a 'goto' to
 * the `error:` label, which must exist in the function
 */
#define sls_check(cond, msg, ...) do { \
  if (!(cond)) { \
    sls_log_err((msg), ##__VA_ARGS__);    \
    goto error;                         \
  }   \
} while (0)


#define sls_checkmem(pointer) do {  \
  sls_check((pointer), "memory error! %s", #pointer); \
} while (0)

#define sls_fail() sls_check(0, "reached fail location %s %d", __FILE__, __LINE__);

#define sls_msg(obj, method, ...) (obj)->method((obj), ##__VA_ARGS__)


/**
 * @brief checks for null parameters at runtime if GNU nonnull extension is not available.
 * @detail expands to nothing if the extension exists, should be used in conjunction with
 * __attribute__((nonnull(...)) or SLS_NONNULL(...)
 */

#if __has_attribute(nonnull)

#define sls_nonnull_param(p)

#else

#define sls_nonnull_param(p) sls_check(p, "parameter %p is null!", p);


#endif

//---------------------------------degugging helpers---------------------------------------


#ifndef _MSC_VER
/**
 * @brief Raises an appropriate signal to break execution in a debugger
 * @detail Because Windows apparently does not respond to SIGNINT, the macro raises
 * SIGABRT to break execution. To allow continued execution, call `signal 0` or a corresponding
 * debugger command.
 */
#   define SLS_DEBUGGER_BREAKPT() raise(SIGINT)
#else
#   define SLS_DEBUGGER_BREAKPT() raise(SIGABRT)
#endif

//---------------------------------C/C++ header guard---------------------------------------
#ifndef __cplusplus
#   define SLS_BEGIN_CDECLS extern "C" {
#   define SLS_END_CDECLS }
#else
#   define SLS_BEGIN_CDECLS
#   define SLS_END_CDECLS
#endif

#endif //DANGERENGINE_SLSMACROS_H
