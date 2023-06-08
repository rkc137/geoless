#include "point.h"

Point::Point()
{
}

Point::Point(QJsonObject json, QString name)
{
    pos.setX(json["x"].toDouble());
    pos.setY(json["y"].toDouble());
    shape = new QGraphicsEllipseItem(pos.x() - available_radius / 2, pos.y() - available_radius / 2, available_radius, available_radius);
    shape->setBrush(QBrush(Qt::red));
    text_shape = new QGraphicsTextItem(name);
    text_shape->setPos(pos.x() - text_shape->boundingRect().width() / 2, pos.y() + available_radius / 2);
}

void Point::set_pos(QPointF point)
{
    pos = point;
    shape->setRect(pos.x() - available_radius / 2, pos.y() - available_radius / 2, available_radius, available_radius);
    text_shape->setPos(pos.x() - text_shape->boundingRect().width() / 2, pos.y() + available_radius / 2);
}

qreal Point::distance(QPointF point)
{
    return std::sqrt(std::pow(point.x() - pos.x(), 2) + std::pow(point.y() - pos.y(), 2));
}
