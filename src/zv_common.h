#ifndef _ZV_COMMON_H_
#define _ZV_COMMON_H_

#include <stdlib.h>

#define S_FREE(p) do {  \
    if (p) {            \
        free(p);        \
        p = NULL;       \
    }                   \
}while(0);

#define SKIP_SPACE(p) do { \
}while (p && *(++p) == ' ');

#define SKIP_NEWLINE(p) do { \
}while(p && *(++p) == '\n');

#endif