#include "tagstack.h"

typedef struct blocknode {
    t_tag tag;
    int nops;
    char **ops;
    struct blocknode *next;
} t_blocknode;

t_blocknode *blocknode_create(t_tag tag, int nops, ...);
char* blocknode_parse(t_blocknode *node);
void blocklist_parse();

