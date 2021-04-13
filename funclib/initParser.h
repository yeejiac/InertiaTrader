#ifndef INIPARSER_H_
#define INIPARSER_H_

#include <iostream>
#include <algorithm>
#include <fstream>
#include <string>
#include <map>
#include <vector>
#include "logwriter.h"

class InitParser
{
public:
    InitParser(std::string path, std::string target);
    void findLine();
    void readLine();
    std::map<std::string, std::string> iniContainer;
    Logwriter *logwrite;
private:
    std::vector<int> tagLineNum_;
    int startPos_ = 0;
    int endPos_ = 0;
    std::string target_;
    std::string path_;
};




#endif