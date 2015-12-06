#include "textViewCmds.h"
#include "techCmds.h"
#include "stringEditCmds.h"
#include "stringInsertCmds.h"
#include "assistFunctions.h"
#include "struct.h"
#include <termios.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/ioctl.h>

#define NUMOFCMD 16

char *comands[NUMOFCMD], *parametrs = NULL, *userString = NULL, *fileName = NULL;
int tabWidth = 8, wrapMod = 0, screenNumY = 0, isFileSaved = 1, userStringSize = 0, readOnlyMod = 0;
struct listOfStrings *pointerForStrings;
struct listOfStrings *tmpStrPointer;
struct listOfChars *tmpCharPointer;
struct winsize screenSize;
int screenCol = 80, screenRow = 25;

int main(int argc, const char * argv[]) {
    fileName = "e.txt";
    parametrs = "1 8 W";
    initFile();
    //deleteRange();
    insertSymbol();
    writeF();
    freeTheList();
    return 0;
}