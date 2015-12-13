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
int tabWidth = 8, wrapMod = 0, screenNumY = 0, isFileSaved = 1, userStringSize = 0;
struct listOfStrings *pointerForStrings;
struct listOfStrings *tmpStrPointer;
struct listOfChars *tmpCharPointer;
struct winsize screenSize;
int screenCol = 80, screenRow = 25;

int main(int argc, char *argv[]){
    
    int exit = 0, returningInt;
    
    /*
     ioctl(0, TIOCGWINSZ, &screenSize);
     screenCol = screenSize.ws_col;
     screenRow = screenSize.ws_row;
     */
    
    fileName = "l.txt";
    initFile(fileName);
    printPages();
    
    /*if (argc == 2){
        fileName = argv[1];
        if(initFile(fileName) == 9){
            freeTheList();
            return 0;
        }
    }
    
    initCmd();
    
    while (!exit) {
        fprintf(stderr, "Editor: ");
        returningInt = readCmd();
        
        if (returningInt == 9) {
            freeTheList();
            return 0;
        }
        
        if (returningInt == 1) {
            continue;
        }
        
        returningInt = recognizeCmd();
        
        if (returningInt == -9) {
            freeTheList();
            return 0;
        }
        
        if (returningInt == 0) {
            continue;
        }
        
        switch (returningInt) {
                
            case 0:{
                setTabWidth();
                break;
            }
            case 1:{
                printPages();
                
                break;
            }
            case 2:{
                printRange();
                break;
            }
            case 3:{
                setWrap();
                //clearOutBuffer();
                break;
            }
                
            case 4:{
                insertAfter();
                //clearOutBuffer();
                break;
            }
                
            case 5:{
                 editString();
                 break;
            }
             
            case 6:{
                insertSymbol();
                break;
            }
             
            case 7:{
                //replaceSubstring();
                break;
            }

            case 8:{
                deleteRange();
                break;
            }

            case 9:{
                deleteBraces();
                break;
            }

            case 10:{
                exit = exitF();
                break;
            }

            case 11:{
                if(readF() == 9){
                    freeTheList();
                    return 0;
                }
                break;
            }
                
            case 12:{
                if(openF() == 9){
                    freeTheList();
                    return 0;
                }
                break;
            }
                
            case 13:{
                writeF();
                break;
            }
                
            case 14:{
                if(setName() == 9){
                    freeTheList();
                    return 0;
                }
                break;
            }
                
            case 15:{
                if(helpF() == 9){
                    freeTheList();
                    return 0;
                }
                break;
            }
                
            default: break;
        }
    }
    
    freeTheList();*/
    return 0;
}