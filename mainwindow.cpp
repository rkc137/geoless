#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->tabWidget->removeTab(1);

    scena = new Custom_QGScene(ui->graphicsView);
    scena->_points = &points;
    scena->is_editing_mode = &is_editing_mode;
    scena->label = ui->points_count;

    connect(ui->listWidget, &QListWidget::itemClicked, scena, &Custom_QGScene::set_point);

    connect(ui->action_open_file, &QAction::triggered, this, &MainWindow::open_file);
    connect(ui->action_save_file, &QAction::triggered, this, &MainWindow::save_file);

    connect(ui->action_start_test, &QAction::triggered, this, &MainWindow::start_test);

    connect(ui->delete_all_points_button, &QPushButton::clicked, this, &MainWindow::delete_all_points);
    connect(ui->delete_point_button, &QPushButton::clicked, this, &MainWindow::delete_point);
    connect(ui->add_point_button, &QPushButton::clicked, this, &MainWindow::add_point);
    connect(ui->choose_map_image_button, &QPushButton::clicked, this, &MainWindow::choose_map);
    connect(ui->size_slider, &QSlider::valueChanged, this, &MainWindow::change_point_size);
    connect(ui->listWidget, &QListWidget::itemChanged, this, [this](QListWidgetItem *item)
    {
        Point *p = points[item];
        p->text_shape->setPlainText(item->text());
        p->text_shape->setX(p->pos.x() - p->text_shape->boundingRect().width() / 2);
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::set_editing_mode(bool set_editing)
{
    is_editing_mode = set_editing;

    for(int i = 0; i < points.size(); i++)
    {
        QListWidgetItem *list_item = ui->listWidget->item(i);

        list_item->setFlags(list_item->flags() | Qt::ItemIsSelectable);
        list_item->setIcon(QIcon(":/res/marker.svg"));
    }

    if(is_editing_mode)
    {
        ui->edit_box->show();
        for(int i = 0; i < points.size(); i++)
        {
            QListWidgetItem *list_item = ui->listWidget->item(i);
            list_item->setFlags(list_item->flags () | Qt::ItemIsEditable);
        }
    }
    else
    {
        ui->edit_box->hide();
        for(int i = 0; i < points.size(); i++)
        {
            QListWidgetItem *list_item = ui->listWidget->item(i);
            list_item->setFlags(list_item->flags() & ~Qt::ItemIsEditable);
        }
    }
}

void MainWindow::open_file()
{
    QFileDialog *dialog_window = new QFileDialog;
    dialog_window->setDefaultSuffix("json");
    QString file_path = dialog_window->getOpenFileName(this, "открыть файл", "где этот текст", "json файлы(*.json)");

    if(file_path.isEmpty())
        return;

    scena->clear();
    points.clear();
    ui->listWidget->clear();

    QFile file(file_path);
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    QString val = file.readAll();
    file.close();
    QJsonObject doc = QJsonDocument::fromJson(val.toUtf8()).object();

    ui->test_name_line_edit->setText(doc["test_name"].toString());

    QString map_image_path = doc["map_image"].toString();
    map_image_name = map_image_path;
    map_image_path = file_path;
    map_image_path.replace(QFileInfo(file_path).fileName(), map_image_name);
    scena->addPixmap(QPixmap(map_image_path));

    Point::available_radius = doc["available_radius"].toDouble();
    ui->size_label->setNum(Point::available_radius);
    ui->size_slider->setValue(int(Point::available_radius));

    QJsonArray Array = doc["points"].toArray();
    for(int i = 0; i < Array.size(); i++)
    {
        QListWidgetItem *list_item = new QListWidgetItem(Array[i].toObject()["PropertyName"].toString());
        list_item->setFlags(list_item->flags () | Qt::ItemIsEditable);
        points.insert(list_item, new Point(Array[i].toObject(), list_item->text()));
        scena->addItem(points[list_item]->shape);
        scena->addItem(points[list_item]->text_shape);
        list_item->setIcon(QIcon(":/res/marker.svg"));
        ui->listWidget->addItem(list_item);
    }

    if(points.size() != 0)
        ui->points_count->setText("всего точек: " + QString::number(points.size()));
    else
        ui->points_count->setText("нет точек");
}

void MainWindow::save_file()
{
    QJsonArray json_points;
    QMap<QListWidgetItem*, Point*>::iterator i;
    for (i = points.begin(); i != points.end(); i++)
    {
        QJsonObject json_point;
        json_point["PropertyName"] = i.key()->text();
        json_point["x"] = i.value()->pos.x();
        json_point["y"] = i.value()->pos.y();
        json_points.push_back(json_point);
    }

    QJsonObject root;
    root["test_name"] = ui->test_name_line_edit->text();
    root["map_image"] = map_image_name;
    root["available_radius"] = Point::available_radius;
    root["points"] = json_points;

    QString file_path = QFileDialog::getSaveFileName(this, tr("сохранить тест"), "://" + ui->test_name_line_edit->text() + ".json", tr("*.json"));
    QFile json_file(file_path);
    json_file.open(QIODevice::WriteOnly);
    json_file.write(QJsonDocument(root).toJson());
}


void MainWindow::delete_all_points()
{
    ui->listWidget->clear();
    for(QMapIterator<QListWidgetItem*, Point*> i(points);i.hasNext();)
    {
        i.next();
        scena->removeItem(i.value()->shape);
        scena->removeItem(i.value()->text_shape);
    }
    points.clear();
    ui->points_count->setText("нет точек");
    scena->is_setting_point = 0;
    scena->view->setDragMode(QGraphicsView::ScrollHandDrag);
}

void MainWindow::delete_point()
{
    QListWidgetItem *item = ui->listWidget->currentItem();

    if(item == nullptr)
        return;

    scena->removeItem(points[item]->shape);
    scena->removeItem(points[item]->text_shape);
    delete points[item]->shape;
    delete points[item]->text_shape;
    points.remove(item);
    ui->listWidget->removeItemWidget(item);
    delete item;
    if(points.size() != 0)
        ui->points_count->setText("всего точек: " + QString::number(points.size()));
    else
        ui->points_count->setText("нет точек");
    scena->is_setting_point = 0;
    scena->view->setDragMode(QGraphicsView::ScrollHandDrag);
}

void MainWindow::add_point()
{
    QListWidgetItem *list_item = new QListWidgetItem("новая точка");
    list_item->setFlags(list_item->flags () | Qt::ItemIsEditable);
    list_item->setIcon(QIcon(":/res/marker.svg"));
    points.insert(list_item, new Point);
    Point *p = points[list_item];
    p->shape = new QGraphicsEllipseItem(0,0,0,0);
    p->text_shape = new QGraphicsTextItem("новая точка");
    p->shape->setBrush(QBrush(Qt::red));
    scena->addItem(p->shape);
    scena->addItem(p->text_shape);
    ui->listWidget->addItem(list_item);
    ui->points_count->setText("всего точек: " + QString::number(points.size()));
}

void MainWindow::choose_map()
{
    QString map_image_path = QFileDialog::getOpenFileName(this, "открыть изображение", "://");
    if(map_image_path.isEmpty())
        return;
    scena->clear();
    scena->addPixmap(QPixmap(map_image_path));//not safe :)))))))
    map_image_name = QFileInfo(map_image_path).fileName();
}

void MainWindow::change_point_size()
{
    Point::available_radius = ui->size_slider->value();
    ui->size_label->setNum(Point::available_radius);
    for(auto &p: points)
    {
        p->shape->setRect(p->pos.x() - Point::available_radius / 2,
                          p->pos.y() - Point::available_radius / 2,
                                       Point::available_radius,
                                       Point::available_radius);
        p->text_shape->setPos(p->pos.x() - p->text_shape->boundingRect().width() / 2, p->pos.y() + Point::available_radius / 2);
    }
}

void MainWindow::start_test()
{
    is_editing_mode = !is_editing_mode;
    set_editing_mode(is_editing_mode);
    ui->menu->setDisabled(!is_editing_mode);
    for(auto &p: points)
    {
        p->shape->setBrush(QBrush(Qt::red));
        p->text_shape->setVisible(is_editing_mode);
    }
    if(is_editing_mode)
    {
        ui->action_start_test->setText("начать тестирование");
        if(scena->points_solved != 0)
            QMessageBox::about(this, "итог", "решено задач: " + QString::number(scena->points_solved) + " из " + QString::number(points.size()));
        ui->points_count->setText("всего точек: " + QString::number(points.size()));
    }
    else
    {
        ui->action_start_test->setText("закончить тестирование");
        scena->points_solved = 0;
    }
}
