#include <stdio.h>
#include <stdlib.h>
//argc:放命令行参数；argv[]:可执行程序的名称；env[]：系统调用main函数，把环境变量传给env
int main(int argc,char *argv[],char *env[])
{
    int i = 0;
    //每个程序维护一张环境表，环境表是一个字符指针数组，最后一个元素的下一个是NULL
    for(;env[i];i++)
    {
        //打印出所有环境变量，类似于env的作用
        printf("%s\n",env[i]);
    }
    return 0;
}
//##########################################################################################
//通过第三方变量environ获取
//int main(int argc, char *argv[])
//{
//    //libc中定义的全局变量environ指向环境变量表，environ没有包含在任何头文件中，所以在使用时要用extern声明
//    extern char **environ;
//    int i = 0;
//    for(;environ[i];i++)
//    {
//        printf("%s\n",environ[i]);
//    }
//    return 0;
//}
//####################################################################################################
//通过系统调用来获取环境变量
//int main()
//{
//    char *env = getenv("PATH");
//    if(env)
//    {
//        printf("%s\n",env);
//    }
//    return 0;
//}
//
