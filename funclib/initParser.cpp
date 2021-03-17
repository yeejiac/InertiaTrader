#include "initParser.h"

InitParser::InitParser(std::string path, std::string target):target_(target), path_(path){}

void InitParser::findLine()
{
    std::ifstream myfile;
    myfile.open(path_);
    std::string line;
    int linenum = 0;
    if (myfile.is_open())
    {
        while (std::getline(myfile,line))
        {   
            linenum +=1;
            if(line[0]=='[')
                tagLineNum_.push_back(linenum);
            if(line.find('[' + target_ + ']') != std::string::npos)
                startPos_ = linenum;
        }
        if(startPos_ != 0)
        {
            auto it = std::find(tagLineNum_.begin(), tagLineNum_.end(), startPos_);
            int index = std::distance(tagLineNum_.begin(), it);
            endPos_ = tagLineNum_[index+1];
            if(endPos_==0)
                endPos_ = linenum+1;
        }
        linenum = -1;
        myfile.close();
    }
}

void InitParser::readLine()
{
    findLine();
    std::ifstream myfile;
    myfile.open(path_);
    std::string line;
    int linenum = 0;
    if (myfile.is_open())
    {
        while (std::getline(myfile,line))
        {   
            linenum++;
            if(linenum>startPos_&&linenum<endPos_)
            {
                std::string delimiter = "=";
                size_t pos_start = 0;
                size_t pos_end = 0;
                std::string token1;
                std::string token2;
                size_t line_length = line.length();
                while ((pos_end = line.find (delimiter, pos_start)) != std::string::npos) 
                {
                    token1 = line.substr(pos_start, pos_end - pos_start);
                    token2 = line.substr(pos_end+1, line_length);
                    pos_start = pos_end + 1;
                    iniContainer[token1] = token2;
                }
            }
        }
        myfile.close();
    }
}

// int main()
// {
//     InitParser *ip = new InitParser("../doc/settings.ini", "socket");
//     ip->readLine();
//     std::cout<<ip->iniContainer["addr"]<<std::endl;
//     std::cout<<ip->iniContainer["port"]<<std::endl;
//     return 0;
// }

// g++ -std=c++11 -Wall initParser.cpp -o test.out