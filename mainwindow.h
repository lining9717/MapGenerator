#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "configure.h"
#include "gridscene.h"
#include "worldfilegenerator.h"
#include <QLabel>
#include <QMainWindow>
#include <QMessageBox>
#include <QStatusBar>
#include <QDebug>
#include <qinputdialog.h>
#include <QFileDialog>

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
    MainWindow(WINDOWS_TYPES type, QWidget* parent = nullptr);
    ~MainWindow();
    void setMapFile(const QString& map_file_path);

protected:
    void createGrid(int width, int height);
    void switchDrawing();

private slots:
    void on_generate_grid_bt_clicked();

    void on_start_toolButton_clicked();

private:
    Ui::MainWindow* ui;
    GridScene* grid_scene;
    QString map_file;
    QLabel* status_bar_label;
    void setStatusText(const QString& text);
};

#endif // MAINWINDOW_H
