#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include <functional>

#include "../Scheduler.hpp"

using ::testing::Expectation;
using ::testing::StrictMock;

class ITask
{
    virtual void task() = 0;

  protected:
    virtual ~ITask() = default;
};

class MockTask : public ITask
{
  public:
    MOCK_METHOD0(task, void());
};

class SchedulerTest : public ::testing::Test
{
  public:
    Scheduler scheduler;
};

TEST_F(SchedulerTest, executeOne)
{
    StrictMock<MockTask> mockTask;
    scheduler.schedule(std::bind(&MockTask::task, &mockTask));
    EXPECT_CALL(mockTask, task()).Times(1);

    EXPECT_TRUE(scheduler.executeOne());
    EXPECT_FALSE(scheduler.executeOne());
}

TEST_F(SchedulerTest, executeTwo)
{
    StrictMock<MockTask> mockTask1, mockTask2;
    scheduler.schedule(std::bind(&MockTask::task, &mockTask1));
    scheduler.schedule(std::bind(&MockTask::task, &mockTask2));

    Expectation task1call = EXPECT_CALL(mockTask1, task()).Times(1);
    EXPECT_CALL(mockTask2, task()).Times(1).After(task1call);

    EXPECT_TRUE(scheduler.executeOne());
    EXPECT_TRUE(scheduler.executeOne());
    EXPECT_FALSE(scheduler.executeOne());
}

TEST_F(SchedulerTest, executeAll)
{
    StrictMock<MockTask> mockTask1, mockTask2;
    scheduler.schedule(std::bind(&MockTask::task, &mockTask1));
    scheduler.schedule(std::bind(&MockTask::task, &mockTask2));

    Expectation task1call = EXPECT_CALL(mockTask1, task()).Times(1);
    EXPECT_CALL(mockTask2, task()).Times(1).After(task1call);

    EXPECT_TRUE(scheduler.executeAll());
    EXPECT_FALSE(scheduler.executeAll());
}
