#include<stdio.h>
#include<stdlib.h>
void main(){
    FILE *sfile, *dfile;
    char ch;
    sfile = fopen("Untitled1.c", "r");
    if(sfile == NULL){
        printf("Can't open destination file. \n");

    }
    dfile = fopen("output.c", "w");
    if(dfile == NULL){
        printf("error");
        fclose(sfile);
    }
    while((ch = fgetc(sfile))!= EOF){
        fputc(ch, dfile);
    }
    printf("Copied successfully");
    fclose(sfile);
    fclose(dfile);
    return 0;
}
