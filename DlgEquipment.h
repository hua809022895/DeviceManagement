#pragma once

#include <QDialog>
#include "ui_DlgEquipment.h"

class DlgEquipment : public QDialog
{
	Q_OBJECT

public:
	DlgEquipment(QWidget *parent = nullptr);
	~DlgEquipment();

	void refreshA();//重新刷新表格数据
	void refreshB();//重新刷新表格数据
public slots:
	void onItemClicked(QTableWidgetItem *item);
	void on_addBtn_click();
	void on_Midify_click();	
	void on_deleteBtn_click();
	void on_AllDeleteBtn_click();	
	void on_exitBtn_click();
private:
public:
	Ui::DlgEquipment	 ui;
};
