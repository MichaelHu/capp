#include "tagstack.h"

typedef struct {
    t_tag tag;
    int nops;
    char **ops;
} t_blocknode;

t_blocknode *blocknode_create(t_tag tag, int nops, ...);
char* blocknode_parse(t_blocknode *node);

