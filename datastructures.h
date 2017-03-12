
/*--------------------1.INT STACK---------------------*/
/*data structure of int stack to store decimal values.*/
typedef struct IntStack IntStack;

struct IntStack{
    int val;
    IntStack *ptr;
};

/*Push, pop and peek functions of int stack.*/
void pushInt(IntStack **top, int data) {
    IntStack *temp;
    if (*top == NULL)
    {
        *top =(IntStack *)malloc(1*sizeof(IntStack));
        (*top)->ptr = NULL;
        (*top)->val = data;
    }
    else
    {
        temp =(IntStack *)malloc(sizeof(IntStack));
        temp->ptr = *top;
        temp->val = data;
        *top = temp;
    }
    printf("->Pushed : %d\n", (*top)->val);
}

int popInt(IntStack **top, int *err) {
    if (*top == NULL)
    {
        printf("Error : Empty stack\n");
        *err = TRUE;
        return ERROR;
    }
    else {
        int val = (*top)->val;
        IntStack *temp;
        temp = *top;
        temp = temp->ptr;
        printf("Popped : %d\n", val);
        free(*top);
        *top = temp;
        return val;
    }
}

int peekInt(IntStack *top) {
     if (top == NULL)
    {
        return ERROR;
    }
    else {
        return top->val;
    }
}













/*-------------------2.OPERATOR STACK------------------*/
/*data structure of operator stack to store operators.*/
typedef struct OpStack OpStack;
struct OpStack{
    char val;
    struct OpStack *ptr;
};


/*Push, pop and peek functions of operator stack.*/
void pushOp(OpStack **top, char data) {
    OpStack *temp;
    if (*top == NULL)
    {
        *top =(OpStack *)malloc(1*sizeof(OpStack));
        (*top)->ptr = NULL;
        (*top)->val = data;
    }
    else
    {
        temp =(OpStack *)malloc(sizeof(OpStack));
        temp->ptr = *top;
        temp->val = data;
        *top = temp;
    }
    printf("->Pushed : %c\n", (*top)->val);
}

char popOp(OpStack **top, int *err) {
    if (*top == NULL)
    {
        printf("Error : Empty stack\n");
        *err = 1;
        return ERROR;
    }
    else {
        char val = (*top)->val;
        OpStack *temp;
        temp = *top;
        temp = temp->ptr;
        printf("Popped : %c\n", val);
        free(*top);
        *top = temp;
        return val;
    }
}

char peekOp(OpStack *top) {
     if (top == NULL)
    {
        return ERROR;
    }
    else {
        return top->val;
    }
}






/*----------------------3.HEX STACK-----------------------*/
/*data structure of hex stack to store hexadecimal values.*/
typedef struct HexStack HexStack;
struct HexStack{
    char *val;
    struct HexStack *ptr;
};


/*Push, pop and peek functiosn of hex stack.*/
void pushHex(HexStack **top, char *data) {
    HexStack *temp;
    if (*top == NULL)
    {
        *top =(HexStack *) malloc(sizeof(HexStack));
        (*top)->ptr = NULL;
        (*top)->val = (char*) malloc(sizeof(char)*strlen(data));
        strcpy((*top)->val,data);
    }
    else
    {
        temp =(HexStack *)malloc(sizeof(HexStack));
        temp->ptr = *top;
        temp->val = (char*)malloc(sizeof(char)*strlen(data));
        strcpy(temp->val,data);
        *top = temp;
    }
    printf("->Pushed : %s\n", (*top)->val);
}

char* popHex(HexStack **top, int *err) {
    if (*top == NULL)
    {
        printf("Error : Empty stack\n");
        *err = 1;
        return ERROR;
    }
    else {
        char *val;
        val=(char*)malloc(sizeof(char)*30);
        strcpy(val,(*top)->val);
        HexStack *temp;
        temp = *top;
        temp = temp->ptr;
        printf("Popped : %s\n", val);
        free(*top);
        *top = temp;
        return val;
    }
}

char* peekHex(HexStack *top) {
     if (top == NULL)
    {
        return NULL;
    }
    else {
        return top->val;
    }
}







/*-----------------------4.QUEUE------------------------*/
/*data structure, enqueue and dequeue functions of queue.
 *using for store and print results of expressions.*/
typedef struct Queue Queue;
struct Queue{
    char *str;
    struct Queue *next;
};


void enqueue(Queue **f, Queue **r, char *result) {
	Queue *temp = (Queue*)malloc(sizeof(Queue));
    temp->str = (char *)malloc(sizeof(char *) * strlen(result));
	strcpy(temp->str,result);
	temp->next = NULL;
	if(*f == NULL && *r == NULL){
		*f = *r = temp;
		return;
	}
	(*r)->next = temp;
	*r = temp;
}

int dequeue(Queue **f, Queue **r, FILE *out) {
	Queue *temp = *f;
	if(*f == NULL) {
        /*Queue is empty.*/
		return ERROR;
	}
	if(*f == *r) {
		*f = *r = NULL;
	}
	else {
		*f = (*f)->next;
	}
	/*Specially designed for our purpose, prints dequeued files to output.*/
    fprintf(out, "%s\n", temp->str);
	free(temp);
	return 1;
}



/*--------------------5.COMMON FUNCTIONS--------------------*/
/*true if op1 has higher priority, false otherwise.*
 *If it returns false, operation 'opr1' will be applied.
 *If it returns true , operator just will be pushed to stack. */
int hasHigherPriority(char opr1, char opr2) {
    switch(opr1) {
        case '/' : return opr2!='*' && opr2!='/';
        case '*' : return opr2!='*' && opr2!='/';
        case '-' : return opr2=='(' || opr2==FALSE;
        case '+' : return opr2=='(' || opr2==FALSE;
        default : return 0;
    }
}
