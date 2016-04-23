#pragma once
#ifndef ROBOS_ROBOS_HPP
#define ROBOS_ROBOS_HPP

// SYSTEM INCLUDES
#include <string>                   // std::string

// C++ PROJECT INCLUDES
#include "Robos/LibraryExport.hpp"
#include "Robos/NodeBase.hpp"
#include "Robos/InitNodeBase.hpp"
#include "RobosConfig.hpp"

/**
 * @namespace Robos The Robos namespace.
 */
namespace Robos
{

    /**
     * Initializes Robos and sets up the environment for use.
     * This function should only be called once (subsequent calls will have no effect),
     * and has no effect if called after Robos::Stop().
     * @param[in]   asyncConfigPath     The absolute path to the Async config file.
     * @param[out]  bool                True if successful, false otherwise.
     */
    ROBOS_API bool Init(std::string asyncConfigPath=Config::ASYNC_CONFIG_PATH);

    /**
     * Starts Robos. During a call to Robos::Start(), the master node will start
     * executing Nodes. A call to this function will only have effect upon the first
     * call and if called after Robos::Init().
     */
    ROBOS_API void Start();

    /**
     * Registers a NodeBase with master node. The master node is not exposed to client
     * code, so this is the only way a NodeBase can be registered for execution.
     * Once a call to Robos::Start() has been made (and Robos is already initialized),
     * this function will have no effect.
     * @param[out]  bool    True if registration was successful, false otherwise.
     */
    ROBOS_API bool Register(const NodeBasePtr& pNode);

    /**
     * Registers an InitNodeBase with master node. The master node is not exposed to client
     * code, so this is the only way an InitNodeBase can be registered for execution.
     * Once a call to Robos::Start() has been made (and Robos is already initialized),
     * this function will have no effect.
     * param[out]   bool    True if registration was successful, false otherwise.
     */
    ROBOS_API bool Register(const InitNodeBasePtr& pNode);

    /**
     * Stops and shuts down Robos. The execution graph is purged, the environment is
     * tore down, and the master node will not relay any more messages.
     * @param[out]   bool    True if shutdown was successful, false otherwise.
     */
    ROBOS_API bool Stop();

    /**
     * Utility function for loops invariants that include running for the entirety of Robos.
     * @param[out]  bool    True after Robos::Start() and before Robos::Stop() has been called, false otherwise.
     */
    ROBOS_API bool IsRunning();

} // end of namespace Robos

#endif // end of ROBOS_ROBOS_HPP
