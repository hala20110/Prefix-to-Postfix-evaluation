#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct
{
    float data;
    struct node *next;
}node;

node* newnode(float x)
{
    node* n = malloc(sizeof(node));
    n->data = x;
    n->next = NULL;
    return n;
}

typedef struct
{
    node* top;
}stack;

stack* initialize()
{
    stack* s = malloc(sizeof(stack));
    s->top = NULL;
    return s;
}

float pop(stack* s)
{
    node* temp = s->top;
    float x = temp->data;
    s->top = temp->next;
    free(temp);
    return x;
}

void push(stack* s, float value)
{
    node* n = newnode(value);
    n->next = s->top;
    s->top = n;
}

float peek(stack* s)
{
    return s->top->data;
}

int isEmpty(stack* s)
{
    if (s->top == NULL)
        return 1;
    else
        return 0;
}

void display (stack* s)
{
    stack* c = initialize ();
    while (!isEmpty(s))
    {
        float z = pop(s);
        printf ("%.2f\n", z);
        push(c, z);
    }
    while (!isEmpty(c))
    {
        push(s, pop(c));
    }
}

int priority(char x)
{
    if (x == '+' || x == '-')
        return 0;
    if (x == '*' || x == '/' || x == '%')
        return 1;
    if (x == '^')
        return 2;
    if (x == '(')
        return 3;
}

char* infixTopostfix(char* infix)
{
    stack* s = initialize();
    char* postfix = malloc((strlen(infix) * 2));
    int j = 0, prevWasoperator = 1;
    for (int i = 0; i < strlen(infix); i++)
    {
        if (prevWasoperator && (isdigit(infix[i]) || infix[i] == '-'))
        {
            postfix[j++] = ' ';
            postfix[j++] = infix[i];
            prevWasoperator = 0;
        }
        else if (isdigit(infix[i]) || infix[i] == '.')
        {
            postfix[j++] = infix[i];
            prevWasoperator = 0;
        }
        else if (infix[i] == ')')
        {
            while (!isEmpty(s) && peek(s) != '(')
            {
                postfix[j++] = ' ';
                postfix[j++] = (char)pop(s);
            }
            pop(s);
            prevWasoperator = 0;
        }
        else if (infix[i] == '(')
        {
            push(s, (float)infix[i]);
            prevWasoperator = 1;
        }
        else if (isEmpty(s))
        {
            push(s, (float)infix[i]);
            prevWasoperator = 1;
        }
        else if (infix[i] == ' ');
        else
        {
            while (!isEmpty(s) && (priority(peek(s)) >= priority(infix[i])) && peek(s) != '(')
            {
                postfix[j++] = ' ';
                postfix[j++] = (char)pop(s);
            }
            push(s, (float)infix[i]);
            prevWasoperator = 1;
        }
    }
    while(!isEmpty(s))
       {
         postfix[j++] = ' ';
         postfix[j++] = (char)pop(s);
       }
    postfix[j] = '\0';
    return postfix;
}
float evaluatePostfix(char* postfix)
{
    stack* s = initialize();
    for (int i = 0; postfix[i] != '\0'; i++)
    {
        if (isdigit(postfix[i]) || (postfix[i] == '-' && isdigit(postfix[i + 1])))
        {
            char* endptr;
            float num = strtof(&postfix[i], &endptr);
            i += endptr - &postfix[i] - 1;
            push(s, num);
        }
        else if (postfix[i] != ' ')
        {
            float operand2 = pop(s);
            float operand1 = pop(s);

            switch (postfix[i])
            {
                case '+':
                    push(s, operand1 + operand2);
                    break;
                case '-':
                    push(s, operand1 - operand2);
                    break;
                case '*':
                    push(s, operand1 * operand2);
                    break;
                case '/':
                    push(s, operand1 / operand2);
                    break;
                case '%':
                    push(s, fmod(operand1, operand2));
                    break;
                case '^':
                    push(s, pow(operand1, operand2));
                    break;
                default:
                    printf("Invalid character encountered: %c\n", postfix[i]);
                    break;
            }
        }
    }

    float result = pop(s);
    free(s);
    return result;
}

int main()
{

    char input[100];
    printf("Enter the infix expression: ");
    gets(input);
    input[strcspn(input, "\n")] = '\0';
    char* postfix = infixTopostfix(input);
    printf("Postfix expression: %s\n", postfix);
    float result = evaluatePostfix(postfix);
    printf("Result: %.6f\n", result);
    free(postfix);

    return 0;
}




