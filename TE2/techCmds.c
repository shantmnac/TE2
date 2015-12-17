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

int fileNameLength = 0;

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
    
    free(parametrs);
    parametrs = NULL;
    
    if (isForceExit) {
        return 1;
    }
    else{
        if (!isFileSaved) {
            fprintf(stderr, "Файл не сохранен!\n");
            return 0;
        }
        else {
            return 1;
        }
    }
    
    return 0;
}
//+
int readF(void){
    int i = 0, j = 0;
    char *tempFileName1 = NULL;
    
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
    tempFileName1 = (char*)realloc(tempFileName1, j+1);
    if (tempFileName1 == NULL){
        fprintf(stderr, "Переполнение памяти!\n");
        free(tempFileName1);
        return 9;
    }
    tempFileName1[j] = '\0';
    
    initFile(tempFileName1);
    
    free(tempFileName1);
    free(parametrs);
    parametrs = NULL;
    return 0;
}
//+
int openF(void){
    int i = 0;
    
    if (parametrs == NULL) {
        return 0;
    }
    
    if (parametrs[i] == '"') {
        i++;
    }
    
    fileNameLength = 0;
    
    while (parametrs[i] != '\0') {
        fileName = (char*)realloc(fileName, fileNameLength+1);
        if (fileName == NULL){
            fprintf(stderr, "Переполнение памяти!\n");
            free(fileName);
            return 9;
        }
        fileName[fileNameLength] = parametrs[i];
        fileNameLength++;
        i++;
    }
    
    fileName = (char*)realloc(fileName, fileNameLength+1);
    if (fileName == NULL){
        fprintf(stderr, "Переполнение памяти!\n");
        free(fileName);
        return 9;
    }
    fileName[fileNameLength] = '\0';
    
    free(parametrs);
    parametrs = NULL;
    
    if (initFile(fileName) == 8){
        free(fileName);
        fileNameLength = 0;
    }
    
    return 0;
}
//+
int setName(void){
    int i = 0;
    
    if (parametrs == NULL) {
        return 0;
    }
    
    if (parametrs[i] == '"') {
        i++;
    }
    
    fileNameLength = 0;
    
    while (parametrs[i] != '\0') {
        fileName = (char*)realloc(fileName, fileNameLength+1);
        if (fileName == NULL){
            fprintf(stderr, "Переполнение памяти!\n");
            free(fileName);
            return 9;
        }
        fileName[fileNameLength] = parametrs[i];
        fileNameLength++;
        i++;
    }
    fileName = (char*)realloc(fileName, fileNameLength+1);
    if (fileName == NULL){
        fprintf(stderr, "Переполнение памяти!\n");
        free(fileName);
        return 9;
    }
    fileName[fileNameLength] = '\0';
    
    
    free(parametrs);
    parametrs = NULL;
    
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

int helpF(void){
    int returnValue = 0;
    
    returnValue = initFile("help.txt");
    
    if(returnValue == 9){
        return 9;
    }
    
    if(returnValue == 8){
        return 0;
    }
    
    printPages();
    freeTheList();
    
    returnValue = initFile(fileName);
    
    if(returnValue == 9){
        return 9;
    }
    
    return 0;
}