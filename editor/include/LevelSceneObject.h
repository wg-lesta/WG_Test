#ifndef __LEVEL_SCENE_OBJECT_INCLUDED__
#define __LEVEL_SCENE_OBJECT_INCLUDED__

#include <game/forward.h>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>

class LevelSceneObject : public QObject, public QGraphicsPixmapItem
{
    Q_OBJECT

public:
    LevelSceneObject(int object_id, const QPixmap &pixmap);

    int getId() const;

signals:
    
    void selected(int);
    void remove(int);
    void moved(int, game::point);

protected:

    QVariant itemChange(GraphicsItemChange change, const QVariant &value) Q_DECL_OVERRIDE;

private:

    int idObject;
};

#endif // __LEVEL_SCENE_OBJECT_INCLUDED__