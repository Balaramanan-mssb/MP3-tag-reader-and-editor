#ifndef VIEW_H
#define VIEW_H


typedef struct {
    char tag_name[50];
    char tag_info[50];
} info;
void view_tag(const char *fptr);
void convert(int *size);
#endif
