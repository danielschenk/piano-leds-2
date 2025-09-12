#include "Scheduler.h"

Scheduler::Scheduler() : queue(), mutex() {}

Scheduler::~Scheduler() {}

void Scheduler::schedule(Scheduler::TTask task)
{
    std::lock_guard<std::mutex> lock(mutex);

    queue.push(task);
}

bool Scheduler::executeOne()
{
    std::lock_guard<std::mutex> lock(mutex);

    if (!queue.empty())
    {
        // Call the task at the front of the queue
        queue.front()();
        // Remove it
        queue.pop();
        return true;
    }
    else
    {
        return false;
    }
}

bool Scheduler::executeAll()
{
    std::lock_guard<std::mutex> lock(mutex);

    bool executed = false;
    while (!queue.empty())
    {
        // Call the task at the front of the queue
        queue.front()();
        // Remove it
        queue.pop();
        executed = true;
    }

    return executed;
}
