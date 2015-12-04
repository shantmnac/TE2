#ifndef struct_h
#define struct_h

struct listOfChars{
    char curChar;
    struct listOfChars *next;
    struct listOfChars *prev;
};

struct listOfStrings{
    struct listOfChars *curString;
    struct listOfStrings *next;
    struct listOfStrings *prev;
};

struct tabDest{
    int numOfSpaces;
    int position;
};

#endif
