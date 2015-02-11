#ifndef __PROPERTY_POPULATE_INCLUDED__
#define __PROPERTY_POPULATE_INCLUDED__

#include <game/forward.h>
#include <game/property.h>

class QTreeWidget;

class propertyPopulate : public game::property_read_visitor
{
public:

    propertyPopulate(QTreeWidget * widget, const game::object & obj, const game::size & sizeLevel);

    void visit(const game::property_int & prop) const override;
    void visit(const game::property_float & prop) const override;
    void visit(const game::property_point & prop) const override;
    void visit(const game::property_bool & prop) const override;
    void visit(const game::property_string & prop) const override;

private:

    QTreeWidget * treeWidget;
    game::size sizeLevel;
};


#endif //__PROPERTY_POPULATE_INCLUDED__
