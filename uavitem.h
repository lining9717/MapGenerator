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
    int getX() const;
    int getY() const;

signals:
    void send_uav_id(int id);

private slots:
    void on_edit_toolButton_clicked();

private:
    Ui::UavItem* ui;
    int m_id;
    int m_x;
    int m_y;
};

#endif // UAVITEM_H
