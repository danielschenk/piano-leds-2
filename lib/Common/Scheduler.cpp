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
    // Pop a task under lock, then execute it outside the lock.
    Task task;
    {
        std::lock_guard<std::mutex> lock(mutex);
        if (queue.empty())
        {
            return false;
        }
        task = queue.front();
        queue.pop();
    }

    // Execute outside of the mutex to avoid lock inversion with user code.
    task();
    return true;
}

bool Scheduler::executeAll()
{
    bool executed = false;
    for (;;)
    {
        Task task;
        {
            std::lock_guard<std::mutex> lock(mutex);
            if (queue.empty())
            {
                break;
            }
            task = queue.front();
            queue.pop();
        }

        // Execute outside of the mutex to avoid lock inversion with user code.
        task();
        executed = true;
    }

    return executed;
}
