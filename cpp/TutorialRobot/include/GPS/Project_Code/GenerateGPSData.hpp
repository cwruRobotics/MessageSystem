#pragma once
#ifndef TUTORIALROBOT_GENERATEGPSDATA_HPP
#define TUTORIALROBOT_GENERATEGPSDATA_HPP


// SYSTEM INCLUDES
#include <vector>

// C++ PROJECT INCLUDES

namespace TutorialRobot
{
namespace GPS
{

    void InitializeGPSData(int sampleSize);

    std::vector<double> GenerateGPSData(int position);

} // end of namespace GPS
} // end of namespace TutorialRobot

#endif // end of TUTORIALROBOT_GENERATEGPSDATA_HPP
