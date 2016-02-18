#ifndef ROBOS_INTERNAL_NODEDATABASE_HPP
#define ROBOS_INTERNAL_NODEDATABASE_HPP

// SYSTEM INCLUDES
#include <functional>   // std::less
#include <memory>       // std::shared_ptr
#include <vector>       // std::vector

// C++ PROJECT INCLUDES
#include "Utilities/btree_map.h"

#include "Robos/NodeBase.hpp"

namespace Robos
{
namespace Internal
{

    using NodeDatabaseImpl = stx::btree_map<std::string, std::vector<NodeBasePtr>,
                                            std::less<std::string>, stx::btree_default_set_traits<std::string> >;
    struct NodeDatabase
    {
        NodeDatabaseImpl map;

        NodeDatabase();

        ~NodeDatabase();

    };

    using NodeDatabasePtr = std::shared_ptr<NodeDatabase>;

} // end of namespace Internal
} // end of namespace Robos


#endif // end of ROBOS_INTERNAL_NODEDATABASE_HPP
