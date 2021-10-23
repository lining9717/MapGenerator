#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "configure.h"
#include "gridscene.h"
#include "uavitem.h"
#include "worldfilegenerator.h"
#include <QDebug>
#include <QFile>
#include <QFileDialog>
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QStatusBar>
#include <qinputdialog.h>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget* parent = nullptr); //create a new world
    MainWindow(const QString& map_file_path, QWidget* parent = nullptr); // load an exist world
    void resizeEvent(QResizeEvent* event);
    ~MainWindow();

protected:
    void createGrid(int width, int height);
    void switchDrawing();

private slots:
    void on_generate_grid_bt_clicked();
    void on_start_toolButton_clicked();

    void on_add_toolButton_clicked();

    void on_delete_toolButton_clicked();

private:
    int uav_num;
    QVector<UavItem*> uav_items;
    Ui::MainWindow* ui;
    GridScene* grid_scene;
    QLabel* status_bar_label;
    void setStatusText(const QString& text);
    void init();
    bool loadWorld(const QString& map_file_path);
};

#endif // MAINWINDOW_H
