#include "gridscene.h"

GridScene::GridScene(int width, int height)
    : m_width(width)
    , m_height(height)
    , status(IDLE)
    , start_point(-1, -1)
{
    m_grid_width_num = static_cast<int>(width / cell_size);
    m_grid_height_num = static_cast<int>(height / cell_size);
}

GridScene::~GridScene() {}

void GridScene::drawBackground(QPainter* painter, const QRectF& rect)
{
    QVarLengthArray<QLineF, 100> lines;
    for (qreal x = 0; x <= m_width; x += cell_size)
    {
        lines.append(QLineF(x, 0, x, m_height));
    }
    for (qreal y = 0; y <= m_height; y += cell_size)
    {
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

void GridScene::getWalls(const QPointF& start, const QPointF& end)
{
    current_walls.clear();
    qreal start_x = start.x(), start_y = start.y();
    qreal end_x = end.x(), end_y = end.y();
//    qDebug() << "start:" << start_x << "," << start_y << ", "
//             << "end:" << end_x << "," << end_y;
    qreal len_x = qAbs(start_x - end_x);
    qreal len_y = qAbs(start_y - end_y);
    if (start_x == end_x)
    {
        if (start_y <= end_y)
        {
            while (start_y <= end_y)
            {
                current_walls.push_back({ start_x, start_y });
                ++start_y;
            }
        }
        else
        {
            while (start_y >= end_y)
            {
                current_walls.push_back({ start_x, start_y });
                --start_y;
            }
        }
        return;
    }
    if (start_y == end_y)
    {
        if (start_x <= end_x)
        {
            while (start_x <= end_x)
            {
                current_walls.push_back({ start_x, start_y });
                ++start_x;
            }
        }
        else
        {
            while (start_x >= end_x)
            {
                current_walls.push_back({ start_x, start_y });
                --start_x;
            }
        }
        return;
    }
    if (start_x <= end_x and start_y <= end_y)
    {
        if (len_x >= len_y)
        {
            while (start_x <= end_x)
            {
                current_walls.push_back({ start_x, start_y });
                ++start_x;
            }
            start_x = start_x >= m_grid_width_num ? m_grid_width_num - 1 : start_x;
            while (start_y <= end_y)
            {
                current_walls.push_back({ start_x, start_y });
                ++start_y;
            }
        }
        else
        {
            while (start_y <= end_y)
            {
                current_walls.push_back({ start_x, start_y });
                ++start_y;
            }
            start_y = start_y >= m_grid_height_num ? m_grid_height_num - 1 : start_y;
            while (start_x <= end_x)
            {
                current_walls.push_back({ start_x, start_y });
                ++start_x;
            }
        }
    }
    else if (start_x <= end_x and start_y > end_y)
    {
        if (len_x >= len_y)
        {
            while (start_x <= end_x)
            {
                current_walls.push_back({ start_x, start_y });
                ++start_x;
            }
            start_x = start_x >= m_grid_width_num ? m_grid_width_num - 1 : start_x;
            while (start_y >= end_y)
            {
                current_walls.push_back({ start_x, start_y });
                --start_y;
            }
        }
        else
        {
            while (start_y >= end_y)
            {
                current_walls.push_back({ start_x, start_y });
                --start_y;
            }
            start_y = start_y < 0 ? 0 : start_y;
            while (start_x <= end_x)
            {
                current_walls.push_back({ start_x, start_y });
                ++start_x;
            }
        }
    }
    else if (start_x > end_x and start_y <= end_y)
    {
        if (len_x >= len_y)
        {
            while (start_x >= end_x)
            {
                current_walls.push_back({ start_x, start_y });
                --start_x;
            }
            start_x = start_x < 0 ? 0 : start_x;
            while (start_y <= end_y)
            {
                current_walls.push_back({ start_x, start_y });
                ++start_y;
            }
        }
        else
        {
            while (start_y <= end_y)
            {
                current_walls.push_back({ start_x, start_y });
                ++start_y;
            }
            start_y = start_y >= m_grid_height_num ? m_grid_height_num - 1 : start_y;
            while (start_x >= end_x)
            {
                current_walls.push_back({ start_x, start_y });
                --start_x;
            }
        }
    }
    else
    {
        if (len_x >= len_y)
        {
            while (start_x >= end_x)
            {
                current_walls.push_back({ start_x, start_y });
                --start_x;
            }
            start_x = start_x < 0 ? 0 : start_x;
            while (start_y >= end_y)
            {
                current_walls.push_back({ start_x, start_y });
                --start_y;
            }
        }
        else
        {
            while (start_y >= end_y)
            {
                current_walls.push_back({ start_x, start_y });
                --start_y;
            }
            start_y = start_y < 0 ? 0 : start_y;
            while (start_x >= end_x)
            {
                current_walls.push_back({ start_x, start_y });
                --start_x;
            }
        }
    }
}

void GridScene::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    rects_to_draw_list.clear();
    current_walls.clear();
    if (mouseEvent->button() == Qt::LeftButton)
    {
        QPointF real_position = mouseEvent->scenePos();
        if (real_position.rx() < 0 or real_position.rx() >= m_width or real_position.ry() < 0 or real_position.ry() >= m_height)
        {
            QGraphicsScene::mousePressEvent(mouseEvent);
            return;
        }
        QPointF coordinate_position(
            static_cast<int>(real_position.rx() / cell_size),
            static_cast<int>(real_position.ry() / cell_size));
        current_walls.push_back(coordinate_position);
        start_point = { coordinate_position.rx(), coordinate_position.ry() };
        QGraphicsScene::mousePressEvent(mouseEvent);
    }
    else if(mouseEvent->button() == Qt::RightButton)
    {
        QPointF real_position = mouseEvent->scenePos();
        if (real_position.rx() < 0 or real_position.rx() >= m_width or real_position.ry() < 0 or real_position.ry() >= m_height)
        {
            QGraphicsScene::mousePressEvent(mouseEvent);
            return;
        }
        QPointF coordinate_position(
            static_cast<int>(real_position.rx() / cell_size),
            static_cast<int>(real_position.ry() / cell_size));
        QGraphicsRectItem* curr_rect = qgraphicsitem_cast<QGraphicsRectItem*>(
                                           itemAt(real_position, QTransform()));
        removeItem(curr_rect);
        delete curr_rect;
        QVector<QVector<int>> delete_index;
        for(int i=0; i<walls_record.size(); ++i)
        {
            for(int j=0; j<walls_record[i].size(); ++j)
            {
                if(walls_record[i][j]==coordinate_position)
                {
                    delete_index.push_back({i,j});
                }
            }
        }
        for(auto &ind:delete_index)
        {
            walls_record[ind[0]].erase(walls_record[ind[0]].begin()+ind[1]);
        }
    }

}

void GridScene::mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    if (start_point.rx() != -1)
    {
        QPointF real_position = mouseEvent->scenePos();
        if (real_position.rx() < 0)
        {
            real_position.setX(0);
        }
        else if (real_position.rx() >= m_width)
        {
            real_position.setX(m_width - 1);
        }
        if (real_position.ry() < 0)
        {
            real_position.setY(0);
        }
        else if (real_position.ry() >= m_height)
        {
            real_position.setY(m_height - 1);
        }
        QPointF coordinate_position(
            static_cast<int>(real_position.rx() / cell_size),
            static_cast<int>(real_position.ry() / cell_size));
        if (coordinate_position.rx() == start_point.rx() and coordinate_position.ry() == start_point.ry())
        {
            QGraphicsScene::mouseMoveEvent(mouseEvent);
            return;
        }
//        qDebug() << "mouse Move position: (" << coordinate_position.rx() << ","
//                 << coordinate_position.ry() << ")";
        if (!rects_to_draw_list.empty())
        {
            for (auto& item_point : rects_to_draw_list)
            {
                QGraphicsRectItem* curr_rect = qgraphicsitem_cast<QGraphicsRectItem*>(
                                                   itemAt(item_point, QTransform()));
                removeItem(curr_rect);
                delete curr_rect;
            }
        }
        rects_to_draw_list.clear();
        getWalls(start_point, coordinate_position);
        if (!current_walls.empty())
        {
            for (const auto& p : current_walls)
            {
                QGraphicsRectItem* rect = new QGraphicsRectItem();
                rect->setRect(p.x() * cell_size, p.y() * cell_size, cell_size,
                              cell_size);
                rect->setBrush(QBrush(Qt::black));
                addItem(rect);
                rects_to_draw_list.push_back(
                { p.x() * cell_size + 1, p.y() * cell_size + 1 });
            }
        }
    }
    else
    {
        QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void GridScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    QPointF real_position = mouseEvent->scenePos();
    QPointF coordinate_position(
        static_cast<int>(real_position.rx() / cell_size),
        static_cast<int>(real_position.ry() / cell_size));
//    qDebug() << "mouse Release position: (" << coordinate_position.rx() << ","
//             << coordinate_position.ry() << ")";
    if (coordinate_position.rx() == start_point.rx() and coordinate_position.ry() == start_point.ry())
    {
        QGraphicsRectItem* rect = new QGraphicsRectItem();
        rect->setRect(coordinate_position.x() * cell_size,
                      coordinate_position.y() * cell_size, cell_size,
                      cell_size);
        rect->setBrush(QBrush(Qt::black));
        addItem(rect);
        rects_to_draw_list.push_back({ coordinate_position.x() * cell_size + 1,
                                       coordinate_position.y() * cell_size + 1 });
        current_walls.push_back(coordinate_position);
    }
    if (!current_walls.empty())
    {
        walls_record.push_back(current_walls);
    }
    start_point.setX(-1);
    start_point.setY(-1);
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

QVector<QVector<QPointF>> GridScene::getWalls() const
{
    return walls_record;
}

int GridScene::getGridWidthNumber() const
{
    return m_grid_width_num;
}
int GridScene::getGridHeightNumber() const
{
    return m_grid_height_num;
}
