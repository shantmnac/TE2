#include "techCmds.h"
#include "assistFunctions.h"
#include "textViewCmds.h"
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
    char *tempFileName1 = NULL, *tempFileName2 = NULL;
    
    tempFileName2 = fileName;
    
    if (parametrs == NULL) {
        return 0;
    }
    
    if (parametrs[i] == '"') {
        i++;
    }
    
    while (parametrs[i] != '\0') {
        tempFileName1 = (char*)realloc(tempFileName1, j+1);
        if (tempFileName1 == NULL){
            fprintf(stderr, "Переполнение памяти!\n");
            free(tempFileName1);
            return 9;
        }
        tempFileName1[j] = parametrs[i];
        j++;
        i++;
    }
    
    fileName = tempFileName1;
    initFile();
    
    free(tempFileName1);
    fileName = tempFileName2;
    free(parametrs);
    parametrs = NULL;
    return 0;
}
//+
int openF(void){
    int i = 0, j = 0;
    char *tempFileName = NULL;
    
    if (parametrs == NULL) {
        return 0;
    }
    
    if (parametrs[i] == '"') {
        i++;
    }
    
    while (parametrs[i] != '\0') {
        tempFileName = (char*)realloc(tempFileName, j+1);
        if (tempFileName == NULL){
            fprintf(stderr, "Переполнение памяти!\n");
            free(tempFileName);
            return 9;
        }
        tempFileName[j] = parametrs[i];
        j++;
        i++;
    }
    
    free(parametrs);
    parametrs = NULL;
    
    fileName = tempFileName;
    free(tempFileName);
    
    initFile();
    isFileSaved = 0;
    return 0;
}
//+
int setName(void){
    int i = 0, j = 0;
    char *tempFileName = NULL;
    
    if (parametrs == NULL) {
        return 0;
    }
    
    if (parametrs[i] == '"') {
        i++;
    }
    
    while (parametrs[i] != '\0') {
        tempFileName = (char*)realloc(tempFileName, j+1);
        if (tempFileName == NULL){
            fprintf(stderr, "Переполнение памяти!\n");
            free(tempFileName);
            return 9;
        }
        tempFileName[j] = parametrs[i];
        j++;
        i++;
    }
    
    free(parametrs);
    parametrs = NULL;
    
    fileName = tempFileName;
    free(tempFileName);
    
    isFileSaved = 0;
    return 0;
}
//+
void writeF(void){
    if (fileName == NULL) {
        fprintf(stderr, "Нет ассоциированного файла!\n");
        return;
    }
    
    if (isFileSaved) {
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

void helpF(void){
    char *tempFileName;
    
    tempFileName = fileName;
;
    fileName = "help.txt";
    
    initFile();
    printPages();
    freeTheList();
    
    fileName = tempFileName;
}