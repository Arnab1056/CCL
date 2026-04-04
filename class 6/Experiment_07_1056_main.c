#include <stdio.h>
#include <string.h>

int main() {
    char choice;

    do {
        char nonTerminal;
        char prod[10][20];
        char alpha[10][20], beta[10][20];
        int n, i, j;

        int alphaCount = 0, betaCount = 0;

        printf("\nEnter Non-Terminal: ");
        scanf(" %c", &nonTerminal);

        printf("Enter number of productions: ");
        scanf("%d", &n);

        printf("Enter productions (RHS only):\n");
        for(i = 0; i < n; i++) {
            scanf("%s", prod[i]);
        }

        // 🔍 Check Left Recursion
        int hasLeftRecursion = 0;
        for(i = 0; i < n; i++) {
            if(prod[i][0] == nonTerminal) {
                hasLeftRecursion = 1;
                strcpy(alpha[alphaCount++], prod[i] + 1);
            } else {
                strcpy(beta[betaCount++], prod[i]);
            }
        }

        if(hasLeftRecursion) {
            printf("\nLeft Recursion Found!\n");

            printf("\nAfter Removing Left Recursion:\n");

            printf("%c -> ", nonTerminal);
            for(i = 0; i < betaCount; i++) {
                printf("%s%c'", beta[i], nonTerminal);
                if(i != betaCount - 1) printf(" | ");
            }

            printf("\n%c' -> ", nonTerminal);
            for(i = 0; i < alphaCount; i++) {
                printf("%s%c' | ", alpha[i], nonTerminal);
            }
            printf("ε\n");
        }
        else {
            printf("\nNo Left Recursion Found.\n");

            // 🔍 Check Left Factoring
            char prefix[20];
            strcpy(prefix, prod[0]);

            for(i = 1; i < n; i++) {
                j = 0;
                while(prefix[j] == prod[i][j] && prefix[j] != '\0') {
                    j++;
                }
                prefix[j] = '\0';
            }

            if(strlen(prefix) > 0) {
                printf("\nLeft Factoring Possible.\n");

                printf("\nAfter Left Factoring:\n");

                printf("%c -> %s%c'\n", nonTerminal, prefix, nonTerminal);

                printf("%c' -> ", nonTerminal);
                for(i = 0; i < n; i++) {
                    if(strlen(prod[i]) == strlen(prefix)) {
                        printf("ε");
                    } else {
                        printf("%s", prod[i] + strlen(prefix));
                    }

                    if(i != n - 1) printf(" | ");
                }
                printf("\n");
            }
            else {
                printf("\nNo Left Factoring Needed.\n");
            }
        }

        // 🔁 Ask user to continue
        printf("\nDo you want to enter another grammar? (y/n): ");
        scanf(" %c", &choice);

    } while(choice == 'y' || choice == 'Y');

    printf("\nProgram Ended.\n");

    return 0;
}
