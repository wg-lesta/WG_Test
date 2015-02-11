#ifndef __PROPERTY_DELIVER_INCLUDED__
#define __PROPERTY_DELIVER_INCLUDED__

#include <game/forward.h>
#include <game/property.h>

class QTreeWidgetItem;

class propertyDeliver : public game::property_write_visitor
{
public:

    explicit propertyDeliver(QTreeWidgetItem * item);

    void visit(game::property_int & prop) override;
    void visit(game::property_float & prop) override;
    void visit(game::property_point & prop) override;
    void visit(game::property_bool & prop) override;
    void visit(game::property_string & prop) override;

private:

    QTreeWidgetItem * item;
};

#endif //__PROPERTY_DELIVER_INCLUDED__
