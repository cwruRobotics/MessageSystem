// SYSTEM INCLUDES
#include <atomic>
#include <mutex>
#include <Async/Async.hpp>
#include <Utilities/Semaphore.hpp>
#include <iostream>


// C++ PROJECT INCLUDES
#include "Robos/Robos.hpp"
#include "Robos/MasterNode.hpp"
#include "Robos/MessageBase.hpp"
#include "RobosConfig.hpp"

namespace Robos
{
    std::mutex              masterLock;
    std::atomic<bool>       masterInitFlag(false);
    Internal::MasterNodePtr pMaster = nullptr;
    Utilities::Semaphore    robosSemaphore(0);

    Internal::MasterNodePtr GetMasterNode()
    {
        bool val = false;
        {
            std::lock_guard<std::mutex> lock(masterLock);
            val = masterInitFlag;
        }
        if(!val || (val && !pMaster))
        {
            return nullptr;
        }
        return pMaster;
    }

    bool Init(std::string asyncConfigPath)
    {
        if(asyncConfigPath == "")
        {
            asyncConfigPath = Config::ASYNC_CONFIG_PATH;
        }
        bool val = false;
        {
            std::lock_guard<std::mutex> lock(masterLock);
            val = masterInitFlag;
        }
        if(val)
        {
            return false;
        }
        pMaster = std::make_shared<Internal::MasterNode>();
        Async::Start(asyncConfigPath);
        masterInitFlag = true;
        return masterInitFlag;
    }

    void Start()
    {
        std::lock_guard<std::mutex> lock(masterLock);
        if(!masterInitFlag || (masterInitFlag && !pMaster))
        {
            return;
        }
        pMaster->Start();
        masterInitFlag = true;
        return;
    }

    bool Register(const NodeBasePtr& pNode)
    {
        Internal::MasterNodePtr pMaster = GetMasterNode();
        if(!pMaster)
        {
            return false;
        }
        return pMaster->Register(pNode);
    }

    bool Register(const InitNodeBasePtr& pNode)
    {
        Internal::MasterNodePtr pMaster = GetMasterNode();
        if(!pMaster)
        {
            return false;
        }
        return pMaster->RegisterInitNode(pNode);
    }

    bool Stop()
    {
        bool val = false;
        {
            std::lock_guard<std::mutex> lock(masterLock);
            val = masterInitFlag;
        }
        if(!val)
        {
            return false;
        }
        masterInitFlag = false;
        Async::Stop();
        pMaster->Stop();
        pMaster = nullptr;
        return true;
    }

    bool IsRunning()
    {
        bool val = false;
        {
            std::lock_guard<std::mutex> lock(masterLock);
            val = masterInitFlag;
        }
        return val && (pMaster != nullptr);
    }

    void WaitForShutdown()
    {
        robosSemaphore.wait();
    }

    void ShutdownRobosFromNode()
    {
        robosSemaphore.signal();
    }

    void Publish(MessageBasePtr pMessage)
    {
        pMaster->InvokeSubscribers(pMessage);
    }

} // end of namespace Robos
