#ifndef __OBJECT_INCLUDED__
#define __OBJECT_INCLUDED__

#include <string>
#include "types.h"
#include "prototype.h"

/*
    {
        "prototype": "object name from config",
        "properties":
        [
            {"name": "property name", "value": "different from the default value"}
        ]
    }
*/

namespace game
{
    class object
    {
    public:

        explicit object(int id, const prototype & proto);
        explicit object(int id, prototype && proto);

        int get_id() const;
        const std::string & get_name() const;

        void accept(property_write_visitor & visitor);
        void accept(property_read_visitor & visitor) const;

        void accept(const std::string & name, property_write_visitor & visitor);
        void accept(const std::string & name, property_read_visitor & visitor) const;

        size_t get_properties_count() const;

    private:

        int id;
        prototype proto;
    };

} // namespace game

#endif // __PROTOTYPE_INCLUDED__