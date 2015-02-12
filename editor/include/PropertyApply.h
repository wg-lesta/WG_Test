#ifndef __PROPERTY_APPLY_INCLUDED__
#define __PROPERTY_APPLY_INCLUDED__

#include <QVariant>

#include <game/forward.h>
#include <game/property.h>

class propertyApply : public game::property_write_visitor
{
public:

    explicit propertyApply(const QVariant & data);

    void visit(game::property_int & prop) override;
    void visit(game::property_float & prop) override;
    void visit(game::property_point & prop) override;
    void visit(game::property_bool & prop) override;
    void visit(game::property_string & prop) override;

private:

    QVariant data;
};

#endif //__PROPERTY_APPLY_INCLUDED__
