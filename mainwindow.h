#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QStatusBar>
#include <QLabel>
#include "gridscene.h"
#include "configure.h"

QT_BEGIN_NAMESPACE
namespace Ui
{
    class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(WINDOWS_TYPES type, QWidget *parent = nullptr);
    ~MainWindow();
    void setMapFile(const QString &map_file_path);


protected:
    void createGrid(int width, int height);
    void switchDrawing();



private:
    Ui::MainWindow *ui;
    GridScene *grid_scene;
    QString map_file;
    QLabel *status_bar_label;
    int cell_size = 25;
    void setStatusText(const QString &text);

};

#endif // MAINWINDOW_H
