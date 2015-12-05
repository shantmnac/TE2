#include "stringEditCmds.h"
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
extern struct listOfStrings *tmpStrPointer;
extern struct listOfChars *tmpCharPointer;
extern struct listOfStrings *pointerForStrings;
extern struct winsize screenSize;
extern int isItOk;

int deleteRange(void){
    int i = 0, j = 0, startR = 0, endR = 0;
    struct listOfStrings *tmpStr, *nextStr, *firstStr;
    
    if ((pointerForStrings == NULL) || (pointerForStrings -> curString == NULL)) {
        return 0;
    }
    
    if ((parametrs == NULL) || (parametrs[i] == '\0')) {
        fprintf(stderr, "Неккоректный параметр!\n");
        return 0;
    }
    
    while ((parametrs[i] != ' ') && (parametrs[i] != '\0')) {
        if (isdigit(parametrs[i])) {
            startR += atoi(&parametrs[i]) * degree(10, j);
            i++;
            j++;
        }
        else {
            fprintf(stderr, "Неккоректный параметр!\n");
            //free(parametrs);
            parametrs = NULL;
            return 0;
        }
    }
    
    j = 0;
    
    if (startR == 0) {
        fprintf(stderr, "Неккоректный параметр!\n");
        //free(parametrs);
        parametrs = NULL;
        return 0;
    }
    
    if (parametrs[i] == ' ') {
        i++;
    }
    
    if (parametrs[i] != '\0') {
        while ((parametrs[i] != ' ') && (parametrs[i] != '\0')) {
            if (isdigit(parametrs[i])) {
                endR += atoi(&parametrs[i]) * degree(10, j);
                i++;
                j++;
            }
            else {
                fprintf(stderr, "Неккоректный параметр!\n");
                //free(parametrs);
                parametrs = NULL;
                return 0;
            }
        }
    }
    else {
        endR = -1;
    }
    
    if (endR != -1) {
        if (startR > endR) {
            fprintf(stderr, "Неккоректный параметр!\n");
            //free(parametrs);
            parametrs = NULL;
            return 0;
        }
    }
    
    nextStr = pointerForStrings;
    
    if (endR == -1) {
        if (startR == 1) {
            pointerForStrings = NULL;
            while (1) {
                if (nextStr -> next != NULL) {
                    tmpStr = nextStr-> next;
                    freeTheString(nextStr -> curString);
                    free(nextStr);
                    nextStr = tmpStr;
                }
                else{
                    freeTheString(nextStr -> curString);
                    free(nextStr);
                    firstStr -> next = NULL;
                    //free(parametrs);
                    parametrs = NULL;
                    return 0;
                }
            }
            //free(parametrs);
            parametrs = NULL;
            return 0;
        }
        else{
            for (i = 1; i < startR; i++) {
                if (nextStr -> next != NULL) {
                    nextStr = nextStr-> next;
                }
                else {
                    fprintf(stderr, "Неккоректный параметр!\n");
                    //free(parametrs);
                    parametrs = NULL;
                    return 0;
                }
            }
            
            firstStr = nextStr -> prev;
            firstStr -> next = NULL;
            
            while (1) {
                if (nextStr -> next != NULL) {
                    tmpStr = nextStr-> next;
                    freeTheString(nextStr -> curString);
                    free(nextStr);
                    nextStr = tmpStr;
                }
                else{
                    freeTheString(nextStr -> curString);
                    free(nextStr);
                    firstStr -> next = NULL;
                    //free(parametrs);
                    parametrs = NULL;
                    return 0;
                }
            }
        }
    }
    else {
        if (startR == 1) {
            for (i = 0; i < endR; i++) {
                if (nextStr -> next != NULL) {
                    tmpStr = nextStr-> next;
                    freeTheString(nextStr -> curString);
                    free(nextStr);
                    nextStr = tmpStr;
                }
                else{
                    freeTheString(nextStr -> curString);
                    free(nextStr);
                    pointerForStrings = NULL;
                    //free(parametrs);
                    parametrs = NULL;
                    return 0;
                }
            }
            nextStr -> prev = NULL;
            pointerForStrings = nextStr;
            //free(parametrs);
            parametrs = NULL;
            return 0;
        }
        else{
            for (i = 1; i < startR; i++) {
                if (nextStr -> next != NULL) {
                    nextStr = nextStr-> next;
                }
                else {
                    fprintf(stderr, "Неккоректный параметр!\n");
                    //free(parametrs);
                    parametrs = NULL;
                    return 0;
                }
            }
            
            firstStr = nextStr -> prev;
            
            for (i = startR; i < (endR + 1); i++) {
                if (nextStr -> next != NULL) {
                    tmpStr = nextStr-> next;
                    freeTheString(nextStr -> curString);
                    free(nextStr);
                    nextStr = tmpStr;
                }
                else{
                    freeTheString(nextStr -> curString);
                    free(nextStr);
                    firstStr -> next = NULL;
                    //free(parametrs);
                    parametrs = NULL;
                    return 0;
                }
            }
            
            firstStr -> next = nextStr;
            nextStr -> prev = firstStr;
            //free(parametrs);
            parametrs = NULL;
            return 0;
        }
    }
}
