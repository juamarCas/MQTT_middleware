#ifndef THREAD_QUEUE_H
#define THREAD_QUEUE_H
#include <queue>
#include <mutex>

template<typename T>
class ThreadQueue{
    private: 
    std::uint16_t _maxCapacity;
    std::queue<T> _queue;
    std::mutex _queueMutex;

    public:
    ThreadQueue(std::uint16_t maxCapacity): _maxCapacity(maxCapacity){}
    bool GetValue(T& item){
         std::lock_guard<std::mutex> lg(_queueMutex);
         if(_queue.empty()){
           
            return false;
         }

         item = _queue.front();
         _queue.pop();
         return true;
    }

    bool SendValue(T value){
        std::lock_guard<std::mutex> lg(_queueMutex);
        if(_queue.size() >= _maxCapacity){
            std::cout<<"Queue max capacity"<<std::endl;
            return false;
        }
        _queue.push(value);
        return true;
    }

    ~ThreadQueue(){}

};

#endif