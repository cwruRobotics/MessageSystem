#ifndef STRUCTURES_SEMAPHORE_HPP
#define STRUCTURES_SEMAPHORE_HPP

//SYSTEM INCLUDES
#include <atomic>
#include <mutex>
#include <condition_variable>

// C++ PROJECT INCLUDES
#include "Utilities/LibraryExport.hpp"

namespace Utilities
{

    class UTILITIES_API Semaphore
    {
    private:

        std::atomic<int>        _count;
        std::condition_variable _cv;
        std::mutex              _mutex;

    public:

        Semaphore(int init);

        ~Semaphore();

        void wait();

        void signal();

    };

} // end of namespace Utilities

#endif // end of STRUCTURES_SEMAPHORE_HPP
