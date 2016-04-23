#pragma once
#ifndef ROBOS_MESSAGEBASE_HPP
#define ROBOS_MESSAGEBASE_HPP


// SYSTEM INCLUDES
#include <memory>   // std::shared_ptr, std::enable_shared_from_this
#include <string>   // std::string


// C++ PROJECT INCLUDES
#include "Robos/LibraryExport.hpp"

/**
 * @namespace Robos The Robos namespace
 */
namespace Robos
{

    /**
     * Base class for all Message instances.
     * Messages are the data structure that Robos uses to pass data between client programs,
     * also known as "Nodes." Derived Messages can contain arbitrary data types
     * with the restriction that clients need to handle data type conversions to embedded
     * languages.
     */
    struct ROBOS_API MessageBase : public std::enable_shared_from_this<MessageBase>
    {

        /**
         * A summary of the data contained in a MessageBase instance.
         * This value matches an element of the vector of Node subscriptions in
         * the NodeBaseHelper class if and only if that Node will be executed when an instance
         * of this MessageBase is published.
         */
        const std::string topic;

        /**
         * A constructor.
         * @param[in]   s   The message topic.
         */
        MessageBase(std::string s);

        /**
         * A destructor. Virtual to allow inheritance.
         */
        virtual ~MessageBase();

    };

    /**
     * an alias for a shared pointer to a MessageBase instance.
     */
    using MessageBasePtr = std::shared_ptr<MessageBase>;

}

#endif // end of ROBOS_MESSAGEBASE_HPP
