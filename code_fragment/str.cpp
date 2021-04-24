#include <iostream>
#include <string>
int main()
{
    std::string str;
    std::string str1("asdfghjkl");
    std::string str2(str1, 2);
    std::string str3(str1 , 2 ,2);
    std::string str4("asdfghjkl", 4);
    std::string str5(10 , 'a');
    std::cout << "str2: " << str2 << std::endl;
    std::cout << "str3: " << str3 << std::endl;
    std::cout << "str4 : " << str4 << std::endl;
    std::cout << "str5: " << str5 << std::endl;


    int a = 78 ;
    int &b = a ;
    int c = 9 ;
     c = b;
     std::cout << c << std::endl;
    return 0;

}