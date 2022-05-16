//
// Created by dries on 12/17/21.
//

#include <iostream>
#include <fstream>
#include <sys/stat.h>

using namespace std;


#include "testUtils.h"

/**
Auxiliary functions for testing.
**/


bool DirectoryExists(const std::string dirname) {
    struct stat st;
    return stat(dirname.c_str(), &st) == 0;
}

bool FileCompare(const std::string leftFileName, const std::string rightFileName) {
    // TODO : Checken of bestanden succesvol geopend zijn.
    fstream lFile, rFile;
    lFile.open(leftFileName.c_str());
    rFile.open(rightFileName.c_str());
    char rChr = rFile.peek();
    for(char lChr = lFile.peek(); !(lFile.eof()); lChr = lFile.peek()){
        if(lChr != rChr){
            return false;
        }
        lFile.get(lChr);
        rFile.get(rChr);
        rChr = rFile.peek();
    }
    if(!(rFile.eof())){
        return false;
    }
    lFile.close();
    rFile.close();
    return true;
}