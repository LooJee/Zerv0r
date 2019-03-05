#ifndef _ZV_COMMON_H_
#define _ZV_COMMON_H_

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <ctype.h>
#include <stdint.h>

#define CRLF    "\r\n"
#define STR_EOF '\0'
#define SPC     ' '
#define CR      '\r'
#define LF      '\n'
#define SLASH   '/'
#define DOT     '.'
#define COLON   ':'

#define ZV_S_FREE(p) do {  \
    if (p) {            \
        free(p);        \
        p = NULL;       \
    }                   \
}while(0);

#define SKIP_CHARACTOR(p, c) while((p != NULL) && *(p) != c) {  \
    ++p;                                                \
};

#define SKIP_SPACE(p) SKIP_CHARACTOR(p, ' ')
#define SKIP_NEWLINE(p) SKIP_CHARACTOR(p, '\n')
#define SKIP_COLON(p) SKIP_CHARACTOR(p, ':')

typedef enum {
    ZV_200 = 200,
    ZV_404 = 404,
}ZV_HTTP_CODE_E;

#endif