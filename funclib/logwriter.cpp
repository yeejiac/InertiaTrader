#include "logwriter.h"

std::string to_zero_lead(const int value, const unsigned precision)
{
     std::ostringstream oss;
     oss << std::setw(precision) << std::setfill('0') << value;
     return oss.str();
}

Logwriter::Logwriter(std::string logType, std::string filePath):logType_(logType)
{
    std::time_t t = std::time(0); 
    std::tm* now = std::localtime(&t);
    std::string datetiming = std::to_string(now->tm_year + 1900) + "-" + to_zero_lead(now->tm_mon + 1, 2)+ "-" + to_zero_lead(now->tm_mday,2);
    filePath_ = filePath + datetiming + ".log";
}

std::string Logwriter::getTime()
{
    std::time_t t = std::time(0);   // get time now
    std::tm* now = std::localtime(&t);
    std::string datetiming = std::to_string(now->tm_year + 1900) + "-" + to_zero_lead(now->tm_mon + 1, 2)+ "-" + to_zero_lead(now->tm_mday,2);
    std::string timing = to_zero_lead(now->tm_hour, 2) + ":" + to_zero_lead(now->tm_min, 2)+ ":" + to_zero_lead(now->tm_sec, 2);
    return datetiming + " " + timing;
}

void Logwriter::write(LogLevel level, std::string documentation)
{
    documentation_ = documentation;
    if(level == LogLevel::FATAL)
    {
        logcontent_ = getTime() + " | Fatal | " + logType_;
    }
    else if(level == LogLevel::WARN)
    {
        logcontent_ = getTime() + " | Warn | " + logType_+ " |";
    }
    else if(level == LogLevel::ERROR)
    {
        logcontent_ = getTime() + " | Error | " + logType_+ " |";
    }
    else if(level == LogLevel::DEBUG)
    {
        logcontent_ = getTime() + " | Debug | " + logType_+ " |";
    }
    else if(level == LogLevel::TRACE)
    {
        logcontent_ = getTime() + " | Trace | " + logType_ + " |";
    }
    else if(level == LogLevel::INFO)
    {
        logcontent_ = getTime() + " | Info | " + logType_+ " |";
    }
    std::ofstream log_file(filePath_.c_str(), std::ios_base::out | std::ios_base::app );
    log_file << logcontent_ + " " + documentation_ << std::endl;
    std::cout<<logcontent_ + " " + documentation_<<std::endl;
}


// int main()
// {
//     Logwriter *logwriter = new Logwriter("FD", "../log/");
//     logwriter->write(LogLevel::ERROR, "testing");
// }
