#include "PropertyPopulate.h"
#include "PropertyRoles.h"

#include <game/object.h>
#include <QTreeWidget>
#include <QTreeWidgetItem>

Q_DECLARE_METATYPE(game::property_int)
Q_DECLARE_METATYPE(game::property_float)

propertyPopulate::propertyPopulate(QTreeWidget * widget, const game::object & obj, const game::size & sizeLevel)
    : treeWidget(widget)
    , sizeLevel(sizeLevel)
{
    QTreeWidgetItem * itemProto = new QTreeWidgetItem(treeWidget);
    itemProto->setText(0, "Prototype");
    itemProto->setText(1, QString::fromStdString(obj.get_name()));
}

void propertyPopulate::visit(const game::property_int & prop) const
{
    QTreeWidgetItem * item = new QTreeWidgetItem(treeWidget);
    QString name = QString::fromStdString(prop.get_name());

    item->setText(0, name);
    item->setText(1, QString("%1").arg(prop.get_value()));
    item->setFlags(item->flags() | Qt::ItemIsEditable);

    if (prop.read_only())
        item->setFlags(Qt::NoItemFlags);
    else
    {
        item->setData(1, PropertyRole::roleData, QVariant::fromValue(prop));
        item->setData(1, PropertyRole::roleName, QVariant::fromValue(name));
    }
}

void propertyPopulate::visit(const game::property_float & prop) const
{
    QTreeWidgetItem * item = new QTreeWidgetItem(treeWidget);
    QString name = QString::fromStdString(prop.get_name());

    item->setText(0, name);
    item->setText(1, QString("%1").arg(prop.get_value()));
    item->setFlags(item->flags() | Qt::ItemIsEditable);

    if (prop.read_only())
        item->setFlags(Qt::NoItemFlags);
    else
    {
        item->setData(1, PropertyRole::roleData, QVariant::fromValue(prop));
        item->setData(1, PropertyRole::roleName, QVariant::fromValue(name));
    }

}

void propertyPopulate::visit(const game::property_point & prop) const
{
    QTreeWidgetItem *item = new QTreeWidgetItem(treeWidget);
    QString name = QString::fromStdString(prop.get_name());

    item->setText(0, name);
    item->setText(1, QString("[%1, %2]").arg(prop.get_value().x).arg(prop.get_value().y));

    if (prop.read_only())
        item->setFlags(Qt::NoItemFlags);
    else
    {
        QTreeWidgetItem *itemX = new QTreeWidgetItem(item);
        itemX->setText(0, "x");
        itemX->setText(1, QString("%1").arg(prop.get_value().x));
        itemX->setFlags(item->flags() | Qt::ItemIsEditable);

        game::property_int propX("x", prop.read_only());
        propX.set_value(prop.get_value().x);
        propX.set_range(0, sizeLevel.cx);
        itemX->setData(1, PropertyRole::roleData, QVariant::fromValue(propX));
        itemX->setData(1, PropertyRole::roleName, QVariant::fromValue(name));

        QTreeWidgetItem *itemY = new QTreeWidgetItem(item);
        itemY->setText(0, "y");
        itemY->setText(1, QString("%1").arg(prop.get_value().y));
        
        itemY->setFlags(item->flags() | Qt::ItemIsEditable);

        game::property_int propY("y", prop.read_only());
        propY.set_value(prop.get_value().y);
        propY.set_range(0, sizeLevel.cy);
        itemY->setData(1, PropertyRole::roleData, QVariant::fromValue(propY));
        itemY->setData(1, PropertyRole::roleName, QVariant::fromValue(name));
    }
}

void propertyPopulate::visit(const game::property_bool & prop) const
{
    QTreeWidgetItem * item = new QTreeWidgetItem(treeWidget);
    QString name = QString::fromStdString(prop.get_name());

    item->setText(0, name);
    item->setCheckState(1, prop.get_value() ? Qt::Checked : Qt::Unchecked);

    if (prop.read_only())
        item->setFlags(Qt::NoItemFlags);
    else
        item->setData(1, PropertyRole::roleName, QVariant::fromValue(name));
}

void propertyPopulate::visit(const game::property_string & prop) const
{
    QTreeWidgetItem * item = new QTreeWidgetItem(treeWidget);
    QString name = QString::fromStdString(prop.get_name());

    item->setText(0, name);
    item->setText(1, QString::fromStdString(prop.get_value()));
    item->setFlags(item->flags() | Qt::ItemIsEditable);

    if (prop.read_only())
        item->setFlags(Qt::NoItemFlags);
    else
        item->setData(1, PropertyRole::roleName, QVariant::fromValue(name));
}
