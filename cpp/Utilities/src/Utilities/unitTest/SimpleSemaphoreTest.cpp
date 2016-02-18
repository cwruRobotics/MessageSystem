// SYSTEM INCLUDES
#include <iostream>
#include <memory>
#include <thread>
#include <vector>

// C++ PROJECT INCLUDES
#include "catch/catch.hpp"

#include "Utilities/unitTest/SimpleSemaphoreTest.hpp"
#include "Utilities/unitTest/CommonMemory.hpp"
#include "Utilities/unitTest/Signaler.hpp"
#include "Utilities/unitTest/Waiter.hpp"


namespace Utilities
{
namespace Tests
{

    void SimpleSemaphoreTest(int numThreads)
    {
        std::cout << "Entering SimpleSemaphoreTest()" << std::endl;
        SimpleSemaphoreInterleavedTest(numThreads);

        SimpleSemaphoreLinearTest(numThreads);
        // spawn all the waiters first...waiters should wait until signalers come and signal them
        std::cout << "Exiting SimpleSemaphoreTest()" << std::endl;
    }

    void SimpleSemaphoreInterleavedTest(int numThreads)
    {
        CommonMemoryPtr pCommon = std::make_shared<CommonMemory>(0);

        std::vector<std::thread> threads(numThreads);
        std::thread additionalSignaler;

        // want to interleave waiters and signalers
        std::cout << "Spawning " << numThreads << " Threads" << std::endl;
        for(int i = 0; i < numThreads; ++i)
        {
            if(i % 2 == 0)
            {
                std::cout << "Spawning Waiter thread" << std::endl;
                threads[i] = std::thread(&Waiter, pCommon);
            }
            else
            {
                std::cout << "Spawning Signaler thread" << std::endl;
                threads[i] = std::thread(&Signaler, pCommon);
            }
        }
        if(numThreads % 2 != 0)
        {
            std::cout << "Spawning additional Signaler thread" << std::endl;
            // spawn another signaler thread
            additionalSignaler = std::thread(&Signaler, pCommon);
        }

        std::cout << "Waiting for threads to finish" << std::endl;
        for(int i = 0; i < numThreads; ++i)
        {
            if(threads[i].joinable())
            {
                threads[i].join();
            }
        }
        if(additionalSignaler.joinable())
        {
            std::cout << "Waiting for additional Signaler thread to finish" << std::endl;
            additionalSignaler.join();
        }
    }

    void SimpleSemaphoreLinearTest(int numThreads)
    {
        int numSignalers = numThreads >> 1;
        int numWaiters = numThreads >> 1;
        CommonMemoryPtr pCommon = std::make_shared<CommonMemory>(0);

        std::vector<std::thread> threads(numSignalers + numWaiters);

        // want to interleave waiters and signalers
        std::cout << "Spawning " << numThreads << " Threads" << std::endl;
        for(int i = 0; i < numWaiters; ++i)
        {
            std::cout << "Spawning Waiter thread" << std::endl;
            threads[i] = std::thread(&Waiter, pCommon);
        }
        for(int i = 0; i < numSignalers; ++i)
        {
            std::cout << "Spawning Signaler thread" << std::endl;
            threads[numWaiters + i] = std::thread(&Signaler, pCommon);
        }

        std::cout << "Waiting for threads to finish" << std::endl;
        for(int i = 0; i < numSignalers + numWaiters; ++i)
        {
            if(threads[i].joinable())
            {
                threads[i].join();
            }
        }

    }

} // end of namespace Tests
} // end of namespace Utilities
