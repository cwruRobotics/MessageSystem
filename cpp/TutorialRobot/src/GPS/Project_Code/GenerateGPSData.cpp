// SYSTEM INCLUDES
#include <random>

// C++ PROJECT INCLUDES
#include "Project_Code/GenerateGPSData.hpp"

namespace TutorialRobot
{
namespace GPS
{

std::vector<std::vector<double> > globalGPSData;

    void InitializeData(int sampleSize)
    {
        globalGPSData.reserve(sampleSize);
        std::default_random_engine xGen(0);
        std::default_random_engine yGen(0);
        std::normal_distribution<> x(0.0, 1.0);
        std::normal_distribution<> y(0.0, 1.0);
        for(int i = 0; i < sampleSize; ++i)
        {
            globalGPSData[i] = std::vector<double>{x(xGen), y(yGen)};
        }
    }

    std::vector<double> GenerateGPSData(int position)
    {
        std::lock_guard<std::mutex> lock(positionMutex);
        if(position < globalGPSData.size())
        {
            return globalGPSData[position];
        }
        return std::vector<double>{0, 0};
    }

} // end of namespace GPS
} // end of namespace TutorialRobot