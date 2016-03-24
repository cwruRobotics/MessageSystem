// SYSTEM INCLUDES
#include <iostream>

// C++ PROJECT INCLUDES
#include "Async/unitTest/TestUtilities.hpp"

namespace Async
{
namespace Tests
{
namespace Utilities
{

    void WriteUnitTestNameToConsole(std::string unitTestName)
    {
        std::cout << "Running[" << unitTestName.c_str() << "]" << std::endl;
    }

    std::vector<Types::JobPriority> GetDefaultSchedulerConfig()
    {
        std::vector<Types::JobPriority> vec = {Types::JobPriority::IMMEDIATE,
                                               Types::JobPriority::RELAXED,
                                               Types::JobPriority::OTHER};
        return vec;
    }

    std::vector<std::string> GetSchedulersToCheckFor(std::string configFilePath)
    {
        std::vector<std::string> schedulersInConfigFile;
        const char engineXMLId[] = "Engine";
        const char schedulerXMLNodeId[] = "Scheduler";
        const char schedulerNameAttributeXMLId[] = "name";
        rapidxml::file<> configFile(configFilePath.c_str());
        rapidxml::xml_document<> doc;
        doc.parse<rapidxml::parse_declaration_node | rapidxml::parse_no_data_nodes>(configFile.data());

        rapidxml::xml_node<>* node = doc.first_node(engineXMLId)->first_node(schedulerXMLNodeId);
        while(node != nullptr)
        {
            schedulersInConfigFile.push_back(node->first_attribute(schedulerNameAttributeXMLId)->value());
            node = node->next_sibling(schedulerXMLNodeId);
        }
        return schedulersInConfigFile;
    }

} // end of namespace Utilities
} // end of namespace Tests
} // end of namespace Async