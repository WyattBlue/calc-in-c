#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <setjmp.h>

#include <readline/readline.h>
#include <readline/history.h>

enum type{INTEGER, PLUS, MINUS, MUL, DIV, LPAREN, RPAREN, TOK_EOF};

typedef struct {
  unsigned int type;
  int value;
} token;


// typedef struct {
//   token token;
//   struct binop *left;
//   struct binop *right;
// } binop;


jmp_buf buf;
unsigned int pos;
char *text;
char current_char;
token current_token;


// Lexer
void advance(){
    pos++;
    current_char = text[pos];
}

char peek(){
    if (text[pos] == 0){
        return 0;
    } else {
        return text[pos + 1];
    }
}

void skip_whitespace(){
    while (current_char != 0 && isspace(current_char)){
        advance();
    }
}

int integer(int is_minus){
    int nums[9];
    int i;
    for(i=0;i<9;i++){nums[i]=-1;}

    i = 0;
    while (current_char != 0 && isdigit(current_char)){
        if (i > 8) {
            fprintf(stderr, "Number too big\n");
            longjmp(buf, 1);
        }
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
    if (is_minus){
        return -result;
    } else {
        return result;
    }
}

// Parser
token get_next_token(){
    while (current_char != 0){
        if (isspace(current_char)){
            skip_whitespace();
            continue;
        }
        if (isdigit(current_char)) {
            return (token){INTEGER, integer(0)};
        }
        if (current_char == '+') {
            char peek_char = peek();
            advance();
            if (isdigit(peek_char)) {
                return (token){INTEGER, integer(0)};
            } else {
                return (token){PLUS, 0};
            }
        }
        if (current_char == '-') {
            char peek_char = peek();
            advance();
            if (isdigit(peek_char)) {
                return (token){INTEGER, integer(1)};
            } else {
                return (token){MINUS, 0};
            }
        }
        if (current_char == '*') {
            advance();
            return (token){MUL, 0};
        }
        if (current_char == '/') {
            advance();
            return (token){DIV, 0};
        }
        if (current_char == '(') {
            advance();
            return (token){LPAREN, 0};
        }
        if (current_char == ')') {
            advance();
            return (token){RPAREN, 0};
        }
        
        fprintf(stderr, "Bad character: %c\n", current_char);
        longjmp(buf, 1);
    }
    return (token){TOK_EOF, 0};
}

// Interpreter
int expr(void);

void eat(unsigned int type){
    if (current_token.type == type){
        current_token = get_next_token();
    } else {
        if (type == RPAREN) {
            fprintf(stderr, "Expected closing ')'\n");
        } else {
            fprintf(stderr, "Expected %d, got %d\n", type, current_token.type);
        }
        longjmp(buf, 1);
    }    
}

int factor(){
    token token = current_token;
    if (token.type == INTEGER){
        eat(INTEGER);
        return token.value;
    }
    eat(LPAREN);
    int result = expr();
    eat(RPAREN);
    return result;
}

int term(){
    int result = factor();

    while (current_token.type == MUL || current_token.type == DIV){
        if (current_token.type == MUL){
            eat(MUL);
            result *= factor();
        } else if (current_token.type == DIV){
            eat(DIV);
            result /= factor();
        }
    }
    return result;
}

int expr(){
    int result = term();
    while (current_token.type == PLUS || current_token.type == MINUS){
        if (current_token.type == PLUS) {
            eat(PLUS);
            result += term();
        } else if (current_token.type == MINUS) {
            eat(MINUS);
            result -= term();
        }
    }
    return result;
}

int my_init(){
    pos = 0;
    current_char = text[pos];
    current_token = get_next_token();
    return expr();
}


int main(){
    printf("C calc program, 2022\n");
    while (1) {
        text = readline("> ");
        add_history(text);
        if (setjmp(buf) == 0) {
            printf("%d\n", my_init());
        }
    }
    return 0;
} 
