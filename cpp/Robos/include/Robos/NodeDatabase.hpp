#ifndef ROBOS_INTERNAL_NODEDATABASE_HPP
#define ROBOS_INTERNAL_NODEDATABASE_HPP

// SYSTEM INCLUDES
#include <functional>   // std::less
#include <map>
#include <memory>       // std::shared_ptr
#include <vector>       // std::vector
#include <Utilities/btree_map.h>


// C++ PROJECT INCLUDES
#include "Robos/NodeBase.hpp"


namespace Robos
{
namespace Internal
{

    
    using NodeDatabaseImpl = stx::btree_map<unsigned int, std::vector<NodeBasePtr>,
                                            std::less<unsigned int>,
                                            stx::btree_default_map_traits<unsigned int, std::vector<NodeBasePtr> > >;

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
