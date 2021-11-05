#ifndef GRIDGRIDSCENE_H
#define GRIDGRIDSCENE_H

#include "configure.h"
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMap>
#include <QPainter>
#include <QSet>

const int cell_size = 20;

class GridScene : public QGraphicsScene {
    Q_OBJECT
private:
    int m_width;
    int m_height;
    int m_grid_width_num; // grid width number
    int m_grid_height_num; // grid height number
    int curr_uav_id;
    GRID_STATUS status;
    QPointF start_point;
    QList<QPointF> rects_to_draw_list;
    QVector<QPointF> current_walls;
    QSet<QPointF> walls_set;
    QMap<int, QPointF> uav_positions_map;

protected:
    void drawBackground(QPainter* painter, const QRectF& rect) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent) override;
    void getWalls(const QPointF& start, const QPointF& end);
    void setRectColor(const QPointF& coor, QBrush color);

public:
    GridScene(int width, int height);
    ~GridScene();
    GRID_STATUS getGridStarus();
    void setGridStatus(GRID_STATUS s);
    QVector<QPointF> getWalls() const;
    int getGridWidthNumber() const;
    int getGridHeightNumber() const;
    void setUAVId(int id);
    void removeUAV(int id);
    int getCurrUAVId() const;
    void addWall(QPointF position);
    void setUavColor(int id, QBrush color);

signals:
    void send_message(QString text);
    void send_position(int uav_id, int x, int y);
};

#endif // GRIDGRIDSCENE_H
