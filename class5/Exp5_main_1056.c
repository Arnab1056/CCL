//write a program to get the number of unique key words from a c program for compiler construction

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<ctype.h>

void detect_identifiers(void);

void main() {
    FILE *sfile, *dfile;
    int ch, next;
    int in_comment = 0;

    sfile = fopen("Exp5_Input_1056.c", "r");
    if(sfile == NULL) {
        printf("Can't open source file. \n");
        return;
    }

    dfile = fopen("Exp5_output_1056.c", "w");
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
    FILE *in = fopen("Exp5_output_1056.c", "r");
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

    detect_identifiers();

    return;
}

void detect_identifiers() {
    FILE *in = fopen("Exp5_output_1056.c", "r");
    if (in == NULL) {
        printf("Can't open cleaned file for identifier detection.\n");
        return;
    }

    const char *keywords[] = {
        "auto","break","case","char","const","continue","default","do",
        "double","else","enum","extern","float","for","goto","if",
        "inline","int","long","register","restrict","return","short","signed",
        "sizeof","static","struct","switch","typedef","union","unsigned",
        "void","volatile","while","_Bool","_Complex","_Imaginary",
    };
    int nkeywords = sizeof(keywords) / sizeof(keywords[0]);

    typedef struct { char name[64]; int count; } Ident;
    Ident ids[1000];
    int nid = 0;

    char buf[256];
    int idx = 0;
    int c;
    int in_string = 0;

    while ((c = fgetc(in)) != EOF) {

        if (c == '"') {
            in_string = !in_string;
            continue;
        }
        if (in_string) continue;

        if (isalpha(c) || c == '_') {
            idx = 0;
            if (idx < (int)sizeof(buf) - 1) buf[idx++] = (char)c;
            while ((c = fgetc(in)) != EOF && (isalnum(c) || c == '_')) {
                if (idx < (int)sizeof(buf) - 1) buf[idx++] = (char)c;
            }
            buf[idx] = '\0';

            int is_keyword = 0;
            for (int k = 0; k < nkeywords; k++) {
                if (strcmp(buf, keywords[k]) == 0) { is_keyword = 1; break; }
            }

            if (!is_keyword) {
                if (strcmp(buf, "include") == 0) {
                } else {
                    int found = -1;
                    for (int i = 0; i < nid; i++) {
                        if (strcmp(ids[i].name, buf) == 0) { found = i; break; }
                    }
                    if (found != -1) {
                        ids[found].count++;
                    } else if (nid < (int)(sizeof(ids) / sizeof(ids[0]))) {
                        strncpy(ids[nid].name, buf, sizeof(ids[nid].name) - 1);
                        ids[nid].name[sizeof(ids[nid].name) - 1] = '\0';
                        ids[nid].count = 1;
                        nid++;
                    }
                }
            }

            if (c == EOF) break;
            ungetc(c, in);
        }
    }

    if (nid == 0) printf("\nNo identifiers found.\n");
    else {
        printf("\nIdentifier Frequency:\n");
        for (int i = 0; i < nid; i++) {
            printf("%-30s : %d\n", ids[i].name, ids[i].count);
        }
    }

    fclose(in);
}

