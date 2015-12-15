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

extern char *comands[], *parametrs, *userString, *secondStringForReplace;
extern char fileName[];
extern int screenCol, screenRow, screenNumY, tabWidth, wrapMod, isFileSaved;
extern struct listOfStrings *tmpStrPointer;
extern struct listOfChars *tmpCharPointer;
extern struct listOfStrings *pointerForStrings;
extern struct winsize screenSize;

void deleteRange(void){
    int i = 0, j = 0, startR = 0, endR = 0;
    struct listOfStrings *tmpStr, *nextStr, *firstStr;
    
    if ((pointerForStrings == NULL) || (pointerForStrings -> curString == NULL)) {
        //free(parametrs);
        parametrs = NULL;
        return;
    }
    
    if ((parametrs == NULL) || (parametrs[i] == '\0')) {
        fprintf(stderr, "Неккоректный параметр!\n");
        return;
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
            return;
        }
    }
    
    j = 0;
    
    if (startR == 0) {
        fprintf(stderr, "Неккоректный параметр!\n");
        ////free(parametrs);
        parametrs = NULL;
        return;
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
                return;
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
            return;
        }
    }
    
    nextStr = pointerForStrings;
    isFileSaved = 0;
    
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
                    return;
                }
            }
            ////free(parametrs);
            parametrs = NULL;
            return;
        }
        else{
            for (i = 1; i < startR; i++) {
                if (nextStr -> next != NULL) {
                    nextStr = nextStr-> next;
                }
                else {
                    fprintf(stderr, "Неккоректный параметр!\n");
                    isFileSaved = 1;
                    ////free(parametrs);
                    parametrs = NULL;
                    return;
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
                    return;
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
                    return;
                }
            }
            nextStr -> prev = NULL;
            pointerForStrings = nextStr;
            ////free(parametrs);
            parametrs = NULL;
            return;
        }
        else{
            for (i = 1; i < startR; i++) {
                if (nextStr -> next != NULL) {
                    nextStr = nextStr-> next;
                }
                else {
                    fprintf(stderr, "Неккоректный параметр!\n");
                    isFileSaved = 1;
                    ////free(parametrs);
                    parametrs = NULL;
                    return;
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
                    return;
                }
            }
            
            firstStr -> next = nextStr;
            nextStr -> prev = firstStr;
            ////free(parametrs);
            parametrs = NULL;
            return;
        }
    }
}

void editString(void){
    int strPosition = 0, charPosition = 0, i = 0, j = 0;
    char insChar;
    struct listOfStrings *strPointer, *nextStrPointer, *tmpStrPointer;
    struct listOfChars *charPointer;
    
    if ((pointerForStrings == NULL) || (pointerForStrings -> curString == NULL)) {
        //free(parametrs);
        parametrs = NULL;
        return;
    }
    
    if (parametrs == NULL){
        fprintf(stderr, "Неккоректный параметр!\n");
        return;
    }
    
    if (parametrs == NULL){
        fprintf(stderr, "Неккоректный параметр!\n");
        //free(parametrs);
        return;
    }
    
    while (parametrs[i] != ' ') {
        if (parametrs[i] == '\0') {
            fprintf(stderr, "Неккоректный параметр!\n");
            //free(parametrs);
            parametrs = NULL;
            return;
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
            return;
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
            return;
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
            return;
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
    isFileSaved = 0;
    
    for (i = 1; i < strPosition; i++) {
        if (strPointer -> next != NULL) {
            strPointer = strPointer -> next;
        }
        else {
            fprintf(stderr, "Неккоректный параметр!\n");
            isFileSaved = 1;
            return;
        }
    }
    
    charPointer = strPointer -> curString;
    
    for (i = 1; i < charPosition; i++) {
        if (charPointer -> next != NULL) {
            charPointer = charPointer -> next;
        }
        else {
            fprintf(stderr, "Неккоректный параметр!\n");
            isFileSaved = 1;
            return;
        }
    }
    
    if (insChar != '\n') {
        charPointer -> curChar = insChar;
        return;
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
                    return;
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
                    return;
                }
            }
            else {
                charPointer -> curChar = insChar;
                return;
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
                        return;
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
                        return;
                    }
                }
                else{
                    return;
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
                    return;
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
                    return;
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
    isFileSaved = 0;
    
    for (i = 1; i < strPosition; i++) {
        if (strPointer -> next != NULL) {
            strPointer = strPointer -> next;
        }
        else {
            fprintf(stderr, "Неккоректный параметр!\n");
            isFileSaved = 1;
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

void deleteBraces(void){
    int i = 0, j = 0, startR = 0, endR = 0, bracesCounter = 0, iterationCounter = 0, toDelete = 0, toMergeStrings = 0, braceOnTheStartOfString = 0, inTheBraces = 0;
    struct listOfChars *charPointer, *nextCharPointer, *startCharPointer = NULL;
    struct listOfStrings *strPointer;
    
    if ((pointerForStrings == NULL) || (pointerForStrings -> curString == NULL)) {
        free(parametrs);
        parametrs = NULL;
        return;
    }
    
    if (parametrs != NULL) {
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
                //free(parametrs);
                parametrs = NULL;
                return;
            }
        }
        else{
            endR = -1;
        }
        
        //free(parametrs);
        parametrs = NULL;
    } 
    
    if (endR != -1) {
        iterationCounter = endR - startR + 1;
    }
    
    strPointer = pointerForStrings;
    isFileSaved = 0;
    
    for (i = 1; i < startR; i++) {
        if (strPointer -> next != NULL) {
            strPointer = strPointer -> next;
        }
        else {
            fprintf(stderr, "Выход за диапазон строк!\n");
            return;
        }
    }
    
    charPointer = strPointer -> curString;
    
    if (iterationCounter != 0) {
        for (i = 0; i < iterationCounter; i++) {
            if (charPointer != NULL) {
                while (1) {
                    if (charPointer -> curChar != '{') {
                        if (charPointer -> curChar == '}') {
                            bracesCounter--;
                        }
                        
                        if ((!bracesCounter) && (inTheBraces)) {
                            if (charPointer -> next != NULL) {
                                nextCharPointer = charPointer -> next;
                                free(charPointer);
                                charPointer = nextCharPointer;
                                
                                if (braceOnTheStartOfString) {
                                    strPointer -> curString = charPointer;
                                    charPointer -> prev = NULL;
                                    braceOnTheStartOfString = 0;
                                }
                                else {
                                    startCharPointer -> next = charPointer;
                                    charPointer -> prev = startCharPointer;
                                }
                                toDelete = 0;
                                inTheBraces = 0;
                            }
                            else{
                                free(charPointer);
                                
                                if (braceOnTheStartOfString) {
                                    strPointer -> curString = NULL;
                                    braceOnTheStartOfString = 0;
                                }
                                else {
                                    startCharPointer -> next = NULL;
                                }
                                
                                toDelete = 0;
                                inTheBraces = 0;
                                break;
                            }
                        }
                        
                        if (toDelete) {
                            if (charPointer -> next != NULL) {
                                nextCharPointer = charPointer -> next;
                                free(charPointer);
                                charPointer = nextCharPointer;
                            }
                            else{
                                free(charPointer);
                                if (strPointer -> next != NULL) {
                                    charPointer = strPointer -> next -> curString;
                                    toMergeStrings = 1;
                                }
                                else {
                                    if (braceOnTheStartOfString) {
                                        strPointer -> curString = NULL;;
                                    }
                                    else {
                                        startCharPointer -> next = NULL;
                                    }
                                }
                                break;
                            }
                        }
                        else {
                            if (charPointer -> next != NULL) {
                                charPointer = charPointer -> next;
                            }
                            else {
                                break;
                            }
                        }
                    }
                    else{
                        bracesCounter++;
                        if (bracesCounter == 1) {
                            inTheBraces = 1;
                            toDelete = 1;
                            
                            if (charPointer -> prev != NULL) {
                                startCharPointer = charPointer -> prev;
                            }
                            else{
                                braceOnTheStartOfString = 1;
                            }
                        }
                        
                        if (charPointer -> next != NULL) {
                            nextCharPointer = charPointer -> next;
                            free(charPointer);
                            charPointer = nextCharPointer;
                        }
                        else {
                            free(charPointer);
                            if (strPointer -> next != NULL) {
                                charPointer = strPointer -> next -> curString;
                                toMergeStrings = 1;
                            }
                            else {
                                charPointer = NULL;
                                
                                if(braceOnTheStartOfString){
                                    
                                }
                                break;
                            }
                            break;
                        }
                    }
                }
            }
            
            if ((strPointer -> next != NULL) && (strPointer -> curString != NULL)) {
                if (toMergeStrings) {
                    if (strPointer -> next -> next != NULL) {
                        strPointer -> next = strPointer -> next -> next;
                        free(strPointer -> next -> prev);
                        strPointer -> next -> prev = strPointer;
                    }
                    else {
                        free(strPointer -> next);
                        strPointer -> next = NULL;
                    }
                    toMergeStrings = 0;
                }
                else{
                    strPointer = strPointer -> next;
                    charPointer = strPointer -> curString;
                }
            }
            else{
                break;
            }
        }
    }
    else{
        for (; ;) {
            if (charPointer != NULL) {
                while (1) {
                    if (charPointer -> curChar != '{') {
                        if (charPointer -> curChar == '}') {
                            bracesCounter--;
                        }
                        
                        if ((!bracesCounter) && (inTheBraces)) {
                            if (charPointer -> next != NULL) {
                                nextCharPointer = charPointer -> next;
                                free(charPointer);
                                charPointer = nextCharPointer;
                                
                                if (braceOnTheStartOfString) {
                                    strPointer -> curString = charPointer;
                                    charPointer -> prev = NULL;
                                    braceOnTheStartOfString = 0;
                                }
                                else {
                                    startCharPointer -> next = charPointer;
                                    charPointer -> prev = startCharPointer;
                                }
                                toDelete = 0;
                                inTheBraces = 0;
                            }
                            else{
                                free(charPointer);
                                
                                if (braceOnTheStartOfString) {
                                    strPointer -> curString = NULL;
                                    braceOnTheStartOfString = 0;
                                }
                                else {
                                    startCharPointer -> next = NULL;
                                }
                                
                                toDelete = 0;
                                inTheBraces = 0;
                                break;
                            }
                        }
                        
                        if (toDelete) {
                            if (charPointer -> next != NULL) {
                                nextCharPointer = charPointer -> next;
                                free(charPointer);
                                charPointer = nextCharPointer;
                            }
                            else{
                                free(charPointer);
                                if (strPointer -> next != NULL) {
                                    charPointer = strPointer -> next -> curString;
                                    toMergeStrings = 1;
                                }
                                else {
                                    if (braceOnTheStartOfString) {
                                        strPointer -> curString = NULL;;
                                    }
                                    else {
                                        startCharPointer -> next = NULL;
                                    }
                                }
                                break;
                            }
                        }
                        else {
                            if (charPointer -> next != NULL) {
                                charPointer = charPointer -> next;
                            }
                            else {
                                break;
                            }
                        }
                    }
                    else{
                        bracesCounter++;
                        if (bracesCounter == 1) {
                            inTheBraces = 1;
                            toDelete = 1;
                            
                            if (charPointer -> prev != NULL) {
                                startCharPointer = charPointer -> prev;
                            }
                            else{
                                braceOnTheStartOfString = 1;
                            }
                        }
                        
                        if (charPointer -> next != NULL) {
                            nextCharPointer = charPointer -> next;
                            free(charPointer);
                            charPointer = nextCharPointer;
                        }
                        else {
                            free(charPointer);
                            if (strPointer -> next != NULL) {
                                charPointer = strPointer -> next -> curString;
                                toMergeStrings = 1;
                            }
                            else {
                                charPointer = NULL;
                                
                                if(braceOnTheStartOfString){
                                    
                                }
                                break;
                            }
                            break;
                        }
                    }
                }
            }
            
            if ((strPointer -> next != NULL) && (strPointer -> curString != NULL)) {
                if (toMergeStrings) {
                    if (strPointer -> next -> next != NULL) {
                        strPointer -> next = strPointer -> next -> next;
                        free(strPointer -> next -> prev);
                        strPointer -> next -> prev = strPointer;
                    }
                    else {
                        free(strPointer -> next);
                        strPointer -> next = NULL;
                    }
                    toMergeStrings = 0;
                }
                else{
                    strPointer = strPointer -> next;
                    charPointer = strPointer -> curString;
                }
            }
            else{
                break;
            }
        }
    }
}

/*int replaceSubstring(void){
    int i = 0, j = 0, startR = 0, endR = 0, putOnEnd = 0, putOnStart = 0, firstIteration = 1, firstString = 1, iterationCounter = 0, coolSubstring = 0;
    char *substring1 = NULL;
    struct listOfChars *tmpSubstring = NULL, *tmpPrevSubstring = NULL, *begPointer = NULL, *endPointer = NULL, *firstCharUntilReplace = NULL;
    struct listOfStrings *newString = NULL;
    
    if ((pointerForStrings == NULL) || (pointerForStrings -> curString == NULL)){
        fprintf(stderr, "Пустой файл!\n");
        //free(parametrs);
        parametrs = NULL;
        return 0;
    }
    
    if (secondStringForReplace == NULL) {
        fprintf(stderr, "Неккоректная строка на замену!\n");
        free(parametrs);
        parametrs = NULL;
        return 0;
    }
    
    if (parametrs != NULL) {
        if ((parametrs[i] != '"') && (parametrs[i] != '^') && (parametrs[i] != '$')) {
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
                    break;
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
                        //free(parametrs);
                        parametrs = NULL;
                        return 0;
                    }
                }
                
                if (startR > endR) {
                    fprintf(stderr, "Неккоректный параметр!\n");
                    //free(parametrs);
                    parametrs = NULL;
                    return 0;
                }
                else{
                    iterationCounter = endR - startR + 1;
                }
            }
            else{
                endR = -1;
            }
        }
    }
    else{
        return 0;
    }
    
    if (parametrs[i] == ' ') {
        i++;
    }
    
    j = 0;
    
    if (parametrs[i] == '$') {
        putOnEnd = 1;
    }
    else {
        if (parametrs[i] == '^') {
            putOnStart = 1;
        }
        else{
            if (parametrs[i] == '"') {
                i++;
                while (parametrs[i] != '\0') {
                    substring1 = (char*)realloc(substring1, j + 1);
                    substring1[j] = parametrs[i];
                    i++;
                    j++;
                }
                
                if (j == 0) {
                    fprintf(stderr, "Неккоректный параметр!\n");
                    free(parametrs);
                    parametrs = NULL;
                    return 0;
                }
                
                substring1 = (char*)realloc(substring1, j + 1);
                substring1[j] = '\0';
                
                free(parametrs);
                parametrs = NULL;
            }
            else{
                fprintf(stderr, "Неккоректный параметр!\n");
                free(parametrs);
                parametrs = NULL;
                return 0;
            }
        }
    }
    
    tmpStrPointer = pointerForStrings;
    
    j = 0;
    isFileSaved = 0;
    
    for (i = 1; i < startR; i++) {
        if (tmpStrPointer -> next != NULL) {
            tmpStrPointer = tmpStrPointer -> next;
        }
        else {
            fprintf(stderr, "Выход за диапазон строк!\n");
            return 0;
        }
    }
    
    tmpCharPointer = tmpStrPointer -> curString;
    
    if (putOnStart) {
        if (!iterationCounter) {
            while (1) {
                if ((tmpStrPointer != NULL) && (tmpCharPointer != NULL)) {
                    while(secondStringForReplace[j] != '\0'){
                        
                        tmpSubstring = (struct listOfChars*)malloc(sizeof(struct listOfChars));
                        
                        if (firstIteration){
                            begPointer = tmpSubstring;
                            tmpSubstring->prev = NULL;
                            tmpPrevSubstring = tmpSubstring;
                            firstIteration = 0;
                        }
                        
                        tmpSubstring->curChar = secondStringForReplace[j];
                        tmpSubstring->prev = tmpPrevSubstring;
                        tmpPrevSubstring->next = tmpSubstring;
                        tmpPrevSubstring = tmpSubstring;
                        
                        if (secondStringForReplace[j] == '\n') {
                            tmpSubstring -> next = NULL;
                            newString = (struct listOfStrings*)malloc(sizeof(struct listOfStrings));
                            newString -> prev = tmpStrPointer;
                            newString -> next = tmpStrPointer -> next;
                            tmpStrPointer -> next -> prev = newString;
                            tmpStrPointer -> next = newString;
                            newString -> curString = tmpCharPointer;
                            firstIteration = 1;
                        }
                        j++;
                    }
                    
                    tmpSubstring -> next = tmpCharPointer;
                    tmpCharPointer -> prev = tmpSubstring;
                    
                    if (firstString){
                        pointerForStrings -> curString = begPointer;
                        firstString = 0;
                    }
                    else{
                        tmpStrPointer -> curString = begPointer;
                    }
                    
                    if (tmpStrPointer -> next != NULL) {
                        j = 0;
                        firstIteration = 1;
                        tmpStrPointer = tmpStrPointer -> next;
                        tmpCharPointer = tmpStrPointer -> curString;
                    }
                    else{
                        return 0;
                    }
                }
            }
        }
        else{
            for (i = 0; i < iterationCounter; i++) {
                if ((tmpStrPointer != NULL) && (tmpCharPointer != NULL)) {
                    while(secondStringForReplace[j] != '\0'){
                        
                        tmpSubstring = (struct listOfChars*)malloc(sizeof(struct listOfChars));
                        
                        if (firstIteration){
                            begPointer = tmpSubstring;
                            tmpSubstring->prev = NULL;
                            tmpPrevSubstring = tmpSubstring;
                            firstIteration = 0;
                        }
                        
                        tmpSubstring->curChar = secondStringForReplace[j];
                        tmpSubstring->prev = tmpPrevSubstring;
                        tmpPrevSubstring->next = tmpSubstring;
                        tmpPrevSubstring = tmpSubstring;
                        j++;
                    }
                    
                    tmpSubstring -> next = tmpCharPointer;
                    tmpCharPointer -> prev = tmpSubstring;
                    
                    if (firstString){
                        pointerForStrings -> curString = begPointer;
                        firstString = 0;
                    }
                    else{
                        tmpStrPointer -> curString = begPointer;
                    }
                    
                    if (tmpStrPointer -> next != NULL) {
                        j = 0;
                        firstIteration = 1;
                        tmpStrPointer = tmpStrPointer -> next;
                        tmpCharPointer = tmpStrPointer -> curString;
                    }
                    else{
                        return 0;
                    }
                }
            }
        }
    }
    else {
        if (putOnEnd) {
            if (!iterationCounter) {
                while (1) {
                if ((tmpStrPointer != NULL) && (tmpCharPointer != NULL)) {
                    
                    while (tmpCharPointer -> next != NULL) {
                        if (tmpCharPointer -> next -> curChar != '\n') {
                            tmpCharPointer = tmpCharPointer -> next;
                        }
                        else{
                            free(tmpCharPointer -> next);
                            break;
                        }
                    }
                    
                    while(secondStringForReplace[j] != '\0'){
                        
                        tmpSubstring = (struct listOfChars*)malloc(sizeof(struct listOfChars));
                        
                        if (firstIteration){
                            begPointer = tmpSubstring;
                            tmpSubstring->prev = tmpCharPointer;
                            tmpCharPointer -> next = begPointer;
                            tmpPrevSubstring = tmpSubstring;
                            firstIteration = 0;
                        }
                        
                        tmpSubstring->curChar = secondStringForReplace[j];
                        tmpSubstring->prev = tmpPrevSubstring;
                        tmpPrevSubstring->next = tmpSubstring;
                        tmpPrevSubstring = tmpSubstring;
                        j++;
                    }
                    
                    tmpSubstring = (struct listOfChars*)malloc(sizeof(struct listOfChars));
                    tmpSubstring->curChar = '\n';
                    tmpSubstring->prev = tmpPrevSubstring;
                    tmpPrevSubstring->next = tmpSubstring;
                    tmpSubstring -> next = NULL;
                    
                    if (tmpStrPointer -> next != NULL) {
                        j = 0;
                        firstIteration = 1;
                        tmpStrPointer = tmpStrPointer -> next;
                        tmpCharPointer = tmpStrPointer -> curString;
                    }
                    else{
                        return 0;
                    }
                }
            }
            }
            else{
                for (i = 0; i<iterationCounter; i++) {
                    if ((tmpStrPointer != NULL) && (tmpCharPointer != NULL)) {
                        
                        while (tmpCharPointer -> next != NULL) {
                            if (tmpCharPointer -> next -> curChar != '\n') {
                                tmpCharPointer = tmpCharPointer -> next;
                            }
                            else{
                                free(tmpCharPointer -> next);
                                break;
                            }
                        }
                        
                        while(secondStringForReplace[j] != '\0'){
                            
                            tmpSubstring = (struct listOfChars*)malloc(sizeof(struct listOfChars));
                            
                            if (firstIteration){
                                begPointer = tmpSubstring;
                                tmpSubstring->prev = tmpCharPointer;
                                tmpCharPointer -> next = begPointer;
                                tmpPrevSubstring = tmpSubstring;
                                firstIteration = 0;
                            }
                            
                            tmpSubstring->curChar = secondStringForReplace[j];
                            tmpSubstring->prev = tmpPrevSubstring;
                            tmpPrevSubstring->next = tmpSubstring;
                            tmpPrevSubstring = tmpSubstring;
                            j++;
                        }
                        
                        tmpSubstring = (struct listOfChars*)malloc(sizeof(struct listOfChars));
                        tmpSubstring->curChar = '\n';
                        tmpSubstring->prev = tmpPrevSubstring;
                        tmpPrevSubstring->next = tmpSubstring;
                        tmpSubstring -> next = NULL;
                        
                        if (tmpStrPointer -> next != NULL) {
                            j = 0;
                            firstIteration = 1;
                            tmpStrPointer = tmpStrPointer -> next;
                            tmpCharPointer = tmpStrPointer -> curString;
                        }
                        else{
                            return 0;
                        }
                    }
                }
            }
        }
        else{
            if (!iterationCounter) {
                while (1) {
                    while (1) {
                        if (secondStringForReplace[j] != '\0') {
                            if (tmpCharPointer -> curChar == secondStringForReplace[j]) {
                                if (!coolSubstring) {
                                    firstCharUntilReplace = tmpCharPointer -> prev;
                                    coolSubstring = 1;
                                }
                                j++;
                                if (tmpCharPointer -> next != NULL) {
                                    tmpCharPointer = tmpCharPointer -> next;
                                }
                                else {
                                    coolSubstring = 0;
                                    j = 0;
                                    break;
                                }
                            }
                            else{
                                coolSubstring = 0;
                                j = 0;
                                if (tmpCharPointer -> next != NULL) {
                                    tmpCharPointer = tmpCharPointer -> next;
                                }
                                else {
                                    coolSubstring = 0;
                                    j = 0;
                                    break;
                                }
                            }
                        }
                    }
                }
            }
            else {
                for (i = 0; i < iterationCounter; i++) {
                    
                }
            }
        }
    }
    
    return 0;
}*/