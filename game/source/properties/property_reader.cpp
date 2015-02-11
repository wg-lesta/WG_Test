#include "property_reader.h"
#include <json/json.h>

namespace game
{
    object_property_reader::object_property_reader(Json::Value & raw)
        : raw_property(raw) {}

    void object_property_reader::visit(const property_int & prop) const
    {
        if (!prop.read_only())
        {
            Json::Value raw;
            raw["name"] = prop.get_name();
            raw["value"] = prop.get_value();
            raw_property.append(raw);
        }
    }
    void object_property_reader::visit(const property_float & prop) const
    {
        if (!prop.read_only())
        {
            Json::Value raw;
            raw["name"] = prop.get_name();
            raw["value"] = prop.get_value();
            raw_property.append(raw);
        }
    }
    void object_property_reader::visit(const property_point & prop) const
    {
        if (!prop.read_only())
        {
            Json::Value raw;
            raw["name"] = prop.get_name();
            raw["value"]["x"] = prop.get_value().x;
            raw["value"]["y"] = prop.get_value().y;
            raw_property.append(raw);
        }
    }
    void object_property_reader::visit(const property_bool & prop) const
    {
        if (!prop.read_only())
        {
            Json::Value raw;
            raw["name"] = prop.get_name();
            raw["value"] = prop.get_value();
            raw_property.append(raw);
        }
    }
    void object_property_reader::visit(const property_string & prop) const
    {
        if (!prop.read_only())
        {
            Json::Value raw;
            raw["name"] = prop.get_name();
            raw["value"] = prop.get_value();
            raw_property.append(raw);
        }
    }
}