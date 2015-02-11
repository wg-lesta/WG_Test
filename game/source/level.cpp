#include "level.h"
#include "enviroment.h"
#include <json/json.h>

#include <algorithm>

#include "properties/property_reader.h"
#include "properties/property_writer.h"

namespace game
{
    level::level()
        : version("1.0")
        , next_object_id(1) {}

    level::level(const std::string & name, size s)
        : version("1.0")
        , name(name)
        , levelsize(s)
        , next_object_id(1) {}


    int level::insert(const std::string & protoname, const enviroment & env)
    {
        prototype proto = env.create_prototype(protoname);
        if (!proto.empty())
        {
            object obj(next_object_id, proto);
            objects.push_back(std::move(obj));
            return next_object_id++;
        }
        return 0;
    }

    void level::remove(int id)
    {
        iterator it = find(id);
        if (it != objects.end())
            objects.erase(it);
    }

    level::const_iterator level::find(int id) const
    {
        const_iterator it = std::lower_bound(objects.cbegin(),
            objects.cend(), id, [](const object & obj, int id)
        {
            return obj.get_id() < id;
        });
        if (it != objects.cend() && it->get_id() == id)
            return it;
        return objects.cend();
    }

    level::iterator level::find(int id)
    {
        iterator it = std::lower_bound(objects.begin(),
            objects.end(), id, [](const object & obj, int id)
        {
            return obj.get_id() < id;
        });
        if (it != objects.cend() && it->get_id() == id)
            return it;
        return objects.end();
    }

    void level::swap(level & other)
    {
        std::swap(version, other.version);
        std::swap(name, other.name);
        std::swap(levelsize, other.levelsize);
        std::swap(objects, other.objects);
        std::swap(next_object_id, other.next_object_id);
    }

    bool level::load(std::istream & in, const enviroment & env)
    {
        level new_level;
        if (new_level.load_impl(in, env))
        {
            swap(new_level);
            return true;
        }
        return false;
        
    }

    bool level::load_impl(std::istream & in, const enviroment & env)
    {
        Json::Reader reader;
        Json::Value root;
        if (!reader.parse(in, root))
            return false;

        version = root.get("version", "1.0").asString();
        name = root.get("name", "unknown").asString();

        Json::Value size_node = root["size"];

        levelsize.cx = size_node.get("width", 0).asInt();
        levelsize.cy = size_node.get("height", 0).asInt();

        Json::Value objects_node = root["objects"];
        for (const Json::Value & raw_object : objects_node)
        {
            std::string protoname = raw_object["prototype"].asString();

            prototype proto = env.create_prototype(protoname);
            if (!proto.empty())
            {
                object object(next_object_id, std::move(proto));

                Json::Value props_node = raw_object["properties"];
                for (const Json::Value & raw_prop : props_node)
                {
                    std::string name = raw_prop["name"].asString();
                    object_property_writer visitor(raw_prop);
                    object.accept(name, visitor);
                }

                objects.push_back(std::move(object));
                ++next_object_id;
            }
        }
        return true;
    }

    void level::save(std::ostream & out) const
    {
        Json::Value root;

        root["version"] = version;
        root["name"] = name;

        root["size"]["width"] = levelsize.cx;
        root["size"]["height"] = levelsize.cy;
        
        Json::Value objects_array(Json::arrayValue);

        for (const object & obj : objects)
        {
            Json::Value object_node;
            object_node["prototype"] = obj.get_name();

            Json::Value proprs_array(Json::arrayValue);
            object_property_reader visitor(proprs_array);
            obj.accept(visitor);
            if (!proprs_array.empty())
                object_node["properties"] = proprs_array;

            objects_array.append(object_node);
        }

        if (!objects_array.empty())
            root["objects"] = objects_array;

        Json::StyledStreamWriter write;
        write.write(out, root);
    }

    const std::string & level::get_name() const
    {
        return name;
    }

    const std::string & level::get_version() const
    {
        return version;
    }

    size level::get_size() const
    {
        return levelsize;
    }
}