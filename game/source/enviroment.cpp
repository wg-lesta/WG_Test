#include "enviroment.h"

#include <fstream>
#include <algorithm>
#include <json/json.h>

#include "properties/property_creators.h"

namespace game
{
    enviroment::enviroment()
        :version("1.0")
    {

    }
    const std::string & enviroment::get_version() const
    {
        return version;
    }

    const std::string & enviroment::get_tileset() const
    {
        return tileset;
    }

    size enviroment::get_tilesize() const
    {
        return tilesize;
    }

    bool enviroment::empty() const
    {
        return tilesize.empty();
    }

    enviroment::const_iterator enviroment::find_prototype(const std::string name) const
    {
        prototypes_t::const_iterator it = std::lower_bound(prototypes.cbegin(),
            prototypes.cend(), name, [](const prototype & proto, const std::string & name)
        {
            return proto.get_name() < name;
        });
        if (it != prototypes.cend() && it->get_name() == name)
            return it;
        return prototypes.cend();
    }

    prototype enviroment::create_prototype(const std::string & name) const
    {
        prototypes_t::const_iterator it = find_prototype(name);
        if (it != prototypes.cend())
            return *it;
        return prototype{};
    }

    void enviroment::request_property(const std::string & protoname,
        property_read_visitor & visitor) const
    {
        prototypes_t::const_iterator it = find_prototype(protoname);
        if (it != prototypes.cend())
            it->accept(visitor);
    }

    bool enviroment::load(std::istream & is)
    {
        enviroment new_env;
        if (new_env.load_impl(is))
        {
            swap(new_env);
            return true;
        }
        return false;
    }

    bool enviroment::load_impl(std::istream & is)
    {
        Json::Value root;
        Json::Reader reader;
        if (!reader.parse(is, root))
            return false;

        version = root.get("version", "1.0").asString();
        tileset = root.get("tileset", "game_32X32.png").asString();

        Json::Value size_node = root["tilesize"];
        tilesize.cx = size_node.get("width", 0).asInt();
        tilesize.cy = size_node.get("height", 0).asInt();

        property_factory_ptr factory = create_properties_factory();

        Json::Value protos_node = root["prototypes"];
        for (const Json::Value & raw_prototype : protos_node)
        {
            std::string protoname = raw_prototype.get("protoname", "").asString();
            if (!protoname.empty())
            {
                Json::Value props_node = raw_prototype["properties"];
                prototype::properties_t properties;
                for (const Json::Value & raw_property : props_node)
                {
                    std::string name = raw_property["name"].asString();
                    std::string type = raw_property["type"].asString();
                    properties.push_back(factory->create(type, raw_property));
                }

                if (!properties.empty())
                    prototypes.emplace_back(protoname, std::move(properties));
            }
        }

        sort_prototypes();

        return true;
    }

    void enviroment::sort_prototypes()
    {
        std::sort(prototypes.begin(), prototypes.end(), [](const prototype & l, const prototype & r)
        {
            return l.get_name() < r.get_name();
        });
    }

    void enviroment::swap(enviroment & env)
    {
        std::swap(version, env.version);
        std::swap(tileset, env.tileset);
        std::swap(tilesize, env.tilesize);
        std::swap(prototypes, env.prototypes);
    }
}

