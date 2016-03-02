// SYSTEM INCLUDES

// C++ PROJECT INCLUDES
#include "Utilities/Semaphore.hpp"

namespace Utilities
{

    Semaphore::Semaphore(int init) : _count(init), _cv(), _mutex()
    {
    }

    Semaphore::~Semaphore()
    {
    }

    void Semaphore::wait()
    {
        std::unique_lock<std::mutex> mutexLock(this->_mutex);
        //this->_count--;
        if(--this->_count < 0)
        {
            this->_cv.wait(mutexLock);
        }
    }

    void Semaphore::signal()
    {
        std::lock_guard<std::mutex> mutexLock(this->_mutex);
        //this->_count++;
        if(++this->_count <= 0)
        {
            this->_cv.notify_one();
        }
    }

} // end of namespace Utilities
