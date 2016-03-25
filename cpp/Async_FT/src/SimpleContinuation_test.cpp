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

    TEST(SimpleContinuationTest, TrivialContinuationCall)
    {
        std::string configFilePath = Utilities::OS::GetCurrentDirectory(__FILE__) +
            Utilities::OS::GetPathSep() + "TestEngineConfig.xml";
        Async::Start(configFilePath);

        std::string schedulerName = "TestScheduler";
        Async::PromisePtr<int> pPromise = Async::Execute<int>([]() -> int
        {
            return 5;
        }, schedulerName)->Then<int>([](int a) -> int
        {
            return a + 5;
        }, schedulerName);

        for(int i = 0; i < 1000 && pPromise->GetState() != Async::States::SettlementState::SUCCESS; ++i)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }

        EXPECT_EQ(pPromise->GetState(), Async::States::SettlementState::SUCCESS);
        EXPECT_EQ(pPromise->GetResult(), 10) << "Promise should return 5";

        Async::Stop();
    }

    TEST(SimpleContinuationTest, FunctionCallInContinuation)
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
        }, schedulerName)->Then<int>([](int a) -> int
        {
            return MathUtils::Square(a);
        }, schedulerName);

        for(int i = 0; i < 1000 && pPromise->GetState() != Async::States::SettlementState::SUCCESS; ++i)
        {
            std::this_thread::sleep_for(std::chrono::milliseconds(2));
        }

        EXPECT_EQ(pPromise->GetState(), Async::States::SettlementState::SUCCESS);
        EXPECT_EQ(pPromise->GetResult(), MathUtils::Square(MathUtils::Square(squareArg))) << "Promise should return 25^2";

        Async::Stop();
    }

    TEST(SimpleContinuationTest, ContinuationOutlivesCallingMacro)
    {
        std::string configFilePath = Utilities::OS::GetCurrentDirectory(__FILE__) +
            Utilities::OS::GetPathSep() + "TestEngineConfig.xml";
        Async::Start(configFilePath);

        std::string schedulerName = "TestScheduler";
        //Async::PromisePtr<int> pPromise =
        Async::Execute<int>([]() -> int
        {
            std::cout << "Executing Promise" << std::endl;
            return 5;
        }, schedulerName)->Then<int>([](int a) -> int
        {
            std::cout << "Executing Continuation" << std::endl;
            std::this_thread::sleep_for(std::chrono::seconds(2));
            return a + 5;
        }, schedulerName);

    }

    TEST(SimpleContinuationTest, CheckingSingletonStillExistsAndForceQuitting)
    {
        std::this_thread::sleep_for(std::chrono::seconds(4));
        std::cout << "Calling Async::Stop()" << std::endl;
        ASSERT_EQ(Async::Stop(), true);
    }

} // end of namespace Async_FT
