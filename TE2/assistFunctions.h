#ifndef assistFunctions_h
#define assistFunctions_h
#include "struct.h"

int degree(int, int);
void clrscr(void);
void moveTxtY(char);
void moveTxtX(char);
void initCmd(void);
int initFile(char*);
int readCmd(void);
void setKeypress(void);
void resetKeypress(void);
int recognizeCmd(void);
void freeTheList(void);
void freeTheString(struct listOfChars*);

#endif