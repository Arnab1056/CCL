#include <stdio.h>
#include <string.h>


int match(char *r, char *t) {
    if (*r == '\0' && *t == '\0')
        return 1;
    if (*(r + 1) == '*') {
        if (match(r + 2, t))
            return 1;
        if (*t && (*r == *t))
            return match(r, t + 1);
        return 0;
    }
    if (*r == *t && *t)
        return match(r + 1, t + 1);

    return 0;
}

int preprocess(char *regex, char patterns[][100]) {
    char temp[100];
    int i = 0, j = 0;
    while (regex[i]) {
        if (regex[i] != '(' && regex[i] != ')' && regex[i] != '^')
            temp[j++] = regex[i];
        i++;
    }
    temp[j] = '\0';
    char *slash = strchr(temp, '/');
    if (slash) {
        int pos = slash - temp;
        char left[100], right[100];
        strncpy(left, temp, pos);
        left[pos] = '\0';
        strcpy(right, slash + 1);

        char prefix[100];
        strcpy(prefix, left);
        prefix[strlen(prefix) - 1] = '\0';

        strcpy(patterns[0], left);
        sprintf(patterns[1], "%s%s", prefix, right);
        return 2;
    }
    strcpy(patterns[0], temp);
    return 1;
}

int main() {
    char regex[100], text[100];
    char patterns[2][100];
    printf("Enter Regular Expression: ");
    scanf("%s", regex);
    printf("Enter String: ");
    scanf("%s", text);
    int total = preprocess(regex, patterns);
    int possible = 0;
    for (int i = 0; i < total; i++) {
        if (match(patterns[i], text)) {
            possible = 1;
            break;
        }
    }
    if (possible)
        printf("Possible (Match Found)\n");
    else
        printf("Not Possible (No Match)\n");

    return 0;
}
