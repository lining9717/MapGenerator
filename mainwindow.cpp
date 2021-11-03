#include "mainwindow.h"
#include "./ui_mainwindow.h"

// 打开一个新地图构造函数
MainWindow::MainWindow(QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , grid_scene(nullptr)
{
    //初始化
    init();
    //设置默认网格
    createGrid(20, 20);
    setStatusText("创建世界就绪");
}

MainWindow::MainWindow(const QString& map_file_path, QWidget* parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , grid_scene(nullptr)
{
    //初始化
    init();
    if (loadWorld(map_file_path))
        setStatusText("加载世界成功: " + map_file_path);
    else
        setStatusText("加载世界失败: " + map_file_path);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete status_bar_label;
    delete grid_scene;
}

void MainWindow::init()
{
    uav_num = -1;
    ui->setupUi(this);
    this->setWindowTitle(SIMULATION_LAUNCH_NAME + " " + VERSION); //设置窗口标题
    this->setWindowIcon(QIcon(":/new/image/img/logo2.png")); //设置窗口logo
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
    ui->delete_toolButton->setIcon(QIcon(":/new/image/img/delete.png"));
    ui->delete_toolButton->setStyleSheet("border:none");

    //设置无人机listWidget
    ui->listWidget->setResizeMode(QListView::Adjust);
    ui->listWidget->setStyleSheet("QListWidget::item { border-bottom: 1px solid black; }");
    ui->listWidget->setMovement(QListView::Static); //禁止元素拖拽
    ui->listWidget->setFocusPolicy(Qt::NoFocus);

    //信号槽绑定
    connect(ui->edit_draw_action, &QAction::triggered, this, &MainWindow::switchDrawingAction);
    connect(ui->generate_map_file_action, &QAction::triggered, this, &MainWindow::generateMapFileAction);
}

void MainWindow::setStatusText(QString text)
{
    status_bar_label->setText(text);
}

bool MainWindow::loadWorld(const QString& map_file_path)
{
    QFile f(map_file_path);
    if (!f.open(QIODevice::ReadOnly)) {
        QMessageBox::information(this,
            tr("失败"),
            tr("打开地图文件失败!"));
        return false;
    }
    int map_width = 0, map_height = 0;
    QTextStream in(&f);
    QVector<QString> grid;
    while (!in.atEnd()) {
        QString line = in.readLine();
        grid.push_back(line);
        map_width = max(map_width, line.size() - 2);
    }
    map_height = grid.size() - 2;
    ui->width_spinBox->setValue(map_width);
    ui->height_spinBox->setValue(map_height);
    ui->width_spinBox->setEnabled(false);
    ui->height_spinBox->setEnabled(false);
    ui->generate_grid_bt->setEnabled(false);
    createGrid(map_width, map_height);
    for (int i = 1; i < map_height + 2; ++i) {
        for (int j = 1; j < map_width + 2; ++j) {
            if (grid[i][j] == '#') {
                grid_scene->addWall(QPointF(j - 1, i - 1));
                grid_scene->addRect((j - 1) * cell_size, (i - 1) * cell_size, cell_size, cell_size)
                    ->setBrush(QBrush(Qt::black));
            }
        }
    }
    return true;
}

void MainWindow::createGrid(int num_of_row, int num_of_col)
{
    QGraphicsView* grid_view = ui->grid_view;
    int width = cell_size * num_of_row;
    int height = cell_size * num_of_col;
    GridScene* new_grid_scene = new GridScene(width, height);
    grid_view->setScene(new_grid_scene);
    grid_view->setSceneRect(0, 0, width, height);
    grid_view->update();
    delete grid_scene;
    grid_scene = new_grid_scene;
    //初始化画板状态
    grid_scene->setGridStatus(IDLE);
    connect(grid_scene, &GridScene::send_message, this, &MainWindow::setStatusText);
    connect(grid_scene, &GridScene::send_position, this, &MainWindow::updateUavPosition);
}

void MainWindow::switchDrawingAction()
{
    if (grid_scene == nullptr)
        return;
    if (grid_scene->getGridStarus() == IDLE) {
        grid_scene->setGridStatus(EDIT_DRAWING);
        ui->edit_draw_action->setText("停止绘制");
        ui->uav_draw_action->setEnabled(false);
        ui->generate_map_file_action->setEnabled(false);
        ui->add_toolButton->setEnabled(false);
        ui->width_spinBox->setEnabled(false);
        ui->height_spinBox->setEnabled(false);
        ui->delete_toolButton->setEnabled(false);
        ui->generate_grid_bt->setEnabled(false);
        setStatusText("绘制地图中...");
    } else if (grid_scene->getGridStarus() == EDIT_DRAWING) {
        grid_scene->setGridStatus(IDLE);
        ui->edit_draw_action->setText("绘制地图");
        ui->uav_draw_action->setEnabled(true);
        ui->generate_map_file_action->setEnabled(true);
        ui->add_toolButton->setEnabled(true);
        ui->delete_toolButton->setEnabled(true);
        ui->generate_grid_bt->setEnabled(true);
        ui->width_spinBox->setEnabled(true);
        ui->height_spinBox->setEnabled(true);
        setStatusText("停止绘制");
    }
}

void MainWindow::generateMapFileAction()
{
    if (grid_scene == nullptr)
        return;

    setStatusText("生成地图文件...");
    QVector<QPointF> walls = grid_scene->getWalls();
    int height = grid_scene->getGridHeightNumber();
    int width = grid_scene->getGridWidthNumber();
    QVector<QString> map(height + 2, QString(width + 2, '.'));

    for (int i = 0; i < walls.size(); ++i) {
        map[int(walls[i].ry()) + 1][int(walls[i].rx()) + 1] = '#';
    }

    QString filename = QFileDialog::getSaveFileName(
        this,
        tr("选择保存目录文件"),
        "/home/ln/ros_ws/src/uavs_explore_indoor_environment/maps/",
        tr("text files(*.txt)"));
    QFile f(filename);
    if (f.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&f);
        for (QString& line : map) {
            out << line << endl;
        }
    } else {
        qWarning("打开文件失败");
    }
    setStatusText("生成world文件...");
    WorldFileGenerator mg;
    mg.setMapFile(filename.toStdString());
    QStringList list = filename.split('.');
    QStringList file_content_list = list[0].split('/');
    QString map_file_name = file_content_list.back();
    mg.generateWorldFile("/home/ln/ros_ws/src/uavs_explore_indoor_environment/worlds/" + map_file_name.toStdString() + ".world");
    setStatusText("生成world文件成功：" + list[0]);
    QFile uavs_position_file("/home/ln/ros_ws/src/uavs_explore_indoor_environment/positions/" + map_file_name + ".txt");
    if (uavs_position_file.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&uavs_position_file);
        for (auto& uav : uav_items) {
            out << uav->getX() << "," << uav->getY() << endl;
        }
    } else {
        qWarning("打开文件失败");
    }
}

void MainWindow::on_generate_grid_bt_clicked()
{
    QMessageBox::StandardButton reply;
    reply = QMessageBox::information(this, "生成新网格地图", "确定之后将会清除已绘制的地图",
        QMessageBox::Yes | QMessageBox::No, QMessageBox::No);
    if (reply == QMessageBox::Yes) {
        int width_num = ui->width_spinBox->value();
        int height_num = ui->height_spinBox->value();
        createGrid(width_num, height_num);
        while (uav_num >= 0) {
            QListWidgetItem* item = ui->listWidget->item(uav_num);
            ui->listWidget->removeItemWidget(item);
            delete item;
            item = nullptr;
            grid_scene->removeUAV(uav_num);
            UavItem* p = uav_items[uav_num];
            uav_items.remove(uav_num);
            delete p;
            --uav_num;
        }
    }
}

void MainWindow::on_start_toolButton_clicked()
{
}

void MainWindow::resizeEvent(QResizeEvent* event)
{
    for (int i = 0; i < ui->listWidget->count(); i++) {
        QListWidgetItem* item = ui->listWidget->item(i);
        item->setSizeHint(QSize(ui->listWidget->width(), 52));
    }
}

void MainWindow::on_add_toolButton_clicked()
{
    UavItem* p_uav_item = new UavItem(++uav_num, this);
    QListWidgetItem* pItem = new QListWidgetItem();
    pItem->setSizeHint(QSize(ui->listWidget->width(), 52));
    ui->listWidget->addItem(pItem);
    ui->listWidget->setItemWidget(pItem, p_uav_item);

    uav_items.push_back(p_uav_item);
    connect(p_uav_item, &UavItem::send_uav_id, this, &MainWindow::set_uav_position);
}

void MainWindow::on_delete_toolButton_clicked()
{
    if (uav_num == -1)
        return;
    QListWidgetItem* item = ui->listWidget->item(uav_num);
    ui->listWidget->removeItemWidget(item);
    delete item;
    item = nullptr;
    grid_scene->removeUAV(uav_num);
    UavItem* p = uav_items[uav_num];
    uav_items.remove(uav_num);
    --uav_num;
    delete p;
}

void MainWindow::updateUavPosition(int id, int x, int y)
{
    uav_items[id]->setItem(x, y);
}

void MainWindow::set_uav_position(int id)
{
    if (grid_scene == nullptr)
        return;

    if (grid_scene->getGridStarus() == IDLE) {
        grid_scene->setUAVId(id);
        grid_scene->setGridStatus(UAV_DRAWING);
        ui->edit_draw_action->setEnabled(false);
        ui->uav_draw_action->setEnabled(false);
        ui->generate_map_file_action->setEnabled(false);
        ui->add_toolButton->setEnabled(false);
        ui->delete_toolButton->setEnabled(false);
        ui->generate_grid_bt->setEnabled(false);
        ui->width_spinBox->setEnabled(false);
        ui->height_spinBox->setEnabled(false);
        if (id <= uav_num) {
            uav_items[id]->setPicSelecting();
            for (int i = 0; i <= uav_num; ++i) {
                if (i != id)
                    uav_items[i]->setEditEnable(false);
            }
        }
        setStatusText("选择无人机" + QString::number(id) + "位置");
    } else if (grid_scene->getGridStarus() == UAV_DRAWING) {
        if (grid_scene->getCurrUAVId() != id)
            return;
        grid_scene->setUAVId(-1);
        grid_scene->setGridStatus(IDLE);
        ui->edit_draw_action->setEnabled(true);
        ui->uav_draw_action->setEnabled(true);
        ui->generate_map_file_action->setEnabled(true);
        ui->add_toolButton->setEnabled(true);
        ui->delete_toolButton->setEnabled(true);
        ui->generate_grid_bt->setEnabled(true);
        ui->width_spinBox->setEnabled(true);
        ui->height_spinBox->setEnabled(true);
        if (id <= uav_num) {
            uav_items[id]->setPicDefault();
            for (int i = 0; i <= uav_num; ++i) {
                if (i != id)
                    uav_items[i]->setEditEnable(true);
            }
        }
        setStatusText("选择无人机" + QString::number(id) + "位置成功");
    }
}
