#ifndef SIMULATIONLAUNCHER_H
#define SIMULATIONLAUNCHER_H

#include <QDialog>
#include <QPixmap>
#include <QLabel>
#include <QMessageBox>
#include <QDebug>
#include <QFileDialog>
#include "configure.h"
#include "mainwindow.h"

namespace Ui
{
    class SimulationLauncher;
}

class SimulationLauncher : public QDialog
{
    Q_OBJECT

public:
    explicit SimulationLauncher(QWidget *parent = nullptr);
    ~SimulationLauncher();

private slots:
    void on_about_bt_clicked();
    void on_load_map_bt_clicked();

    void on_create_map_bt_clicked();

private:
    Ui::SimulationLauncher *ui;
    MainWindow *main_window;
};

#endif // SIMULATIONLAUNCHER_H
