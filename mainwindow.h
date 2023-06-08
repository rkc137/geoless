#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QMouseEvent>
#include <QGraphicsSceneMouseEvent>
#include <QGraphicsScene>
#include <QGraphicsPixmapItem>
#include <QListWidgetItem>
#include <QFile>
#include <QFileDialog>
#include <QMessageBox>

#include <QJsonObject>
#include <QJsonDocument>
#include <QJsonArray>

#include "dialog_tests_name.h"
#include "custom_qgscene.h"


QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    Custom_QGScene *scena;
    QMap<QListWidgetItem*, Point*> points;
    QString map_image_name;
    bool is_editing_mode = 1;
    void set_editing_mode(bool set_editing);


public slots:
    void open_file();
    void save_file();

    void start_test();

    void delete_all_points();
    void delete_point();
    void add_point();
    void choose_map();
    void change_point_size();

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
