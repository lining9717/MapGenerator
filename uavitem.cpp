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

    setPicDefault();
    ui->edit_toolButton->setStyleSheet("border:none");
    ui->uav_id_label->setText("UAV" + QString::number(uav_id));
    m_id = uav_id;
    state = Qt::Unchecked;
    setItem(NULL_POSITION, NULL_POSITION);
}

UavItem::~UavItem()
{
    delete ui;
}

void UavItem::setEditEnable(bool b)
{
    ui->edit_toolButton->setEnabled(b);
}

void UavItem::setStateEnable(bool b)
{
    ui->init_uav_checkBox->setEnabled(b);
}

void UavItem::setItem(int position_x, int position_y)
{
    m_x = position_x;
    m_y = position_y;
    if (m_x == NULL_POSITION or m_x == NULL_POSITION)
        ui->uav_position_label->setText("未选择位置");
    else
        ui->uav_position_label->setText("[" + QString::number(m_x) + ", " + QString::number(m_y) + "]");
}

void UavItem::setPicDefault()
{
    ui->edit_toolButton->setIcon(QIcon(":/new/image/img/edit_uav.png"));
}

void UavItem::setPicSelecting()
{
    ui->edit_toolButton->setIcon(QIcon(":/new/image/img/editing_uav.png"));
}

void UavItem::on_edit_toolButton_clicked()
{
    emit send_uav_id(m_id);
}

int UavItem::getX() const
{
    return m_x;
}

int UavItem::getY() const
{
    return m_y;
}

int UavItem::getStatus() const
{
    return state;
}

void UavItem::on_init_uav_checkBox_stateChanged(int arg1)
{
    state = arg1;
    emit send_uav_box(m_id, arg1);
}
