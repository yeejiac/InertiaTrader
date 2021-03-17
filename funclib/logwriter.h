#ifndef LOGWRITER_H_
#define LOGWRITER_H_
#include <iostream>
#include <ctime>
#include <sstream>
#include <fstream>
#include <iomanip>

enum LogLevel
{
    FATAL,
    WARN,
    ERROR,
    DEBUG,
    TRACE,
    INFO
};

class Logwriter
{
public:
    Logwriter(std::string logType, std::string filePath);
    void write(LogLevel level, std::string documentation);
    std::string getTime();
    
private:
    std::string logType_;
    std::string logcontent_;
    std::string documentation_;
    std::string filePath_;
};

#endif 