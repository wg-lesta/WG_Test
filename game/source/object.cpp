#include "object.h"

namespace game
{
    object::object(int id, const prototype & proto) 
        : id(id)
        , proto(proto) {}

    object::object(int id, prototype && proto)
        : id(id)
        , proto(std::move(proto)) {}

    int object::get_id() const
    {
        return id;
    }

    const std::string & object::get_name() const
    {
        return proto.get_name();
    }

    void object::accept(property_write_visitor & visitor)
    {
        proto.accept(visitor);
    }

    void object::accept(const std::string & name, property_write_visitor & visitor)
    {
        proto.accept(name, visitor);
    }

    void object::accept(property_read_visitor & visitor) const
    {
        proto.accept(visitor);
    }

    void object::accept(const std::string & name, property_read_visitor & visitor) const
    {
        proto.accept(name, visitor);
    }

    size_t object::get_properties_count() const
    {
        return proto.get_properties_count();
    }
}