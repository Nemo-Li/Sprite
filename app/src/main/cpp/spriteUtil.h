//
// Created by Nemo li on 2020/5/6.
//

#ifndef SPRITE_SPRITEUTIL_H
#define SPRITE_SPRITEUTIL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#define ARRAY_SIZE_IN_ELEMENTS(a) (sizeof(a)/sizeof(a[0]))
#define SNPRINTF snprintf
#define INVALID_UNIFORM_LOCATION 0xffffffff

long long GetCurrentTimeMillis();

#endif //SPRITE_SPRITEUTIL_H
