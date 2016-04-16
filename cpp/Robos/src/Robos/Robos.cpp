// SYSTEM INCLUDES
#include <atomic>
#include <mutex>
#include <Async/Async.hpp>
#include <iostream>


// C++ PROJECT INCLUDES
#include "Robos/Robos.hpp"
#include "Robos/MasterNode.hpp"
#include "Robos/MessageBase.hpp"

namespace Robos
{
    std::mutex              masterLock;
    std::atomic<bool>       masterInitFlag(false);
    Internal::MasterNodePtr pMaster = nullptr;

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

    void Publish(MessageBasePtr pMessage)
    {
        pMaster->InvokeSubscribers(pMessage);
    }

} // end of namespace Robos
