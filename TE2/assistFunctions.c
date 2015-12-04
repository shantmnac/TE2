#include "assistFunctions.h"
#include "struct.h"
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/ioctl.h>

extern char *comands[], *parametrs, *userString, *fileName;
extern int screenCol, screenRow, screenNumY, tabWidth, wrapMod, userStringSize;
extern struct listOfStrings *tmpStrPointer;
extern struct listOfChars *tmpCharPointer;
extern struct listOfStrings *pointerForStrings;
static struct termios stored_settings;
int isItOk = 1;

//+
int degree(int num, int deg){
    int temp = num;
    
    if (deg == 0){
        return 1;
    }
    else {
        while (deg > 1) {
            temp *= num;
            deg--;
        }
    }
    return temp;
}
//+
void clrscr(void){
    printf("\033[2J");
    printf("\033[0;0f");
}

void moveTxtY(char dir){
    int rowNum = 0, colNum = 0, i;
    
    if (dir == 'U'){
        for (i = 1; i < (2 * screenRow); i++) {
            if ((tmpStrPointer -> prev) != NULL) {
                tmpStrPointer = tmpStrPointer -> prev;
            }
            else break;
        }
    }
    
    resetKeypress();
    
    if (isItOk || (dir == 'U')) {
        clrscr();
    }
    
    while ((tmpStrPointer != NULL) && (rowNum < screenRow)) {
        if (wrapMod) {
            while ((tmpCharPointer != NULL)) {
                switch (tmpCharPointer -> curChar) {
                    case '\t':{
                        if ((screenCol - colNum - 1) >= tabWidth){
                            for (i = 0; i < tabWidth; i++) {
                                printf(" ");
                            }
                            colNum += tabWidth;
                        }
                        else {
                            printf("\n<--->");
                            for (i = 0; i < tabWidth; i++) {
                                printf(" ");
                            }
                            colNum = tabWidth + 5;
                            rowNum++;
                        }
                        break;
                    }
                        
                    case '\n': {
                        if (rowNum == (screenRow - 1)){
                            rowNum++;
                            break;
                        }
                        printf("\n");
                        rowNum++;
                        colNum = 0;
                        break;
                    }
                        
                    default: {
                        if ((screenCol - colNum) >= 2) {
                            printf("%c", tmpCharPointer -> curChar);
                            colNum++;
                        }
                        else {
                            printf("\n<--->%c", tmpCharPointer -> curChar);
                            colNum = 6;
                            rowNum++;
                        }
                        break;
                    }
                }
                tmpCharPointer = tmpCharPointer -> next;
                if (rowNum >= screenRow){
                    break;
                }
            }

            if (tmpStrPointer -> next != NULL) {
                tmpStrPointer = tmpStrPointer -> next;
                tmpCharPointer = tmpStrPointer -> curString;
                isItOk = 1;
            }
            else{
                isItOk = 0;
                break;
            }
        }
        else {
            while ((colNum < screenCol) && (tmpCharPointer != NULL)) {
                switch (tmpCharPointer -> curChar) {
                    case '\t':{
                        if ((screenCol - colNum) >= tabWidth){
                            for (i = 0; i < tabWidth; i++) {
                                printf(" ");
                            }
                            colNum += tabWidth;
                        }
                        break;
                    }
                        
                    case '\n': {
                        if (rowNum == (screenRow - 1)){
                            rowNum++;
                            break;
                        }
                        printf("\n");
                        rowNum++;
                        break;
                    }
                        
                    default: {
                        printf("%c", tmpCharPointer -> curChar);
                        colNum++;
                        break;
                    }
                }
                tmpCharPointer = tmpCharPointer -> next;
            }
            colNum = 0;
            if (tmpStrPointer -> next != NULL) {
                tmpStrPointer = tmpStrPointer -> next;
                tmpCharPointer = tmpStrPointer -> curString;
                isItOk = 1;
            }
            else{
                isItOk = 0;
                break;
            }
        }
    }
    
    setKeypress();
}

void moveTxtX(char dir){
    int i, colNum = 0, rowNum = 0;
    
    /*struct winsize screenSize;
    ioctl(STDOUT_FILENO, TIOCGWINSZ, &screenSize);
     */
    resetKeypress();
    clrscr();
    
    if (screenNumY == 0) {
        if (dir == 'L'){
            exit(0);
        }
    }
    
    if (dir == 'R') {
        tmpStrPointer = pointerForStrings;
        tmpCharPointer = pointerForStrings -> curString;
        for (i = 0; i < (screenNumY + 1) * screenCol; i++) {
            tmpCharPointer = tmpCharPointer -> next;
        }
        while ((tmpStrPointer != NULL) && (rowNum < screenRow)) {
            while ((tmpCharPointer != NULL) && (colNum < screenCol)) {
                switch (tmpCharPointer -> curChar) {
                    case '\t':{
                        if ((screenCol - colNum) >= tabWidth){
                            for (i = 0; i < tabWidth; i++) {
                                printf(" ");
                            }
                            colNum += tabWidth;
                        }
                        break;
                    }
                        
                    case '\n': {
                        printf("\n");
                        rowNum++;
                        colNum = 0;
                    }
                        
                    default: {
                        printf("%c", tmpCharPointer -> curChar);
                        colNum++;
                        break;
                    }
                }
                tmpCharPointer = tmpCharPointer -> next;
            }
            if (tmpStrPointer -> next != NULL) {
                tmpStrPointer = tmpStrPointer -> next;
                tmpCharPointer = tmpStrPointer -> curString;
            }
            else{
                break;
            }
            for (i = 0; i < (screenNumY + 1) * screenCol; i++) {
                tmpCharPointer = tmpCharPointer -> next;
            }
        }
        screenNumY++;
    }
    else {
        screenNumY--;
        tmpStrPointer = pointerForStrings;
        tmpCharPointer = pointerForStrings -> curString;
        for (i = 0; i < screenNumY * screenCol; i++) {
            tmpCharPointer = tmpCharPointer -> next;
        }
        while ((tmpStrPointer != NULL) && (rowNum < screenRow)) {
            while ((tmpCharPointer != NULL) && (colNum < screenCol)) {
                switch (tmpCharPointer -> curChar) {
                    case '\t':{
                        if ((screenCol - colNum) >= tabWidth){
                            for (i = 0; i < tabWidth; i++) {
                                printf(" ");
                            }
                            colNum += tabWidth;
                        }
                        break;
                    }
                        
                    case '\n': {
                        printf("\n");
                        rowNum++;
                        colNum = 0;
                    }
                        
                    default: {
                        printf("%c", tmpCharPointer -> curChar);
                        colNum++;
                        break;
                    }
                }
                tmpCharPointer = tmpCharPointer -> next;
            }
            tmpStrPointer = tmpStrPointer -> next;
            for (i = 0; i < screenNumY * screenCol; i++) {
                tmpCharPointer = tmpCharPointer -> next;
            }
        }
    }
    setKeypress();
}
//+
void initCmd(void){
    comands[0] = "set tabwidth";
    comands[1] = "print pages";
    comands[2] = "print range";
    comands[3] = "set wrap";
    comands[4] = "insert after";
    comands[5] = "edit string";
    comands[6] = "insert symbol";
    comands[7] = "replace substring";
    comands[8] = "delete range";
    comands[9] = "delete braces";
    comands[10] = "exit";
    comands[11] = "read";
    comands[12] = "open";
    comands[13] = "write";
    comands[14] = "set name";
    comands[15] = "help";
}
//+
int initFile(void){
    int firstIteration = 1;
    char temp = '!';
    struct listOfStrings *tempPrevStr = NULL, *tempNextStr = NULL;
    struct listOfChars *tempPrevChar = NULL, *tempNextChar = NULL;
    FILE *inputFile;
    
    inputFile = fopen(fileName, "rb");
    if (inputFile == NULL) {
        fprintf(stderr, "Такого файла нет!\n");
        return 0;
    }
    
    tempNextStr = (struct listOfStrings*)malloc(sizeof(struct listOfStrings));
    tempNextStr->prev = NULL;
    pointerForStrings = tempNextStr;

    do{
        while(temp != '\n'){
            fread(&temp, sizeof(char), 1, inputFile);
            
            if (feof(inputFile)) {
                break;
            }
            
            tempNextChar = (struct listOfChars*)malloc(sizeof(struct listOfChars));
            if (tempNextChar == NULL) {
                fprintf(stderr, "Недостаточно памяти!\n");
                freeTheList();
                return 9;
            }
            
            if (firstIteration == 1){
                tempNextStr->curString = tempNextChar;
                tempNextChar->prev = NULL;
                tempNextChar->curChar = temp;
                tempPrevChar = tempNextChar;
                firstIteration = 0;
                continue;
            }
            tempNextChar->curChar = temp;
            tempNextChar->prev = tempPrevChar;
            tempPrevChar->next = tempNextChar;
            tempPrevChar = tempNextChar;
        }
        
        if (feof(inputFile)){
            if (firstIteration) {
                free(tempNextStr);
                tempNextStr = tempPrevStr;
            }
            tmpStrPointer = pointerForStrings;
            tmpCharPointer = pointerForStrings -> curString;
            tempNextStr -> next = NULL;
            fclose(inputFile);
            return 0;
        }
        
        temp = '!';
        firstIteration = 1;
        tempNextChar->next = NULL;
        tempPrevStr = tempNextStr;
        tempNextStr = (struct listOfStrings*)malloc(sizeof(struct listOfStrings));
        if (tempNextStr == NULL) {
            fprintf(stderr, "Недостаточно памяти!\n");
            freeTheList();
            return 9;
        }
        tempNextStr->prev = tempPrevStr;
        tempPrevStr->next = tempNextStr;
    }while (1);
}
//+
int readCmd(void){
    char tempCur = '!', tempPrev = '!';
    int firstSymbol = 0, specSymbol = 0;

    userString = NULL;
    userStringSize = 0;
    
    tempCur = getchar();
    tempPrev = tempCur;
    
    if ((tempCur == '#') || (tempCur == '\n'))
    {
        while (tempCur != '\n') {
            tempCur = getchar();
        }
        fprintf(stderr, "Пустая команда!\n");
        return 0;
    }
    
    while (1) {
        if (tempCur != ' '){
            firstSymbol = 1;
            break;
        }
        tempPrev = tempCur;
        tempCur = getchar();
    }
    
    while (1) {
        switch (tempCur) {
            case ' ': {
                if (tempPrev == ' '){
                    tempCur = getchar();
                }
                else {
                    tempPrev = tempCur;
                    userString = (char*)realloc(userString, (userStringSize + 1) * sizeof(char));
                    if (userString == NULL){
                        fprintf(stderr, "Переполнение памяти!\n");
                        free(userString);
                        return 9;
                    }
                    userString[userStringSize] = tempCur;
                    userStringSize++;
                    tempCur = getchar();
                }
                break;
            }
                
            case '"':{
                tempPrev = tempCur;
                tempCur = getchar();
                if (tempCur == '"'){
                    tempPrev = tempCur;
                    tempCur = getchar();
                    if (tempCur == '"'){
                        userString = (char*)realloc(userString, (userStringSize + 1) * sizeof(char));
                        if (userString == NULL){
                            fprintf(stderr, "Переполнение памяти!\n");
                            free(userString);
                            return 9;
                        }
                        userString[userStringSize] = '"';
                        userStringSize++;
                        tempPrev = tempCur;
                        tempCur = getchar();
                        while (1) {
                            if (tempCur == '\\') {
                                tempPrev = tempCur;
                                tempCur = getchar();
                            }
                            
                            if (tempPrev == '\\') {
                                tempPrev = tempCur;
                                userString = (char*)realloc(userString, (userStringSize + 1) * sizeof(char));
                                if (userString == NULL){
                                    fprintf(stderr, "Переполнение памяти!\n");
                                    free(userString);
                                    return 9;
                                }
                                switch (tempCur) {
                                    case 'n':{
                                        userString[userStringSize] = '\n';
                                        userStringSize++;
                                        tempCur = getchar();
                                        break;
                                    }
                                        
                                    case 't':{
                                        userString[userStringSize] = '\t';
                                        userStringSize++;
                                        tempCur = getchar();
                                        break;
                                    }
                                        
                                    case 'b':{
                                        userString[userStringSize] = '\b';
                                        userStringSize++;
                                        tempCur = getchar();
                                        break;
                                    }
                                    
                                    case '\\':{
                                        userString[userStringSize] = '\\';
                                        userStringSize++;
                                        tempCur = getchar();
                                        tempPrev = '!';
                                        break;
                                    }
                                    
                                    case '"':{
                                        userString[userStringSize] = '"';
                                        userStringSize++;
                                        tempCur = getchar();
                                        break;
                                    }
                                        
                                    default:{
                                        userString[userStringSize] = tempCur;
                                        userStringSize++;
                                        tempCur = getchar();
                                        break;
                                    }
                                }
                                continue;
                            }
                            
                            if (tempCur == '"') {
                                tempCur = getchar();
                                if (tempCur == '"') {
                                    tempCur = getchar();
                                    if (tempCur == '"') {
                                        while (tempCur != '\n') {
                                            tempCur = getchar();
                                        }
                                        return 0;
                                    }
                                    else{
                                        fprintf(stderr, "Нарушено сочетание кавычек!\n");
                                        free(userString);
                                        return 0;
                                    }
                                }
                                else{
                                    fprintf(stderr, "Нарушено сочетание кавычек!\n");
                                    free(userString);
                                    return 0;
                                }
                            }
                            else {
                                tempPrev = tempCur;
                                userString = (char*)realloc(userString, (userStringSize + 1) * sizeof(char));
                                if (userString == NULL){
                                    fprintf(stderr, "Переполнение памяти!\n");
                                    free(userString);
                                    return 9;
                                }
                                userString[userStringSize] = tempCur;
                                tempCur = getchar();
                                userStringSize++;
                            }
                        }
                    }
                    else{
                        fprintf(stderr, "Нарушено сочетание кавычек!\n");
                        free(userString);
                        return 0;
                    }
                }
                else{
                    userString = (char*)realloc(userString, (userStringSize + 1) * sizeof(char));
                    if (userString == NULL){
                        fprintf(stderr, "Переполнение памяти!\n");
                        free(userString);
                        return 9;
                    }
                    userString[userStringSize] = '"';
                    userStringSize++;
                    tempPrev = tempCur;
                    tempCur = getchar();
                    while (1) {
                        if (tempCur == '\\') {
                            tempPrev = tempCur;
                            tempCur = getchar();
                        }
                        
                        if (tempPrev == '\\') {
                            tempPrev = tempCur;
                            userString = (char*)realloc(userString, (userStringSize + 1) * sizeof(char));
                            if (userString == NULL){
                                fprintf(stderr, "Переполнение памяти!\n");
                                free(userString);
                                return 9;
                            }
                            switch (tempCur) {
                                case 'n':{
                                    userString[userStringSize] = '\n';
                                    userStringSize++;
                                    tempCur = getchar();
                                    break;
                                }
                                    
                                case 't':{
                                    userString[userStringSize] = '\t';
                                    userStringSize++;
                                    tempCur = getchar();
                                    break;
                                }
                                    
                                case 'b':{
                                    userString[userStringSize] = '\b';
                                    userStringSize++;
                                    tempCur = getchar();
                                    break;
                                }
                                    
                                case '\\':{
                                    userString[userStringSize] = '\\';
                                    userStringSize++;
                                    tempCur = getchar();
                                    tempPrev = '!';
                                    break;
                                }
                                    
                                case '"':{
                                    userString[userStringSize] = '\"';
                                    userStringSize++;
                                    tempCur = getchar();
                                    break;
                                }
                                    
                                default:{
                                    userString[userStringSize] = tempCur;
                                    userStringSize++;
                                    tempCur = getchar();
                                    break;
                                }
                            }
                            continue;
                        }
                        
                        if (tempCur == '"') {
                            while (tempCur != '\n') {
                                tempCur = getchar();
                            }
                            return 0;
                        }
                        else {
                            if (tempCur == '\n'){
                                fprintf(stderr, "Нарушено сочетание кавычек!\n");
                                free(userString);
                                return 0;
                            }
                            tempPrev = tempCur;
                            userString = (char*)realloc(userString, (userStringSize + 1) * sizeof(char));
                            if (userString == NULL){
                                fprintf(stderr, "Переполнение памяти!\n");
                                free(userString);
                                return 9;
                            }
                            userString[userStringSize] = tempCur;
                            tempCur = getchar();
                            userStringSize++;
                        }
                    }
                }
            }
                
            case '#':{
                if (specSymbol) {
                    userString = (char*)realloc(userString, (userStringSize + 1) * sizeof(char));
                    if (userString == NULL){
                        fprintf(stderr, "Переполнение памяти!\n");
                        free(userString);
                        return 9;
                    }
                    userString[userStringSize] = tempCur;
                    userStringSize++;
                    specSymbol = 0;
                    tempPrev = tempCur;
                    tempCur = getchar();
                    break;
                }
                
                while (tempCur != '\n') {
                    tempCur = getchar();
                }
                return 0;
            }
                
            case '\n':{
                return 0;
            }
                
            case '\\':{
                if (specSymbol) {
                    userString = (char*)realloc(userString, (userStringSize + 1) * sizeof(char));
                    if (userString == NULL){
                        fprintf(stderr, "Переполнение памяти!\n");
                        free(userString);
                        return 9;
                    }
                    userString[userStringSize] = '\\';
                    userStringSize++;
                    specSymbol = 0;
                    tempPrev = '\\';
                    tempCur = getchar();
                    break;
                }
                specSymbol = 1;
                tempCur = getchar();
                break;
            }
                
            default: {
                tempPrev = tempCur;
                
                userString = (char*)realloc(userString, (userStringSize + 1) * sizeof(char));
                if (userString == NULL){
                    fprintf(stderr, "Переполнение памяти!\n");
                    free(userString);
                    return 9;
                }
                
                if (specSymbol) {
                    switch (tempCur) {
                        case 'n':{
                            userString[userStringSize] = '\n';
                            userStringSize++;
                            tempCur = getchar();
                            break;
                        }
                        
                        case 't':{
                            userString[userStringSize] = '\t';
                            userStringSize++;
                            tempCur = getchar();
                            break;
                        }
                            
                        case 'b':{
                            userString[userStringSize] = '\b';
                            userStringSize++;
                            tempCur = getchar();
                            break;
                        }
                        
                        case '"':{
                            userString[userStringSize] = '"';
                            userStringSize++;
                            tempCur = getchar();
                            break;
                        }
                            
                        default:{
                            userString[userStringSize] = tempCur;
                            userStringSize++;
                            tempCur = getchar();
                            break;
                        }
                    }
                    
                    specSymbol = 0;
                    break;
                }
                
                userString[userStringSize] = tempCur;
                userStringSize++;
                tempCur = getchar();
                break;
            }
        }
    }
}
//+
void setKeypress(void){
    struct termios new_settings;
    
    tcgetattr(0,&stored_settings);
    
    new_settings = stored_settings;
    
    new_settings.c_lflag &= (~ICANON & ~ECHO);
    new_settings.c_cc[VTIME] = 0;
    new_settings.c_cc[VMIN] = 1;
    
    tcsetattr(0,TCSANOW,&new_settings);
    return;
}
//+
void resetKeypress(void){
    tcsetattr(0,TCSANOW,&stored_settings);
    return;
}
//+
int recognizeCmd(void){ // -1 - неккоректная команда
    char temp = '!';
    int cmdNum = 0, symbolPosition = 0, isCmdCorrect = 1, pararmetrsLengthCounter = 0;
    
    if (userString == NULL) {
        fprintf(stderr, "Пустая команда!\n");
        return 0;
    }
    
    for (cmdNum = 0; cmdNum < 16; cmdNum++){
        isCmdCorrect = 1;
        
        while (temp != '\0') {
            temp = comands[cmdNum][symbolPosition];
            if (temp != '\0') {
                if (temp != userString[symbolPosition]){
                    isCmdCorrect = 0;
                    symbolPosition = 0;
                    break;
                }
                symbolPosition++;
            }
        }
    }
        
    if (isCmdCorrect) {
        if (userString[symbolPosition] == ' '){
            symbolPosition++;
        }
        while (symbolPosition <= userStringSize) {
            parametrs = (char*)realloc(parametrs, (pararmetrsLengthCounter + 1) * sizeof(char));
            if (parametrs == NULL) {
                fprintf(stderr, "Переполнение памяти!\n");
                free(userString);
                userString = NULL;
                return 9;
            }
            parametrs[pararmetrsLengthCounter] = userString[symbolPosition];
            pararmetrsLengthCounter++;
            symbolPosition++;
        }
        free(userString);
        userString = NULL;
        return cmdNum;
    }
    else{
        fprintf(stderr ,"Некорректная команда!\n");
        free(userString);
        userString = NULL;
        return 0;
    }
    return 0;
}
//+
void freeTheList(void){
    struct listOfChars *tmpC;
    struct listOfStrings *tmpS;
    
    tmpStrPointer = pointerForStrings;
    tmpCharPointer = pointerForStrings -> curString;
    
    while (1) {
        if (tmpStrPointer -> next != NULL) {
            tmpS = tmpStrPointer -> next;
            tmpCharPointer = tmpStrPointer -> curString;
            while (1) {
                if (tmpCharPointer -> next != NULL) {
                    tmpC = tmpCharPointer -> next;
                    free(tmpCharPointer);
                    tmpCharPointer = tmpC;
                }
                else{
                    free(tmpCharPointer);
                    break;
                }
            }
            free(tmpStrPointer);
            tmpStrPointer = tmpS;
        }
        else{
            tmpCharPointer = tmpStrPointer -> curString;
            while (1) {
                if (tmpCharPointer -> next != NULL) {
                    tmpC = tmpCharPointer -> next;
                    free(tmpCharPointer);
                    tmpCharPointer = tmpC;
                }
                else{
                    free(tmpCharPointer);
                    break;
                }
            }
            free(tmpStrPointer);
            break;
        }
    }
}