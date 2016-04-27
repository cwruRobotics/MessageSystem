#pragma once
#ifndef TUTORIALROBOS_GPSMESSAGE_HPP
#define TUTORIALROBOS_GPSMESSAGE_HPP


// SYSTEM INCLUDES
#include <Robos/MessageBase.hpp>
#include <vector>

// C++ PROJECT INCLUDES
#include "LibraryExport.hpp"


namespace TutorialRobot
{
namespace GPS
{

    struct TUTORIAL_GPS_API GPSMessage : public Robos::MessageBase
    {
        std::vector<double> gpsCoords;

        GPSMessage();

        ~GPSMessage();
    };

    using GPSMessagePtr = std::shared_ptr<GPSMessage>;

} // end of namespace GPS
} // end of namespace TutorialRobot

#endif // end of TUTORIALROBOS_GPSMESSAGE_HPP
