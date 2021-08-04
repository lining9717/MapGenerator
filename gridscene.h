#ifndef GRIDGRIDSCENE_H
#define GRIDGRIDSCENE_H

#include <QGraphicsItem>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QPainter>
#include <QDebug>
#include "configure.h"

class GridScene: public QGraphicsScene
{
private:
    int m_cell_size = 25;
    int m_width;
    int m_height;
    GRID_STATUS status;
    QPointF start_point;
    QList<QGraphicsRectItem*> rects_to_draw_list;
    QVector<QPointF> walls;


protected:
    void drawBackground(QPainter *painter, const QRectF &rect) override;
    void mousePressEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent) override;

public:
    GridScene(int width, int height);
    ~GridScene();
    GRID_STATUS getGridStarus();
    void setGridStatus(GRID_STATUS s);

};


#endif // GRIDGRIDSCENE_H
