#include "simplefunc.h"

std::vector<std::string> split(std::string str, std::string delim) {  
	std::vector<std::string> res;  
	if("" == str) return res;  
	//先將要切割的字串從string型別轉換為char*型別  
	char * strs = new char[str.length() + 1] ; //不要忘了  
	strcpy(strs, str.c_str());   

	char * d = new char[delim.length() + 1];  
	strcpy(d, delim.c_str());  

	char *p = strtok(strs, d);  
	while(p) 
    {  
		std::string s = p; //分割得到的字串轉換為string型別  
		res.push_back(s); //存入結果陣列  
		p = strtok(NULL, d);  
	}  

	return res;  
}

// int main()
// {
//     std::string str = "11|";
//     std::cout<<split(str, "|").size()<<std::endl;;
// }