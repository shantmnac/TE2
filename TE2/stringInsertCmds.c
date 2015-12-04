#include "stringInsertCmds.h"
#include "assistFunctions.h"
#include "struct.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

extern char *parametrs;
extern struct listOfStrings *tmpStrPointer;
extern struct listOfChars *tmpCharPointer;
extern struct listOfStrings *pointerForStrings;


int insertAfter(void){
    int i = 0, strNum = 0, firstIteration = 1;;
    struct listOfStrings *tempPrevStr = NULL, *tempNextStr = NULL, *begPointer = NULL, *tempStr = NULL;
    struct listOfChars *tempPrevChar = NULL, *tempNextChar = NULL;
    
    if (parametrs == NULL) {
        fprintf(stderr, "Неккоректный параметр!\n");
        return 0;
    }
    
    if (parametrs[0] != '"') {
        while ((parametrs[i] != ' ') && (parametrs[i] != '"')) {
            if (parametrs[i] == '\0') {
                fprintf(stderr, "Неккоректный параметр!\n");
                free(parametrs);
                parametrs = NULL;
                return 0;
            }
            if (isdigit(parametrs[i])) {
                strNum += atoi(&parametrs[i]) * degree(10, i);
                i++;
            }
            else {
                fprintf(stderr, "Неккоректный параметр!\n");
                free(parametrs);
                parametrs = NULL;
                return 0;
            }
        }
        
        if (parametrs[i] == ' ') {
            i++;
        }
        
        if (parametrs[i] == '"') {
            i++;
        }
        else {
            fprintf(stderr, "Неккоректный параметр!\n");
            free(parametrs);
            parametrs = NULL;
            return 0;
        }
    }
    else {
        i++;
        strNum = -1;
    }
    
    if (strNum < -1) {
        fprintf(stderr, "Неккоректный параметр!\n");
        free(parametrs);
        parametrs = NULL;
        return 0;
    }
    
    tempNextStr = (struct listOfStrings*)malloc(sizeof(struct listOfStrings));
    tempNextStr->prev = NULL;
    begPointer = tempNextStr;
    
    do{
        while(parametrs[i] != '\n'){
            if (parametrs[i] == '\0') {
                break;
            }
            
            tempNextChar = (struct listOfChars*)malloc(sizeof(struct listOfChars));
            if (tempNextChar == NULL) {
                fprintf(stderr, "Недостаточно памяти!\n");
                free(parametrs);
                parametrs = NULL;
                return 9;
            }
            
            if (firstIteration){
                tempNextStr->curString = tempNextChar;
                tempNextChar->prev = NULL;
                tempNextChar->curChar = parametrs[i];
                tempPrevChar = tempNextChar;
                firstIteration = 0;
                i++;
                continue;
            }
            
            tempNextChar->curChar = parametrs[i];
            tempNextChar->prev = tempPrevChar;
            tempPrevChar->next = tempNextChar;
            tempPrevChar = tempNextChar;
            i++;
        }
        
        if (parametrs[i] == '\0') {
            if (firstIteration) {
                free(tempNextStr);
                tempNextStr = tempPrevStr;
            }
            break;
        }
        
        tempNextChar = (struct listOfChars*)malloc(sizeof(struct listOfChars));
        if (tempNextChar == NULL) {
            fprintf(stderr, "Недостаточно памяти!\n");
            free(parametrs);
            parametrs = NULL;
            return 9;
        }
        
        tempNextChar->curChar = '\n';
        tempNextChar->prev = tempPrevChar;
        tempPrevChar->next = tempNextChar;
        tempPrevChar = tempNextChar;
        i++;
        
        firstIteration = 1;
        tempNextChar->next = NULL;
        tempPrevStr = tempNextStr;
        tempNextStr = (struct listOfStrings*)malloc(sizeof(struct listOfStrings));
        if (tempNextStr == NULL) {
            fprintf(stderr, "Недостаточно памяти!\n");
            free(parametrs);
            parametrs = NULL;
            return 9;
        }
        
        tempNextStr->prev = tempPrevStr;
        tempPrevStr->next = tempNextStr;

    }while (1);
    
    if (strNum == -1) {
        tempStr = pointerForStrings;
        while (tempStr -> next != NULL) {
            tempStr = tempStr -> next;
        }
        tempNextStr -> next = NULL;
        tempStr -> next = begPointer;
        begPointer -> prev = tempStr;
        //free(parametrs);
        parametrs = NULL;
        return 0;
    }
    else {
        if (strNum == 0){
            tempNextStr -> next = pointerForStrings;
            pointerForStrings -> prev = tempNextStr;
            pointerForStrings = tempNextStr;
            //free(parametrs);
            parametrs = NULL;
            return 0;
        }
        else {
            tempStr = pointerForStrings;
            for (i = 0; i < strNum; i++) {
                if (tempStr -> next != NULL) {
                    tempStr = tempStr -> next;
                }
                else {
                    tempNextStr -> next = NULL;
                    tempStr -> next = begPointer;
                    begPointer -> prev = tempStr;
                    //free(parametrs);
                    parametrs = NULL;
                    return 0;
                }
            }
            tempNextStr -> next = tempStr -> next;
            tempStr -> next = begPointer;
            begPointer -> prev = tempStr;
            //free(parametrs);
            parametrs = NULL;
            return 0;
        }
    }
    return 0;
}