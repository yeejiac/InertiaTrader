#include <iostream>

struct example
{
    std::string a;
    std::string b;
};

int main()
{
    example *e = new example;
    e->a = "test123";
    std::cout<<e->a<<std::endl;
    // free(e);
}