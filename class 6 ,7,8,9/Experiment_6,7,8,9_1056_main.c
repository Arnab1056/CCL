#include <stdio.h>
#include <string.h>
#include <ctype.h>

#define MAX_PROD 100
#define MAX_RHS  100
#define MAX_NT   26
#define MAX_SYM  128
#define EPSILON  '#'

typedef struct {
    char lhs;
    char rhs[MAX_RHS];
} Production;

int isNonTerminal(char c) {
    return (c >= 'A' && c <= 'Z');
}

void printGrammar(Production prods[], int n) {
    for (int i = 0; i < n; i++) {
        printf("%c -> %s\n", prods[i].lhs, prods[i].rhs);
    }
}

int addSymbol(int set[MAX_SYM], char symbol) {
    if (!set[(int)symbol]) {
        set[(int)symbol] = 1;
        return 1;
    }
    return 0;
}

int addSet(int set[MAX_SYM], int src[MAX_SYM], int ignoreEpsilon) {
    int changed = 0;
    for (int c = 0; c < MAX_SYM; c++) {
        if (src[c]) {
            if (ignoreEpsilon && c == (int)EPSILON) continue;
            changed |= addSymbol(set, (char)c);
        }
    }
    return changed;
}

int derivesEpsilon(int first[MAX_SYM]) {
    return first[(int)EPSILON];
}

void removeLeftRecursion(Production inProds[], int n, Production outProds[], int *outCount) {
    *outCount = 0;

    for (char nt = 'A'; nt <= 'Z'; nt++) {
        int alphaCount = 0, betaCount = 0;
        Production alpha[MAX_PROD];
        Production beta[MAX_PROD];
        int found = 0;

        for (int i = 0; i < n; i++) {
            if (inProds[i].lhs == nt) {
                found = 1;
                if (inProds[i].rhs[0] == nt) {
                    strcpy(alpha[alphaCount++].rhs, inProds[i].rhs + 1);
                } else {
                    strcpy(beta[betaCount++].rhs, inProds[i].rhs);
                }
            }
        }

        if (!found) continue;

        if (alphaCount == 0) {
            for (int i = 0; i < n; i++) {
                if (inProds[i].lhs == nt) {
                    outProds[(*outCount)++] = inProds[i];
                }
            }
        } else {
            for (int i = 0; i < betaCount; i++) {
                outProds[(*outCount)].lhs = nt;
                snprintf(outProds[(*outCount)++].rhs, MAX_RHS, "%s%c'", beta[i].rhs, nt);
            }

            for (int i = 0; i < alphaCount; i++) {
                outProds[(*outCount)].lhs = nt + 1;
                snprintf(outProds[(*outCount)++].rhs, MAX_RHS, "%s%c'", alpha[i].rhs, nt);
            }
            outProds[(*outCount)].lhs = nt + 1;
            strcpy(outProds[(*outCount)++].rhs, "#");
        }
    }
}

void factorGrammar(Production prods[], int n) {
    printf("\nLeft Factoring Result:\n");
    for (char nt = 'A'; nt <= 'Z'; nt++) {
        int count = 0;
        int indexes[MAX_PROD];
        for (int i = 0; i < n; i++) {
            if (prods[i].lhs == nt) {
                indexes[count++] = i;
            }
        }
        if (count < 2) continue;

        int prefixLen = strlen(prods[indexes[0]].rhs);
        for (int i = 1; i < count; i++) {
            int j = 0;
            while (prods[indexes[0]].rhs[j] && prods[indexes[i]].rhs[j] &&
                   prods[indexes[0]].rhs[j] == prods[indexes[i]].rhs[j]) {
                j++;
            }
            prefixLen = prefixLen < j ? prefixLen : j;
        }

        if (prefixLen == 0) continue;

        char prefix[MAX_RHS];
        strncpy(prefix, prods[indexes[0]].rhs, prefixLen);
        prefix[prefixLen] = '\0';

        printf("%c -> %s%c'\n", nt, prefix, nt);
        printf("%c' -> ", nt);
        for (int i = 0; i < count; i++) {
            char *s = prods[indexes[i]].rhs + prefixLen;
            if (*s == '\0') {
                printf("ε");
            } else {
                printf("%s", s);
            }
            if (i != count - 1) printf(" | ");
        }
        printf("\n");
    }
}

void computeFirst(int first[MAX_SYM][MAX_SYM], Production prods[], int n, char nonterminals[], int ntCount) {
    for (int i = 0; i < ntCount; i++) {
        for (int c = 0; c < MAX_SYM; c++) {
            first[(int)nonterminals[i]][c] = 0;
        }
    }

    int changed;
    do {
        changed = 0;
        for (int i = 0; i < n; i++) {
            char lhs = prods[i].lhs;
            char *rhs = prods[i].rhs;
            int k = 0;
            int canEmpty = 1;

            while (rhs[k] != '\0') {
                char symbol = rhs[k];
                if (isNonTerminal(symbol)) {
                    changed |= addSet(first[(int)lhs], first[(int)symbol], 1);
                    if (!derivesEpsilon(first[(int)symbol])) {
                        canEmpty = 0;
                        break;
                    }
                } else {
                    changed |= addSymbol(first[(int)lhs], symbol);
                    canEmpty = 0;
                    break;
                }
                k++;
            }

            if (canEmpty) {
                changed |= addSymbol(first[(int)lhs], EPSILON);
            }
        }
    } while (changed);
}

void computeFollow(int follow[MAX_SYM][MAX_SYM], int first[MAX_SYM][MAX_SYM], 
    
    Production prods[], int n, char nonterminals[], int ntCount, char startSymbol) {
    for (int i = 0; i < ntCount; i++) {
        for (int c = 0; c < MAX_SYM; c++) {
            follow[(int)nonterminals[i]][c] = 0;
        }
    }
    addSymbol(follow[(int)startSymbol], '$');

    int changed;
    do {
        changed = 0;
        for (int i = 0; i < n; i++) {
            char lhs = prods[i].lhs;
            char *rhs = prods[i].rhs;
            int len = strlen(rhs);
            for (int j = 0; j < len; j++) {
                char symbol = rhs[j];
                if (!isNonTerminal(symbol)) continue;

                int tempSet[MAX_SYM] = {0};
                int epsilonReachable = 1;
                for (int k = j + 1; k < len; k++) {
                    char next = rhs[k];
                    if (isNonTerminal(next)) {
                        addSet(tempSet, first[(int)next], 1);
                        if (!derivesEpsilon(first[(int)next])) {
                            epsilonReachable = 0;
                            break;
                        }
                    } else {
                        addSymbol(tempSet, next);
                        epsilonReachable = 0;
                        break;
                    }
                }

                changed |= addSet(follow[(int)symbol], tempSet, 0);
                if (epsilonReachable) {
                    changed |= addSet(follow[(int)symbol], follow[(int)lhs], 0);
                }
            }
        }
    } while (changed);
}

void printSet(const char *label, int set[MAX_SYM]) {
    printf("%s = { ", label);
    int first = 1;
    for (int c = 0; c < MAX_SYM; c++) {
        if (set[c]) {
            if (!first) printf(", ");
            first = 0;
            if (c == (int)EPSILON) {
                printf("ε");
            } else {
                printf("%c", (char)c);
            }
        }
    }
    printf(" }\n");
}

int main() {
    Production prods[MAX_PROD];
    int n = 0;
    char buffer[256];

    printf("Enter number of productions: ");
    scanf("%d", &n);
    getchar();

    int prodCount = 0;
    for (int i = 0; i < n; i++) {
        printf("Enter production %d (example A->aB|b): ", i + 1);
        fgets(buffer, sizeof(buffer), stdin);
        buffer[strcspn(buffer, "\r\n")] = '\0';
        if (strlen(buffer) == 0) {
            i--;
            continue;
        }

        char lhs = buffer[0];
        char *arrow = strstr(buffer, "->");
        if (!arrow) {
            printf("Invalid production format. Use A->xyz\n");
            i--;
            continue;
        }
        char *rhs = arrow + 2;
        char *token = strtok(rhs, "|");
        while (token) {
            if (prodCount >= MAX_PROD) break;
            prods[prodCount].lhs = lhs;
            strcpy(prods[prodCount].rhs, token);
            prodCount++;
            token = strtok(NULL, "|");
        }
    }
    n = prodCount;

    printf("\nOriginal Grammar:\n");
    printGrammar(prods, n);

    Production removedProds[MAX_PROD];
    int removedCount = 0;
    removeLeftRecursion(prods, n, removedProds, &removedCount);
    printf("\nGrammar after removing left recursion:\n");
    printGrammar(removedProds, removedCount);

    factorGrammar(prods, n);

    char nonterminals[MAX_NT];
    int ntCount = 0;
    int seen[MAX_SYM] = {0};
    for (int i = 0; i < n; i++) {
        if (isNonTerminal(prods[i].lhs) && !seen[(int)prods[i].lhs]) {
            nonterminals[ntCount++] = prods[i].lhs;
            seen[(int)prods[i].lhs] = 1;
        }
    }

    if (ntCount == 0) {
        printf("No nonterminals found.\n");
        return 0;
    }

    int first[MAX_SYM][MAX_SYM] = {0};
    int follow[MAX_SYM][MAX_SYM] = {0};
    computeFirst(first, prods, n, nonterminals, ntCount);
    computeFollow(follow, first, prods, n, nonterminals, ntCount, nonterminals[0]);

    printf("\nFIRST sets:\n");
    for (int i = 0; i < ntCount; i++) {
        char label[16];
        snprintf(label, sizeof(label), "FIRST(%c)", nonterminals[i]);
        printSet(label, first[(int)nonterminals[i]]);
    }

    printf("\nFOLLOW sets:\n");
    for (int i = 0; i < ntCount; i++) {
        char label[16];
        snprintf(label, sizeof(label), "FOLLOW(%c)", nonterminals[i]);
        printSet(label, follow[(int)nonterminals[i]]);
    }

    return 0;
}
