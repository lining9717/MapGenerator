#ifndef UAVITEM_H
#define UAVITEM_H

#include <QMessageBox>
#include <QString>
#include <QWidget>
#include <QDebug>

namespace Ui {
class UavItem;
}

class UavItem : public QWidget {
    Q_OBJECT

public:
    explicit UavItem(int uav_id, QWidget* parent = nullptr);
    ~UavItem();

    void setItem(int position_x, int position_y);

private slots:
    void on_edit_toolButton_clicked();

private:
    Ui::UavItem* ui;
    int m_id;
    int m_x;
    int m_y;
};

#endif // UAVITEM_H
