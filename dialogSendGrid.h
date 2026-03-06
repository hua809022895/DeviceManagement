#ifndef DIALOGSENDGRID_H
#define DIALOGSENDGRID_H

#include <QDialog>

namespace Ui {
class DialogSendGrid;
}

class DialogSendGrid : public QDialog
{
    Q_OBJECT
public:
    explicit DialogSendGrid( int* sendtype,QWidget *parent = nullptr );
    ~DialogSendGrid();
    int* mSendtype;         //发送类型，0给飞机重置，1仅发送给地面站
public slots:

    void setSendType();
private:
    Ui::DialogSendGrid *ui;
};

#endif // DIALOGSENDGRID_H
