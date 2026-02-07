#include<stdio.h>
#include<stdlib.h>

void main() {
    FILE *sfile, *dfile;
    char ch, next;
    int in_comment = 0;

    sfile = fopen("code.c", "r");
    if(sfile == NULL) {
        printf("Can't open source file. \n");
        return;
    }

    dfile = fopen("output.c", "w");
    if(dfile == NULL) {
        printf("error");
        fclose(sfile);
        return;
    }

    while((ch = fgetc(sfile)) != EOF) {

        if (in_comment == 1) {
            if (ch == '\n') in_comment = 0;
            else continue;
        }

        else if (in_comment == 2) {
            if (ch == '*') {
                next = fgetc(sfile);
                if (next == '/') in_comment = 0;
                else ungetc(next, sfile);
            }
            continue;
        }

        else if (ch == '/') {
            next = fgetc(sfile);
            if (next == '/') {
                in_comment = 1;
                continue;
            } else if (next == '*') {
                in_comment = 2;
                continue;
            } else {
                ungetc(next, sfile);
            }
        }


        if (in_comment == 0) {
            fputc(ch, dfile);
        }
    }

    printf("Copied successfully");
    fclose(sfile);
    fclose(dfile);
    return;
}
