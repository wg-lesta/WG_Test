#ifndef __PROPERTY_CREATORS_INCLUDED__
#define __PROPERTY_CREATORS_INCLUDED__

#include <memory>
#include <json/forwards.h>
#include "property_factory.h"
#include "property.h"

namespace game
{
    struct property_creator_int : property_creator_base
    {
        std::unique_ptr<property> create(const Json::Value & raw) override;
    };

    struct property_creator_float : property_creator_base
    {
        std::unique_ptr<property> create(const Json::Value & raw) override;
    };

    struct property_creator_point : property_creator_base
    {
        std::unique_ptr<property> create(const Json::Value & raw) override;
    };

    struct property_creator_bool : property_creator_base
    {
        std::unique_ptr<property> create(const Json::Value & raw) override;
    };

    struct property_creator_string : property_creator_base
    {
        std::unique_ptr<property> create(const Json::Value & raw) override;
    };

    using property_factory_ptr = std::unique_ptr<property_factory>;
    property_factory_ptr create_properties_factory();
}


#endif //__PROPERTY_FACTORY_INCLUDED__
