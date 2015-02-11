#include "property_creators.h"
#include "types.h"
#include <json\json.h>

namespace game
{
    std::unique_ptr<property> property_creator_int::create(const Json::Value & raw)
    {
        std::string name = raw["name"].asString();
        bool readonly = raw.get("readonly", false).asBool();
        std::unique_ptr<property_int> prop(new property_int(name, readonly));

        prop->set_value(raw.get("value", 0).asInt());
        
        Json::Value range = raw["range"];
        prop->set_range(range.get("min", 0).asInt(), range.get("max", 0).asInt());

        return std::unique_ptr<property>(prop.release());
    }

    std::unique_ptr<property> property_creator_float::create(const Json::Value & raw)
    {
        std::string name = raw["name"].asString();
        bool readonly = raw.get("readonly", false).asBool();
        std::unique_ptr<property_float> prop(new property_float(name, readonly));

        prop->set_value(raw.get("value", 0).asFloat());

        Json::Value range = raw["range"];
        prop->set_range(range.get("min", 0).asFloat(), range.get("max", 0).asFloat());

        return std::unique_ptr<property>(prop.release());
    }

    std::unique_ptr<property> property_creator_point::create(const Json::Value & raw)
    {
        std::string name = raw["name"].asString();
        bool readonly = raw.get("readonly", false).asBool();
        std::unique_ptr<property_point> prop(new property_point(name, readonly));

        Json::Value pos_node = raw["value"];
        point result =
        {
            pos_node.get("x", 0).asInt(),
            pos_node.get("y", 0).asInt()
        };
        prop->set_value(result);

        return std::unique_ptr<property>(prop.release());
    }

    std::unique_ptr<property> property_creator_bool::create(const Json::Value & raw)
    {
        std::string name = raw["name"].asString();
        bool readonly = raw.get("readonly", false).asBool();
        std::unique_ptr<property_bool> prop(new property_bool(name, readonly));

        prop->set_value(raw.get("value", 0).asBool());

        return std::unique_ptr<property>(prop.release());
    }

    std::unique_ptr<property> property_creator_string::create(const Json::Value & raw)
    {
        std::string name = raw["name"].asString();
        bool readonly = raw.get("readonly", false).asBool();
        std::unique_ptr<property_string> prop(new property_string(name, readonly));

        prop->set_value(raw.get("value", 0).asString());

        return std::unique_ptr<property>(prop.release());
    }

    property_factory_ptr create_properties_factory()
    {
        property_factory_ptr factory = std::make_unique<property_factory>();
        factory->register_property<property_creator_int>("int");
        factory->register_property<property_creator_float>("float");
        factory->register_property<property_creator_point>("point");
        factory->register_property<property_creator_bool>("bool");
        factory->register_property<property_creator_string>("string");
        return factory;
    }

}