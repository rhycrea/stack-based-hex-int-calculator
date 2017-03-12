#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define OK 1
#define TRUE 1
#define ERROR 0
#define FALSE 0

#define isDigit(c) (c>47 && c<58) ? 1 : 0
#define isOp(c) c=='+' || c=='-' || c=='*' || c=='/' ? 1 : 0
#define isBracket(c) c=='(' || c==')' ? 1 : 0
#define isHex(c) ((c>47 && c<58) || (c>96 && c<103) || (c>64 && c<71)) ? 1 : 0

/*datastructures header file contains 3 stack and 1 queue implementation, plus a common function.
 *3 Stack implementation needed for operate decimal, hexadecimal and char(operator&bracket) values.
 *3 functions per stack exists: push(), pop() and peek().
 *Queue implementation have enqueue() and dequeue(), dequeue() also prints out to file.
 *Lastly, contains hasHigherPriority() function to decide operator precedence.*/
#include "datastructures.h"
/*int and hex header files provides necessary functions to operate and solve arithmatic experssions.
 *both them have similiar functions, hex header have surplus hexToInt() and ýntToHex() functions.
 *1.Experssions first scanned by InputReviser() to validate given characters.
 *2.ExpressionSolver() scans the expression string char by char does
 *everything by itself(error handling, manuplating data) except operating on 2 values(done by Operate()).
 *In each loop:
    *2.1.First scans for operands.
    *2.2.Secondly scans for brackets.
    *2.3.Lastly scans for operators.
 *3.If it finishes scanning the expression string, it will operate the leftovers.
 *If no errors found, result will be stored onto queue in worker() function.*/
#include "int.h"
#include "hex.h"

/*worker function decides the command and calls it, gives them necessary parameters.*/
void worker(FILE* in, FILE* out);
/*printer function dequeues the results and prints them to output file.*/
void printer(Queue **f, Queue **r, FILE *out);
/*main function just handles the file operations for sake of simplicity.*/
int main(int argc, char* argv[])
{
    char const* const inputFileName = argv[1];
    char const* const outputFileName = argv[2];
    FILE *in, *out;
    in = fopen(inputFileName, "r");
    out = fopen(outputFileName, "w+");

    worker(in,out);

    fclose(in);
    fclose(out);
    return 0;
}


void worker(FILE* in, FILE* out) {
    /*This function has 2 main job:
     *First, it decide command and run the necessary function
     *Second, it declare and maintain result datas which will used via a queue.*/
    char line[50000], *command, *type, *expression, result[500], *hexResult;
    int intResult;
    Queue *front = NULL, *rear = NULL;

    while (fgets(line, sizeof(line), in)) {
        /*First token is command which one of below ifs will execute.*/
        /*To parse line \r character will be necessary if input.txt produced in windows,
         *because return carriage in linux&unix it is \n but in windows it is \r\n.*/
        command = strtok(line, " \r\n");

        intResult = 0;

        if(!strcmp(command,"calculate")) {
            type = strtok(NULL, " \n");
            expression = strtok(NULL, " \n");
            if(!strcmp(type,"integer")) {
                /*2 kind of error may occur:
                 *InputReviser functions catchs invalid character error.
                 *ExperssionSolver function catches the logical errors like unbalanced brackets.*/
                if(!intInputReviser(expression)) {
                    enqueue(&front,&rear,"error");
                }
                else if(intExpressionSolver(expression, &intResult) == OK ) {
                    /*If solved succesfully, it will be enqueued.*/
                    sprintf(result, "%s %d",type,intResult);
                    enqueue(&front,&rear,result);
                    printf("-----IntResult:%d\n\n", intResult);
                }
                else
                    enqueue(&front,&rear,"error");
            }

            if(!strcmp(type,"hex")) {

                if(!hexInputReviser(expression)) {
                    enqueue(&front,&rear,"error");
                }
                else if(hexExpressionSolver(expression, &hexResult) == OK ) {
                    sprintf(result, "%s %s",type,hexResult);
                    enqueue(&front,&rear,result);
                    printf("-----HexResult:%s\n\n", result);
                }
                else
                    enqueue(&front,&rear,"error");

            }
        }
        else if(!strcmp(command,"print")) {
            printer(&front,&rear,out);
        }
    }
}

void printer(Queue **f, Queue **r, FILE *out) {
    while(dequeue(f,r,out));
}
