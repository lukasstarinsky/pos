#include "MessageQueue.hpp"

void MessageQueue::Push(const std::string& message)
{
    m_Mutex.lock();
    m_Queue.push(message);
    m_Mutex.unlock();
}

void MessageQueue::Swap(std::queue<std::string>& queue)
{
    m_Mutex.lock();
    std::swap(m_Queue, queue);
    m_Mutex.unlock();
}