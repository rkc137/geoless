#ifndef CUSTOM_QGSCENE_H
#define CUSTOM_QGSCENE_H

#include <QGraphicsScene>
#include <QGraphicsView>
#include <QGraphicsSceneWheelEvent>
#include <QGraphicsSceneMouseEvent>
#include <QListWidgetItem>
#include <QLabel>
#include <QtDebug>

#include "point.h"


class Custom_QGScene : public QGraphicsScene
{
public:
    explicit Custom_QGScene(QObject *parent = nullptr);
    explicit Custom_QGScene(QGraphicsView *_view, QWidget * parent = nullptr);
    QGraphicsView *view;
    QLabel *label;
    QMap<QListWidgetItem*, Point*> *_points;
    QListWidgetItem *curret_list_item;
    bool is_setting_point = 0;
    bool *is_editing_mode;
    int points_solved = 0;

public slots:
    void set_point(QListWidgetItem *item);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent *event);
    void wheelEvent(QGraphicsSceneWheelEvent *event);
};

#endif // CUSTOM_QGSCENE_H
