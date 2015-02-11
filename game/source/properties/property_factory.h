#ifndef __PROPERTY_FACTORY_INCLUDED__
#define __PROPERTY_FACTORY_INCLUDED__

#include <memory>
#include <map>
#include <json/forwards.h>
#include "property.h"

namespace game
{
    struct property_creator_base
    {
        virtual std::unique_ptr<property> create(const Json::Value & raw) = 0;
        virtual ~property_creator_base() {}
    };

    class property_factory
    {
        using factory_map = std::map<std::string, property_creator_base*>;

    public:

        property_factory() = default;
        property_factory(const property_factory &) = delete;
        property_factory & operator=(const property_factory &) = delete;

        ~property_factory();

        template<typename _T>
        void register_property(const std::string & type)
        {
            register_property(type, new _T());
        }

        std::unique_ptr<property> create(const std::string & type, const Json::Value & raw) const;

    private:

        void register_property(const std::string & type, property_creator_base* creator);

        factory_map factory;
    };
}


#endif //__PROPERTY_FACTORY_INCLUDED__
