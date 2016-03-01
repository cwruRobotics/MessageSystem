#ifndef UTILITES_VECTORUTILS_HPP
#define UTILITES_VECTORUTILS_HPP

// SYSTEM INCLUDES
#include <initializer_list>
#include <vector>

// C++ PROJECT INCLUDES
#include "Utilities/LibraryExport.hpp"
#include "Utilities/VectorUtilsImpl.hpp"

namespace Utilties
{
    template<typename T>
    UTILITIES_API std::vector<T> MakeVector(T... args)
    {
        return std::vector<T>(std::initializer_list<T>(args...));
    }
}

#endif // end of UTILITES_VECTORUTILS_HPP
