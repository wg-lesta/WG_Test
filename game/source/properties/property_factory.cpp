#include "property_factory.h"
#include "types.h"

namespace game
{
    
    property_factory::~property_factory()
    {
        for (auto & value : factory)
            delete value.second;
    }

    std::unique_ptr<property> property_factory::create(const std::string & type, const Json::Value & raw) const
    {
        factory_map::const_iterator it = factory.find(type);
        if (it != factory.cend())
            return it->second->create(raw);
        return std::unique_ptr<property>{};
    }

    void property_factory::register_property(const std::string & type, property_creator_base * creator)
    {
        factory_map::const_iterator it = factory.find(type);
        if (it == factory.cend())
            factory[type] = creator;
    }
    
}