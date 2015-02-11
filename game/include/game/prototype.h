#ifndef __PROTOTYPE_INCLUDED__
#define __PROTOTYPE_INCLUDED__

#include <memory>
#include <string>
#include <vector>

#include "property.h"

/*
    {
        "name": "protoname",
        "properties":
        [
            {
                "name": "property",
                "type": "int|float|bool|string|point",
                "value": value,
                "range": { "min":value, "max":value }
            }
        ]
    }
*/

namespace game
{
 
    class prototype
    {
    public:

        using property_ptr = std::unique_ptr<property>;
        using properties_t = std::vector<property_ptr>;
        
        prototype();

        prototype(const std::string & name, const properties_t & props);
        prototype(const std::string & name, properties_t && props);

        prototype(const prototype & other);
        prototype(prototype && other);

        prototype & operator=(const prototype & other);
        prototype & operator=(prototype && other);

        const std::string & get_name() const;

        void accept(property_write_visitor & visitor);
        void accept(property_read_visitor & visitor) const;

        void accept(const std::string & name, property_write_visitor & visitor);
        void accept(const std::string & name, property_read_visitor & visitor) const;

        size_t get_properties_count() const;

        void swap(prototype & other);  
        bool empty();

    private:

        void clone_properties(const properties_t & props);
        void sort_properties();
        
    private:

        std::string protoname;
        properties_t properties;
    };

} // namespace game

#endif // __PROTOTYPE_INCLUDED__