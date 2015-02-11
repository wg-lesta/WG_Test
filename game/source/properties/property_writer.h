#ifndef __PROPERTY_WRITER_INCLUDED__
#define __PROPERTY_WRITER_INCLUDED__

#include "property.h"
#include <json/forwards.h>

namespace game
{
    struct object_property_writer : public property_write_visitor
    {
        object_property_writer(const Json::Value & raw);

        void visit(property_int & prop);
        void visit(property_float & prop);
        void visit(property_point & prop);
        void visit(property_bool & prop);
        void visit(property_string & prop);

    private:
        const Json::Value & raw_property;
    };

}

#endif // __PROPERTY_WRITER_INCLUDED__