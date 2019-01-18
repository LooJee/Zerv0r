#ifndef _ZV_COMMON_H_
#define _ZV_COMMON_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define S_FREE(p) do {  \
    if (p) {            \
        free(p);        \
        p = NULL;       \
    }                   \
}while(0);

#define SKIP_CHARACTOR(p, c) do { \
}while (p && *(++p) == c)

#define SKIP_SPACE(p) SKIP_CHARACTOR(p, ' ')

#define SKIP_NEWLINE(p) SKIP_CHARACTOR(p, '\n')

#define SKIP_COLON(p) SKIP_CHARACTOR(p, ':')

#endif