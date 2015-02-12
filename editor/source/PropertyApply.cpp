#include "PropertyApply.h"
#include "PropertyRoles.h"

#include <game/object.h>
#include <QTreeWidgetItem>

Q_DECLARE_METATYPE(game::property_int)
Q_DECLARE_METATYPE(game::property_float)

propertyApply::propertyApply(const QVariant & data) 
 : data(data) {}

void propertyApply::visit(game::property_int & prop)
{
    game::property_int newProp = qvariant_cast<game::property_int>(data);
    prop.set_value(newProp.get_value());
}

void propertyApply::visit(game::property_float & prop)
{
    game::property_float newProp = qvariant_cast<game::property_float>(data);
    prop.set_value(newProp.get_value());
}

void propertyApply::visit(game::property_point & prop)
{
    game::property_int newProp = qvariant_cast<game::property_int>(data);
    if (newProp.get_name() == "x")
        prop.set_value(game::point(newProp.get_value(), prop.get_value().y));
    else if (newProp.get_name() == "y")
        prop.set_value(game::point(prop.get_value().x, newProp.get_value()));
}

void propertyApply::visit(game::property_bool & prop)
{
    prop.set_value(data.toBool());
}

void propertyApply::visit(game::property_string & prop)
{
    prop.set_value(data.toString().toStdString());
}
