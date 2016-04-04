#ifndef ROBOS_NODEBASE_HPP
#define ROBOS_NODEBASE_HPP

// SYSTEM INCLUDES
#include <memory>   // std::shared_ptr, std::enable_shared_from_this
#include <string>   // std::string
#include <vector>   // std::vector

// C++ PROJECT INCLUDES
#include "Robos/LibraryExport.hpp"
#include "Robos/MessageBase.hpp"

namespace Robos
{

    class ROBOS_API NodeBase : public std::enable_shared_from_this<NodeBase>
    {
    private:

        const std::string               _name;

        // this *should* correspond to the Scheduler name (in Async) that
        // this Node should run on.
        std::string                     _executionTopic;

        const std::vector<std::string>  _subscriptions;

        virtual MessageBasePtr MainCallbackImpl(const MessageBasePtr pMessage) = 0;

    public:

        NodeBase(std::string name, std::string toRunOn,
                 std::vector<std::string> subscriptions);

        virtual ~NodeBase();

        const std::string& GetName();

        std::string& GetExecutionTopic();

        const std::vector<std::string>& GetSubscriptions();

        MessageBasePtr MainCallback(const MessageBasePtr pMessage);
    };

    using NodeBasePtr = std::shared_ptr<NodeBase>;

}

#endif // end of ROBOS_NODEBASE_HPP
