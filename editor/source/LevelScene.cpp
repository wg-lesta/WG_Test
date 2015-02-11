#include <QApplication>
#include <QPainter>
#include <QGraphicsSceneMouseEvent>
#include <QKeyEvent>

#include <game/level.h>

#include "LevelScene.h"
#include "LevelSceneObject.h"
#include "PropertyRequests.h"


LevelScene::LevelScene(QSize sizeLevel, QSize sizeTile)
    : sizeLevel(sizeLevel)
    , sizeTile(sizeTile)
    , transformToScene(sizeTile.width(), 0, 0, sizeTile.height(), 0, 0)
    , transformToObject(1.0 / sizeTile.width(), 0, 0, 1.0 / sizeTile.height(), 0, 0)
    , activeCell(nullptr)
{
    setSceneRect(0, 0, sizeLevel.width() * sizeTile.width(), sizeLevel.height() * sizeTile.height());
    setBackgroundBrush(Qt::gray);

    createActiveCell();
}

void LevelScene::createActiveCell()
{
    QPixmap pixmap(sizeTile);
    pixmap.fill(QColor(0, 0, 200, 100));
    activeCell = addPixmap(pixmap);
    activeCell->setVisible(false);
}

LevelSceneObject * LevelScene::addObject(const game::object & obj, const QPixmap & tileset)
{
    readTile visitor;
    obj.accept("Tile", visitor);
    game::point posTile = visitor.get_tile();

    QPixmap pixmap = tileset.copy(sizeTile.width() * posTile.x, sizeTile.height() * posTile.y,
        sizeTile.width(), sizeTile.height()).transformed(QTransform().rotate(180));

    LevelSceneObject *sceneObject = new LevelSceneObject(obj.get_id(), pixmap);
    
    readPosition positionProp;
    obj.accept("Position", positionProp);

    sceneObject->setPos(objectToScene(positionProp.get_position()));
    this->addItem(sceneObject);


    return sceneObject;
}

QPointF LevelScene::closestCellPosition(const QPointF & pos) const
{
    QRectF rect = sceneRect() - QMarginsF(0, 0, sizeTile.width(), sizeTile.height());
    QPointF result
    {
        std::min(std::max(pos.x(), rect.left()), rect.right()),
        std::min(std::max(pos.y(), rect.top()), rect.bottom())
    };
    result.setX(std::round(result.x() / sizeTile.width()) * sizeTile.width());
    result.setY(std::round(result.y() / sizeTile.height()) * sizeTile.height());

    return result;
}

void LevelScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QGraphicsScene::drawBackground(painter, rect);

    QVector<QLineF> lines;
    
    QRectF levelRect = sceneRect();

    for (qreal x = levelRect.left(); x <= levelRect.right(); x += sizeTile.width())
        lines.append(QLineF(x, levelRect.top(), x, levelRect.bottom()));
    for (qreal y = levelRect.top(); y <= levelRect.bottom(); y += sizeTile.height())
        lines.append(QLineF(levelRect.left(), y, levelRect.right(), y));

    QPen pen;
    pen.setColor(QColor(32, 32, 32));
    pen.setStyle(Qt::DotLine);
    painter->setPen(pen);
    painter->drawLines(lines.data(), lines.size());
}

void LevelScene::mouseMoveEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    QGraphicsScene::mouseMoveEvent(mouseEvent);

    QPointF pos = mouseEvent->scenePos();

    activeCell->setVisible(sceneRect().contains(pos) &&
        QApplication::mouseButtons() == Qt::NoButton);

    pos.setX(std::floor(pos.x() / sizeTile.width()) * sizeTile.width());
    pos.setY(std::floor(pos.y() / sizeTile.height()) * sizeTile.height());

    activeCell->setPos(pos);
}

void LevelScene::mousePressEvent(QGraphicsSceneMouseEvent * mouseEvent)
{
    if (sceneRect().contains(mouseEvent->scenePos()))
    {
        QGraphicsScene::mousePressEvent(mouseEvent);
        emit cellSelected(sceneToObject(mouseEvent->scenePos()));
    }
}

void LevelScene::keyPressEvent(QKeyEvent *event)
{
    QGraphicsScene::keyPressEvent(event);
    if (event->key() == Qt::Key_Delete)
    {
        QList<QGraphicsItem*> selItems = selectedItems();
        foreach(QGraphicsItem * item, selItems)
            removeItem(item);
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
