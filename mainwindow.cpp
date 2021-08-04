#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(WINDOWS_TYPES type, QWidget *parent)
    : QMainWindow(parent), ui(new Ui::MainWindow), grid_scene(nullptr)
{
    ui->setupUi(this);
    this->setWindowTitle(SIMULATION_LAUNCH_NAME + " " + VERSION); //设置窗口标题
    this->setWindowIcon(QIcon(":/new/image/img/logo2.png"));  //设置窗口logo
    //设置默认网格
    createGrid(20, 20);
    //设置底部状态栏
    QFont font("Microsoft YaHei", 8, QFont::Normal, true);
    status_bar_label = new QLabel();
    status_bar_label->setFont(font);
    status_bar_label->setAlignment(Qt::AlignCenter);
    status_bar_label->setMinimumSize(status_bar_label->sizeHint());
    statusBar()->setStyleSheet(QString("QStatusBar::item{border: 0px}")); // 不显示label的边框
    statusBar()->setSizeGripEnabled(false); //不显示右边的大小控制点
    statusBar()->addWidget(status_bar_label);
    //设置启动按钮
    ui->add_toolButton->setIcon(QIcon(":/new/image/img/add.png"));
    ui->add_toolButton->setStyleSheet("border:none");
    ui->delete_all_toolButton->setIcon(QIcon(":/new/image/img/delete.png"));
    ui->delete_all_toolButton->setStyleSheet("border:none");
    //初始化画板状态
    grid_scene->setGridStatus(IDLE);
    //初始化状态
    if(type == LOADWORLD) {
        ui->width_spinBox->setEnabled(false);
        ui->height_spinBox->setEnabled(false);
        ui->draw_pushButton->setEnabled(false);
        ui->erase_pushButton->setEnabled(false);
        setStatusText("加载世界就绪");
    } else {
        setStatusText("创建世界就绪");
    }
    //信号槽绑定
    connect(ui->edit_draw_action, &QAction::triggered, this, &MainWindow::switchDrawing);
    connect(ui->draw_pushButton, &QPushButton::clicked, this, &MainWindow::switchDrawing);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete status_bar_label;
    delete grid_scene;
}

void MainWindow::setStatusText(const QString &text)
{
    status_bar_label->setText(text);
}

void MainWindow::setMapFile(const QString &map_file_path)
{
    map_file = map_file_path;
    setStatusText("加载世界就绪: " + map_file);
}

void MainWindow::createGrid(int num_of_row, int num_of_col)
{
    delete grid_scene;
    QGraphicsView *grid_view = ui->grid_view;
    int width = cell_size * num_of_row;
    int height = cell_size * num_of_col;
    grid_scene = new GridScene(width, height);
    grid_view->resize(width, height);
    grid_view->setScene(grid_scene);
    grid_view->setSceneRect(0, 0, width, height);
    grid_view->scene()->addRect(0, 0, cell_size, cell_size)->setBrush(QBrush(Qt::black));
    grid_view->show();
}

void MainWindow::switchDrawing()
{
    if(grid_scene == nullptr) {
        return;
    }
    if(grid_scene->getGridStarus() != DRAWING) {
        grid_scene->setGridStatus(DRAWING);
        ui->edit_draw_action->setText("停止绘制");
        ui->draw_pushButton->setText("停止绘制");
        setStatusText("绘制地图中...");
    } else {
        grid_scene->setGridStatus(IDLE);
        ui->edit_draw_action->setText("开始绘制");
        ui->draw_pushButton->setText("开始绘制");
        setStatusText("停止绘制");
    }
}
