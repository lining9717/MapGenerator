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
    uav_num = 0;
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
    ui->listWidget->setViewMode(QListView::IconMode);
    ui->listWidget->setStyleSheet("QListWidget::item { border-bottom: 1px solid black; }");
    ui->listWidget->setMovement(QListView::Static); //禁止元素拖拽

    //信号槽绑定
    connect(ui->edit_draw_action, &QAction::triggered, this, &MainWindow::switchDrawing);
}

void MainWindow::setStatusText(const QString& text)
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
    createGrid(map_width, map_height);
    for (int i = 1; i < map_height + 2; ++i) {
        for (int j = 1; j < map_width + 2; ++j) {
            if (grid[i][j] == '#') {
                grid_scene->addRect((i - 1) * cell_size, (j - 1) * cell_size, cell_size, cell_size)
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
    grid_view->resize(width, height);
    grid_view->setScene(new_grid_scene);
    grid_view->setSceneRect(0, 0, width, height);
    grid_view->update();
    //    grid_view->show();
    delete grid_scene;
    grid_scene = new_grid_scene;
    //初始化画板状态
    grid_scene->setGridStatus(IDLE);

    // 无人机位置，暂时使用
    int x_bias = num_of_row / 2;
    int y_bias = num_of_col / 2;
    QVector<QVector<int> > offset{ { 0, 0 }, { 1, 0 }, { 1, -1 }, { 0, -1 } };
    for (auto& p : offset) {
        grid_scene->addRect((x_bias + p[0]) * cell_size, (y_bias - p[1]) * cell_size,
                      cell_size, cell_size)
            ->setBrush(QBrush(Qt::blue));
    }
}

void MainWindow::switchDrawing()
{
    if (grid_scene == nullptr) {
        return;
    }
    if (grid_scene->getGridStarus() != DRAWING) {
        grid_scene->setGridStatus(DRAWING);
        ui->edit_draw_action->setText("停止绘制");
        setStatusText("绘制地图中...");
    } else {
        grid_scene->setGridStatus(IDLE);
        ui->edit_draw_action->setText("开始绘制");
        setStatusText("停止绘制");
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
    }
}

void MainWindow::on_start_toolButton_clicked()
{
    setStatusText("生成地图文件...");
    //    QVector<QVector<QPointF>> walls = grid_scene->getWalls();
    QVector<QPointF> walls = grid_scene->getWalls();
    int height = grid_scene->getGridHeightNumber();
    int width = grid_scene->getGridWidthNumber();
    QVector<QString> map(height + 2, QString(width + 2, '.'));
    //    for(int i=0; i<walls.size(); ++i)
    //    {
    //        for(int j=0; j<walls[i].size(); ++j)
    //        {
    //            map[int(walls[i][j].ry())+1][int(walls[i][j].rx())+1] = '#';
    //        }
    //    }
    for (int i = 0; i < walls.size(); ++i) {
        map[int(walls[i].ry()) + 1][int(walls[i].rx()) + 1] = '#';
    }

    QString filename = QFileDialog::getSaveFileName(
        this,
        tr("选择保存目录文件"),
        "/home/ln/map_world/map.txt",
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
    list[0] += ".world";
    mg.generateWorldFile(list[0].toStdString());
    setStatusText("生成world文件成功：" + list[0]);
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
}

void MainWindow::on_delete_toolButton_clicked()
{
    if (uav_num == 0)
        return;
    QListWidgetItem* item = ui->listWidget->item(uav_num - 1);
    ui->listWidget->removeItemWidget(item);
    delete item;
    item = nullptr;
    uav_items.remove(uav_num - 1);
    --uav_num;
}
