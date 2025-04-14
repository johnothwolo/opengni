//
//  helper_macros.h
//  gni
//
//  Created by John Othwolo on 4/8/25.
//

#ifndef helper_macros_h
#define helper_macros_h

#include <sys/cdefs.h>

#ifdef __APPLE__
#if defined(__GNUC__)
#define    __GNUC_PREREQ__(ma, mi)    \
    (__GNUC__ > (ma) || __GNUC__ == (ma) && __GNUC_MINOR__ >= (mi))
#else
#define    __GNUC_PREREQ__(ma, mi)    0
#endif // __GNUC__

#if __GNUC_PREREQ__(2, 96)
#define    __predict_true(exp)     __builtin_expect((exp), 1)
#define    __predict_false(exp)    __builtin_expect((exp), 0)
#else
#define    __predict_true(exp)     (exp)
#define    __predict_false(exp)    (exp)
#endif
#endif // __GNUC_PREREQ__

#define __Require_Msg(cond, TAG, message...) ({\
    if (__predict_false(!(cond))) { gni::common::LogE(TAG, message); abort(); }\
})


#endif /* helper_macros_h */
