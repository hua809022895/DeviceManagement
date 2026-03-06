#pragma once

#include <QDialog>
#include "ui_dataBackDialog.h"

class mavlink5Thread;
class dataBackDlg : public QDialog
{
	Q_OBJECT
public:
	dataBackDlg(QWidget *parent = nullptr);
	~dataBackDlg();

	mavlink5Thread		*m_pMavThread = nullptr;
	QStandardItemModel	*m_pModel;
public slots:
	void on_setTitle();
	void on_addBtn_click();
	void on_deleteBtn_click();
	void on_queryIdBtn_click();
	void on_queryBtn_click();
	void on_exitBtn_click();
	void on_pauseBtn_click();
	void on_exportBtn_click();
protected:
	void showEvent(QShowEvent *e) override;
private:
	Ui::dataBackDlgClass ui;
};
