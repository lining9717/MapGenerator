#include "simulationlauncher.h"
#include "ui_simulationlauncher.h"

SimulationLauncher::SimulationLauncher(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SimulationLauncher)
{
    ui->setupUi(this);
    this->setFixedSize(300, 500);  //固定窗口大小
    this->setWindowTitle(SIMULATION_LAUNCH_NAME + " " + VERSION); //设置窗口标题
    this->setWindowIcon(QIcon(":/new/image/img/logo2.png"));  //设置窗口logo
    this->setWindowFlags(Qt::CustomizeWindowHint | Qt::WindowCloseButtonHint);   //设置窗口
    QString img_launch_file_path(":/new/image/img/launch.png");
    QImage *launch_img = new QImage();
    if(! ( launch_img->load(img_launch_file_path) ) ) { //加载图像
        QMessageBox::information(this,
                                 tr("打开图像失败"),
                                 tr("打开图像失败!"));
        delete launch_img;
        return;
    }
    launch_img->scaled(ui->label->size(), Qt::KeepAspectRatio);
    ui->label->setScaledContents(true);
    ui->label->setPixmap(QPixmap::fromImage(*launch_img));
    delete launch_img;
    connect(ui->exit_bt, &QPushButton::clicked, qApp, &QApplication::quit);
}

SimulationLauncher::~SimulationLauncher()
{
    delete ui;
}


void SimulationLauncher::on_about_bt_clicked()
{
    QMessageBox::about(this, "关于",
                       "多无人机协同探索未知室内空间仿真程序 \n"
                       "系统要求：Ubuntu 16.04、ROS Knetic\n"
                       "                       By LiNing");
}

void SimulationLauncher::on_load_map_bt_clicked()
{

    QString fileName = QFileDialog::getOpenFileName(
                           this,
                           tr("打开地图文件"),
                           "/home/ln/ros_ws/src/uavs_explore_indoor_environment/maps/",
                           tr("text files(*.txt)"));
    if(fileName.isEmpty()) {
        return;
    } else {
        this->hide();
        qDebug() << fileName;
        main_window = new MainWindow(fileName);
        main_window->show();
    }
}

void SimulationLauncher::on_create_map_bt_clicked()
{
    this->hide();
    main_window = new MainWindow();
    main_window->show();
}
