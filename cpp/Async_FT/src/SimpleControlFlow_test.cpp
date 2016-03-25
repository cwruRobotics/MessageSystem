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

    TEST(SimpleControlFlowTest, ServiceRequestAfterStop)
    {
        std::string configFilePath = Utilities::OS::GetCurrentDirectory(__FILE__) +
            Utilities::OS::GetPathSep() + "TestEngineConfig.xml";
        std::string schedulerName = "TestScheduler";

        Async::Start(configFilePath);
        std::this_thread::sleep_for(std::chrono::seconds(1));
        Async::Stop();

        Async::PromisePtr<int> pPromise = Async::Execute<int>([]() -> int
        {
            return MathUtils::Square(5);
        }, schedulerName);

        EXPECT_EQ(pPromise->GetState(), Async::States::SettlementState::PENDING);
    }

    TEST(SimpleControlFlowTest, ContinuationAfterStop)
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
        std::cout << "Calling Async::Stop()" << std::endl;
        ASSERT_EQ(Async::Stop(), true);

    }

} // end of namespace Async_FT
