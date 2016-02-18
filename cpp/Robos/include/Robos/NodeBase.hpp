#ifndef ROBOS_INODE_HPP
#define ROBOS_INODE_HPP

// SYSTEM INCLUDES
#include <memory>   // std::shared_ptr, std::enable_shared_from_this
#include <string>   // std::string

// C++ PROJECT INCLUDES
#include "Robos/LibraryExport.hpp"
#include "Robos/MessageBase.hpp"

namespace Robos
{

    class ROBOS_API NodeBase : public std::enable_shared_from_this<NodeBase>
    {
    private:

        const std::string _name;

        virtual MessageBasePtr MainCallbackImpl(MessageBasePtr& pMessage) = 0;

    public:

        NodeBase(std::string name);

        virtual ~NodeBase();

        MessageBasePtr MainCallback(MessageBasePtr& pMessage);
    };

    using NodeBasePtr = std::shared_ptr<NodeBase>;

}

#endif // end of ROBOS_INODE_HPP
