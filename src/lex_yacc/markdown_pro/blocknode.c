#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "htmltags.h"
#include "blocknode.h"

t_blocknode *node_list = NULL, *node_current = NULL;

/* inner function prototypes */
void pop_higher_level_blocknode(int level);
char *blocknode_glue(t_blocknode *top, t_blocknode *current); /* glue two nodes: top level <= current level */
char *blocknode_close(t_blocknode *node); /* simply close higher-level node */
char *blocknode_output(t_blocknode *node); /* simply output node content without tags */

char *get_open_header(t_blocknode *node);
char *get_close_header(t_blocknode *node);






void pop_higher_level_blocknode(level){
    t_blocknode *item = blocknode_top_stack();

    while( item
        && item->indent_level > level){
        blocknode_close(blocknode_pop_stack());
        /*
        printf("pop tag: %s, level %d\n", get_tag_type(item->tag), item->indent_level);
        */
        item = blocknode_top_stack();
    }
}

char *blocknode_output(t_blocknode *node){
    switch(node->tag){

        case TAG_P:
        case TAG_OL:
        case TAG_UL: 
        case TAG_QUOTE_P:
        case TAG_QUOTE_UL:
        case TAG_QUOTE_OL:
        case TAG_QUOTE_H:
            printf("%s", node->ops[0]); 
            break;

        case TAG_PRE: 
        case TAG_INDENT_PRE:
            printf("%s", html_escape(node->ops[0])); 
            break;

        case TAG_H:
        case TAG_INDENT_P:
        case TAG_INDENT_UL:
        case TAG_INDENT_OL:
            printf("%s", node->ops[1]); 
            break;

        /* to be implemented */
        case TAG_QUOTE_PRE:

        case TAG_BLANK: 
        case TAG_EOF:
        case TAG_ERROR:
        case TAG_QUOTE_BLANK:
            printf(""); 
            break;

    }

    return "";
}


/* simply close higher-level node */
char *blocknode_close(t_blocknode *node) {
    switch(node->tag){

        case TAG_P: printf("</p>"); break;
        case TAG_OL: printf("</li></ol>"); break;
        case TAG_UL:  printf("</li></ul>"); break;
        case TAG_H: printf("</h1>"); break;
        case TAG_QUOTE_P: printf("</p></blockquote>"); break;
        case TAG_QUOTE_UL: printf("</li></ul></blockquote>"); break;
        case TAG_QUOTE_OL: printf("</li></ol></blockquote"); break;
        case TAG_QUOTE_H: printf("%s", str_format("%s</blockquote>", get_close_header(node))); break;

        case TAG_PRE:  printf("</code></pre>"); break;
        case TAG_INDENT_P: printf("</p>"); break;
        case TAG_INDENT_UL: printf("</li></ul>"); break;
        case TAG_INDENT_OL: printf("</li></ol>"); break;
        case TAG_INDENT_PRE: printf("</code></pre>"); break;

        /* to be implemented */
        case TAG_QUOTE_PRE: printf("</code></pre></blockquote>"); break;

        case TAG_BLANK:
        case TAG_EOF:
        case TAG_ERROR:
        case TAG_QUOTE_BLANK:
            printf("");
            break;

    }

    printf("\n");

    return "";
}




t_blocknode *blocknode_create(t_tag tag, int level, int nops, ...){
    va_list args;
    t_blocknode *p;
    int i;

    if( ( p = 
            (t_blocknode *)malloc(sizeof(t_blocknode)) ) 
        == NULL){
        printf("out of memory");
        exit(1);
    }

    if( ( p -> ops = 
            (char **)malloc(nops * sizeof(char *)) ) 
        == NULL){
        printf("out of memory");
        exit(1);
    }

    p->tag = tag; 
    p->indent_level = level; 
    p->nops = nops;
    p->next = NULL;
    va_start(args, nops);
    for(i=0; i<nops; i++){
        p->ops[i] = va_arg(args, char*);
    }
    va_end(args);


    if(!node_list){
        node_list = p;
    }
    if(node_current){
        node_current -> next = p;
    }
    node_current = p;

    return p;
}

char *blocknode_show(t_blocknode *node){
    char *s, *format;

    switch(node -> nops){
        case 1:
            format = "%-*s (%d):\t\t[%s]";
            s = str_format(
                format
                , 15
                , get_tag_type(node->tag)
                , node->indent_level
                , node->ops[0]
            );
            break;
        case 2:
            format = "%-*s (%d):\t\t[%s], [%s]";
            s = str_format(
                format
                , 15
                , get_tag_type(node->tag)
                , node->indent_level
                , node->ops[0]
                , node->ops[1]
            );
            break;
        case 3:
            format = "%-*s (%d):\t\t[%s], [%s], [%s]";
            s = str_format(
                format
                , 15
                , get_tag_type(node->tag)
                , node->indent_level
                , node->ops[0]
                , node->ops[1]
                , node->ops[2]
            );
            break;

        default:
            format = "%-*s (%d)";
            s = str_format(
                format
                , 15
                , get_tag_type(node->tag)
                , node->indent_level
            );
            break;
    }

    printf("blocknode: %s\n", s);

    return s;
}

char* blocknode_parse(t_blocknode *node){
    return "";
}

void blocklist_parse(){
    t_blocknode *p = node_list, *top;

    while(p){

        /* close poped nodes */
        pop_higher_level_blocknode(p->indent_level);


        top = blocknode_top_stack();
        blocknode_glue(top, p);

        /* push current node except NULL, EOF, BLANK and QUOTE_BLANK */
        if(
            TAG_ERROR != p -> tag
            && TAG_EOF != p -> tag
            && TAG_BLANK != p -> tag
            && TAG_QUOTE_BLANK != p -> tag
        ){
            blocknode_push_stack(p);
        }
        /* outpue current node */
        blocknode_output(p);

        /*
        blocknode_show(p);
        */
        p = p->next;
    }

}

/** 
 * glue two nodes: top level <= current level 
 * 1. no push stack tags: TAG_ERROR, TAG_BLANK, TAG_QUOTE_BLANK, TAG_EOF
 */

char *blocknode_glue(t_blocknode *top, t_blocknode *current) { 
    char *glue;

    /** 
     * top level < current level or top is NULL
     * no pop, and simply open current node 
     */
    if(
        !top
        || top -> indent_level < current -> indent_level
    ){


        

        switch(current -> tag){

            case TAG_ERROR:
            case TAG_EOF:
                glue = ""; 
                break;

            case TAG_BLANK:
            case TAG_QUOTE_BLANK:
                glue = "\n";
                break;

            case TAG_P: glue = "<p>"; break;
            case TAG_UL: glue = "<ul><li>"; break;
            case TAG_OL: glue = "<ol><li>"; break;
            case TAG_PRE: glue = "<pre><code>"; break;
            case TAG_H: glue = get_open_header(current); break;

            case TAG_QUOTE_P: glue = "<blockquote><p>"; break;
            case TAG_QUOTE_UL: glue = "<blockquote><ul><li>"; break;
            case TAG_QUOTE_OL: glue = "<blockquote><ol><li>"; break;
            case TAG_QUOTE_PRE: glue = "<blockquote><pre><code>"; break;
            case TAG_QUOTE_H: glue = str_format("<blockquote>%s", get_open_header(current)); break;

            case TAG_INDENT_P: glue = "<p>"; break;
            case TAG_INDENT_UL: glue = "<ul><li>"; break;
            case TAG_INDENT_OL: glue = "<ol><li>"; break;
            case TAG_INDENT_PRE: glue = "<pre><code>"; break;

        }




    } 



    /** 
     * top level == current level
     * when top is processed , pop it 
     */
    else{





        switch(current -> tag){

            /* ignore @error */
            case TAG_ERROR: glue = ""; break;

            /* when eof, the stack should be empty */
            case TAG_EOF: glue = ""; break; 

            case TAG_BLANK: 
                switch(top -> tag){

                    /* tags no push */
                    case TAG_ERROR:
                    case TAG_BLANK:
                    case TAG_QUOTE_BLANK:
                    case TAG_EOF:
                        printf("stack error\n");
                        exit(1);
                        break;

                    /* blank will close p , indent p, h and quoted elements */
                    case TAG_P: glue = "</p>\n"; blocknode_pop_stack(); break;
                    case TAG_H: glue = str_format("%s\n", get_close_header(top)); blocknode_pop_stack(); break; 

                    case TAG_INDENT_P: glue = "</p>\n"; blocknode_pop_stack(); break;

                    case TAG_QUOTE_P: glue = "</p></blockquote>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_UL: glue = "</li</ul></blockquote>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_OL: glue = "</li</ol></blockquote>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_PRE: glue = "</code></pre></blockquote>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_H: glue = str_format("%s</blockquote>", get_close_header(top)); blocknode_pop_stack(); break;

                    /* no effect on tags below */
                    case TAG_UL: 
                    case TAG_OL:

                    case TAG_INDENT_UL:
                    case TAG_INDENT_OL:
                        glue = "";
                        break;

                    /* append newline to PRE and INDENT_PRE */
                    case TAG_INDENT_PRE:
                    case TAG_PRE:
                        glue = "\n";
                        break;

                }
                break;
            
            case TAG_QUOTE_BLANK: 
                switch(top -> tag){

                    /* tags no push */
                    case TAG_ERROR:
                    case TAG_BLANK:
                    case TAG_QUOTE_BLANK:
                    case TAG_EOF:

                    /* higher level indent elements shouldn't exist in stack */
                    case TAG_INDENT_P:
                    case TAG_INDENT_UL:
                    case TAG_INDENT_OL:
                    case TAG_INDENT_PRE:
                        printf("stack error\n");
                        exit(1);
                        break;

                    /* quoted blank will close p , quoted p, and quoted h */
                    case TAG_P: glue = "</p>\n"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_P: glue = "</p></blockquote>\n"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_H: glue = str_format("%s</blockquote>\n", get_close_header(top)); blocknode_pop_stack(); break;

                    /* and has no effects on other quoted elements */
                    case TAG_QUOTE_UL:
                    case TAG_QUOTE_OL:
                    case TAG_QUOTE_PRE:

                    case TAG_UL: 
                    case TAG_OL:
                    case TAG_PRE:
                    case TAG_H: 
                        glue = "";
                        break;

                }
                break;

            case TAG_P: 
                switch(top -> tag){

                    /* tags no push */
                    case TAG_ERROR:
                    case TAG_BLANK:
                    case TAG_QUOTE_BLANK:
                    case TAG_EOF:

                    /* higher level indent elements shouldn't exist in stack */
                    case TAG_INDENT_P:
                    case TAG_INDENT_UL:
                    case TAG_INDENT_OL:
                    case TAG_INDENT_PRE:
                        printf("stack error\n");
                        exit(1);
                        break;

                    /* only append */
                    case TAG_P: glue = "\n"; blocknode_pop_stack(); break;

                    /* close other nodes with the same levels and start p */
                    case TAG_UL: glue = "</li></ul>\n<p>"; blocknode_pop_stack();break;
                    case TAG_OL: glue = "</li></ol>\n<p>"; blocknode_pop_stack();break;
                    case TAG_PRE: glue = "</code></pre>\n<p>"; blocknode_pop_stack();break;
                    case TAG_H: glue = str_format("%s\n<p>", get_close_header(top)); blocknode_pop_stack();break;

                    case TAG_QUOTE_P: glue = "</p></blockquote>\n<p>"; blocknode_pop_stack();break;
                    case TAG_QUOTE_UL: glue = "</li></ul></blockquote>\n<p>"; blocknode_pop_stack();break;
                    case TAG_QUOTE_OL: glue = "</li></ol></blockquote>\n<p>"; blocknode_pop_stack();break;
                    case TAG_QUOTE_PRE: glue = "</code></pre></blockquote>\n<p>"; blocknode_pop_stack();break;
                    case TAG_QUOTE_H: glue = str_format("%s</blockquote>\n<p>", get_close_header(top)); blocknode_pop_stack();break;

                }
                break;

            case TAG_UL: 
                switch(top -> tag){

                    /* tags no push */
                    case TAG_ERROR:
                    case TAG_BLANK:
                    case TAG_QUOTE_BLANK:
                    case TAG_EOF:

                    /* higher level indent elements shouldn't exist in stack */
                    case TAG_INDENT_P:
                    case TAG_INDENT_UL:
                    case TAG_INDENT_OL:
                    case TAG_INDENT_PRE:
                        printf("stack error\n");
                        exit(1);
                        break;

                    /* be the same: close last item and open new item */
                    case TAG_UL: glue = "</li>\n<li>"; blocknode_pop_stack(); break;

                    /* close other nodes with the same levels and open ul */
                    case TAG_P: glue = "</p>\n<ul><li>"; blocknode_pop_stack();  break;
                    case TAG_OL: glue = "</li></ol>\n<ul><li>"; blocknode_pop_stack(); break;
                    case TAG_PRE: glue = "</code></pre>\n<ul><li>"; blocknode_pop_stack(); break;
                    case TAG_H: glue = str_format("%s\n<ul><li>", get_close_header(top)); blocknode_pop_stack(); break;

                    case TAG_QUOTE_P: glue = "</p></blockquote>\n<ul><li>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_UL: glue = "</li></ul></blockquote>\n<ul><li>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_OL: glue = "</li></ol></blockquote>\n<ul><li>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_PRE: glue = "</code></pre></blockquote>\n<ul><li>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_H: glue = str_format("%s</blockquote>\n<ul><li>", get_close_header(top)); blocknode_pop_stack(); break;

                }
                break;

            case TAG_OL: 
                switch(top -> tag){

                    /* tags no push */
                    case TAG_ERROR:
                    case TAG_BLANK:
                    case TAG_QUOTE_BLANK:
                    case TAG_EOF:

                    /* higher level indent elements shouldn't exist in stack */
                    case TAG_INDENT_P:
                    case TAG_INDENT_UL:
                    case TAG_INDENT_OL:
                    case TAG_INDENT_PRE:
                        printf("stack error\n");
                        exit(1);
                        break;

                    /* be the same: close last item and open new item */
                    case TAG_OL: glue = "</li>\n<li>"; blocknode_pop_stack();  break;

                    /* close other nodes with the same levels and open ul */
                    case TAG_P: glue = "</p>\n<ol><li>"; blocknode_pop_stack();  break;
                    case TAG_UL: glue = "</li></ul>\n<ol><li>"; blocknode_pop_stack(); break;
                    case TAG_PRE: glue = "</code></pre>\n<ol><li>"; blocknode_pop_stack(); break;
                    case TAG_H: glue = str_format("%s\n<ol><li>", get_close_header(top)); blocknode_pop_stack(); break;

                    case TAG_QUOTE_P: glue = "</p></blockquote>\n<ol><li>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_UL: glue = "</li></ul></blockquote>\n<ol><li>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_OL: glue = "</li></ol></blockquote>\n<ol><li>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_PRE: glue = "</code></pre></blockquote>\n<ol><li>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_H: glue = str_format("%s</blockquote>\n<ol><li>", get_close_header(top)); blocknode_pop_stack(); break;
                }
                break;

            case TAG_PRE: 
                switch(top -> tag){

                    /* tags no push */
                    case TAG_ERROR:
                    case TAG_BLANK:
                    case TAG_QUOTE_BLANK:
                    case TAG_EOF:

                    /* higher level indent elements shouldn't exist in stack */
                    case TAG_INDENT_P:
                    case TAG_INDENT_UL:
                    case TAG_INDENT_OL:
                    case TAG_INDENT_PRE:
                        printf("stack error\n");
                        exit(1);
                        break;

                    /* be the same: append only */
                    case TAG_PRE: glue = "\n"; blocknode_pop_stack(); break;

                    /* close other nodes with the same levels and open codeblock */
                    case TAG_P: glue = "</p>\n<pre><code>"; blocknode_pop_stack();  break;
                    case TAG_UL: glue = "</li></ul>\n<pre><code>"; blocknode_pop_stack(); break;
                    case TAG_OL: glue = "</li></ol>\n<pre><code>"; blocknode_pop_stack(); break;
                    case TAG_H: glue = str_format("%s\n<pre><code>", get_close_header(top)); blocknode_pop_stack(); break;

                    case TAG_QUOTE_P: glue = "</p></blockquote>\n<pre><code>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_UL: glue = "</li></ul></blockquote>\n<pre><code>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_OL: glue = "</li></ol></blockquote>\n<pre><code>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_PRE: glue = "</code></pre></blockquote>\n<pre><code>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_H: glue = str_format("%s</blockquote>\n<pre><code>", get_close_header(top)); blocknode_pop_stack(); break;
                }
                break;

            case TAG_H: 
                switch(top -> tag){

                    /* tags no push */
                    case TAG_ERROR:
                    case TAG_BLANK:
                    case TAG_QUOTE_BLANK:
                    case TAG_EOF:

                    /* higher level indent elements shouldn't exist in stack */
                    case TAG_INDENT_P:
                    case TAG_INDENT_UL:
                    case TAG_INDENT_OL:
                    case TAG_INDENT_PRE:
                        printf("stack error\n");
                        exit(1);
                        break;

                    /* be the same: append only */
                    case TAG_H: glue = str_format(
                            "%s\n%s"
                            , get_close_header(top)
                            , get_open_header(current)
                        ); 
                        blocknode_pop_stack();
                        break;

                    /* close other nodes with the same levels and open header */
                    case TAG_P: glue = str_format("</p>\n%s", get_open_header(current)); blocknode_pop_stack();  break;
                    case TAG_UL: glue = str_format("</li></ul>\n%s", get_open_header(current)); blocknode_pop_stack(); break;
                    case TAG_OL: glue = str_format("</li></ol>\n%s", get_open_header(current)); blocknode_pop_stack(); break;
                    case TAG_PRE: glue = str_format("</code></pre>\n%s", get_open_header(current)); blocknode_pop_stack(); break;

                    case TAG_QUOTE_P: glue = str_format("</p></blockquote>\n%s", get_open_header(current)); blocknode_pop_stack(); break;
                    case TAG_QUOTE_UL: glue = str_format("</li></ul></blockquote>\n%s", get_open_header(current)); blocknode_pop_stack(); break;
                    case TAG_QUOTE_OL: glue = str_format("</li></ol></blockquote>\n%s", get_open_header(current)); blocknode_pop_stack(); break;
                    case TAG_QUOTE_PRE: glue = str_format("</code></pre></blockquote>\n%s", get_open_header(current)); blocknode_pop_stack(); break;
                    case TAG_QUOTE_H: glue = str_format("%s</blockquote>\n%s", get_close_header(top), get_open_header(current)); blocknode_pop_stack(); break;
                }
                break;

            case TAG_QUOTE_P: 
                switch(top -> tag){

                    /* tags no push */
                    case TAG_ERROR:
                    case TAG_BLANK:
                    case TAG_QUOTE_BLANK:
                    case TAG_EOF:

                    /* higher level indent elements shouldn't exist in stack */
                    case TAG_INDENT_P:
                    case TAG_INDENT_UL:
                    case TAG_INDENT_OL:
                    case TAG_INDENT_PRE:
                        printf("stack error\n");
                        exit(1);
                        break;

                    /* be the same: append only */
                    case TAG_QUOTE_P: glue = ""; blocknode_pop_stack(); break;

                    /* close other nodes with the same levels and open quoted p */
                    case TAG_P: glue = "</p>\n<blockquote><p>"; blocknode_pop_stack();  break;
                    case TAG_UL: glue = "</li></ul>\n<blockquote><p>"; blocknode_pop_stack(); break;
                    case TAG_OL: glue = "</li></ol>\n<blockquote><p>"; blocknode_pop_stack(); break;
                    case TAG_PRE: glue = "</code></pre>\n<blockquote><p>"; blocknode_pop_stack(); break;
                    case TAG_H: glue = str_format("%s\n<blockquote><p>", get_close_header(top)); blocknode_pop_stack(); break;

                    /* close other quoted nodes with the same levels and open p */ 
                    case TAG_QUOTE_UL: glue = "</li></ul>\n<p>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_OL: glue = "</li></ol>\n<p>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_PRE: glue = "</code></pre>\n<p>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_H: glue = str_format("%s\n<p>", get_close_header(top)); blocknode_pop_stack(); break;
                }
                break;

            case TAG_QUOTE_UL: 
                switch(top -> tag){

                    /* tags no push */
                    case TAG_ERROR:
                    case TAG_BLANK:
                    case TAG_QUOTE_BLANK:
                    case TAG_EOF:

                    /* higher level indent elements shouldn't exist in stack */
                    case TAG_INDENT_P:
                    case TAG_INDENT_UL:
                    case TAG_INDENT_OL:
                    case TAG_INDENT_PRE:
                        printf("stack error\n");
                        exit(1);
                        break;

                    /* be the same: close last item and open new item */
                    case TAG_QUOTE_UL: glue = "</li>\n<li>"; blocknode_pop_stack(); break;

                    /* close other nodes with the same levels and open quoted ul */
                    case TAG_P: glue = "</p>\n<blockquote><ul><li>"; blocknode_pop_stack();  break;
                    case TAG_UL: glue = "</li></ul>\n<blockquote><ul><li>"; blocknode_pop_stack(); break;
                    case TAG_OL: glue = "</li></ol>\n<blockquote><ul><li>"; blocknode_pop_stack(); break;
                    case TAG_PRE: glue = "</code></pre>\n<blockquote><ul><li>"; blocknode_pop_stack(); break;
                    case TAG_H: glue = str_format("%s\n<blockquote><ul><li>", get_close_header(top)); blocknode_pop_stack(); break;

                    /* close other quoted nodes with the same levels and open ul */ 
                    case TAG_QUOTE_P: glue = "</p>\n<ul><li>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_OL: glue = "</li></ol>\n<ul><li>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_PRE: glue = "</code></pre>\n<ul><li>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_H: glue = str_format("%s\n<ul><li>", get_close_header(top)); blocknode_pop_stack(); break;
                }
                break;

            case TAG_QUOTE_OL: 
                switch(top -> tag){

                    /* tags no push */
                    case TAG_ERROR:
                    case TAG_BLANK:
                    case TAG_QUOTE_BLANK:
                    case TAG_EOF:

                    /* higher level indent elements shouldn't exist in stack */
                    case TAG_INDENT_P:
                    case TAG_INDENT_UL:
                    case TAG_INDENT_OL:
                    case TAG_INDENT_PRE:
                        printf("stack error\n");
                        exit(1);
                        break;

                    /* be the same: close last item and open new item */
                    case TAG_QUOTE_OL: glue = "</li>\n<li>"; blocknode_pop_stack(); break;

                    /* close other nodes with the same levels and open quoted ul */
                    case TAG_P: glue = "</p>\n<blockquote><ol><li>"; blocknode_pop_stack();  break;
                    case TAG_UL: glue = "</li></ul>\n<blockquote><ol><li>"; blocknode_pop_stack(); break;
                    case TAG_OL: glue = "</li></ol>\n<blockquote><ol><li>"; blocknode_pop_stack(); break;
                    case TAG_PRE: glue = "</code></pre>\n<blockquote><ol><li>"; blocknode_pop_stack(); break;
                    case TAG_H: glue = str_format("%s\n<blockquote><ol><li>", get_close_header(top)); blocknode_pop_stack(); break;

                    /* close other quoted nodes with the same levels and open ul */ 
                    case TAG_QUOTE_P: glue = "</p>\n<ol><li>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_UL: glue = "</li></ul>\n<ol><li>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_PRE: glue = "</code></pre>\n<ol><li>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_H: glue = str_format("%s\n<ol><li>", get_close_header(top)); blocknode_pop_stack(); break;
                }
                break;

            case TAG_QUOTE_PRE: 
                switch(top -> tag){

                    /* tags no push */
                    case TAG_ERROR:
                    case TAG_BLANK:
                    case TAG_QUOTE_BLANK:
                    case TAG_EOF:

                    /* higher level indent elements shouldn't exist in stack */
                    case TAG_INDENT_P:
                    case TAG_INDENT_UL:
                    case TAG_INDENT_OL:
                    case TAG_INDENT_PRE:
                        printf("stack error\n");
                        exit(1);
                        break;

                    /* be the same: append only */
                    case TAG_QUOTE_PRE: glue = "\n"; blocknode_pop_stack(); break;

                    /* close other nodes with the same levels and open quoted ul */
                    case TAG_P: glue = "</p>\n<blockquote><pre><code>"; blocknode_pop_stack();  break;
                    case TAG_UL: glue = "</li></ul>\n<blockquote><pre><code>"; blocknode_pop_stack(); break;
                    case TAG_OL: glue = "</li></ol>\n<blockquote><pre><code>"; blocknode_pop_stack(); break;
                    case TAG_PRE: glue = "</code></pre>\n<blockquote><pre><code>"; blocknode_pop_stack(); break;
                    case TAG_H: glue = str_format("%s\n<blockquote><pre><code>", get_close_header(top)); blocknode_pop_stack(); break;

                    /* close other quoted nodes with the same levels and open ul */ 
                    case TAG_QUOTE_P: glue = "</p>\n<pre><code>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_UL: glue = "</li></ul>\n<pre><code>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_OL: glue = "</li></ol>\n<pre><code>"; blocknode_pop_stack(); break;
                    case TAG_QUOTE_H: glue = str_format("%s\n<pre><code>", get_close_header(top)); blocknode_pop_stack(); break;
                }
                break;

            case TAG_QUOTE_H: 
                switch(top -> tag){

                    /* tags no push */
                    case TAG_ERROR:
                    case TAG_BLANK:
                    case TAG_QUOTE_BLANK:
                    case TAG_EOF:

                    /* higher level indent elements shouldn't exist in stack */
                    case TAG_INDENT_P:
                    case TAG_INDENT_UL:
                    case TAG_INDENT_OL:
                    case TAG_INDENT_PRE:
                        printf("stack error\n");
                        exit(1);
                        break;

                    /* be the same: append only */
                    case TAG_QUOTE_H: glue = str_format("%s\n%s", get_close_header(top), get_open_header(current)); blocknode_pop_stack(); break;

                    /* close other nodes with the same levels and open quoted ul */
                    case TAG_P: glue = str_format("</p>\n<blockquote>%s", get_open_header(current)); blocknode_pop_stack();  break;
                    case TAG_UL: glue = str_format("</li></ul>\n<blockquote>%s", get_open_header(current)); blocknode_pop_stack(); break;
                    case TAG_OL: glue = str_format("</li></ol>\n<blockquote>%s", get_open_header(current)); blocknode_pop_stack(); break;
                    case TAG_PRE: glue = str_format("</code></pre>\n<blockquote>%s", get_open_header(current)); blocknode_pop_stack(); break;
                    case TAG_H: glue = str_format("%s\n<blockquote>%s", get_close_header(top), get_open_header(current)); blocknode_pop_stack(); break;

                    /* close other quoted nodes with the same levels and open ul */ 
                    case TAG_QUOTE_P: glue = str_format("</p>\n%s", get_open_header(current)); blocknode_pop_stack(); break;
                    case TAG_QUOTE_UL: glue = str_format("</li></ul>\n%s", get_open_header(current)); blocknode_pop_stack(); break;
                    case TAG_QUOTE_OL: glue = str_format("</li></ol>\n%s", get_open_header(current)); blocknode_pop_stack(); break;
                    case TAG_QUOTE_PRE: glue = str_format("</code></pre>\n%s", get_open_header(current)); blocknode_pop_stack(); break;
                }
                break;








            case TAG_INDENT_P: 
                switch(top -> tag){

                    /* tags no push */
                    case TAG_ERROR:
                    case TAG_BLANK:
                    case TAG_QUOTE_BLANK:
                    case TAG_EOF:

                    /* lower level nodes except list nodes can not be in stack */
                    case TAG_P:
                    case TAG_H:
                    case TAG_PRE:
                    case TAG_QUOTE_P:
                    case TAG_QUOTE_UL:
                    case TAG_QUOTE_OL:
                    case TAG_QUOTE_PRE:
                    case TAG_QUOTE_H:
                        printf("stack error\n");
                        exit(1);
                        break;

                    /* lower level list nodes: append only */
                    case TAG_OL:
                    case TAG_UL: 
                        glue = "\n<p>"; blocknode_pop_stack(); break;

                    /* the same indent node */
                    case TAG_INDENT_P: glue = "\n"; blocknode_pop_stack(); break;

                    /* other indent nodes with the same level */
                    case TAG_INDENT_UL: glue = "</li></ul>\n<p>";  blocknode_pop_stack(); break; 
                    case TAG_INDENT_OL: glue = "</li></ol>\n<p>"; blocknode_pop_stack(); break;
                    case TAG_INDENT_PRE: glue = "</code></pre>\n<p>"; blocknode_pop_stack(); break;

                }
                break;

            case TAG_INDENT_UL: 
                switch(top -> tag){

                    /* tags no push */
                    case TAG_ERROR:
                    case TAG_BLANK:
                    case TAG_QUOTE_BLANK:
                    case TAG_EOF:

                    /* lower level nodes except list nodes can not be in stack */
                    case TAG_P:
                    case TAG_H:
                    case TAG_PRE:
                    case TAG_QUOTE_P:
                    case TAG_QUOTE_UL:
                    case TAG_QUOTE_OL:
                    case TAG_QUOTE_PRE:
                    case TAG_QUOTE_H:
                        printf("stack error\n");
                        exit(1);
                        break;

                    /* lower level list nodes: append only */
                    case TAG_OL:
                    case TAG_UL: 
                        glue = "\n<ul><li>"; blocknode_pop_stack(); break;

                    /* the same indent node */
                    case TAG_INDENT_UL: glue = "</li>\n<li>"; blocknode_pop_stack(); break;

                    /* other indent nodes with the same level */
                    case TAG_INDENT_P: glue = "</p>\n<ul><li>";  blocknode_pop_stack(); break; 
                    case TAG_INDENT_OL: glue = "</li></ol>\n<ul><li>"; blocknode_pop_stack(); break;
                    case TAG_INDENT_PRE: glue = "</code></pre>\n<ul><li>"; blocknode_pop_stack(); break;

                }
                break;

            case TAG_INDENT_OL: 
                switch(top -> tag){

                    /* tags no push */
                    case TAG_ERROR:
                    case TAG_BLANK:
                    case TAG_QUOTE_BLANK:
                    case TAG_EOF:

                    /* lower level nodes except list nodes can not be in stack */
                    case TAG_P:
                    case TAG_H:
                    case TAG_PRE:
                    case TAG_QUOTE_P:
                    case TAG_QUOTE_UL:
                    case TAG_QUOTE_OL:
                    case TAG_QUOTE_PRE:
                    case TAG_QUOTE_H:
                        printf("stack error\n");
                        exit(1);
                        break;

                    /* lower level list nodes: append only */
                    case TAG_OL:
                    case TAG_UL: 
                        glue = "\n<ol><li>"; blocknode_pop_stack(); break;

                    /* the same indent node */
                    case TAG_INDENT_OL: glue = "</li>\n<li>"; blocknode_pop_stack(); break;

                    /* other indent nodes with the same level */
                    case TAG_INDENT_P: glue = "</p>\n<ol><li>";  blocknode_pop_stack(); break; 
                    case TAG_INDENT_UL: glue = "</li></ul>\n<ol><li>"; blocknode_pop_stack(); break;
                    case TAG_INDENT_PRE: glue = "</code></pre>\n<ol><li>"; blocknode_pop_stack(); break;

                }
                break;

            case TAG_INDENT_PRE: 
                switch(top -> tag){

                    /* tags no push */
                    case TAG_ERROR:
                    case TAG_BLANK:
                    case TAG_QUOTE_BLANK:
                    case TAG_EOF:

                    /* lower level nodes except list nodes can not be in stack */
                    case TAG_P:
                    case TAG_H:
                    case TAG_PRE:
                    case TAG_QUOTE_P:
                    case TAG_QUOTE_UL:
                    case TAG_QUOTE_OL:
                    case TAG_QUOTE_PRE:
                    case TAG_QUOTE_H:
                        printf("stack error\n");
                        exit(1);
                        break;

                    /* lower level list nodes: append only */
                    case TAG_OL:
                    case TAG_UL: 
                        glue = "\n<pre><code>"; blocknode_pop_stack(); break;

                    /* the same indent node */
                    case TAG_INDENT_PRE: glue = "\n"; blocknode_pop_stack(); break;

                    /* other indent nodes with the same level */
                    case TAG_INDENT_P: glue = "</p>\n<pre><code>";  blocknode_pop_stack(); break; 
                    case TAG_INDENT_UL: glue = "</li></ul>\n<pre><code>"; blocknode_pop_stack(); break;
                    case TAG_INDENT_OL: glue = "</li></ol>\n<pre><code>"; blocknode_pop_stack(); break;

                }
                break;

        }




    }

    printf("%s", glue);
    return glue;
}


char *get_open_header(t_blocknode *node){
    if(node -> tag != TAG_H
        && node -> tag != TAG_QUOTE_H){
        return ""; 
    }

    return str_format("<h%d>", strlen(node -> ops[0]));
}

char *get_close_header(t_blocknode *node){
    if(node -> tag != TAG_H
        && node -> tag != TAG_QUOTE_H){
        return ""; 
    }

    return str_format("</h%d>", strlen(node -> ops[0]));
}


