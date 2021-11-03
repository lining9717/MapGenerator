#include "gridscene.h"

// QSet中使用的hash函数没有以QPointF为键值的，需要手动定义
inline uint qHash(const QPointF& key)
{
    return qHash(QPair<int, int>(key.x(), key.y()));
}

GridScene::GridScene(int width, int height)
    : m_width(width)
    , m_height(height)
    , status(IDLE)
    , start_point(-1, -1)
{
    m_grid_width_num = static_cast<int>(width / cell_size);
    m_grid_height_num = static_cast<int>(height / cell_size);
    curr_uav_id = -1;
}

GridScene::~GridScene() {}

void GridScene::drawBackground(QPainter* painter, const QRectF& rect)
{
    QVarLengthArray<QLineF, 100> lines;
    for (qreal x = 0; x <= m_width; x += cell_size) {
        lines.append(QLineF(x, 0, x, m_height));
    }
    for (qreal y = 0; y <= m_height; y += cell_size) {
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

void GridScene::setUAVId(int id)
{
    curr_uav_id = id;
}

int GridScene::getCurrUAVId() const
{
    return curr_uav_id;
}

void GridScene::removeUAV(int id)
{
    qDebug() << "ID:" << id;
    for (auto it = uav_positions_map.begin(); it != uav_positions_map.end(); ++it) {
        qDebug() << it.key() << ":" << it.value();
    }
    QPointF curr_uav_old_position = uav_positions_map[id];
    QGraphicsRectItem* curr_rect = qgraphicsitem_cast<QGraphicsRectItem*>(
        itemAt({ curr_uav_old_position.x() * cell_size, curr_uav_old_position.y() * cell_size },
            QTransform()));
    removeItem(curr_rect);
    delete curr_rect;
    uav_positions_map.remove(id);
}

void GridScene::getWalls(const QPointF& start, const QPointF& end)
{
    current_walls.clear();
    qreal start_x = start.x(), start_y = start.y();
    qreal end_x = end.x(), end_y = end.y();
    qreal len_x = qAbs(start_x - end_x);
    qreal len_y = qAbs(start_y - end_y);
    if (start_x == end_x) {
        if (start_y <= end_y) {
            while (start_y <= end_y) {
                current_walls.push_back({ start_x, start_y });
                ++start_y;
            }
        } else {
            while (start_y >= end_y) {
                current_walls.push_back({ start_x, start_y });
                --start_y;
            }
        }
        return;
    }
    if (start_y == end_y) {
        if (start_x <= end_x) {
            while (start_x <= end_x) {
                current_walls.push_back({ start_x, start_y });
                ++start_x;
            }
        } else {
            while (start_x >= end_x) {
                current_walls.push_back({ start_x, start_y });
                --start_x;
            }
        }
        return;
    }
    if (start_x <= end_x and start_y <= end_y) {
        if (len_x >= len_y) {
            while (start_x <= end_x) {
                current_walls.push_back({ start_x, start_y });
                ++start_x;
            }
            start_x = start_x >= m_grid_width_num ? m_grid_width_num - 1 : start_x;
            while (start_y <= end_y) {
                current_walls.push_back({ start_x, start_y });
                ++start_y;
            }
        } else {
            while (start_y <= end_y) {
                current_walls.push_back({ start_x, start_y });
                ++start_y;
            }
            start_y = start_y >= m_grid_height_num ? m_grid_height_num - 1 : start_y;
            while (start_x <= end_x) {
                current_walls.push_back({ start_x, start_y });
                ++start_x;
            }
        }
    } else if (start_x <= end_x and start_y > end_y) {
        if (len_x >= len_y) {
            while (start_x <= end_x) {
                current_walls.push_back({ start_x, start_y });
                ++start_x;
            }
            start_x = start_x >= m_grid_width_num ? m_grid_width_num - 1 : start_x;
            while (start_y >= end_y) {
                current_walls.push_back({ start_x, start_y });
                --start_y;
            }
        } else {
            while (start_y >= end_y) {
                current_walls.push_back({ start_x, start_y });
                --start_y;
            }
            start_y = start_y < 0 ? 0 : start_y;
            while (start_x <= end_x) {
                current_walls.push_back({ start_x, start_y });
                ++start_x;
            }
        }
    } else if (start_x > end_x and start_y <= end_y) {
        if (len_x >= len_y) {
            while (start_x >= end_x) {
                current_walls.push_back({ start_x, start_y });
                --start_x;
            }
            start_x = start_x < 0 ? 0 : start_x;
            while (start_y <= end_y) {
                current_walls.push_back({ start_x, start_y });
                ++start_y;
            }
        } else {
            while (start_y <= end_y) {
                current_walls.push_back({ start_x, start_y });
                ++start_y;
            }
            start_y = start_y >= m_grid_height_num ? m_grid_height_num - 1 : start_y;
            while (start_x >= end_x) {
                current_walls.push_back({ start_x, start_y });
                --start_x;
            }
        }
    } else {
        if (len_x >= len_y) {
            while (start_x >= end_x) {
                current_walls.push_back({ start_x, start_y });
                --start_x;
            }
            start_x = start_x < 0 ? 0 : start_x;
            while (start_y >= end_y) {
                current_walls.push_back({ start_x, start_y });
                --start_y;
            }
        } else {
            while (start_y >= end_y) {
                current_walls.push_back({ start_x, start_y });
                --start_y;
            }
            start_y = start_y < 0 ? 0 : start_y;
            while (start_x >= end_x) {
                current_walls.push_back({ start_x, start_y });
                --start_x;
            }
        }
    }
}

void GridScene::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    if (status == IDLE)
        return;
    QPointF real_position = mouseEvent->scenePos();
    if (real_position.rx() < 0 or real_position.rx() >= m_width or real_position.ry() < 0 or real_position.ry() >= m_height) {
        QGraphicsScene::mousePressEvent(mouseEvent);
        return;
    }
    QPointF coordinate_position(
        static_cast<int>(real_position.rx() / cell_size),
        static_cast<int>(real_position.ry() / cell_size));
    if (status == EDIT_DRAWING) {
        rects_to_draw_list.clear();
        current_walls.clear();
        if (mouseEvent->button() == Qt::LeftButton) {
            start_point = { coordinate_position.rx(), coordinate_position.ry() };
        } else if (mouseEvent->button() == Qt::RightButton) {
            if (!walls_set.contains(coordinate_position)) {
                QGraphicsScene::mousePressEvent(mouseEvent);
                return;
            }
            walls_set.remove(coordinate_position);
            QGraphicsRectItem* curr_rect = qgraphicsitem_cast<QGraphicsRectItem*>(
                itemAt(real_position, QTransform()));
            removeItem(curr_rect);
            delete curr_rect;
        }
        QGraphicsScene::mousePressEvent(mouseEvent);
    } else if (status == UAV_DRAWING) {
        if (uav_positions_map.count(curr_uav_id) != 0) {
            QPointF curr_uav_old_position = uav_positions_map[curr_uav_id];
            QGraphicsRectItem* curr_rect = qgraphicsitem_cast<QGraphicsRectItem*>(
                itemAt({ curr_uav_old_position.x() * cell_size, curr_uav_old_position.y() * cell_size },
                    QTransform()));
            removeItem(curr_rect);
            delete curr_rect;
        }
        QGraphicsRectItem* rect = new QGraphicsRectItem();
        rect->setRect(coordinate_position.x() * cell_size,
            coordinate_position.y() * cell_size, cell_size,
            cell_size);
        rect->setBrush(QBrush(Qt::blue));
        addItem(rect);
        rects_to_draw_list.push_back({ coordinate_position.x() * cell_size + 1,
            coordinate_position.y() * cell_size + 1 });
        uav_positions_map[curr_uav_id] = coordinate_position;
        int x_bias = m_grid_width_num / 2;
        int y_bias = m_grid_height_num / 2;
        emit send_message("选择(" + QString::number(coordinate_position.rx() - x_bias) + ", " + QString::number(y_bias - coordinate_position.ry()) + ")");
        emit send_position(curr_uav_id, coordinate_position.rx() - x_bias, y_bias - coordinate_position.ry());
    }
}

void GridScene::mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    if (status == IDLE)
        return;
    if (status == EDIT_DRAWING) {
        if (start_point.rx() != -1) {
            QPointF real_position = mouseEvent->scenePos();
            if (real_position.rx() < 0) {
                real_position.setX(0);
            } else if (real_position.rx() >= m_width) {
                real_position.setX(m_width - 1);
            }
            if (real_position.ry() < 0) {
                real_position.setY(0);
            } else if (real_position.ry() >= m_height) {
                real_position.setY(m_height - 1);
            }
            QPointF coordinate_position(
                static_cast<int>(real_position.rx() / cell_size),
                static_cast<int>(real_position.ry() / cell_size));
            if (coordinate_position.rx() == start_point.rx() and coordinate_position.ry() == start_point.ry()) {
                QGraphicsScene::mouseMoveEvent(mouseEvent);
                return;
            }
            if (!rects_to_draw_list.empty()) {
                for (auto& item_point : rects_to_draw_list) {
                    QGraphicsRectItem* curr_rect = qgraphicsitem_cast<QGraphicsRectItem*>(
                        itemAt(item_point, QTransform()));
                    removeItem(curr_rect);
                    delete curr_rect;
                }
            }
            rects_to_draw_list.clear();
            getWalls(start_point, coordinate_position);
            if (!current_walls.empty()) {
                for (const auto& p : current_walls) {
                    if (walls_set.contains(p))
                        continue;
                    QGraphicsRectItem* rect = new QGraphicsRectItem();
                    rect->setRect(p.x() * cell_size, p.y() * cell_size, cell_size,
                        cell_size);
                    rect->setBrush(QBrush(Qt::black));
                    addItem(rect);
                    rects_to_draw_list.push_back(
                        { p.x() * cell_size + 1, p.y() * cell_size + 1 });
                }
            }
        } else
            QGraphicsScene::mouseMoveEvent(mouseEvent);
    }
}

void GridScene::mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    if (status == IDLE)
        return;

    if (status == EDIT_DRAWING) {
        QPointF real_position = mouseEvent->scenePos();
        QPointF coordinate_position(
            static_cast<int>(real_position.rx() / cell_size),
            static_cast<int>(real_position.ry() / cell_size));
        if (coordinate_position.rx() == start_point.rx() and coordinate_position.ry() == start_point.ry()) {
            if (walls_set.contains(coordinate_position)) {
                start_point.setX(-1);
                start_point.setY(-1);
                QGraphicsScene::mouseReleaseEvent(mouseEvent);
                return;
            }
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
        if (!current_walls.empty()) {
            for (auto& w : current_walls) {
                walls_set.insert(w);
            }
        }
        start_point.setX(-1);
        start_point.setY(-1);
    }
    QGraphicsScene::mouseReleaseEvent(mouseEvent);
}

void GridScene::addWall(QPointF position)
{
    walls_set.insert(position);
}

QVector<QPointF> GridScene::getWalls() const
{
    QVector<QPointF> res;
    for (QSet<QPointF>::const_iterator iter = walls_set.begin(); iter != walls_set.end(); iter++) {
        res.push_back(*iter);
    }
    return res;
}

int GridScene::getGridWidthNumber() const
{
    return m_grid_width_num;
}
int GridScene::getGridHeightNumber() const
{
    return m_grid_height_num;
}
