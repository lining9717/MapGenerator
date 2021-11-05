#ifndef UAVITEM_H
#define UAVITEM_H

#include <QMessageBox>
#include <QString>
#include <QWidget>
#include <QDebug>

const int NULL_POSITION = 0xfedc;

namespace Ui {
class UavItem;
}

class UavItem : public QWidget {
    Q_OBJECT

public:
    explicit UavItem(int uav_id, QWidget* parent = nullptr);
    ~UavItem();

    void setItem(int position_x, int position_y);
    void setPicDefault();
    void setPicSelecting();
    void setEditEnable(bool b);
    void setStateEnable(bool b);
    int getX() const;
    int getY() const;
    int getStatus() const;

signals:
    void send_uav_id(int id);
    void send_uav_box(int id, int state);

private slots:
    void on_edit_toolButton_clicked();

    void on_init_uav_checkBox_stateChanged(int arg1);

private:
    Ui::UavItem* ui;
    int m_id;
    int m_x;
    int m_y;
    int state;
};

#endif // UAVITEM_H
