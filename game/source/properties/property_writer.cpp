#include "property_writer.h"
#include <json/json.h>

namespace game
{
    object_property_writer::object_property_writer(const Json::Value & raw)
        : raw_property(raw) {}

    void object_property_writer::visit(property_int & prop)
    {
        prop.set_value(raw_property.get("value", 0).asInt());
    }
    void object_property_writer::visit(property_float & prop)
    {
        prop.set_value(raw_property.get("value", 0.0).asFloat());
    }
    void object_property_writer::visit(property_point & prop)
    {
        Json::Value pos_node = raw_property["value"];
        point result =
        {
            pos_node.get("x", 0).asInt(),
            pos_node.get("y", 0).asInt()
        };
        prop.set_value(result);
    }
    void object_property_writer::visit(property_bool & prop)
    {
        prop.set_value(raw_property.get("value", false).asBool());
    }
    void object_property_writer::visit(property_string & prop)
    {
        prop.set_value(raw_property.get("value", "").asString());
    }
}