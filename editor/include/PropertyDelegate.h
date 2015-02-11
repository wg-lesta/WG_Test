#ifndef __PROPERTY_DELEGATE_INCLUDED__
#define __PROPERTY_DELEGATE_INCLUDED__

#include <game/forward.h>
#include <QStyledItemDelegate>

class PropertyDelegate : public QStyledItemDelegate
{
public:

    explicit PropertyDelegate(QObject *parent = 0);

    QWidget *createEditor(QWidget *parent, const QStyleOptionViewItem &option,
        const QModelIndex &index) const;

    void setEditorData(QWidget *editor, const QModelIndex &index) const;
    void setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const;
};

#endif //__PROPERTY_DELEGATE_INCLUDED__
