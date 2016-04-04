#ifndef ROBOS_ROBOS_HPP
#define ROBOS_ROBOS_HPP

// SYSTEM INCLUDES
#include <string>

// C++ PROJECT INCLUDES
#include "Robos/LibraryExport.hpp"
#include "Robos/NodeBase.hpp"
#include "Robos/InitNodeBase.hpp"
#include "RobosConfig.hpp"

namespace Robos
{

    ROBOS_API bool Init(std::string asyncConfigPath=Config::ASYNC_CONFIG_PATH);

    ROBOS_API void Start();

    ROBOS_API bool Register(const NodeBasePtr& pNode);

    ROBOS_API bool Register(const InitNodeBasePtr& pNode);

    ROBOS_API bool Stop();

} // end of namespace Robos

#endif // end of ROBOS_ROBOS_HPP
