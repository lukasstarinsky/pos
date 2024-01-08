#pragma once

#include <string>
#include <queue>
#include <mutex>

class MessageQueue
{
public:
    MessageQueue() = default;

    void Push(const std::string& message);
    void Swap(std::queue<std::string>& queue);
private:
    std::queue<std::string> m_Queue;
    std::mutex m_Mutex;
};