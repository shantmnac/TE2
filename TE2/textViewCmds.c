#include "textViewCmds.h"
#include "assistFunctions.h"
#include "struct.h"
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/ioctl.h>

extern char *comands[], *parametrs, *userString;
extern char fileName[];
extern int screenCol, screenRow, screenNumY, tabWidth, wrapMod;
struct tabDest *whereToPutTab;
extern struct listOfStrings *tmpStrPointer;
extern struct listOfChars *tmpCharPointer;
extern struct listOfStrings *pointerForStrings;
extern struct winsize screenSize;
extern int isItOk;



void printPages(void){
    char temp = '!';
    int i, j = 1, rowNum = 0, colNum = 0, partOfTabToWrite = 0;
   
/*
    ioctl(0, TIOCGWINSZ, &screenSize);
    screenCol = screenSize.ws_col;
    screenRow = screenSize.ws_row;
    
*/
    if ((pointerForStrings == NULL) || (pointerForStrings -> curString == NULL)){
        fprintf(stderr, "Пустой файл!\n");
        free(parametrs);
        parametrs = NULL;
        return;
    }
    
    tmpStrPointer = pointerForStrings;
    tmpCharPointer = pointerForStrings -> curString;
    
    if (!isatty(1)) {
        while (tmpStrPointer != NULL) {
            tmpCharPointer = tmpStrPointer -> curString;
            while (tmpCharPointer != NULL) {
                printf("%c", tmpCharPointer -> curChar);
                tmpCharPointer = tmpCharPointer -> next;
            }
            tmpStrPointer = tmpStrPointer -> next;
        }
    }
    else {
        while ((tmpStrPointer != NULL) && (rowNum < screenRow)) {
            if (wrapMod) {
                while ((tmpCharPointer != NULL)) {
                    switch (tmpCharPointer -> curChar) {
                        case '\t':{
                            partOfTabToWrite = tabWidth;
                            while (1) {
                                if ((screenCol - colNum - 1) >= partOfTabToWrite){
                                    for (i = 0; i < partOfTabToWrite; i++) {
                                        printf(" ");
                                    }
                                    colNum += partOfTabToWrite;
                                    partOfTabToWrite = 0;
                                    break;
                                }
                                else {
                                    for (i = 0; i < screenCol - colNum - 1; i++) {
                                        printf(" ");
                                    }
                                    partOfTabToWrite = partOfTabToWrite - (screenCol - colNum - 1);
                                    rowNum++;
                                    colNum = 0;
                                    
                                    if (rowNum == screenRow) {
                                        break;
                                    }
                                    else{
                                        printf("\n->");
                                        colNum = 2;
                                    }
                                }
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
                                rowNum++;
                                colNum = 0;
                                
                                if (rowNum == screenRow) {
                                    break;
                                }
                                else{
                                    printf("\n->%c", tmpCharPointer -> curChar);
                                    colNum = 3;
                                }
                            }
                            break;
                        }
                    }
                    
                    tmpCharPointer = tmpCharPointer -> next;
                    if (rowNum == screenRow){
                        break;
                    }
                }
                
                if (tmpStrPointer -> next != NULL) {
                    tmpStrPointer = tmpStrPointer -> next;
                    tmpCharPointer = tmpStrPointer -> curString;
                }
                else tmpStrPointer = NULL;
            }
            else {
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
                                for (i = 0; i < screenCol - colNum - 1; i++) {
                                    printf(" ");
                                }
                                printf("\n");
                                rowNum++;
                                whereToPutTab = (struct tabDest*)realloc(whereToPutTab, sizeof(struct tabDest) * j);
                                j++;
                                whereToPutTab -> numOfSpaces = tabWidth - (screenCol - colNum - 1);
                                whereToPutTab -> position = rowNum;
                                colNum = screenCol;
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
                                colNum = screenCol;
                                rowNum++;
                                printf("\n");
                            }
                            break;
                        }
                    }
                    
                    tmpCharPointer = tmpCharPointer -> next;
                    if ((rowNum == screenRow) || (colNum == screenCol)){
                        break;
                    }
                }
                if (rowNum == screenRow) {
                    break;
                }
                
                if (tmpStrPointer -> next != NULL) {
                    tmpStrPointer = tmpStrPointer -> next;
                    tmpCharPointer = tmpStrPointer -> curString;
                    colNum = 0;
                }
                else tmpStrPointer = NULL;
            }
        }
        
        setKeypress();
        do{
            temp = getchar();
            if ((temp != 'q') && (temp != 'Q')) {
                switch (temp) {
                        
                    case ' ': case 's': case 'S':{
                        moveTxtY('D');
                        break;
                    }
                        
                    case '<': case 'a': case 'A':{
                        if (!wrapMod) {
                            moveTxtX('L');
                        }
                        break;
                    }
                        
                    case '>': case 'd': case 'D':{
                        if (!wrapMod) {
                            moveTxtX('R');
                        }
                        break;
                    }
                    
                    case 'w': case 'W':{
                        moveTxtY('U');
                        break;
                    }
                        
                    case 27:{
                        temp = getchar();
                        if (temp == '[') {
                            temp = getchar();
                            switch (temp) {
                                case 'A':{
                                    moveTxtY('U');
                                    break;
                                }
                                    
                                case 'B':{
                                    moveTxtY('D');
                                    break;
                                }
                                    
                                case 'C':{
                                    if (!wrapMod) {
                                        moveTxtX('L');
                                    }
                                    break;
                                }
                                    
                                case 'D':{
                                    if (!wrapMod) {
                                        moveTxtX('R');
                                    }
                                    break;
                                }
                                    
                                default:{
                                    break;
                                }
                            }
                        }
                    }
                            
                    default: break;
                }
            }
        } while ((temp != 'q') && (temp != 'Q'));
    }
    free(whereToPutTab);
    resetKeypress();
    isItOk = 1;
    printf("\n");
}

void printRange(void){
    int i = 0, j = 0, startR = 0, endR = 0;
    struct listOfStrings *prevStr, *nextStr, *firstStr, *lastStr, *veryImportantString = NULL;
    
    if ((pointerForStrings == NULL) || (pointerForStrings -> curString == NULL)){
        fprintf(stderr, "Пустой файл!\n");
        free(parametrs);
        parametrs = NULL;
        return;
    }
    
    if (parametrs == NULL) {
        printPages();
        return;
    }
    
    while (parametrs[i] != ' ') {
        if (parametrs[i] == '\0') {
            break;
        }
        if (isdigit(parametrs[i])) {
            startR = startR * 10 + (int)parametrs[i] - 48;
            i++;
            j++;
        }
        else {
            fprintf(stderr, "Неккоректный параметр!\n");
            free(parametrs);
            parametrs = NULL;
            return;
        }
    }
    
    if (parametrs[i] == ' ') {
        i++;
        j = 0;
        
        while (parametrs[i] != ' ') {
            if (parametrs[i] == '\0') {
                break;
            }
            if (isdigit(parametrs[i])) {
                endR = endR * 10 + (int)parametrs[i] - 48;
                i++;
                j++;
            }
            else {
                fprintf(stderr, "Неккоректный параметр!\n");
                free(parametrs);
                parametrs = NULL;
                return;
            }
        }
        
        if (startR > endR) {
            fprintf(stderr, "Неккоректный параметр!\n");
            free(parametrs);
            parametrs = NULL;
            return;
        }
    }
    else{
        endR = -1;
    }
    
    free(parametrs);
    parametrs = NULL;
    
    tmpStrPointer = pointerForStrings;
    
    for (i = 1; i < startR; i++) {
        if (tmpStrPointer -> next != NULL) {
            tmpStrPointer = tmpStrPointer -> next;
        }
        else {
            fprintf(stderr, "Выход за диапазон строк!\n");
            return;
        }
    }
    
    firstStr = tmpStrPointer;
    prevStr = tmpStrPointer -> prev;
    
    if (endR != -1){
        tmpStrPointer = pointerForStrings;
        
        for (i = 1; i < endR; i++) {
            if (tmpStrPointer -> next != NULL) {
                tmpStrPointer = tmpStrPointer -> next;
            }
            else {
                endR = -1;
                break;
            }
        }
    }
    
    if (endR == -1) {
        firstStr -> prev = NULL;
        prevStr -> next = NULL;
        veryImportantString = pointerForStrings;
        pointerForStrings = firstStr;
        printPages();
        pointerForStrings = veryImportantString;
        firstStr -> prev = prevStr;
        prevStr -> next = firstStr;
        return;
    }
    else {
        lastStr = tmpStrPointer;
        nextStr = tmpStrPointer -> next;
    }
    
    firstStr -> prev = NULL;
    
    if (prevStr != NULL) {
        prevStr -> next = NULL;
    }
    
    lastStr -> next = NULL;
    nextStr -> prev = NULL;
    
    veryImportantString = pointerForStrings;
    pointerForStrings = firstStr;
    
    printPages();
    
    pointerForStrings = veryImportantString;
    firstStr -> prev = prevStr;
    prevStr -> next = firstStr;
    lastStr -> next = nextStr;
    nextStr -> prev = lastStr;
    return;
}

void setWrap(void){
    int i = 0;
    char yesM[3] = "yes";
    char noM[2] = "no";
    
    if (parametrs == NULL) {
        fprintf(stderr, "Пустой параметр!\n");
        return;
    }
    
    while (parametrs[i] != '\0') {
        if (parametrs[i] == yesM[i]){
            i++;
            if (i == 3){
                wrapMod = 1;
                free(parametrs);
                parametrs = NULL;
                return;
            }
        }
        else break;
    }
    
    i = 0;
    while (parametrs[i] != '\0') {
        if (parametrs[i] == noM[i]){
            i++;
            if (i == 2){
                wrapMod = 0;
                free(parametrs);
                parametrs = NULL;
                return;
            }
        }
        else break;
    }
    free(parametrs);
    parametrs = NULL;
    fprintf(stderr, "Некорректный параметр!\n");
}

void setTabWidth(void){
    int temp;
    temp = atoi(parametrs);
    if (temp > 0) {
        tabWidth = temp;
    }
    else{
        fprintf(stderr, "Некорректный параметр!\n");
    }
    free(parametrs);
    parametrs = NULL;
}