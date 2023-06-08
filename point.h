#ifndef POINT_H
#define POINT_H

#include <math.h>
#include <QGraphicsPixmapItem>
#include <QGraphicsEllipseItem>
#include <QGraphicsTextItem>
#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>
#include <QBrush>

class Point
{
public:
    inline static qreal available_radius = 10;
    QPointF pos;
    QGraphicsEllipseItem *shape;
    QGraphicsTextItem *text_shape;
    bool is_solved = 0;
    Point(QJsonObject json, QString name);
    Point();
    void set_pos(QPointF point);
    qreal distance(QPointF point);
};

#endif // POINT_H
