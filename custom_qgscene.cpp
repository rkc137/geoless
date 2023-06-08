#include "custom_qgscene.h"

Custom_QGScene::Custom_QGScene(QObject *parent)
    : QGraphicsScene{parent}
{
}

Custom_QGScene::Custom_QGScene(QGraphicsView *_view, QWidget *parent)
    : QGraphicsScene{parent}
{
    view = _view;
    view->setScene(this);
}


void Custom_QGScene::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
    if(!is_setting_point)
        return;
    is_setting_point = 0;
    view->setDragMode(QGraphicsView::ScrollHandDrag);


    Point* curret_point = _points->value(curret_list_item);

    if(*is_editing_mode)
    {
        for(auto &p: *_points)
            if(p->distance(event->scenePos()) < Point::available_radius && curret_point != p)
                return;
        curret_point->set_pos(event->scenePos());
    }
    else if(!curret_point->is_solved)
    {
        if(curret_point->distance(event->scenePos()) < Point::available_radius)
        {
            curret_list_item->setFlags(curret_list_item->flags() & ~Qt::ItemIsSelectable);
            curret_list_item->setIcon(QIcon(":/res/check.svg"));
            curret_point->is_solved = 1;
            curret_point->shape->setBrush(QBrush(Qt::green));
            points_solved++;
            label->setText("решено " + QString::number(points_solved) + '/' + QString::number(_points->size()));
        }
        else
        {
            curret_list_item->setIcon(QIcon(":/res/cross.svg"));
        }
    }
}

void Custom_QGScene::wheelEvent(QGraphicsSceneWheelEvent *event)
{
    if(event->delta() > 0)
        view->scale(1.5, 1.5);
    else
        view->scale(0.75,0.75);
}

void Custom_QGScene::set_point(QListWidgetItem *item)
{
    curret_list_item = item;

    view->setDragMode(QGraphicsView::NoDrag);
    view->setCursor(Qt::CrossCursor);
    is_setting_point = 1;
}
