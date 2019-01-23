#include <stdio.h>
#include "zv_httpd.h"
#include "zv_resp.h"

int main(int argc, char **argv)
{
    respInit();
    return powerOn();
}
