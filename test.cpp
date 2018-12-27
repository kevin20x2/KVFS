#include <iostream>
int main()
{
    std::string a = "";
    std::string b = "";
    for(int i = 0;i<100;++i)
        b+= "a";
    std::cout << sizeof(a) <<" " << sizeof(b) ;
    return 0;
}
