#ifndef ROBOS_INITNODEBASE_HPP
#define ROBOS_INITNODEBASE_HPP

// SYSTEM INCLUDES
#include <memory>   // std::shared_ptr, std::enable_shared_from_this
#include <string>   // std::string

// C++ PROJECT INCLUDES
#include "Robos/LibraryExport.hpp"
#include "Robos/MessageBase.hpp"

namespace Robos
{

    class ROBOS_API InitNodeBase : public std::enable_shared_from_this<InitNodeBase>
    {
    private:

        const std::string               _name;

        // this *should* correspond to the Scheduler name (in Async) that
        // this Node should run on.
        std::string                     _executionTopic;

        virtual MessageBasePtr MainCallbackImpl() = 0;

    public:

        InitNodeBase(std::string name, std::string toRunOn);

        virtual ~InitNodeBase();

        const std::string& GetName();

        std::string& GetExecutionTopic();

        MessageBasePtr MainCallback();
    };

    using InitNodeBasePtr = std::shared_ptr<InitNodeBase>;

}

#endif // end of ROBOS_INITNODEBASE_HPP
