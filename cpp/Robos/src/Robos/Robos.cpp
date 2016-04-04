// SYSTEM INCLUDES
#include <atomic>
#include <mutex>
#include <Async/Async.hpp>


// C++ PROJECT INCLUDES
#include "Robos/Robos.hpp"
#include "Robos/MasterNode.hpp"

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
        bool val = false;
        {
            std::lock_guard<std::mutex> lock(masterLock);
            val = masterInitFlag;
        }
        if(!val || (val && !pMaster))
        {
            return;
        }
        pMaster->Start();
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
        Async::Stop();
        pMaster = nullptr;
        masterInitFlag = false;
        return true;
    }

} // end of namespace Robos
