// SYSTEM INCLUDES
#include <chrono>
#include <iostream>

#include <gtest/gtest.h>
#include <Utilities/OSUtils.hpp>
#include <Async/Async.hpp>
#include <Async/AsyncExecution.hpp>

// C++ PROJECT INCLUDES
#include "MathUtils.hpp"

namespace Async_FT
{

    TEST(SimpleExecutionTest, TrivialCallExecution)
    {
        std::string configFilePath = Utilities::OS::GetCurrentDirectory(__FILE__) + "\\TestEngineConfig.xml";
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
        std::string configFilePath = Utilities::OS::GetCurrentDirectory(__FILE__) + "\\TestEngineConfig.xml";
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
        EXPECT_EQ(pPromise->GetResult(), MathUtils::Square(squareArg)) << "Promise should return 5";

        Async::Stop();
    }

} // end of namespace Async_FT
