#include "Scheduler.hpp"

Scheduler::Scheduler() : queue(), mutex() {}

Scheduler::~Scheduler() {}

void Scheduler::schedule(Scheduler::Task task)
{
    std::lock_guard<std::mutex> lock(mutex);

    queue.push(task);
}

bool Scheduler::executeOne()
{
    Task task;

    {
        std::lock_guard<std::mutex> lock(mutex);
        if (queue.empty())
            return false;

        task = queue.front();
        queue.pop();
    }

    task();
    return true;
}

bool Scheduler::executeAll()
{
    bool executed = false;
    while (true)
    {
        Task task;

        {
            std::lock_guard<std::mutex> lock(mutex);
            if (queue.empty())
                break;

            task = queue.front();
            queue.pop();
        }

        task();
        executed = true;
    }

    return executed;
}
