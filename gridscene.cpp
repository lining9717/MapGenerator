#include "gridscene.h"

GridScene::GridScene(int width, int height): m_width(width), m_height(height), status(IDLE), start_point(-1, -1) {}

GridScene::~GridScene() {}


void GridScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    QVarLengthArray<QLineF, 100> lines;
    for (qreal x = 0; x <= m_width; x += m_cell_size) {
        lines.append(QLineF(x, 0, x, m_height));
    }
    for (qreal y = 0; y <= m_height; y += m_cell_size) {
        lines.append(QLineF(0, y, m_width, y));
    }
    painter->drawLines(lines.data(), lines.size());
}


GRID_STATUS GridScene::getGridStarus()
{
    return status;
}

void GridScene::setGridStatus(GRID_STATUS s)
{
    status = s;
}

void GridScene::mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    for(auto &item : rects_to_draw_list) {
        delete  item;
    }
    rects_to_draw_list.clear();
    QPointF real_position = mouseEvent->scenePos();
    if(real_position.rx() < 0 or real_position.rx() >= m_width or
       real_position.ry() < 0 or real_position.ry() >= m_height) {
        QGraphicsScene::mousePressEvent(mouseEvent);
        return;
    }
    QPointF coordinate_position(static_cast<int>(real_position.rx() / m_cell_size),
                                static_cast<int>(real_position.ry() / m_cell_size));
    qDebug() << "mouse Press position: (" << coordinate_position.rx() << "," << coordinate_position.ry() << ")";
    walls.push_back(coordinate_position);
    start_point = {coordinate_position.rx() * m_cell_size, coordinate_position.ry() * m_cell_size};
    QGraphicsRectItem* rect = new QGraphicsRectItem();
    rect->setRect(start_point.rx(), start_point.ry(), m_cell_size, m_cell_size);
    rect->setBrush(QBrush(Qt::black));
    addItem(rect);
    QGraphicsScene::mousePressEvent(mouseEvent);
}

void GridScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
}

void GridScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
}
