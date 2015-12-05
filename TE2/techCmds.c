#include "techCmds.h"
#include "assistFunctions.h"
#include "struct.h"
#include <stdio.h>
#include <stdlib.h>

extern char *parametrs, *userString, *fileName;
extern int isFileSaved;
extern struct listOfStrings *tmpStrPointer, *pointerForStrings;
extern struct listOfChars *tmpCharPointer;
//+
int exitF(void){
    int i = 0, isForceExit = 0;
    char *force = "force";
    
    while (parametrs[i] != '\0'){
        if (force[i] != parametrs[i]) {
            isForceExit = 0;
            break;
        }
        isForceExit = 1;
        i++;
    }
    
    if (isForceExit) {
        free(parametrs);
        parametrs = NULL;
        return 1;
    }
    else{
        if (!isFileSaved) {
            fprintf(stderr, "Файл не сохранен!\n");
            free(parametrs);
            parametrs = NULL;
            return 0;
        }
    }
    
    return 0;
}
//+
int readF(void){
    int i = 0, j = 0;
    
    if (parametrs == NULL) {
        return 0;
    }
    
    if (parametrs[i] == '"') {
        i++;
    }
    
    while (parametrs[i] != '\0') {
        fileName = (char*)realloc(fileName, j+1);
        if (fileName == NULL){
            fprintf(stderr, "Переполнение памяти!\n");
            free(fileName);
            return 9;
        }
        fileName[j] = parametrs[i];
        j++;
        i++;
    }
    
    initFile();
    
    free(fileName);
    fileName = NULL;
    free(parametrs);
    parametrs = NULL;
    return 0;
}
//+
int openF(void){
    int i = 0, j = 0;
    
    if (parametrs == NULL) {
        return 0;
    }
    
    if (parametrs[i] == '"') {
        i++;
    }
    
    while (parametrs[i] != '\0') {
        fileName = (char*)realloc(fileName, j+1);
        if (fileName == NULL){
            fprintf(stderr, "Переполнение памяти!\n");
            free(fileName);
            return 9;
        }
        fileName[j] = parametrs[i];
        j++;
        i++;
    }
    
    free(parametrs);
    parametrs = NULL;
    
    initFile();
    isFileSaved = 0;
    return 0;
}
//+
int setName(void){
    int i = 0, j = 0;
    
    if (parametrs == NULL) {
        return 0;
    }
    
    if (parametrs[i] == '"') {
        i++;
    }
    
    while (parametrs[i] != '\0') {
        fileName = (char*)realloc(fileName, j+1);
        if (fileName == NULL){
            fprintf(stderr, "Переполнение памяти!\n");
            free(fileName);
            return 9;
        }
        fileName[j] = parametrs[i];
        j++;
        i++;
    }
    
    free(parametrs);
    parametrs = NULL;
    
    isFileSaved = 0;
    return 0;
}
//+
void writeF(void){
    if (fileName == NULL) {
        fprintf(stderr, "Нет ассоциированного файла!\n");
        return;
    }
    
    FILE *outputFile = fopen(fileName, "wb");
    
    if ((pointerForStrings == NULL) || (pointerForStrings -> curString == NULL)) {
        fclose(outputFile);
        return;
    }
    
    tmpStrPointer = pointerForStrings;
    
    while (tmpStrPointer != NULL) {
        tmpCharPointer = tmpStrPointer -> curString;
        while (tmpCharPointer != NULL) {
            fwrite(&(tmpCharPointer -> curChar), sizeof(char), 1, outputFile);
            tmpCharPointer = tmpCharPointer -> next;
        }
        tmpStrPointer = tmpStrPointer -> next;
    }
    
    isFileSaved = 1;
    
    fclose(outputFile);
}