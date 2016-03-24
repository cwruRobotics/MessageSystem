
// SYSTEM INCLUDES
#include <gtest/gtest.h>
#include <Utilities/OSUtils.hpp>
#include <Async/Async.hpp>
#include <Async/IEngine.hpp>

// C++ PROJECT INCLUDES


namespace Async_FT
{

    TEST(AsyncSingletonTest, StartAsyncSingleton)
    {
        ASSERT_EQ(Async::GetEngineSingleton(), nullptr) << "Engine should be null before a call to Async::Start()";

        std::string configFilePath = Utilities::OS::GetCurrentDirectory(__FILE__) + "\\TestEngineConfig.xml";
        Async::Start(configFilePath);

        ASSERT_NE(Async::GetEngineSingleton(), nullptr) << "Engine should not be null after" <<
            "a call to Async::Start() assuming the path to the config file is a valid xml file";

        Async::Stop();

        ASSERT_EQ(Async::GetEngineSingleton(), nullptr) << "Engine should be null after a call to Async::Stop()";
    }

} // end of namespace Async_FT

