#include <iostream>
#include <pthread.h>
using namespace std;
template <typename T>
//饿汉
class Singleton
{
  static T data;
  public:
    static T* GetInstance()
      {
          return &data;
      }
};
//懒汉:这种方式是线程不安全的，如果两个线程同时调用，可能会创建两份T对象
template <typename T>
class Singleton
{
    static T* inst;
public:
    static T* GetInstance()
    {
        if (inst == NULL)
        {
            inst = new T();
        }
        return &data;
    }
};
//线程安全的懒汉,空间换时间，类似于创建一份缓存
template <typename T>
class Singleton
{
   //设置volatile关键字，保持内存可见性
     volatile static T* inst;
     static std::mutex lock;
public:
     static T* GetInstance()
     {
         //双重判定可避免每次进入GetInstance方法时都要同步（竞争锁），提高了性能
         if(inst == NULL)
         {
             //使用互斥锁，保证每次只有一个线程执行，调用new，创建一个对象
             lock.lock();
             if(inst == NULL)
             {
                 inst = new T();
             }
             lock.unlock();
         }
         return inst;
     }
};
