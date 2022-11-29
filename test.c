#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include <readline/readline.h>
#include <readline/history.h>

enum type{NONE, INTEGER, PLUS, MINUS, MUL, DIV, MEOF};

typedef struct {
  unsigned int type;
  int value;
} token;

unsigned int pos;
char *text;
unsigned int text_len;
token current_token;
char current_char;

void advance(){
    pos++;
    if (pos > text_len - 1){
        current_char = 0;
    } else {
        current_char = text[pos];
    }
}

void skip_whitespace(){
    while (current_char != 0 && isspace(current_char)){
        advance();
    }
}

int integer(){
    int nums[9];
    int i;
    for(i=0;i<9;i++){nums[i]=-1;}

    i = 0;
    while (current_char != 0 && isdigit(current_char)){
        nums[i] = current_char - 48;
        advance();
        i++;
    }

    unsigned int place = 1;
    int result = 0;
    for(i=8;i>=0;i--){
        if (nums[i] != -1){
            result += (nums[i] * place);
            place *= 10;
        }
    }
    return result;
}

token get_next_token(){
    while (current_char != 0){
        if (isspace(current_char)){
            skip_whitespace();
            continue;
        }
        if (isdigit(current_char)) {
            token token = {INTEGER, integer()};
            return token; 
        }
        if (current_char == '+') {
            advance();
            token token = {PLUS, 0};
            return token;
        }
        if (current_char == '-') {
            advance();
            token token = {MINUS, 0};
            return token;
        }
        if (current_char == '*') {
            advance();
            token token = {MUL, 0};
            return token;
        }
        if (current_char == '/') {
            advance();
            token token = {DIV, 0};
            return token;
        }
        fprintf(stderr, "Bad character: %c", current_char);
        exit(1);
    }
    token token = {MEOF, 0};
    return token;
}

void eat(unsigned int type){
    if (current_token.type == type){
        current_token = get_next_token();
    } else {
        fprintf(stderr, "Expected %d, got %d", type, current_token.type);
        exit(1);
    }    
}

int factor(){
    int value = current_token.value;
    eat(INTEGER);
    return value; 
}


int expr(){
    text_len = strlen(text);
    pos = 0;
    current_char = text[pos];
    current_token = get_next_token();


    //
    int result = factor();
    while (current_token.type == MUL || current_token.type == DIV){
        if (current_token.type == MUL) {
            eat(MUL);
            result *= factor();
        } else if (current_token.type == DIV) {
            eat(DIV);
            result /= factor();
        }
    }
    return result;
}

int main(){
    printf("C test program, 2022\n");
    while (1) {
        text = readline("> ");
        add_history(text);
        printf("%d\n", expr());
    }
    return 0;
} 
