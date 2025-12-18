#ifndef COMMON_SCHEDULER_H_
#define COMMON_SCHEDULER_H_

#include <functional>
#include <mutex>
#include <queue>

class Scheduler
{
  public:
    typedef std::function<void()> Task;

    Scheduler() = default;
    ~Scheduler() = default;

    void schedule(Task task);
    bool executeOne();
    bool executeAll();
    void clear();

  private:
    std::queue<Task> queue;
    std::mutex mutex;
};

#endif /* COMMON_SCHEDULER_H_ */
