
/*this function's first job is spell checking of input.
 *and the second job is removing the quotes.*/
int intInputReviser(char* str) {
    int i;
    char c;
    for ( i = 0; ; i++ ) {
        if(str[i+1] == '\"')
            break;
        str[i] = str[i+1];
        c = str[i];
        if( !( (isBracket(c)) || (isDigit(c)) || (isOp(c)) ) ) {
            return ERROR;
        }
    }
    str[i] = '\0';
    return OK;
}

/*This function decides and applies basic operations to popped two operand and push the result.*/
void operateInt(IntStack **topInt, OpStack **topOp, int *err) {
    int val1, val2;
    char op;
    /*if one of operands doesnt exist, error occurs and function returns -1*/
    val1 = popInt(topInt,err);
    val2 = popInt(topInt,err);
    op = popOp(topOp,err);
    if(*err) return ;

    int result;
    switch(op) {
        case '/' :
            result = val2 / val1;
            break;
        case '*' :
            result = val2 * val1;
            break;
        case '-' :
            result = val2 - val1;
            break;
        case '+' :
            result = val2 + val1;
            break;
        default :
            *err = TRUE;
            return;
    }
    pushInt(topInt, result);
}


int intExpressionSolver(char *str, int *result) {
    printf("%s\n", str);
    /*some below parameters will be explained with comment where it used.*/
    /*err flag: 0 means no error, 1 means error.*/
    int i=0, j=0, length=strlen(str), err=0, signFlag=0;
    /*isBrackedSigned is an array, which stores minus signs(as TRUE) of brackets by bracketIndex.
     *bracketArraySize will be exponent of 2 and will be reallocated since we need a dynamic array.*/
    int *isBracketSigned, bracketArraySize=2, bracketIndex=0;
    isBracketSigned = (int*) malloc(bracketArraySize*sizeof(int));
    /*val parameter takes operand,operator or bracket as string temporary.*/
    char val[50];
    /*below stack pointers points to header of relative linked list stack.*/
    IntStack *topInt = NULL;
    OpStack *topOp = NULL;

    while(i<length) {
        /*--------------1.OPERANDS-----------------*/
        for(j=1;isDigit(str[i]); j++) {
            if(++i<length) {
                /*if reading the value has finished, copy it to a string, convert to int and push it to int stack.*/
                if(!isDigit(str[i])) {
                    val[j]='\0';
                    /*if the previous character is a minus sign,
                     *and two previous one is a minus or plus sign:
                     signFlag will be triggered (at OPERATOR part of loop)*/
                    strncpy(val, str+i-j, j);
                    if(signFlag) {
                        pushInt(&topInt, -1*atoi(val));
                        signFlag=0;
                    }
                    else
                        pushInt(&topInt, atoi(val));
                    break;
                }
            }
            /*if all expression scanned, push the value and break the loop.*/
            else {
                val[j]='\0';
                strncpy(val, str+i-j, j);
                if(signFlag) {
                    pushInt(&topInt, -1*atoi(val));
                    signFlag=0;
                }
                else
                    pushInt(&topInt, atoi(val));
                break;
            }
        }
        /*break the loop if scanning expression finished.*/
        if(i>=length) break;


        /*--------------2.BRACKETS---------------*/
       if(str[i] == '(') {
            /*if previous char is a minus sign*/
            if(signFlag) {
                isBracketSigned[bracketIndex]=1;
                signFlag = FALSE;
            }
            else
                isBracketSigned[bracketIndex]=FALSE;
            /*array will dynamicly grow.*/
            if(++bracketIndex==bracketArraySize) {
                bracketArraySize*=2;
                isBracketSigned = (int *) realloc(isBracketSigned, bracketArraySize*sizeof(int));
            }
            pushOp(&topOp, str[i]);

            /*Because of next char can be a digit, we have to restart loop.*/
            i++;
            continue;
        }

        if(str[i] == ')') {
            while(peekOp(topOp) != '(') {
                operateInt(&topInt, &topOp, &err);
                if(peekOp(topOp) == ERROR) {
                    /*unbalanced paranthesis error.*/
                    return ERROR;
                }
            }
            popOp(&topOp, &err);
            if(err) return ERROR;

            if(isBracketSigned[--bracketIndex]) {
                pushInt(&topInt, -1*popInt(&topInt,&err));
            }
            /*Because of next char can be a digit, we have to restart loop.*/
            i++;
            continue;
        }

        /*--------------3.OPERATORS---------------*/
        for(j=1; isOp(str[i]); j++) {
            /*More then 2 sequential operators causes error.*/
            if(j==3)
                return ERROR;

            if(j==2) {
                /*if first 2 characters of input are operators, error occurs.*/
                if(i==1)
                    return ERROR;
                /*if there is a '(' bracket before two operands, error occurs.*/
                if(str[i-2] == '(')
                    return ERROR;
                /*if any operand exist before mult. or division operator, error occurs.*/
                if(str[i]=='*' || str[i]=='/') {
                    printf("%c,%c", str[i],str[i-1]);
                    return ERROR;
                }
                /*now we know its a plus or minus sign.*/
                /*if it is a minus sign, it must be informed to
                 *operand adder(OPERANDS part of loop) via signFlag.*/
                if (str[i] == '-') {
                    signFlag = TRUE;
                }
                /*plus sign can be ignored smoothly.*/
            }

            if(j==1) {
                /*if its first input and if its a sign:*/
                if(i==0) {
                    /*set the signFlag to 1.*/
                    if(str[i]=='-')
                        signFlag=TRUE;
                    /*there is nothing to do if its a plus sign.*/
                }
                /*'(' bracket have to be checked separetely because str[i-1] can lead a segmentation if i=0.*/
                else if(str[i-1]=='(') {
                    if(str[i]=='-')
                        signFlag=TRUE;
                }


                /*now we are sure its an operator.*/
                else {
                    while( !hasHigherPriority(str[i],peekOp(topOp)) ) {
                        operateInt(&topInt, &topOp, &err);
                    }
                    pushOp(&topOp, str[i]);
                }
            }

            if(++i>=length) break;
        }
    }
    free(isBracketSigned);

    /*-----------4.CALCULATING LEFTOVERS:------------*/
    /*loop has finished to read arithmatic expression, now its time to calculate leftovers.*/
    while(isOp(peekOp(topOp))) {
        operateInt(&topInt, &topOp, &err);
    }
    /*unbalanced paranthesis error:*/
    if(peekOp(topOp) == '(') {
        return ERROR;
    }
    /*if no operators left in operators stack but still have operands in integer stack:*/
    if (peekOp(topOp) == '\0') {
        int lastOne = popInt(&topInt, &err);
        if(err) return ERROR;
        /*if any value left in integer stack, error occurs:*/
        if(topInt!=NULL) return ERROR;
        /*arithmatic expression solved successfully.*/
        *result=lastOne;
        return OK;
    }
    return ERROR;
}


