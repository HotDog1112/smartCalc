#ifndef S21_CALC_POLISH_H
#define S21_CALC_POLISH_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define S21_EPS 1e-6

#define OK 0
#define ERROR 1

typedef struct info {
    char oper;          // оператор  +-*/^
    char type;              // тип элемента: f - function, o - operator, n - number
    char func_name[5];         // буквенное обозначение функции         // если type=n,записываем число
    double number;          // если type=n,записываем число
    int priority;           // узнаем приоритет, если type=n=0
} lex_t;

typedef struct node {
    lex_t *lexema;
    struct node *next;
} node_t;

typedef struct stack {
    struct node* peak;
} tstack_t;

typedef enum { NUMBER, FUNCTION, OPERATOR, X, NO } type_of_data;

lex_t* initLex(lex_t* lex);
node_t* initNode(lex_t* lex);
node_t* addNode(node_t* A, lex_t* lex);
node_t* addLastNode(node_t* head, lex_t* lex);
node_t* deleteNode(node_t* head, node_t* find);
int deleteListOfNodes(node_t* head);

tstack_t* initStack();
void Push(lex_t* lex, tstack_t* s);
lex_t* Pop(tstack_t* s);
lex_t* checkStackNode(tstack_t* s);
void deleteStack(tstack_t* stack);

int isFuncNameCorrect(char *str);
int isOperator(char c);
void isUnary(node_t* head);
int checkPriority(char a);
int checkType(char a);
lex_t* fillLex(lex_t* lexema, char a, char *value, double num);

double reversePolishNotation(node_t* node);
void print_node(node_t* node);
double calcResult(node_t* inode);
double startCalc(char* str, double x_value);


//_________CREDIT________//

int takeCreditInfo(double credit_amount, int term, double rate, int type,
                   double* mounth_sum, double* total_payment, double* overpayment, int mounth);
int creditDiff_Calc(double credit_amount, int term, double rate, double* mounth_sum, double* total_payment, double* overpayment, int mounth);
int getDaysInMounth(int mounth);
int creditAnn_Calc(double credit_amount, int term, double rate, double* mounth_sum, double* total_payment, double* overpayment);

#endif //  S21_CALC_POLISH_H
