#pragma once
#ifndef TUTORIALROBOT_GPSSENSORNODE_HPP
#define TUTORIALROBOT_GPSSENSORNODE_HPP


// SYSTEM INCLUDES
#include <Robos/InitNodeBase.hpp>
#include <vector>

// C++ PROJECT INCLUDES
#include "LibraryExport.hpp"


namespace TutorialRobot
{
namespace GPS
{

    class TUTORIALROBOT_GPS_API GPSSensorNode : Robos::InitNodeBase
    {
    private:

        int _iterations;

    private:

        void MainCallbackImpl() override;

    public:

        GPSSensorNode(int iterations);

        ~GPSSensorNode();

    };

    using GPSSensorNodePtr = std::shared_ptr<GPSSensorNode>;

} // end of namespace GPS
} // end of namespace TutorialRobot

#endif // end of TUTORIALROBOT_GPSSENSORNODE_HPP
