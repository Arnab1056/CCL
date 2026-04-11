#include <stdio.h>
#include <string.h>

int main() {
    char choice;

    do {
        char nonTerminal, production[20][20];
        int i, count;

        printf("Enter the non-terminal: ");
        scanf(" %c", &nonTerminal); 
        printf("Enter number of productions: ");
        scanf("%d", &count);

        printf("Enter the productions (without %c->):\n", nonTerminal);
        for(i = 0; i < count; i++) {
            scanf("%s", production[i]);
        }

        char alpha[20][20], beta[20][20];
        int a = 0, b = 0;

        for(i = 0; i < count; i++) {
            if(production[i][0] == nonTerminal) {
                strcpy(alpha[a], production[i] + 1);
                a++;
            } else {
                strcpy(beta[b], production[i]);
                b++;
            }
        }

        if(a == 0) {
            printf("\nNo left recursion found.\n");
        } else {
            printf("\nGrammar after removing left recursion:\n");

            printf("%c -> ", nonTerminal);
            for(i = 0; i < b; i++) {
                printf("%s%c'", beta[i], nonTerminal);
                if(i != b-1) printf(" | ");
            }

            printf("\n%c' -> ", nonTerminal);
            for(i = 0; i < a; i++) {
                printf("%s%c' | ", alpha[i], nonTerminal);
            }
            printf("ε\n");
        }

        printf("\nDo you want to enter another non-terminal? (y/n): ");
        scanf(" %c", &choice); 
    } while(choice == 'y' || choice == 'Y');

    printf("Program terminated.\n");
    return 0;
}
