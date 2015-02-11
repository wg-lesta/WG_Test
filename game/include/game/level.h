#ifndef __LEVEL_INCLUDED__
#define __LEVEL_INCLUDED__

#include <string>
#include <list>

#include "forward.h"
#include "object.h"
#include "types.h"

/* 
    Game level

    {
        "version": 1.0,
        "name": "levelname",
        "size": { "width": 10, "height": 10 },
        "objects":
        [
            {
                "prototype": "prototype name from enviroment",
                "properties":
                [
                    {"name": "property name", "value": "value"}
                ]
            },
            {
                "prototype": "prototype name from enviroment",
                "properties":
                [
                    {"name": "property name", "value": "value"}
                ]
            }
        ]
    }

*/

namespace game
{
    class level
    {
    public:
        
        using objects_t = std::list<object>;
        using iterator = objects_t::iterator;
        using const_iterator = objects_t::const_iterator;

        level();
        level(const std::string & name, size s);

        bool load(std::istream & in, const enviroment & env);
        void save(std::ostream & out) const;

        const std::string & get_name() const;
        const std::string & get_version() const;

        size get_size() const;

        template<typename _Func>
        bool read_object(int id, const _Func & func) const
        {
            const_iterator it = find(id);
            if (it == objects.end())
                return false;
            func(*it);
            return true;
        }
        
        template<typename _Func>
        bool write_object(int id, _Func & func)
        {
            iterator it = find(id);
            if (it == objects.end())
                return false;
            func(*it);
            return true;
        }

        template<typename _Func>
        bool foreach_objects(const _Func & func) const
        {
            for (const object & object : objects)
                if (func(object))
                    return true;
            return false;
        }

        int  insert(const std::string & protoname, const enviroment & env);
        void remove(int id);

    private:

        const_iterator find(int id) const;
        iterator find(int id);

        bool load_impl(std::istream & in, const enviroment & env);
        void swap(level & other);

    private:

        std::string version;
        std::string name;
        size levelsize;

        int next_object_id;
        objects_t objects;
    };
}

#endif // __LEVEL_INCLUDED__