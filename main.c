#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define SIZE_OF_EXPRESSION 100
#define SIZE_OF_STACK 50
typedef struct stack_1 {
    double *data;
    int max_size;
    int top;

}stack_1;
typedef struct stack_2 {
    int *value;
    int max_size;
    int top;

}stack_2;


stack_1 * new_stack_1(int cap){
    stack_1 *pt = (stack_1*)malloc(sizeof(stack_1));
    pt->max_size = cap;
    pt->top = -1;
    pt->data = (double*)malloc(sizeof(double)*cap);
    return pt;
}
stack_2 * new_stack_2(int cap){
    stack_2 *pt2 = (stack_2*)malloc(sizeof(stack_2));
    pt2->max_size = cap;
    pt2->top = -1;
    pt2->value = (int*)malloc(sizeof(int)*cap);
    return pt2;
}

int is_Full(stack_1 * pt){
    return pt->top == pt->max_size - 1;
}
int is_Full2(stack_2*pt2){
    return pt2->top == pt2->max_size - 1;
}
int is_Empty(stack_1*pt){
    return pt->top == -1;
}
int is_Empty2(stack_2*pt2){
    return pt2->top == -1;
}
void push(stack_1*pt, double x){
    if(is_Full(pt)){
        printf("Overflow\n");
        exit(-1);
    }
    pt->data[++pt->top] = x;
}

void push2(stack_2 * pt2, int x){
    if(is_Full2(pt2)){
        printf("Overflow\n");
        exit(-1);
    }
    pt2->value[++pt2->top] = x;
}

double pop(stack_1*pt){
    return pt->data[pt->top--];
}
double pop2(stack_2*pt2){
    return pt2->value[pt2->top--];
}
double peek(stack_1 *pt){
    return pt->data[pt->top];
}
int peek2(stack_2 *pt2){
    return pt2->value[pt2->top];
}
int priority(char sign){
    if (sign == '('){
        return 0;
    }
    if (sign == '+' || sign == '-'){
        return 1;
    }
    if (sign == '*' || sign == '/'){
        return 2;
    }
    if(sign == '^'){
        return 3;
    }
}

void res(stack_1*pt, stack_2*pt2){
    double second_num = peek(pt);
    pop(pt);
    double first_num = peek(pt);
    pop(pt);
    char sign = (char)peek2(pt2);
    pop2(pt2);
    if(sign == '+'){
        push(pt, first_num+second_num);
    }
    else if(sign == '-'){
        push(pt, first_num-second_num);
    }
    else if(sign == '*'){
        push(pt, first_num*second_num);
    }
    else if(sign == '/'){
        push(pt, first_num/second_num);
    }
    else if(sign == '^'){
        double a = first_num;
        for (int i = 2; i <= second_num; i++){
            a *= first_num;
        }
        push(pt, a);
    }

}

double result_of_expression(char * exp, stack_1 * pt, stack_2*pt2) {
    int x = 0;
    int fraction = 0;
    long int size_fraction = 1;
    int i = 0;
    int flag_fraction = 0;
    int flag_val = 0;
    double val = 0;
    int flag_unary_minus = 0;
    while(i <= strlen(exp)){
        if(exp[i] >= '0' && exp[i] <= '9'){
            if (flag_fraction == 0) {
                x = x * 10 + (exp[i] - 48);
                i++;
            }
            else {
                fraction = fraction * 10 + (exp[i] - 48);
                size_fraction *= 10;
                i++;
            }
        }
        else if (exp[i] == '.'){

            flag_fraction = 1;
            i++;
        }
        else if(exp[i] >= 'a' && exp[i] <= 'z'){
            char variable[20] = "";
            int num_letters_in_var = 0;
            while (exp[i] >= 'a' && exp[i] <= 'z'){
                variable[num_letters_in_var] = exp[i];
                num_letters_in_var++;
                i++;
            }
            printf("Write value of %s  ", variable);
            scanf("%lf", &val);
            flag_val = 1;


        }

        if (i == (strlen(exp)) || (((exp[i] >= 40 && exp[i] <= 47) || exp[i] == 94) && (exp[i] != '.'))){
            double z;
            if (flag_val == 1){
                z = val;
                flag_val = 0;
            }
            else {
                z = (double)x + ((double)fraction)/size_fraction;
            }
            if(z != 0){
                if (flag_unary_minus == 1){
                    z = -z;
                    flag_unary_minus = 0;
                    pop2(pt2);
                }
                push(pt, z);
            }

            if(i != strlen(exp)) {
                if(exp[i] == ')'){
                    while((char)peek2(pt2) != '('){
                        res(pt, pt2);
                    }
                    pop2(pt2);
                }
                else if ((is_Empty2(pt2) == 1) || (priority(exp[i]) > priority((char) peek2(pt2))) || exp[i] == '('){
                    if(exp[i] == '('){
                        if (exp[i+1] == '-'){
                            flag_unary_minus = 1;
                        }
                    }
                    push2(pt2, exp[i]);

                } else if (priority(exp[i]) <= priority((char) peek2(pt2))) {
                    res(pt, pt2);
                    push2(pt2, exp[i]);
                }
            }


            x = 0;
            fraction = 0;
            size_fraction = 1;
            flag_fraction = 0;
            if(i == strlen(exp)){
                while (is_Empty2(pt2) == 0){
                    res(pt, pt2);
                }
            }


            i++;
        }

    }
    return peek(pt);
}


int main() {
    stack_1 *pt = new_stack_1(SIZE_OF_STACK);
    stack_2 * pt2 = new_stack_2(SIZE_OF_STACK);
    char exp[SIZE_OF_EXPRESSION];
    scanf("%s", exp);
    printf("Result: %lf", result_of_expression(exp, pt, pt2));



    return 0;
}

