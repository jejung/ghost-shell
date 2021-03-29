#include <stdlib.h>
#include "ghostshstrings.h"

void gstsh_charp_list_init(gstsh_charp_list_t* list)
{
    list->next = NULL;
    list->data = NULL;
}

gstsh_charp_list_t* gstsh_charp_list_new(void)
{
    gstsh_charp_list_t* list = malloc(sizeof(gstsh_charp_list_t));
    gstsh_charp_list_init(list);
    return list;
}

void gstsh_charp_list_free(gstsh_charp_list_t* list)
{
    if (list->next != NULL)
    {
        gstsh_charp_list_free((gstsh_charp_list_t*)list->next);
    }
    free(list);
}