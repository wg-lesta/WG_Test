#ifndef __PROPERTY_READER_INCLUDED__
#define __PROPERTY_READER_INCLUDED__

#include "property.h"
#include <json/forwards.h>

namespace game
{
    struct object_property_reader : public property_read_visitor
    {
        object_property_reader(Json::Value & raw);

        void visit(const property_int & prop) const override;
        void visit(const property_float & prop) const override;
        void visit(const property_point & prop) const override;
        void visit(const property_bool & prop) const override;
        void visit(const property_string & prop) const override;

    private:
        mutable Json::Value & raw_property;
    };
}

#endif // __PROPERTY_READER_INCLUDED__