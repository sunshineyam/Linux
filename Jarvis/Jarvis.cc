#include "Jarvis.hpp"
int main()
{
    Jarvis* js = new Jarvis();
    if(!js->LoadEtc())
        return 1;
    js->Run();
    /*Robot rb;
    std::string str;
    volatile bool quit = false;
    while(!quit)
    {
        std::cout << "Me#";
        std::cin >> str;
        std::string s = rb.Talk(str);
        std::cout << "TuTu# "<< s << std::endl;
    }*/
    return 0;
}
