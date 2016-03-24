// SYSTEM INCLUDES
#include <Utilities/rapidxml.hpp>
#include <Utilities/rapidxml_utils.hpp>
#include <Logging/Factory.hpp>
#include <Logging/ILogger.hpp>

// C++ PROJECT INCLUDES
#include "Async/Engine.hpp"
#include "Async/Scheduler.hpp"
#include "Async/JobPriorities.hpp"

#include "AsyncLoggingConfig.hpp"

namespace Async
{
namespace EntryPoint
{

    Engine::Engine(std::string configPath) : _schedulerMap(), _schedulerMapMutex(), _running(true)
    {
        if (configPath != "")
        {
            this->Configure(configPath);
        }
    }

    Engine::~Engine()
    {
        if (this->_running)
        {
            this->Shutdown();
        }
    }

    void Engine::Configure(std::string& configPath)
    {
        std::string loggingPath = Async::LoggingConfig::LOGGING_ROOT + "/EngineLog.txt";
        Logging::LoggerPtr pLogger = Logging::Factory::MakeLogger("EngineLogger",
            loggingPath.c_str());
        LOG_DEBUG(pLogger, "Entering %s", "Engine::Configure()");
        // using ConfigMapType = std::map<std::string, std::vector<Types::JobPriority> >;
        // check validity of file path, open if it exists, and try to parse out configurations
        // for Async. Pass down these configurations to each Scheduler.
        // ConfigMapType configuration;

        // check file path validity
        // open file and parse
        LOG_DEBUG(pLogger, "Creating Document");
        rapidxml::file<> configFile(configPath.c_str());
        rapidxml::xml_document<> doc;

        LOG_DEBUG(pLogger, "XML file to parse: %s", configFile.data());
        doc.parse<rapidxml::parse_declaration_node | rapidxml::parse_no_data_nodes>(configFile.data());

        const char engineXMLId[] = "Engine";
        const char schedulerXMLNodeId[] = "Scheduler";
        const char schedulerNameAttributeXMLId[] = "name";
        const char workerThreadXMLId[] = "threadPriority";
        const char workerThreadPriorityXMLId[] = "value";
        rapidxml::xml_node<>* node = doc.first_node(engineXMLId)->first_node(schedulerXMLNodeId);
        rapidxml::xml_node<>* childNode = nullptr;
        while (node != nullptr)
        {
            std::string name = node->first_attribute(schedulerNameAttributeXMLId)->value();
            LOG_DEBUG(pLogger, "Scheduler Node: %s", name.c_str());
            std::vector<Types::JobPriority> vec;
            childNode = node->first_node(workerThreadXMLId);
            while (childNode != nullptr)
            {
                LOG_DEBUG(pLogger, "\t Thread priority: %s", childNode->first_attribute(workerThreadPriorityXMLId)->value());
                vec.push_back(GetPriorityFromString(childNode->first_attribute(workerThreadPriorityXMLId)->value()));
                childNode = childNode->next_sibling(workerThreadXMLId);
            }
            ISchedulerPtr pScheduler = std::make_shared<Scheduler>(vec, name);
            this->_schedulerMap.insert(std::pair<std::string, ISchedulerPtr>(name, pScheduler));

            node = node->next_sibling(schedulerXMLNodeId);
        }

        LOG_DEBUG(pLogger, "Engine built, exiting Engine::Configure()");
        /*
        for(ConfigMapType::iterator it = configuration.begin(); it != configuration.end(); ++it)
        {
            ISchedulerPtr pScheduler = std::make_shared<Scheduler>(it->second, it->first);
            this->_schedulerMap.insert(std::pair<std::string, ISchedulerPtr>(it->first, pScheduler));
        }
        */
    }

    std::map<std::string, ISchedulerPtr>& Engine::GetSchedulerMap()
    {
        return this->_schedulerMap;
    }

    ISchedulerPtr Engine::FindScheduler(std::string& schedulerId)
    {
        auto index = this->GetSchedulerMap().find(schedulerId);
        if (index == this->GetSchedulerMap().end())
        {
            return nullptr;
        }
        return index->second;
    }

    ISchedulerPtr Engine::GetScheduler(std::string& schedulerId)
    {
        if (this->_running)
        {
            std::lock_guard<std::mutex> mapLock(this->_schedulerMapMutex);
            auto index = this->GetSchedulerMap().find(schedulerId);
            if (index == this->GetSchedulerMap().end())
            {
                return nullptr;
            }
            return index->second;
        }
        return nullptr;
    }

    Types::Result_t Engine::ShutdownScheduler(std::string& schedulerId)
    {
        if (this->_running)
        {
            std::lock_guard<std::mutex> mapLock(this->_schedulerMapMutex);
            ISchedulerPtr pScheduler = this->FindScheduler(schedulerId);
            if (pScheduler)
            {
                pScheduler->Shutdown();
            }
            return Types::Result_t::SUCCESS;
        }
        return Types::Result_t::FAILURE;
    }

    Types::Result_t Engine::StartScheduler(std::string& schedulerId, std::vector<Types::JobPriority> config)
    {
        if (this->_running)
        {
            ISchedulerPtr pScheduler = std::make_shared<Scheduler>(config, schedulerId);

            std::lock_guard<std::mutex> mapLock(this->_schedulerMapMutex);
            this->GetSchedulerMap().insert(std::pair<std::string&,
                ISchedulerPtr>(schedulerId, pScheduler));
            return Types::Result_t::SUCCESS;
        }
        return Types::Result_t::FAILURE;
    }

    Types::Result_t Engine::Shutdown()
    {
        std::lock_guard<std::mutex> mapLock(this->_schedulerMapMutex);

        this->_running = false;
        for (auto it = this->GetSchedulerMap().begin(); it != this->GetSchedulerMap().end(); ++it)
        {
            it->second->Shutdown();
        }
        return Types::Result_t::SUCCESS;
    }

} // end of namespace EntryPoint
} // end of namespace Async
