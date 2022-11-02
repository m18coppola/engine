#include "util.h"

char *
utl_file_to_string(char *filepath)
{
    SDL_RWops *io;
    char *source;
    size_t filesize;

    io = SDL_RWFromFile(filepath, "r");
    if (io == NULL) {
        fprintf(
                stderr,
                "UTL: Failed to open file \"%s\". Exiting.\nSDL_Error: %s\n",
                filepath,
                SDL_GetError());
                return NULL;
    }
    filesize = SDL_RWsize(io);
    source = malloc(filesize + 1);
    if (SDL_RWread(io, source, filesize, 1) == 0) {
        fprintf(
                stderr,
                "UTL: Failed to read file \"%s\". Exiting.\nSDL_Error: %s\n",
                filepath,
                SDL_GetError());
                return NULL;
    }
    source[filesize] = '\0';
    SDL_RWclose(io);
    return source;
}


char **
utl_tokenize(char *str)
{
    char **token_list;
    struct utl_Token *head;
    struct utl_Token *ptr;
    head = malloc(sizeof(struct utl_Token));
    head->string = str;
    head->next = NULL;
    ptr = head;
    int i;
    int in_token = 0;
    int token_count = 0;
    
    while (*str != '\0') {
        if (*str <= ' ') {
            if (in_token) {
                *str = '\0';
                in_token = 0;
            }
        } else if (!in_token) {
            token_count++;
            ptr->next = malloc(sizeof(struct utl_Token));
            ptr = ptr->next;
            ptr->string = str;
            ptr->next = NULL;
            in_token = 1;
        }
        str++;
    }
    token_list = malloc(sizeof(char **) * (2 + token_count));
    i = 0;
    while(head != NULL) {
        token_list[i] = head->string;
        ptr = head;
        head = head->next;
        free(ptr);
        i++;
    }
    token_list[i] = NULL;

    return token_list;
}
