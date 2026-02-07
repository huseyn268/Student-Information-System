 #include<stdlib.h>
 #include <string.h>
#include <ctype.h>
#include "student.h"
#include "courses.h"
#include"professor.h"
#include<stdio.h>
#include<time.h>

//Removes trailing \n, \r, spaces and tabs from a string.
// * Used to clean user/file input read via fgets().
void clean_newLine(char *s) {

    size_t len = strlen(s);
    while (len > 0 && (s[len-1] == '\n' || s[len-1] == '\r' ||
                       s[len-1] == ' '  || s[len-1] == '\t')) {
        s[len-1] = '\0';
        len--;
}
}

//Clears leftover characters in stdin after scanf.
 //* Prevents scanf/fgets mixing issues (leftover newline problems).
void clear_input_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

 //Safely reads an integer from user input.
 //* Returns 1 on success, 0 on failure.
int read_int(const char *prompt, int *out)
{
    if (!out) return 0;
    printf("%s", prompt);
    if (scanf("%d", out) != 1) {
        clear_input_buffer();
        return 0;
    }
    clear_input_buffer();
    return 1;
}

//* Performs basic validation for name fields.
 //* Prevents: empty strings, leading/trailing spaces, control chars, and double spaces.
 //* Note: Allows Turkish characters (it only blocks control characters)
int is_valid_name(const char *s)
{
    if (!s || s[0] == '\0') return 0;

    size_t len = strlen(s);
    if (s[0] == ' ' || s[len - 1] == ' ') return 0;

    for (size_t i = 0; i < len; i++) {
        unsigned char c = (unsigned char)s[i];

        /* kontrol karakteri yasak */
        if (c < 32) return 0;

        /* iki boşluk yan yana olmasın */
        if (c == ' ' && i > 0 && s[i - 1] == ' ')
            return 0;
    }
    return 1;
}

// Copies a file in binary mode (used for backups)
int copyFile(const char *src, const char *dst)
{
    FILE *in = fopen(src, "rb");
    if(!in){
        printf("Cannot open source file: %s\n", src);
        return 0;
    }

    FILE *out = fopen(dst, "wb");
    if(!out){
        fclose(in);
        printf("Cannot create backup file: %s\n", dst);
        return 0;
    }

    char buffer[4096];
    size_t n;

    while((n = fread(buffer, 1, sizeof(buffer), in)) > 0){
        fwrite(buffer, 1, n, out);
    }

    fclose(in);
    fclose(out);
    return 1;
}

 //Generates a timestamp string and writes it into out buffer.
 //* Format: YYYYMMDD_HHMMSS (e.g., 20260116_214530)
void makeTimestamp(char *out, size_t size)
{
    time_t t = time(NULL);
    struct tm *tm_info = localtime(&t);

    strftime(out, size, "%Y%m%d_%H%M%S", tm_info);
}
