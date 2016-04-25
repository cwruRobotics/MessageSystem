#pragma once
#ifndef STRUCTURES_SEMAPHORE_HPP
#define STRUCTURES_SEMAPHORE_HPP

//SYSTEM INCLUDES
#include <atomic>
#include <mutex>
#include <condition_variable>

// C++ PROJECT INCLUDES
#include "Utilities/LibraryExport.hpp"

/**
 * @namespace Utilities The Utilities namespace
 */
namespace Utilities
{

    /**
     * @brief   Counting Semaphore concurrency primitive
     *          Semaphores are used to control when a thread (or process) waits during a
     *          concurrent algorithm. A counting Semaphore is a data structure that wraps
     *          around an integer value, and supports two kinds of calls: wait() and signal().
     *          When wait() is called, the integer value is decremented, and if that value is < 0
     *          then the calling thread is blocked. When a call to signal() is made, the integer
     *          value is incremented, and if >=0, then the thread that was waiting the longest
     *          is woken up and resumes execution. This structure only behaves correctly if
     *          threads are blocked and awoken in a FIFO (the first to block is the first woken) order.
     */
    class UTILITIES_API Semaphore
    {
    private:

        std::atomic<int>        _count;
        std::atomic<bool>       _trigger;
        std::condition_variable _cv;
        std::mutex              _mutex;

    public:

        /**
         * A constructor. This constructor wraps an integer value which will be the initial value
         * of the Semaphore instance.
         * @param[in]   init    The initial value of the Semaphore. Init number of threads can
         *                      initially call wait() before a thread (or process) is blocked.
         */
        Semaphore(int init);

        /**
         * A destructor. Not virtual to prevent inheritance.
         */
        ~Semaphore();

        void wait();

        void signal();

    };

} // end of namespace Utilities

#endif // end of STRUCTURES_SEMAPHORE_HPP
