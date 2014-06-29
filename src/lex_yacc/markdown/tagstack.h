
typedef enum {TAG_NULL, TAG_P, TAG_UL, TAG_OL, TAG_PRE, TAG_OTHER} t_tag; 
typedef struct { 
    int size;
    t_tag arr[10];
} t_tag_stack;

char* tag_check_stack(t_tag tag);
void tag_init_stack();
t_tag tag_top_stack();
int tag_empty_stack();
int tag_full_stack();
void tag_push_stack(t_tag tag);
t_tag tag_pop_stack();


