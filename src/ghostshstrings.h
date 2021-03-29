#ifndef GHOST_SHELL_GHOSTSHSTRINGS_H
#define GHOST_SHELL_GHOSTSHSTRINGS_H

typedef struct gstsh_charp_list_s {
    char *data;
    struct gstsh_charp_list_s *next;
} gstsh_charp_list_t;

gstsh_charp_list_t* gstsh_charp_list_new(void);
void gstsh_charp_list_init(gstsh_charp_list_t* list);
void gstsh_charp_list_free(gstsh_charp_list_t* list);

#endif //GHOST_SHELL_GHOSTSHSTRINGS_H
