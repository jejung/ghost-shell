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

gs_charp_list_t* gs_charp_list_append(gs_charp_list_t* list, char* pointer)
{
    if (list->data == NULL)
    {
        list->data = pointer;
        return list;
    }

    gs_charp_list_t* newnode = gs_charp_list_new();
    newnode->data = pointer;

    while (list->next != NULL)
    {
        list = list->next;
    }

    list->next = newnode;

    return newnode;
}

void gs_charp_list_free(gs_charp_list_t* list)
{
    if (list == NULL)
        return;
    gs_charp_list_free((gs_charp_list_t*)list->next);
    free(list);
}
