#include <QApplication>
#include "LevelSceneObject.h"
#include "LevelScene.h"

#include <game/types.h>

LevelSceneObject::LevelSceneObject(int object_id, const QPixmap &pixmap)
    : QGraphicsPixmapItem(pixmap) 
    , idObject(object_id)
{
    setFlags(QGraphicsItem::ItemIsMovable | 
             QGraphicsItem::ItemIsSelectable | 
             QGraphicsItem::ItemSendsGeometryChanges);

    setShapeMode(ShapeMode::BoundingRectShape);
}

QVariant LevelSceneObject::itemChange(GraphicsItemChange change, const QVariant &value)
{
    if(change == ItemPositionChange)
    {
        if (QApplication::mouseButtons() == Qt::LeftButton)
        {
            LevelScene * sceneLevel = qobject_cast<LevelScene*>(scene());
            if (sceneLevel)
            {
                QPointF pos = sceneLevel->closestCellPosition(value.toPointF());
                QGraphicsItem* itemInPos = scene()->itemAt(pos, QTransform());
                if (itemInPos)
                    return scenePos();
                emit moved(idObject, sceneLevel->sceneToObject(pos));
                return pos;
            }
        }
    }
    if (change == ItemSelectedChange)
    {
        emit selected(idObject);
    }
    if (change == ItemSceneChange && isActive())
    {
        emit remove(idObject);
    }

    return QGraphicsPixmapItem::itemChange(change, value);
}

int LevelSceneObject::getId() const
{
    return idObject;
}