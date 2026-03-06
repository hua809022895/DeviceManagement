#pragma once

#include <QDialog>
#include "ui_DlgAddEquipment.h"

class DlgAddEquipment : public QDialog
{
	Q_OBJECT

public:
	DlgAddEquipment(QWidget *parent = nullptr);
	~DlgAddEquipment();

	void insert0();
	QString m_strFile;
public slots:
	void on_okBtn_click();
	void on_addImageBtn_click();
private:
public:
	Ui::DlgAddEquipment	 ui;
};
