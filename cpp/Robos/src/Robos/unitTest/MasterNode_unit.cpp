// SYSTEM INCLUDES
#include <iostream>
#include <Utilities/OSUtils.hpp>
#include <Async/Async.hpp>          // Async::Start(), Async::Stop()

// C++ PROJECT INCLUDES
#include "catch/catch.hpp"
#include "Robos/MasterNode.hpp"

#include "Robos/unitTest/TestMessageA.hpp"
#include "Robos/unitTest/TestMessageB.hpp"
#include "Robos/unitTest/TestNode.hpp"
#include "Robos/unitTest/TestNodeNameTemplate.hpp"
#include "Robos/unitTest/TestNodeSubscriberTemplate.hpp"


namespace OSUtils = Utilities::OS;

namespace Robos
{
namespace Tests
{

    char DigitToChar(int digit)
    {
        switch(digit)
        {
            case 0:
                return '0';
            case 1:
                return '1';
            case 2:
                return '2';
            case 3:
                return '3';
            case 4:
                return '4';
            case 5:
                return '5';
            case 6:
                return '6';
            case 7:
                return '7';
            case 8:
                return '8';
            case 9:
                return '9';
        }
        throw std::exception();
    }

    std::string IntToString(int n)
    {
        std::string s;
        while(n > 0)
        {
            s += DigitToChar(n % 10);
            n /= 10;
        }
        return s;
    }

    TEST_CASE("Instantiating MasterNode", "[MasterNode_unit]")
    {
        Internal::MasterNode mn;
    }

    TEST_CASE("Registering a TestNode with MasterNode", "[MasterNode_unit]")
    {
        Internal::MasterNode mn;
        TestNodePtr pTestNode = std::make_shared<TestNode>();

        REQUIRE( mn.Register(pTestNode) );
    }

    TEST_CASE("Registering Nodes with MasterNode", "[MasterNode_unit]")
    {
        for(int numNodes = 10; numNodes <= 1000; numNodes *= 10)
        {
            // std::cout << "Registering [" << numNodes << "] nodes with Master" << std::endl;
            Internal::MasterNode mn;
            for(int i = 0; i < numNodes; ++i)
            {
                NodeBasePtr pTestTemplate =
                    std::make_shared<TestNodeNameTemplate>("testNode" + IntToString(i));
                // std::cout << "\tRegistering node [" << pTestTemplate->GetName().c_str() << "] with Master" << std::endl;
                REQUIRE( mn.Register(pTestTemplate) );
            }
        }
    }

    TEST_CASE("Registering a Node with many subscriptions", "[MasterNode_unit]")
    {
        std::vector<std::string> subscriptions;
        for(int numSubscriptions = 1; numSubscriptions <= 1000; numSubscriptions *= 10)
        {
            Internal::MasterNode mn;
            subscriptions.push_back("testMessage" + IntToString(numSubscriptions));
            REQUIRE( mn.Register(std::make_shared<TestNodeSubscriberTemplate>(subscriptions)) );
        }
    }

    TEST_CASE("Invoking a TestNode with a Message", "[MasterNode_unit]")
    {
        Async::Start(OSUtils::GetCurrentDirectory(__FILE__) + OSUtils::GetPathSep() + "TestEngineConfig.xml");
        TestMessageAPtr pTestMessageA = std::make_shared<TestMessageA>();
        pTestMessageA->data = 10;

        Internal::MasterNodePtr pMaster = std::make_shared<Internal::MasterNode>();
        TestNodePtr pTestNode = std::make_shared<TestNode>();

        REQUIRE( pMaster->Register(pTestNode) );
        try
        {
            pMaster->InvokeSubscribers(pTestMessageA);
            REQUIRE( true );
        }
        catch(std::exception e)
        {
            Async::Stop();
            std::cout << "[Caught exception]: " << e.what() << std::endl;
            REQUIRE( false );
        }
        Async::Stop();
    }

    TEST_CASE("Invoking multiple nodes with a Message", "[MasterNode_unit]")
    {
        TestMessageAPtr pTestMessageA = std::make_shared<TestMessageA>();
        pTestMessageA->data = 10;

        for(int numNodes = 10; numNodes <= 1000; numNodes *= 10)
        {
            Async::Start(OSUtils::GetCurrentDirectory(__FILE__) + OSUtils::GetPathSep() + "TestEngineConfig.xml");
            // std::cout << "Registering [" << numNodes << "] nodes with Master" << std::endl;
            Internal::MasterNode mn;
            for(int i = 0; i < numNodes; ++i)
            {
                NodeBasePtr pTestTemplate =
                    std::make_shared<TestNodeNameTemplate>("testNode" + IntToString(i));
                // std::cout << "\tRegistering node [" << pTestTemplate->GetName().c_str() << "] with Master" << std::endl;
                REQUIRE( mn.Register(pTestTemplate) );

                try
                {
                    mn.InvokeSubscribers(pTestMessageA);
                    REQUIRE( true );
                }
                catch(std::exception e)
                {
                    Async::Stop();
                    std::cout << "[Caught exception]: " << e.what() << std::endl;
                    REQUIRE( false );
                }
            }
            Async::Stop();
        }
    }

} // end of Tests
} // end of Robos
