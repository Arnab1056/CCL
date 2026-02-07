// Program to check whether a given input matches a regular expression pattern or not.
// This is a simple regex matcher for basic patterns like: a*, b*, (a|b), etc.

#include <stdio.h>
#include <string.h>

// Define the maximum size for regex pattern and input string
#define MAX_SIZE 50

// Global variables to store the regex pattern and input string
char regex[MAX_SIZE];      // The regular expression pattern
char input[MAX_SIZE];      // The input string to check
int regex_pos = 0;         // Current position in regex pattern
int input_pos = 0;         // Current position in input string

// Function declaration
int matchExpression();

// Function: Match a single character
// Returns 1 if the input character matches, 0 otherwise
int matchChar(char c) {
    // Check if current input character matches the given character
    if (input[input_pos] == c) {
        input_pos++;           // Move to next input character
        return 1;              // Match found
    }
    return 0;                  // No match
}

// Function: Match zero or more occurrences of a character (the * operator)
// Returns 1 (always successful - can match 0 or more characters)
int matchStar(char c) {
    // Keep matching as long as input character equals the given character
    while (input[input_pos] == c) {
        input_pos++;           // Move to next input character
    }
    return 1;                  // Star always returns 1 (it can match 0 times)
}

// Function: Match the regular expression recursively
// Returns 1 if the pattern matches, 0 otherwise
int matchExpression() {
    // Base case: we've matched the entire regex
    if (regex[regex_pos] == '\0')
        return 1;

    // Handle alternative pattern: (a|b)
    // This matches either 'a' OR 'b'
    if (regex[regex_pos] == '(') {
        regex_pos++;           // Skip '('
        char first = regex[regex_pos];    // First option (e.g., 'a')
        regex_pos += 2;        // Skip first char and '|'
        char second = regex[regex_pos];   // Second option (e.g., 'b')
        regex_pos++;           // Skip second char

        if (regex[regex_pos] == ')')      // Skip ')'
            regex_pos++;

        // Check if input matches either first or second character
        if (input[input_pos] == first || input[input_pos] == second) {
            input_pos++;       // Move to next input character
        } else {
            return 0;          // No match
        }
    }
    // Handle single lowercase letter
    else if (regex[regex_pos] >= 'a' && regex[regex_pos] <= 'z') {
        char current_char = regex[regex_pos];  // Store the character
        regex_pos++;           // Move to next regex position
        
        // Check if next is *, handle zero or more
        if (regex[regex_pos] == '*') {
            matchStar(current_char);  // Match the character zero or more times
            regex_pos++;           // Move past the *
        } else {
            // Match exactly one occurrence
            if (input[input_pos] != current_char)
                return 0;          // No match
            input_pos++;           // Move to next input character
        }
    }

  
    return matchExpression();
}


int main() {
    printf("=== REGULAR EXPRESSION MATCHER ===\n\n");
    

    printf("Enter Regular Expression: ");
    scanf("%s", regex);

    printf("Enter Input String: ");
    scanf("%s", input);
    

    if (matchExpression() && input[input_pos] == '\0') {
        printf("\n*** Regular expression MATCHES ***\n");
    } else {
        printf("\n*** Regular expression DOES NOT MATCH ***\n");
    }

    return 0;
}