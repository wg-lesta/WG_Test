#ifndef __LEVEL_SCENE_INCLUDED__
#define __LEVEL_SCENE_INCLUDED__

#include <game/forward.h>
#include <QGraphicsScene>

class LevelSceneObject;
class QImage;

class LevelScene : public QGraphicsScene
{
    Q_OBJECT

public:

    enum SceneError
    {
        errorPrototypeEmpty,
        errorObjectUniqueness
    };

    LevelScene(game::level * level, game::enviroment * env, QObject *parent = 0);
    LevelSceneObject * addObject(const game::object & obj, const QPixmap & tileset);

    int selectedObject() const;

    void setPrototype(const std::string &);

private slots:

    void foregroundInvalidate();

signals:

    void sceneObjectAdded(int id);
    void sceneObjectSelected(int id);
    void sceneObjectRemoved(int id);
    void sceneObjectMoved(int id, const game::point & pos);
    void sceneObjectChanged(int id);
    void sceneError(int error);

protected:

    void drawBackground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;
    void drawForeground(QPainter *painter, const QRectF &rect) Q_DECL_OVERRIDE;

    void mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent) Q_DECL_OVERRIDE;
    void mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent) Q_DECL_OVERRIDE;

    void keyPressEvent(QKeyEvent *event);

private:

    bool isUniquenessPrototype(const std::string & name) const;
    
    int objectByPosition(const game::point & position) const;

    QPointF closestCellPosition(const QPointF & pos) const;

    QPointF objectToScene(const game::point & pos);
    game::point sceneToObject(const QPointF & pos);

private:
    
    game::level * level;
    game::enviroment * enviroment;

    std::string protoname;

    QImage tileset;

    QTransform transformToScene;
    QTransform transformToObject;

    int idSelected;
};

#endif // __LEVEL_SCENE_INCLUDED__