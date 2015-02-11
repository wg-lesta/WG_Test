#include "PropertyDelegate.h"
#include "PropertyRoles.h"

#include <QSpinBox>
#include <game/property.h>

Q_DECLARE_METATYPE(game::property_int)
Q_DECLARE_METATYPE(game::property_float)

PropertyDelegate::PropertyDelegate(QObject *parent)
    :QStyledItemDelegate(parent) {}

QWidget * PropertyDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option,
    const QModelIndex &index) const
{
    if (index.data(PropertyRole::roleData).canConvert<game::property_int>())
    {
        return new QSpinBox(parent);
    }
    if (index.data(PropertyRole::roleData).canConvert<game::property_float>())
    {
        return new QDoubleSpinBox(parent);
    }
    return QStyledItemDelegate::createEditor(parent, option, index);
}

void PropertyDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (index.data(PropertyRole::roleData).canConvert<game::property_int>())
    {
        QSpinBox * spin = qobject_cast<QSpinBox*>(editor);
        game::property_int prop = qvariant_cast<game::property_int>(index.data(PropertyRole::roleData));

        spin->setRange(prop.get_min(), prop.get_max());
        spin->setValue(prop.get_value());
    }
    else if (index.data(PropertyRole::roleData).canConvert<game::property_float>())
    {
        QSpinBox * spin = qobject_cast<QSpinBox*>(editor);
        game::property_float prop = qvariant_cast<game::property_float>(index.data(PropertyRole::roleData));

        spin->setRange(prop.get_min(), prop.get_max());
        spin->setValue(prop.get_value());
    }
    else
        QStyledItemDelegate::setEditorData(editor, index);
}

void PropertyDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    if (index.data(PropertyRole::roleData).canConvert<game::property_int>())
    {
        game::property_int prop = qvariant_cast<game::property_int>(index.data(PropertyRole::roleData));
        QSpinBox *spin = qobject_cast<QSpinBox*>(editor);

        prop.set_value(spin->value());
        model->setData(index, prop.get_value());
        model->setData(index, QVariant::fromValue(prop), PropertyRole::roleData);
    }
    else if (index.data(PropertyRole::roleData).canConvert<game::property_float>())
    {
        game::property_float prop = qvariant_cast<game::property_float>(index.data(PropertyRole::roleData));
        QDoubleSpinBox *spin = qobject_cast<QDoubleSpinBox*>(editor);

        prop.set_value(spin->value());
        model->setData(index, prop.get_value());
        model->setData(index, QVariant::fromValue(prop), PropertyRole::roleData);
    }
    else
        QStyledItemDelegate::setModelData(editor, model, index);
}
