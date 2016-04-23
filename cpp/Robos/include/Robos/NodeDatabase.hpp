#pragma once
#ifndef ROBOS_INTERNAL_NODEDATABASE_HPP
#define ROBOS_INTERNAL_NODEDATABASE_HPP

// SYSTEM INCLUDES
#include <functional>   // std::less
// #include <map>          
#include <memory>       // std::shared_ptr
#include <vector>       // std::vector
#include <Utilities/btree_map.h>


// C++ PROJECT INCLUDES
#include "Robos/NodeBase.hpp"


/**
 * @namespace Robos The Robos namespace.
 */
namespace Robos
{

/**
 * @namespace Internal a namespace internal to Robos.
 */
namespace Internal
{

    /*
    using NodeDatabaseImpl = stx::btree_map<unsigned int, std::vector<NodeBasePtr>,
                                            std::less<unsigned int>,
                                            stx::btree_default_map_traits<unsigned int, std::vector<NodeBasePtr> > >;
    */
    /**
     * alias for B+ tree that is used as a Node database.
     * The B+ tree stores (std::string, std::vector<NodeBasePtr>) tuples where std::string is a
     * message topic and std::vector<NodeBasePtr> is the list of Nodes that subscribe to that topic.
     * Note that there may be multiple copies of a Node in this database if and only if
     * a Node subscribes to more than one message topic.
     */
    using NodeDatabaseImpl = stx::btree_map<std::string, std::vector<NodeBasePtr>,
                                            std::less<std::string>,
                                            stx::btree_default_map_traits<std::string, std::vector<NodeBasePtr> > >;

    /**
     * Wrapper for the node database. Handles instantiating and destruction of
     * stx::btree_map instance.
     */
    struct NodeDatabase
    {

        NodeDatabaseImpl map;

        /**
         * A constructor.
         */
        NodeDatabase();

        /**
         * A destructor.
         * Not virtual to prevent inheritance.
         */
        ~NodeDatabase();

    };

    /**
     * alias for a shared pointer to a NodeDatabase instance.
     */
    using NodeDatabasePtr = std::shared_ptr<NodeDatabase>;

} // end of namespace Internal
} // end of namespace Robos


#endif // end of ROBOS_INTERNAL_NODEDATABASE_HPP
