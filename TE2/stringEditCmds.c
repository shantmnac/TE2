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

int deleteRange(void){
    int i = 0, j = 0, startR = 0, endR = 0;
    struct listOfStrings *tmpStr, *nextStr, *firstStr;
    
    if ((pointerForStrings == NULL) || (pointerForStrings -> curString == NULL)) {
        //free(parametrs);
        parametrs = NULL;
        return 0;
    }
    
    if ((parametrs == NULL) || (parametrs[i] == '\0')) {
        fprintf(stderr, "Неккоректный параметр!\n");
        return 0;
    }
    
    while ((parametrs[i] != ' ') && (parametrs[i] != '\0')) {
        if (isdigit(parametrs[i])) {
            startR = startR * 10 + (int)parametrs[i] - 48;
            i++;
            j++;
        }
        else {
            fprintf(stderr, "Неккоректный параметр!\n");
            ////free(parametrs);
            parametrs = NULL;
            return 0;
        }
    }
    
    j = 0;
    
    if (startR == 0) {
        fprintf(stderr, "Неккоректный параметр!\n");
        ////free(parametrs);
        parametrs = NULL;
        return 0;
    }
    
    if (parametrs[i] == ' ') {
        i++;
    }
    
    if (parametrs[i] != '\0') {
        while ((parametrs[i] != ' ') && (parametrs[i] != '\0')) {
            if (isdigit(parametrs[i])) {
                endR = endR * 10 + (int)parametrs[i] - 48;
                i++;
                j++;
            }
            else {
                fprintf(stderr, "Неккоректный параметр!\n");
                ////free(parametrs);
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
            ////free(parametrs);
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
                    ////free(parametrs);
                    parametrs = NULL;
                    return 0;
                }
            }
            ////free(parametrs);
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
                    ////free(parametrs);
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
                    ////free(parametrs);
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
                    ////free(parametrs);
                    parametrs = NULL;
                    return 0;
                }
            }
            nextStr -> prev = NULL;
            pointerForStrings = nextStr;
            ////free(parametrs);
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
                    ////free(parametrs);
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
                    ////free(parametrs);
                    parametrs = NULL;
                    return 0;
                }
            }
            
            firstStr -> next = nextStr;
            nextStr -> prev = firstStr;
            ////free(parametrs);
            parametrs = NULL;
            return 0;
        }
    }
}

int editString(void){
    int strPosition = 0, charPosition = 0, i = 0, j = 0;
    char insChar;
    struct listOfStrings *strPointer, *nextStrPointer, *tmpStrPointer;
    struct listOfChars *charPointer;
    
    if ((pointerForStrings == NULL) || (pointerForStrings -> curString == NULL)) {
        //free(parametrs);
        parametrs = NULL;
        return 0;
    }
    
    if (parametrs == NULL){
        fprintf(stderr, "Неккоректный параметр!\n");
        return 0;
    }
    
    if (parametrs == NULL){
        fprintf(stderr, "Неккоректный параметр!\n");
        //free(parametrs);
        return 0;
    }
    
    while (parametrs[i] != ' ') {
        if (parametrs[i] == '\0') {
            fprintf(stderr, "Неккоректный параметр!\n");
            //free(parametrs);
            parametrs = NULL;
            return 0;
        }
        if (isdigit(parametrs[i])) {
            strPosition = strPosition * 10 + (int)parametrs[i] - 48;
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
        
    if (parametrs[i] == ' ') {
        i++;
    }
    
    j = 0;
    
    while (parametrs[i] != ' ') {
        if (parametrs[i] == '\0') {
            fprintf(stderr, "Неккоректный параметр!\n");
            //free(parametrs);
            parametrs = NULL;
            return 0;
        }
        if (isdigit(parametrs[i])) {
            charPosition = charPosition * 10 + (int)parametrs[i] - 48;
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
    
    if (parametrs[i] == ' ') {
        i++;
    }
    
    if (parametrs[i] == '\\') {
        i++;
        switch (parametrs[i]) {
            case 'n':{
                insChar = '\n';
                break;
            }
                
            case 't':{
                insChar = '\t';
                break;
            }
                
            case 'b':{
                insChar = '\b';
                break;
            }
                
            case '"':{
                insChar = '"';
                break;
            }
                
            case '\\':{
                insChar = '\\';
                break;
            }
                
            default:{
                insChar = parametrs[i];
                break;
            }
        }
    }
    else {
        insChar = parametrs[i];
    }
    
    //free(parametrs);
    parametrs = NULL;
    
    strPointer = pointerForStrings;
    
    for (i = 1; i < strPosition; i++) {
        if (strPointer -> next != NULL) {
            strPointer = strPointer -> next;
        }
        else {
            fprintf(stderr, "Неккоректный параметр!\n");
            return 0;
        }
    }
    
    charPointer = strPointer -> curString;
    
    for (i = 1; i < charPosition; i++) {
        if (charPointer -> next != NULL) {
            charPointer = charPointer -> next;
        }
        else {
            fprintf(stderr, "Неккоректный параметр!\n");
            return 0;
        }
    }
    
    if (insChar != '\n') {
        charPointer -> curChar = insChar;
        return 0;
    }
    else{
        if (charPosition == 1) {
            if (charPointer -> next != NULL){
                if (strPointer -> next != NULL) {
                    charPointer -> curChar = '\n';
                    nextStrPointer = strPointer -> next;
                    tmpStrPointer = (struct listOfStrings*)malloc(sizeof(struct listOfStrings));
                    tmpStrPointer -> curString = charPointer -> next;
                    tmpStrPointer -> curString -> prev = NULL;
                    tmpStrPointer -> prev = strPointer;
                    tmpStrPointer -> next = nextStrPointer;
                    strPointer -> next = tmpStrPointer;
                    nextStrPointer -> prev = tmpStrPointer;
                    charPointer -> next = NULL;
                    return 0;
                }
                else {
                    charPointer -> curChar = '\n';
                    tmpStrPointer = (struct listOfStrings*)malloc(sizeof(struct listOfStrings));
                    tmpStrPointer -> curString = charPointer -> next;
                    tmpStrPointer -> curString -> prev = NULL;
                    tmpStrPointer -> prev = strPointer;
                    tmpStrPointer -> next = NULL;
                    strPointer -> next = tmpStrPointer;
                    charPointer -> next = NULL;
                    return 0;
                }
            }
            else {
                charPointer -> curChar = insChar;
                return 0;
            }
        }
        else {
            if (charPointer -> next == NULL){
                if (charPointer -> curChar != '\n') {
                    if (strPointer -> next != NULL) {
                        charPointer -> curChar = '\n';
                        nextStrPointer = strPointer -> next;
                        tmpStrPointer = (struct listOfStrings*)malloc(sizeof(struct listOfStrings));
                        tmpStrPointer -> curString = (struct listOfChars*)malloc(sizeof(struct listOfChars));
                        tmpStrPointer -> curString -> curChar = '\n';
                        tmpStrPointer -> curString -> prev = NULL;
                        tmpStrPointer -> curString -> next = NULL;
                        tmpStrPointer -> prev = strPointer;
                        strPointer -> next = tmpStrPointer;
                        tmpStrPointer -> next = nextStrPointer;
                        nextStrPointer -> prev = tmpStrPointer;
                        return 0;
                    }
                    else {
                        charPointer -> curChar = '\n';
                        nextStrPointer = NULL;
                        tmpStrPointer = (struct listOfStrings*)malloc(sizeof(struct listOfStrings));
                        tmpStrPointer -> curString = (struct listOfChars*)malloc(sizeof(struct listOfChars));
                        tmpStrPointer -> curString -> curChar = '\n';
                        tmpStrPointer -> curString -> prev = NULL;
                        tmpStrPointer -> curString -> next = NULL;
                        tmpStrPointer -> prev = strPointer;
                        strPointer -> next = tmpStrPointer;
                        tmpStrPointer -> next = NULL;
                        return 0;
                    }
                }
                else{
                    return 0;
                }
            }
            else{
                if (strPointer -> next != NULL) {
                    charPointer -> curChar = '\n';
                    nextStrPointer = strPointer -> next;
                    tmpStrPointer = (struct listOfStrings*)malloc(sizeof(struct listOfStrings));
                    tmpStrPointer -> curString = charPointer -> next;
                    tmpStrPointer -> curString -> prev = NULL;
                    tmpStrPointer -> prev = strPointer;
                    tmpStrPointer -> next = nextStrPointer;
                    strPointer -> next = tmpStrPointer;
                    nextStrPointer -> prev = tmpStrPointer;
                    charPointer -> next = NULL;
                    return 0;
                }
                else {
                    charPointer -> curChar = '\n';
                    tmpStrPointer = (struct listOfStrings*)malloc(sizeof(struct listOfStrings));
                    tmpStrPointer -> curString = charPointer -> next;
                    tmpStrPointer -> curString -> prev = NULL;
                    tmpStrPointer -> prev = strPointer;
                    tmpStrPointer -> next = NULL;
                    strPointer -> next = tmpStrPointer;
                    charPointer -> next = NULL;
                    return 0;
                }
            }
        }
    }
}

int insertSymbol(void){
    int strPosition = 0, charPosition = 0, i = 0, j = 0;
    char insChar;
    struct listOfStrings *strPointer, *tmpStrPointer;
    struct listOfChars *charPointer, *tmpCharPointer;
    
    if (parametrs == NULL){
        fprintf(stderr, "Неккоректный параметр!\n");
        return 0;
    }
    
    if (parametrs == NULL){
        fprintf(stderr, "Неккоректный параметр!\n");
        //free(parametrs);
        return 0;
    }
    
    while (parametrs[i] != ' ') {
        if (parametrs[i] == '\0') {
            fprintf(stderr, "Неккоректный параметр!\n");
            //free(parametrs);
            parametrs = NULL;
            return 0;
        }
        if (isdigit(parametrs[i])) {
            strPosition = strPosition * 10 + (int)parametrs[i] - 48;
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
    
    if (parametrs[i] == ' ') {
        i++;
    }
    
    j = 0;
    
    while (parametrs[i] != ' ') {
        if (parametrs[i] == '\0') {
            fprintf(stderr, "Неккоректный параметр!\n");
            //free(parametrs);
            parametrs = NULL;
            return 0;
        }
        if (isdigit(parametrs[i])) {
            charPosition = charPosition * 10 + (int)parametrs[i] - 48;
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
    
    if (parametrs[i] == ' ') {
        i++;
    }
    
    if (parametrs[i] == '\\') {
        i++;
        switch (parametrs[i]) {
            case 'n':{
                insChar = '\n';
                break;
            }
                
            case 't':{
                insChar = '\t';
                break;
            }
                
            case 'b':{
                insChar = '\b';
                break;
            }
                
            case '"':{
                insChar = '"';
                break;
            }
                
            case '\\':{
                insChar = '\\';
                break;
            }
                
            default:{
                insChar = parametrs[i];
                break;
            }
        }
    }
    else {
        insChar = parametrs[i];
    }
    
    //free(parametrs);
    parametrs = NULL;
    
    strPointer = pointerForStrings;
    
    
    
    for (i = 1; i < strPosition; i++) {
        if (strPointer -> next != NULL) {
            strPointer = strPointer -> next;
        }
        else {
            fprintf(stderr, "Неккоректный параметр!\n");
            return 0;
        }
    }
    
    if (strPointer -> curString != NULL) {
        charPointer = strPointer -> curString;
    }
    else{
        strPointer -> curString = (struct listOfChars*)malloc(sizeof(struct listOfChars));
        charPointer = strPointer -> curString;
        charPointer -> next = NULL;
        charPointer -> prev = NULL;
        charPointer -> curChar = insChar;
        return 0;
    }
    
    for (i = 1; i < charPosition; i++) {
        if (charPointer -> next != NULL) {
            charPointer = charPointer -> next;
        }
        else {
            charPosition = -1;
            break;
        }
    }
    
    if (insChar != '\n') {
        if (charPosition != -1) {
            tmpCharPointer = (struct listOfChars*)malloc(sizeof(struct listOfChars));
            tmpCharPointer -> curChar = insChar;
            if (charPointer -> prev != NULL) {
                tmpCharPointer -> prev = charPointer -> prev;
                tmpCharPointer -> prev -> next = tmpCharPointer;
            }
            else {
                strPointer -> curString = tmpCharPointer;
                tmpCharPointer -> prev = NULL;
            }
            tmpCharPointer -> next = charPointer;
            charPointer -> prev = tmpCharPointer;
            return 0;
        }
        else{
            tmpCharPointer = (struct listOfChars*)malloc(sizeof(struct listOfChars));
            tmpCharPointer -> curChar = insChar;
            tmpCharPointer -> prev = charPointer;
            tmpCharPointer -> next = NULL;
            charPointer -> next = tmpCharPointer;
            return 0;
        }
    }
    else{
        if (charPosition == 1) {
            tmpCharPointer = (struct listOfChars*)malloc(sizeof(struct listOfChars));
            tmpCharPointer -> curChar = '\n';
            tmpCharPointer -> next = NULL;
            tmpCharPointer -> prev = NULL;
            strPointer -> curString = tmpCharPointer;
            
            tmpStrPointer = (struct listOfStrings*)malloc(sizeof(struct listOfStrings));
            tmpStrPointer -> curString = charPointer;
            charPointer -> prev = NULL;
            
            tmpStrPointer -> prev = strPointer;
            tmpStrPointer -> next = strPointer -> next;
            strPointer -> next = tmpStrPointer;
            tmpStrPointer -> next -> prev = tmpStrPointer;
            return 0;
        }
        else {
            if (charPosition == -1) {
                tmpCharPointer = (struct listOfChars*)malloc(sizeof(struct listOfChars));
                tmpCharPointer -> curChar = insChar;
                tmpCharPointer -> prev = charPointer;
                charPointer -> next = tmpCharPointer;
                tmpCharPointer -> next = NULL;
                return 0;
            }
            else{
                tmpCharPointer = (struct listOfChars*)malloc(sizeof(struct listOfChars));
                tmpCharPointer -> curChar = '\n';
                tmpCharPointer -> next = NULL;
                tmpCharPointer -> prev = charPointer -> prev;
                charPointer -> prev -> next = tmpCharPointer;
                
                tmpStrPointer = (struct listOfStrings*)malloc(sizeof(struct listOfStrings));
                tmpStrPointer -> curString = charPointer;
                charPointer -> prev = NULL;
                
                tmpStrPointer -> prev = strPointer;
                tmpStrPointer -> next = strPointer -> next;
                strPointer -> next = tmpStrPointer;
                tmpStrPointer -> next -> prev = tmpStrPointer;
                return 0;
            }
        }
    }
}