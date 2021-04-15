#include <stdlib.h>
#include "ghostshstrings.h"

void gs_charp_list_init(gs_charp_list_t* list)
{
    list->next = NULL;
    list->data = NULL;
}

gs_charp_list_t* gs_charp_list_new(void)
{
    gs_charp_list_t* list = malloc(sizeof(gs_charp_list_t));
    gs_charp_list_init(list);
    return list;
}

void gs_charp_list_free(gs_charp_list_t* list)
{
    if (list->next != NULL)
    {
        gs_charp_list_free((gs_charp_list_t*)list->next);
    }
    free(list);
}