// SYSTEM INCLUDES
#include <chrono>

#include <gtest/gtest.h>
#include <Utilities/OSUtils.hpp>
#include <Async/Async.hpp>
#include <Async/AsyncExecution.hpp>

// C++ PROJECT INCLUDES
#include "MathUtils.hpp"
#include "PromiseFactory.hpp"

namespace Async_FT
{

    TEST(SimpleExecutionTest, TrivialCallExecution)
    {
        std::string configFilePath = Utilities::OS::GetCurrentDirectory(__FILE__) +
            Utilities::OS::GetPathSep() + "TestEngineConfig.xml";
        Async::Start(configFilePath);

        std::string schedulerName = "TestScheduler";
        Async::PromisePtr<int> pPromise = Async::Execute<int>([]() -> int
        {
            return 5;
        }, schedulerName);

        for(int i = 0; i < 1000 && pPromise->GetState() != Async::States::SettlementState::SUCCESS; ++i)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }

        EXPECT_EQ(pPromise->GetState(), Async::States::SettlementState::SUCCESS);
        EXPECT_EQ(pPromise->GetResult(), 5) << "Promise should return 5";

        Async::Stop();
    }

    TEST(SimpleExecutionTest, FunctionCallInPromise)
    {
        std::string configFilePath = Utilities::OS::GetCurrentDirectory(__FILE__) +
            Utilities::OS::GetPathSep() + "TestEngineConfig.xml";
        Async::Start(configFilePath);

        int squareArg = 5;
        ASSERT_EQ(MathUtils::Square(squareArg), squareArg * squareArg) << "Square functionality not correct";
        std::string schedulerName = "TestScheduler";
        Async::PromisePtr<int> pPromise = Async::Execute<int>([squareArg]() -> int
        {
            return MathUtils::Square(squareArg);
        }, schedulerName);

        for(int i = 0; i < 1000 && pPromise->GetState() != Async::States::SettlementState::SUCCESS; ++i)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }

        EXPECT_EQ(pPromise->GetState(), Async::States::SettlementState::SUCCESS);
        EXPECT_EQ(pPromise->GetResult(), MathUtils::Square(squareArg)) << "Promise should return 25";

        Async::Stop();
    }

    TEST(SimpleExecutionTest, PromiseOutlivesCallingMacro)
    {
        std::string configFilePath = Utilities::OS::GetCurrentDirectory(__FILE__) +
            Utilities::OS::GetPathSep() + "TestEngineConfig.xml";
        Async::Start(configFilePath);

        int squareArg = 5;
        ASSERT_EQ(MathUtils::Square(squareArg), squareArg * squareArg) << "Square functionality not correct";
        std::string schedulerName = "TestScheduler";
        PromiseFactory::ExecuteAsynchronously<int>([squareArg]() -> int
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            return MathUtils::Square(squareArg);
        }, schedulerName);
    }

    TEST(SimpleExecutionTest, WaitingForPromiseToSettle)
    {
        std::this_thread::sleep_for(std::chrono::seconds(4));
        ASSERT_EQ(Async::Stop(), true);
    }

    TEST(SimpleExecutionTest, PromiseOutlivesCallingFunction)
    {
        std::string configFilePath = Utilities::OS::GetCurrentDirectory(__FILE__) +
            Utilities::OS::GetPathSep() + "TestEngineConfig.xml";
        Async::Start(configFilePath);

        int squareArg = 5;
        ASSERT_EQ(MathUtils::Square(squareArg), squareArg * squareArg) << "Square functionality not correct";
        std::string schedulerName = "TestScheduler";
        Async::Execute<int>([squareArg]() -> int
        {
            std::this_thread::sleep_for(std::chrono::seconds(2));
            return MathUtils::Square(squareArg);
        }, schedulerName);

        ASSERT_EQ(Async::Stop(), true);
    }

} // end of namespace Async_FT
