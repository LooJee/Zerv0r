typedef struct {
    char method[16];
    char url[128];
    char version[16];
    char Host[64];
    char Connection[64];
    char CacheControl[64];
}REQ_S;