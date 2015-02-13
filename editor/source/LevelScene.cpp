#include <QApplication>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>
#include <QImage>
#include <QTimer>

#include <game/level.h>
#include <game/enviroment.h>
#include <game/object.h>

#include "LevelScene.h"
#include "PropertyRequests.h"


LevelScene::LevelScene(game::level * level, game::enviroment * env, QObject *parent)
    : QGraphicsScene(parent)
    , level(level)
    , enviroment(env)
    , transformToScene(env->get_tilesize().cx, 0, 0, env->get_tilesize().cy, 0, 0)
    , transformToObject(1.0 / env->get_tilesize().cx, 0, 0, 1.0 / env->get_tilesize().cy, 0, 0)
    , idSelected(0)
{
    setSceneRect(0, 0, level->get_size().cx * env->get_tilesize().cx, 
        level->get_size().cy * env->get_tilesize().cy);
    setBackgroundBrush(Qt::gray);

    tileset.load(QString::fromStdString(enviroment->get_tileset()));
    tileset = tileset.mirrored();

    QTimer *timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(foregroundInvalidate()));
    timer->start(1000/60);
}

void LevelScene::setPrototype(const std::string & protoname)
{
    this->protoname = protoname;
}

int LevelScene::selectedObject() const
{
    return idSelected;
}

void LevelScene::foregroundInvalidate()
{
    invalidate(QRectF(), QGraphicsScene::ForegroundLayer);
}

QPointF LevelScene::closestCellPosition(const QPointF & pos) const
{
    game::size tilesize = enviroment->get_tilesize();
    QRectF rect = sceneRect() - QMarginsF(0, 0, tilesize.cx, tilesize.cy);
    QPointF result
    {
        std::min(std::max(pos.x(), rect.left()), rect.right()),
        std::min(std::max(pos.y(), rect.top()), rect.bottom())
    };
    result.setX(std::floor(result.x() / tilesize.cx) * tilesize.cx);
    result.setY(std::floor(result.y() / tilesize.cy) * tilesize.cy);

    return result;
}

void LevelScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsScene::drawBackground(painter, rect);

    QVector<QLineF> lines;
    
    game::size tilesize = enviroment->get_tilesize();
    QRectF levelRect = sceneRect();

    for (qreal x = levelRect.left(); x <= levelRect.right(); x += tilesize.cx)
        lines.append(QLineF(x, levelRect.top(), x, levelRect.bottom()));
    for (qreal y = levelRect.top(); y <= levelRect.bottom(); y += tilesize.cy)
        lines.append(QLineF(levelRect.left(), y, levelRect.right(), y));

    QPen pen;
    pen.setColor(QColor(32, 32, 32));
    pen.setStyle(Qt::DotLine);
    painter->setPen(pen);
    painter->drawLines(lines.data(), lines.size());
}

void LevelScene::drawForeground(QPainter *painter, const QRectF &rect)
{
    QGraphicsScene::drawForeground(painter, rect);

    level->foreach_objects([this, &painter](const game::object & obj)
    {
        readPosition propertyPos;
        obj.accept(propertyPos);

        readTile propertyTile;
        obj.accept(propertyTile);

        QPointF pos(propertyPos.get_position().x, propertyPos.get_position().y);
        pos = pos * transformToScene;

        game::size tilesize = enviroment->get_tilesize();
        painter->drawImage(pos.x(), pos.y(), tileset,
            propertyTile.get_tile().x * tilesize.cx, propertyTile.get_tile().y * tilesize.cy, 
            tilesize.cx, tilesize.cy);

        if (obj.get_id() == idSelected)
        {
            QPen pen(QColor(200, 0, 0));
            pen.setWidth(2);
            pen.setStyle(Qt::DotLine);
            painter->setPen(pen);

            QSizeF margins(3, 3);
            painter->drawRect(pos.x() - margins.width(), pos.y() - margins.height(), 
                tilesize.cx + 2* margins.width(), tilesize.cy + 2 * margins.width());
        }
        return false;
    });
}

void LevelScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    QGraphicsScene::mouseMoveEvent(mouseEvent);
    QPointF pos = mouseEvent->scenePos();

    if (sceneRect().contains(pos) &&
        QApplication::mouseButtons() == Qt::LeftButton && idSelected)
    {
        game::point objectPos = sceneToObject(closestCellPosition(pos));
        if (!objectByPosition(objectPos))
        {
            readPosition propertyPos;
            level->read_object(idSelected, [&propertyPos](const game::object & obj)
            {
                obj.accept(propertyPos);
            });

            if (objectPos != propertyPos.get_position())
            {
                level->write_object(idSelected, [&objectPos](game::object & obj)
                {
                    writePosition propertyPos(objectPos);
                    obj.accept(propertyPos);
                });
                emit sceneObjectChanged(idSelected);
                emit sceneObjectMoved(idSelected, objectPos);
            }
        }
    }
}

void LevelScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    if (sceneRect().contains(mouseEvent->scenePos()))
    {
        QGraphicsScene::mousePressEvent(mouseEvent);
        game::point pos = sceneToObject(mouseEvent->scenePos());

        int idObject = objectByPosition(pos);

        if (!idObject)
        {
            if (protoname.empty())
            {
                emit sceneError(errorPrototypeEmpty);
                return;
            }

            if (isUniquenessPrototype(protoname))
            {
                emit sceneError(errorObjectUniqueness);
                return;
            }

            idObject = level->insert(protoname, *enviroment);
            level->write_object(idObject, [this, &pos](game::object & obj)
            {
                writePosition propertyPos(pos);
                obj.accept(propertyPos);
            });
            emit sceneObjectAdded(idObject);
        }

        idSelected = idObject;
        emit sceneObjectSelected(idSelected);
    }
}

void LevelScene::keyPressEvent(QKeyEvent *event)
{
    QGraphicsScene::keyPressEvent(event);
    if (event->key() == Qt::Key_Delete && idSelected)
    {
        level->remove(idSelected);
        emit sceneObjectRemoved(idSelected);
    }
}

QPointF LevelScene::objectToScene(const game::point & pos)
{
    return QPointF(pos.x, pos.y) * transformToScene;
}

game::point LevelScene::sceneToObject(const QPointF & pos)
{
    QPointF result = pos * transformToObject;
    return game::point(result.x(), result.y());
}

bool LevelScene::isUniquenessPrototype(const std::string & name) const
{
    readUniqueness visitor;
    enviroment->request_property(protoname, visitor);
    if (visitor.is_uniqueness())
    {
        return level->foreach_objects([&name](const game::object & obj)
        {
            return obj.get_name() == name;
        });
    }
    return false;
}

int LevelScene::objectByPosition(const game::point & position) const
{
    int idObject = 0;
    level->foreach_objects([&idObject, &position](const game::object & obj)
    {
        readPosition visitor;
        obj.accept(visitor);
        if (visitor.get_position() == position)
        {
            idObject = obj.get_id();
            return true;
        }
        return false;
    });
    return idObject;
}
