#ifndef __ENVIROMENT_INCLUDED__
#define __ENVIROMENT_INCLUDED__

#include "prototype.h"
#include "types.h"

#include <vector>
#include <string>

/*
    Game enviroment

    {
        "version": 1.0,
        "tileset": "path to image",
        "tilesize": {"width" : value, "height" : value},
        "prototypes":
         [
            {
                "name": "protoname",
                "properties":
                [
                    {
                        "name": "property name",
                        "type": "int|float|bool|string|point",
                        "value": value
                        "range": { "min":value, "max":value }
                    }
                ]
            }
        ]
    }

*/

namespace game
{
    class enviroment
    {
    public:

        using prototypes_t = std::vector<prototype>;
        using const_iterator = prototypes_t::const_iterator;

        enviroment();

        bool load(std::istream & is);

        const std::string & get_version() const;
        const std::string & get_tileset() const;
        size get_tilesize() const;

        void request_property(const std::string & name, const std::string & protoname,
            property_read_visitor & visitor) const;

        void request_properties(const std::string & protoname, 
            property_read_visitor & visitor) const;
        
        template<typename _Func>
        bool foreach_prototype(const _Func & func) const
        {
            for (const prototype & proto : prototypes)
                if (func(proto))
                    return true;
            return false;
        }
        
        prototype create_prototype(const std::string & name) const;

        bool empty() const;

    private:

        const_iterator find_prototype(const std::string name) const;

        void swap(enviroment & env);

        bool load_impl(std::istream & is);
        void sort_prototypes();

    private:

        std::string version;
        std::string tileset;
        size tilesize;

        prototypes_t prototypes;
    };
}

#endif // __ENVIROMENT_INCLUDED__