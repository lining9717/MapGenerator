#include "uavitem.h"
#include "ui_uavitem.h"

UavItem::UavItem(int uav_id, QWidget* parent)
    : QWidget(parent)
    , ui(new Ui::UavItem)
{
    ui->setupUi(this);
    QString img_launch_file_path(":/new/image/img/uav.png");
    QImage* uav_img = new QImage();
    if (!(uav_img->load(img_launch_file_path))) { //加载图像
        QMessageBox::information(this,
            tr("打开图像失败"),
            tr("打开图像失败!"));
        delete uav_img;
        return;
    }
    uav_img->scaled(ui->uav_pic->size(), Qt::KeepAspectRatio);
    ui->uav_pic->setScaledContents(true);
    ui->uav_pic->setPixmap(QPixmap::fromImage(*uav_img));
    delete uav_img;

    ui->edit_toolButton->setIcon(QIcon(":/new/image/img/edit_uav.png"));
    ui->edit_toolButton->setStyleSheet("border:none");
    ui->uav_id_label->setText("UAV" + QString::number(uav_id));
    m_id = uav_id;
    setItem(0, 0);
}

UavItem::~UavItem()
{
    delete ui;
}

void UavItem::setItem(int position_x, int position_y)
{
    m_x = position_x;
    m_y = position_y;
    ui->uav_position_label->setText("[" + QString::number(m_x) + ", " + QString::number(m_y) + "]");
}

void UavItem::on_edit_toolButton_clicked()
{

}
