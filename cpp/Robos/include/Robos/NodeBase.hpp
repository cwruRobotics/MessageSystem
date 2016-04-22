#pragma once
#ifndef ROBOS_NODEBASE_HPP
#define ROBOS_NODEBASE_HPP

// SYSTEM INCLUDES
#include <memory>   // std::shared_ptr, std::enable_shared_from_this
#include <string>   // std::string
#include <vector>   // std::vector

// C++ PROJECT INCLUDES
#include "Robos/LibraryExport.hpp"
#include "Robos/NodeHelper.hpp"
#include "Robos/MessageBase.hpp"

namespace Robos
{

    class ROBOS_API NodeBase : public NodeHelper,
        public std::enable_shared_from_this<NodeBase>
    {
    private:

        const std::vector<std::string>  _subscriptions;

        /**
          * this method must be overriden.
          *
          */
        virtual MessageBasePtr MainCallbackImpl(const MessageBasePtr pMessage) = 0;

    public:

        NodeBase(std::string name, std::string toRunOn,
                 std::vector<std::string> subscriptions,
                 Async::Types::JobPriority priority=Async::Types::JobPriority::OTHER);

        virtual ~NodeBase();

        const std::vector<std::string>& GetSubscriptions();

        MessageBasePtr MainCallback(const MessageBasePtr pMessage);
    };

    using NodeBasePtr = std::shared_ptr<NodeBase>;

}

#endif // end of ROBOS_NODEBASE_HPP
