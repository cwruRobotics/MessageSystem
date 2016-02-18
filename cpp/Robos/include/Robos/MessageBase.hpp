#ifndef ROBOS_MESSAGEBASE_HPP
#define ROBOS_MESSAGEBASE_HPP


// SYSTEM INCLUDES
#include <memory>   // std::shared_ptr, std::enable_shared_from_this
#include <string>   // std::string


// C++ PROJECT INCLUDES
#include "Robos/LibraryExport.hpp"

namespace Robos
{

    struct ROBOS_API MessageBase : public std::enable_shared_from_this<MessageBase>
    {
        const std::string topic;

        MessageBase(std::string s);

        virtual ~MessageBase();

    };

    using MessageBasePtr = std::shared_ptr<MessageBase>;

}

#endif // end of ROBOS_MESSAGEBASE_HPP
