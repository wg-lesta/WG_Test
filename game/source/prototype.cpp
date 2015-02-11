#include "prototype.h"
#include <algorithm>

namespace game
{
    prototype::prototype() {}

    prototype::prototype(const std::string & name, const properties_t & props)
        : protoname(name)
    {
        clone_properties(props);
        sort_properties();
    }

    prototype::prototype(const std::string & name, properties_t && props)
        : protoname(name)
        , properties(std::move(props))
    {
        sort_properties();
    }

    prototype::prototype(const prototype & other) :
        protoname(other.protoname)
    {
        clone_properties(other.properties);
    }
    prototype::prototype(prototype && other)
        : protoname(std::move(other.protoname))
        , properties(std::move(other.properties)) {}

    prototype & prototype::operator=(const prototype & other)
    {
        prototype(other).swap(*this);
        return *this;
    }
    prototype & prototype::operator=(prototype && other)
    {
        std::swap(protoname, other.protoname);
        std::swap(properties, other.properties);
        return *this;
    }

    void prototype::clone_properties(const properties_t & props)
    {
        for (const property_ptr & ptr : props)
            properties.emplace_back(ptr->clone());
    }

    void prototype::sort_properties()
    {
        std::sort(properties.begin(), properties.end(), [](const property_ptr & l, const property_ptr & r)
        {
            return l->get_name() < r->get_name();
        });
    }

    const std::string & prototype::get_name() const
    {
        return protoname;
    }

    void prototype::swap(prototype & other)
    {
        std::swap(protoname, other.protoname);
        std::swap(properties, other.properties);
    }

    bool prototype::empty()
    {
        return properties.empty();
    }

    void prototype::accept(property_write_visitor & visitor)
    {
        for (property_ptr & ptr : properties)
            ptr->accept(visitor);
    }

    void prototype::accept(property_read_visitor & visitor) const
    {
        for (const property_ptr & ptr : properties)
            ptr->accept(visitor);
    }

    void prototype::accept(const std::string & name, property_write_visitor & visitor)
    {
        properties_t::iterator it = std::lower_bound(properties.begin(), properties.end(), 
            name, [](const property_ptr & prop, const std::string & name)
        {
            return prop->get_name() < name;
        });
        if (it != properties.end() && (*it)->get_name() == name)
            (*it)->accept(visitor);
    }

    void prototype::accept(const std::string & name, property_read_visitor  & visitor) const
    {
        properties_t::const_iterator it = std::lower_bound(properties.begin(), properties.end(),
            name, [](const property_ptr & prop, const std::string & name)
        {
            return prop->get_name() < name;
        });
        if (it != properties.end() && (*it)->get_name() == name)
            (*it)->accept(visitor);
    }

    size_t prototype::get_properties_count() const
    {
        return properties.size();
    }
}