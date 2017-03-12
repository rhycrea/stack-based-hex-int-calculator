
/*this function's first job is spell checking of input.
 *and the second job is removing the quotes.*/
int hexInputReviser(char* str) {
    int i;
    char c;
    for ( i = 0; ; i++ ) {
        if(str[i+1] == '\"')
            break;
        str[i] = str[i+1];
        c = str[i];
        if( !( (isBracket(c)) || (isHex(c)) || (isOp(c)) ) ) {
            return ERROR;
        }
    }
    str[i] = '\0';
    return OK;
}

/*Function provides 2 string operation for implying minus sign to any hexadecimal string value.*/
char *addMinusSign(char *str) {
    int i, len=strlen(str);
    /*if hex value has a minus sign already, it must be removed instead adding new one.*/
    if(str[0] == '-') {
        char *newStr = (char*) malloc(len*sizeof(char*)-1);
        for(i=0; i<len-1; i++)
            newStr[i] = str[i+1];
        newStr[i] = '\0';
        return newStr;
    }
    /*new string needed with one more space.*/
    char *newStr = (char*) malloc(len*sizeof(char*)+1);
    newStr[0]='-';
    for(i=0; i<len;i++)
        newStr[i+1] = str[i];
    newStr[i+1]='\0';
    return newStr;
}

/*When operating two operands, first they will convert to int.*/
int hexToInt(char * hex) {
    int i, sum=0, len=strlen(hex), sign=1;
    char c;
    if(hex[0] == '-') {
        sign = -1;
        len--;
    }
    for(i=0; i<len; i++) {
        if(sign<0)
            c = hex[len-i];
        else
            c = hex[len-i-1];

        if(isDigit(c))
            sum += (pow(16,i)) * (c-'0');
        else if(c>64 && c<71)
            sum += (pow(16,i)) * (c-55);
        else if(c>96 && c<103)
            sum += (pow(16,i)) * (c-87);
        else
            return -111;
    }
    return sign*sum;
}

/*After calculation, they will be converted to hex again.*/
char *intToHex(int val) {
    int i, j, rem[200], quot=val;
    if(val<0)
        quot*=-1;
    for(i=0; quot; i++) {
        rem[i]=quot%16;
        quot=quot/16;
    }

    char *hex;
    hex=(char*) malloc(sizeof(char)*i);
    for(j=0; i; j++) {
        int d = rem[--i];
        if(d>=0 && d<10)
            hex[j] = (char)d + '0';
        else
            hex[j] = (char)d + 55;
    }
    hex[j] = '\0';
    if(val<0)
        return addMinusSign(hex);
    else
        return hex;
}

/*This function decides and applies basic operations to popped two operand and push the result.*/
void operateHex(HexStack **topHex, OpStack **topOp, int *err) {
    char val1[200], val2[200];
    char op;
    /*if one of operands doesnt exist, error occurs and function returns -1*/
    strcpy(val1,popHex(topHex,err));
    strcpy(val2,popHex(topHex,err));
    op = popOp(topOp,err);
    if(*err) return;

    char result[20];
    switch(op) {
        case '/' :
            strcpy(result , intToHex( hexToInt(val2) / hexToInt(val1) ) );
            break;
        case '*' :
            strcpy(result , intToHex( hexToInt(val2) * hexToInt(val1) ) );
            break;
        case '-' :
            strcpy(result , intToHex( hexToInt(val2) - hexToInt(val1) ) );
            break;
        case '+' :
            strcpy(result , intToHex( hexToInt(val2) + hexToInt(val1) ) );
            break;
        default :
            *err = TRUE;
            return;
    }
    pushHex(topHex, result);
}


int hexExpressionSolver(char *str, char **result) {
    printf("%s\n", str);
    /*meaning of parameters:*/
    /*err is a flag: 0 means no error, 1 means error.*/
    /*signFlag will be true if an '-' character reveals as a sign, not an operator.*/
    int i=0, j=0, length=strlen(str), err=0, signFlag=0;
    /*isBrackedSigned is an array, which stores minus signs(as TRUE) of brackets by bracketIndex.
     *bracketArraySize will be exponent of 2 and will be reallocated since we need a dynamic array.*/
    int *isBracketSigned, bracketArraySize=2, bracketIndex=0;
    isBracketSigned = (int*) malloc(bracketArraySize*sizeof(int));
    /*val parameter takes operand,operator or bracket as string temporary.*/
    char val[50];
    /*below stack pointers points to header of relative linked list stack.*/
    HexStack *topHex = NULL;
    OpStack *topOp = NULL;

    while(i<length) {
        /*--------------1.OPERANDS-----------------*/
        for(j=1;isHex(str[i]); j++) {
            if(++i<length) {
                /*if reading the value has finished, copy it to a string, convert to int and push it to int stack.*/
                if(!isHex(str[i])) {
                    val[j]='\0';
                    /*if the previous character is a minus sign,
                     *and two previous one is a minus or plus sign:
                     signFlag will be triggered (at OPERATOR part of loop)*/
                    strncpy(val, str+i-j, j);
                    val[j] = '\0';
                    if(signFlag) {
                        pushHex(&topHex, addMinusSign(val));
                        signFlag=FALSE;
                    }
                    else
                        pushHex(&topHex, val);
                    break;
                }
            }
            /*if all expression scanned, push the value and break the loop.*/
            else {
                val[j]='\0';
                strncpy(val, str+i-j, j);
                val[j] = '\0';
                if(signFlag) {
                    pushHex(&topHex, addMinusSign(val));
                    signFlag=FALSE;
                }
                else
                    pushHex(&topHex, val);
                break;
            }
        }
        /*break the loop if scanning expression finished.*/
        if(i>=length) break;


        /*----------------2.BRACKETS---------------*/
        if(str[i] == '(') {
            /*if previous char is a minus sign*/
            if(signFlag) {
                isBracketSigned[bracketIndex]=TRUE;
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
                operateHex(&topHex, &topOp, &err);
                if(peekOp(topOp) == ERROR) {
                    /*unbalanced paranthesis error.*/
                    return ERROR;
                }
            }
            popOp(&topOp, &err);
            if(err) return ERROR;

            if(isBracketSigned[--bracketIndex]) {
                pushHex(&topHex, addMinusSign(popHex(&topHex,&err)));
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
                /*if its first input and its a sign:*/
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
                        operateHex(&topHex, &topOp, &err);
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
        operateHex(&topHex, &topOp, &err);
    }
    /*unbalanced paranthesis error:*/
    if(peekOp(topOp) == '(') {
        return ERROR;
    }
    /*if no operators left in operators stack but still have operands in integer stack:*/
    if (peekOp(topOp) == '\0') {
        char lastOne[50];
        strcpy(lastOne,popHex(&topHex, &err));
        if(err) return ERROR;
        /*if any value left in integer stack, error occurs:*/
        if(topHex!=NULL) return ERROR;
        /*arithmatic expression solved successfully.*/
        *result=lastOne;
        return OK;
    }
    return ERROR;
}


