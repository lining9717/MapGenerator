#ifndef GRIDGRIDSCENE_H
#define GRIDGRIDSCENE_H

#include "configure.h"
#include <QDebug>
#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QSet>

const int cell_size = 20;

class GridScene : public QGraphicsScene {
private:
    int m_width;
    int m_height;
    int m_grid_width_num; // grid width number
    int m_grid_height_num; // grid height number
    GRID_STATUS status;
    QPointF start_point;
    QList<QPointF> rects_to_draw_list;
    QVector<QPointF> current_walls;
    //    QVector<QVector<QPointF>> walls_record;
    QSet<QPointF> walls_set;

protected:
    void drawBackground(QPainter* painter, const QRectF& rect) override;
    void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent) override;
    void getWalls(const QPointF& start, const QPointF& end);

public:
    GridScene(int width, int height);
    ~GridScene();
    GRID_STATUS getGridStarus();
    void setGridStatus(GRID_STATUS s);
    QVector<QPointF> getWalls() const;
    int getGridWidthNumber() const;
    int getGridHeightNumber() const;
};

#endif // GRIDGRIDSCENE_H
