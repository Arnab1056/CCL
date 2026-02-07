//write a program to get the number of unique key words from a c program for compiler construction

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

void main() {
    FILE *sfile, *dfile;
    char ch, next;
    int in_comment = 0;

    sfile = fopen("Exp3_input_1056.c", "r");
    if(sfile == NULL) {
        printf("Can't open source file. \n");
        return;
    }

    dfile = fopen("Exp3_output_1056.c", "w");
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

    printf("Copied successfully\n");
    fclose(sfile);
    fclose(dfile);

{
    FILE *in = fopen("Exp3_output_1056.c", "r");
    if (in == NULL) {
        printf("Can't open cleaned file for keyword counting.\n");
        return;
    }

    const char *keywords[] = {
        "auto","break","case","char","const","continue","default","do",
        "double","else","enum","extern","float","for","goto","if",
        "inline","int","long","register","restrict","return","short","signed",
        "sizeof","static","struct","switch","typedef","union","unsigned",
        "void","volatile","while","_Bool","_Complex","_Imaginary" ,
    };

    int nkeywords = sizeof(keywords) / sizeof(keywords[0]);
    int *count = (int *)calloc(nkeywords, sizeof(int));

    if (count == NULL) {
        fclose(in);
        printf("Memory allocation failed.\n");
        return;
    }

    char buf[256];
    int idx = 0;
    int c;

    while ((c = fgetc(in)) != EOF) {
        if (isalpha(c) || c == '_') {
            if (idx < sizeof(buf) - 1)
                buf[idx++] = (char)c;
        } else {
            if (idx > 0) {
                buf[idx] = '\0';
                for (int i = 0; i < nkeywords; i++) {
                    if (strcmp(buf, keywords[i]) == 0) {
                        count[i]++;
                        break;
                    }
                }
                idx = 0;
            }
        }
    }

    if (idx > 0) {
        buf[idx] = '\0';
        for (int i = 0; i < nkeywords; i++) {
            if (strcmp(buf, keywords[i]) == 0) {
                count[i]++;
                break;
            }
        }
    }

    printf("\nKeyword Frequency:\n");
    for (int i = 0; i < nkeywords; i++) {
        if (count[i] > 0)
            printf("%-10s : %d\n", keywords[i], count[i]);
    }

    free(count);
    fclose(in);
}


    return;
}
