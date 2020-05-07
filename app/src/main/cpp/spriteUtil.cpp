//
// Created by Nemo li on 2020/5/6.
//

#include <sys/time.h>
#include "spriteUtil.h"


long long GetCurrentTimeMillis() {
    timeval t;
    gettimeofday(&t, NULL);

    long long ret = t.tv_sec * 1000 + t.tv_usec / 1000;
    return ret;
}
