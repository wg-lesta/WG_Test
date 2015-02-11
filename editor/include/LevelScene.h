#ifndef __LEVEL_SCENE_INCLUDED__
#define __LEVEL_SCENE_INCLUDED__

#include <game/forward.h>
#include <QGraphicsScene>

class LevelSceneObject;

class LevelScene : public QGraphicsScene
{
    Q_OBJECT

public:

    LevelScene(QSize sizeLivel, QSize sizeTile);
    LevelSceneObject * addObject(const game::object & obj, const QPixmap & tileset);

    QPointF closestCellPosition(const QPointF & pos) const;

    QPointF objectToScene(const game::point & pos);
    game::point sceneToObject(const QPointF & pos);

signals:

    void cellSelected(const game::point & position);

protected:

    void drawBackground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;

    void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent) Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *event);

private:

    void createActiveCell();

private:

    QSize sizeLevel;
    QSize sizeTile;

    QTransform transformToScene;
    QTransform transformToObject;

    QGraphicsPixmapItem * activeCell;
};

#endif // __LEVEL_SCENE_INCLUDED__