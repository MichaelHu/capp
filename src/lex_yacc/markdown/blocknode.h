#include "tagstack.h"

typedef struct blocknode {
    t_tag tag;
    int indent_level;
    int nops;
    char **ops;
    struct blocknode *next;
} t_blocknode;

t_blocknode *blocknode_create(t_tag tag, int level, int nops, ...);
char* blocknode_show(t_blocknode *node);
char* blocknode_parse(t_blocknode *node);
void blocklist_parse();

