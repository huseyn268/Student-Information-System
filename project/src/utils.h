#ifndef UTILS_H
#define UTILS_H

#include <stddef.h>  
void clean_newLine(char *s);
void clear_input_buffer(void);

int read_int(const char *prompt, int *out);
int is_valid_name(const char *s);

int copyFile(const char *src, const char *dst);
void makeTimestamp(char *out, size_t size);
#endif