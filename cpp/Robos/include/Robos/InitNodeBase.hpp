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
namespace Internal
{
    class MasterNode;
} // end of namespace Internal

    class ROBOS_API InitNodeBase : public std::enable_shared_from_this<InitNodeBase>
    {
    private:

        const std::string                       _name;
        std::shared_ptr<Internal::MasterNode>   _pMaster;

        // this *should* correspond to the Scheduler name (in Async) that
        // this Node should run on.
        std::string                             _executionTopic;

        virtual void MainCallbackImpl() = 0;

        void RegisterMaster(std::shared_ptr<Internal::MasterNode> pMaster);

    protected:

        void PublishMessage(MessageBasePtr pMessage);

    public:

        InitNodeBase(std::string name, std::string toRunOn);

        virtual ~InitNodeBase();

        const std::string& GetName();

        std::string& GetExecutionTopic();

        int MainCallback();
    };

    using InitNodeBasePtr = std::shared_ptr<InitNodeBase>;

}

#endif // end of ROBOS_INITNODEBASE_HPP
