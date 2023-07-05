#pragma once

#define ZJ_DEBUG_FLAG(x) bool dbg_flag = x;

// dubugging macro prints value of x on cerr;
#define ZJ_DEBUG(x)                      \
    do {                                 \
        if (dbg_flag) {                  \
            std::cerr << x << std::endl; \
        }                                \
    } while (0)
