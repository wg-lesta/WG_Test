#ifndef __PROPERTY_INCLUDED__
#define __PROPERTY_INCLUDED__

#include "types.h"
#include <string>

namespace game
{
    namespace details
    {
        // для доступа к свойствам используем 
        // двойную диспетчеризацию вместо RTTI

        struct property_visitor_base
        {
            virtual const char * name() { return nullptr; }
        };

        template<typename... _Types>
        struct property_write_visitor;

        template<typename _T>
        struct property_write_visitor<_T> : public property_visitor_base
        {
            virtual void visit(_T & concreate_prop) {}
        };

        template<typename _T, typename... _Types>
        struct property_write_visitor<_T, _Types...> : public property_write_visitor<_Types...>
        {
            using property_write_visitor<_Types...>::visit;
            virtual void visit(_T & concreate_prop) {}
        };

        template<typename... _Types>
        struct property_read_visitor;

        template<typename _T>
        struct property_read_visitor<_T> : public property_visitor_base
        {
            virtual void visit(const _T & concreate_prop) const {}
        };

        template<typename _T, typename... _Types>
        struct property_read_visitor<_T, _Types...> : public property_read_visitor<_Types...>
        {
            using property_read_visitor<_Types...>::visit;
            virtual void visit(const _T & concreate_prop) const {}
        };

        template<typename... _Types>
        struct property_dispatcher
        {
            virtual void accept(property_read_visitor<_Types...> & visitor) const = 0;
            virtual void accept(property_write_visitor<_Types...> & visitor) = 0;
        };

    }
    

    template<typename _T>
    struct property_value;
    template<typename _T>
    struct property_range;

    using property_int = property_range<int>;
    using property_float = property_range<float>;
    using property_point = property_range<point>;
    using property_string = property_value<std::string>;
    using property_bool = property_value<bool>;

    using property_read_visitor = details::property_read_visitor
    <
        property_int,
        property_float,
        property_point,
        property_string,
        property_bool
    >;

    using property_write_visitor = details::property_write_visitor
    <
        property_int,
        property_float,
        property_point,
        property_string,
        property_bool
    >;

    using property_dispatcher = details::property_dispatcher
    <
        property_int,
        property_float,
        property_point,
        property_string,
        property_bool
    >;

    struct property : public property_dispatcher
    {
        explicit property(const std::string & name, bool readonly)
            : name(name)
            , readonly(readonly){}

        virtual ~property() {}

        const std::string & get_name() const
        { 
            return name;
        }

        bool read_only() const
        {
            return readonly;
        }

        virtual property * clone() = 0;

    private:
        std::string name;
        bool readonly;
    };

    template<typename _T>
    struct property_value : public property
    {
        property_value() : property_value("", false) {}

        explicit property_value(const std::string & name, bool readonly)
            : property(name, readonly)
            , value(_T()) {}

        void set_value(const _T & value)
        {
            this->value = value;
        }

        const _T & get_value() const
        {
            return value;
        }

        property * clone() override
        {
            property_value *result = new property_value(get_name(), read_only());
            result->value = value;
            return result;
        }

        void accept(property_write_visitor & visitor) override
        {
            if (!read_only())
                visitor.visit(*this);
        }

        void accept(property_read_visitor & visitor) const override
        {
            visitor.visit(*this);
        }

    private:

        _T value;
    };

    template<typename _T>
    struct property_range : public property
    {
        property_range() : property_range("", false) {}

        explicit property_range(const std::string & name, bool readonly)
            : property(name, readonly)
            , value(_T())
            , min(_T())
            , max(_T()) {}

        void set_value(const _T & value)
        {
            this->value = value;
        }

        const _T & get_value() const
        {
            return value;
        }

        void set_range(const _T & min, const _T & max)
        {
            this->min = min;
            this->max = max;
        }

        const _T & get_min() const
        {
            return min;
        }

        const _T & get_max() const
        {
            return max;
        }

        bool empty_range() const
        {
            return min == max;
        }
            
        property * clone() override
        {
            property_range *result = new property_range(get_name(), read_only());
            result->value = get_value();
            result->min = min;
            result->max = max;
            return result;
        }

        void accept(property_write_visitor & visitor) override
        {
            if (!read_only())
                visitor.visit(*this);
        }

        void accept(property_read_visitor & visitor) const override
        {
            visitor.visit(*this);
        }

    private:

        _T value;
        _T min;
        _T max;
    };
}

#endif // __PROPERTY_INCLUDED__