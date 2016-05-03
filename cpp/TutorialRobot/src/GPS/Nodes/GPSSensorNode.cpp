// SYSTEM INCLUDES
#include <Robos/Robos.hpp>

// C++ PROJECT INCLUDES
#include "Nodes/GPSSensorNode.hpp"
#include "Messages/GPSMessage.hpp"
#include "Project_Code/GenerateGPSData.hpp"

namespace TutorialRobot
{
namespace GPS
{

    GPSSensorNode::GPSSensorNode(int iterations) : Robos::InitNodeBase("GPSSensorNode", "GPSScheduler"), _iterations(iterations)
    {
        // in GenerateGPSData.hpp to intialize path.
        InitializeGPSData(this->_iterations);
    }

    GPSSensorNode::~GPSSensorNode()
    {
    }

    void GPSSensorNode::MainCallbackImpl()
    {
        int currentPosition = 0;
        while(Robos::IsRunning() && currentPosition < this->_iterations)
        {
            GPSMessagePtr pMessage = std::make_shared<GPSMessage>();
            // read off current GPS estimate
            pMessage->gpsCoords = GenerateGPSData(currentPosition);
            this->PublishMessage(pMessage);
        }
        this->ShutdownRobos();
    }

} // end of namespace GPS
} // end of namespace TutorialRobot
