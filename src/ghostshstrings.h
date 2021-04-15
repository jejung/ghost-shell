#ifndef GHOST_SHELL_GHOSTSHSTRINGS_H
#define GHOST_SHELL_GHOSTSHSTRINGS_H

typedef struct gs_charp_list_s {
    char *data;
    struct gs_charp_list_s *next;
} gs_charp_list_t;

gs_charp_list_t* gs_charp_list_new(void);
void gs_charp_list_init(gs_charp_list_t* list);
void gs_charp_list_free(gs_charp_list_t* list);

#endif //GHOST_SHELL_GHOSTSHSTRINGS_H
