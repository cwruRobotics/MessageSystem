#ifndef STRUCTURES_TESTS_SIMPLESEMAPHORETEST_HPP
#define STRUCTURES_TESTS_SIMPLESEMAPHORETEST_HPP


// SYSTEM INCLUDES


// C++ PROJECT INCLUDES


namespace Utilities
{
namespace Tests
{

    void SimpleSemaphoreTest(int numThreads);

    void SimpleSemaphoreInterleavedTest(int numThreads);

    void SimpleSemaphoreLinearTest(int numThreads);

} // end of namespace Tests
} // end of namespace Utilities

#endif // end of STRUCTURES_TESTS_SIMPLESEMAPHORETEST_HPP
