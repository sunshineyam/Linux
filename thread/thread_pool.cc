#include <iostream>
#include <queue>
#include <time.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>
using namespace std;
//实现计算器
int cal(int x, int y, int op);
//处理函数指针
typedef int(*HandlTask_t)(int x,int y,int op);
class Task
{
    private:
        int _x;
        int _y;
        int _op;//0:+,1:-,2:*,3:/
        HandlTask_t _handler;
    public:
        Task(int x = -1, int y = -1, int op = -1)
            :_x(x)
             ,_y(y)
             ,_op(op)
        {
        }
        void Register(HandlTask_t handler)
        {
            _handler = handler;
        }
        void Run()
        {
           int ret = _handler(_x, _y, _op);
           const char* oop = "+-*/";
           cout << "thread: " << pthread_self() << "->  handler result: " << _x << oop[_op] << _y << "=" << ret << endl;
        }
        ~Task()
        {
        }
};
class ThreadPool
{
    private:
        int thread_nums;//创建的线程数量
        int idle_nums;//等待的线程数量
        queue<Task> task_queue;
        pthread_mutex_t mutex;
        pthread_cond_t cond;
    public:
        void LockQueue()
        {
            pthread_mutex_lock(&mutex);
        }
        void UnlockQueue()
        {
            pthread_mutex_unlock(&mutex);
        }
        bool QueueEmpty()
        {
            return task_queue.size() == 0 ? true : false;
        }
        void ThreadIdle()
        {
            idle_nums++;
            pthread_cond_wait(&cond, &mutex);
            idle_nums--;
        }
        void WakeThread()
        {
            pthread_cond_signal(&cond);
        }
        void PopTask(Task &t)
        {
            t = task_queue.front();
            task_queue.pop();
        }
    public:
        ThreadPool(int num_=5)
            :idle_nums(0)
             ,thread_nums(num_)
      {
        pthread_mutex_init(&mutex,NULL);
        pthread_cond_init(&cond,NULL);
      }
        //线程启动后执行的函数
       static void *ThreadRotinue(void *arg)
        {
            //分离线程执行任务
            pthread_detach(pthread_self());
            ThreadPool *tp = (ThreadPool*)arg;
            for(;;)
            {
               tp-> LockQueue();
               //循环判断任务队列是否为空
                while(tp->QueueEmpty())
                {
                    tp->ThreadIdle();
                }
                Task t;
                tp->PopTask(t);
                //Task t = task_queue.front();
                //task_queue.pop();
                tp-> UnlockQueue();
                t.Run();
            }
        }
        void InitThreadPool()
        {
            //pthread_t类型的线程id,具体是什么类型，取决于实现
            //typedef unsigned long int pthread_t;
            pthread_t t;
            for(auto i = 0; i < thread_nums; i++)
            {
                pthread_create(&t, NULL, ThreadRotinue, this);
            }
        }
        void PushTask(const Task &t)
        {
            LockQueue();
            task_queue.push(t);
            WakeThread();
            UnlockQueue();
        }

};
int cal(int x,int y,int op)
{
    int ret = -1;
    switch(op)
    {
        case 0:
            ret = x + y;
            break;
        case 1:
            ret = x - y;
            break;
        case 2:
            ret = x * y;
            break;
        case 3:
            ret = x / y;
            break;
        default:
            cout << "cal error" <<endl;
            break;
    }
    return ret;
}
int main()
{
    ThreadPool tp;
    tp.InitThreadPool();
    srand((unsigned long)time(NULL));
    for(;;)
    {
        sleep(1);
        int x = rand() % 100 + 1;
        int y = rand() % 100 + 1;
        int op = rand() % 4;
        Task t(x, y, op);
        t.Register(cal);
        tp.PushTask(t);
    }
    return 0;
}
