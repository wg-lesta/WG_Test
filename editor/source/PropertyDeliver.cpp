#include "PropertyDeliver.h"
#include "PropertyRoles.h"

#include <game/object.h>
#include <QTreeWidgetItem>

Q_DECLARE_METATYPE(game::property_int)
Q_DECLARE_METATYPE(game::property_float)

propertyDeliver::propertyDeliver(QTreeWidgetItem * item) :
    item(item) {}

void propertyDeliver::visit(game::property_int & prop)
{
    game::property_int newProp = qvariant_cast<game::property_int>(item->data(1, PropertyRole::roleData));
    prop.set_value(newProp.get_value());
}

void propertyDeliver::visit(game::property_float & prop)
{
    game::property_float newProp = qvariant_cast<game::property_float>(item->data(1, PropertyRole::roleData));
    prop.set_value(newProp.get_value());
}

void propertyDeliver::visit(game::property_point & prop)
{
    game::property_int newProp = qvariant_cast<game::property_int>(item->data(1, PropertyRole::roleData));
    if (newProp.get_name() == "x")
        prop.set_value(game::point(newProp.get_value(), prop.get_value().y));
    else if (newProp.get_name() == "y")
        prop.set_value(game::point(prop.get_value().x, newProp.get_value()));
}

void propertyDeliver::visit(game::property_bool & prop)
{
    prop.set_value(item->data(1, Qt::EditRole).toBool());
}

void propertyDeliver::visit(game::property_string & prop)
{
    prop.set_value(item->data(1, Qt::EditRole).toString().toStdString());
}
