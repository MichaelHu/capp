
char *str_format(char *format, ...);
char *str_concat(char *s1, char *s2);
char *str_padding_left(char *s, int count);

char *create_image(char *title, char *href);
char *create_link(char *title, char *href);
char *create_hn(char *s, int level);
char *create_emphasis(char *s);
char *create_strong(char *s);
char *create_codespan(char *s);

char *html_escape(char *s);


