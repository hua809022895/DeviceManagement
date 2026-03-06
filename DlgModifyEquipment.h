#pragma once

#include <QDialog>
#include "ui_DlgModifyEquipment.h"

class DlgModifyEquipment : public QDialog
{
	Q_OBJECT

public:
	DlgModifyEquipment(int id,bool bDuo,QWidget *parent = nullptr);
	~DlgModifyEquipment();

	bool	m_bDuo = true;//是否是多旋翼，true表示多旋翼，false表示固定翼
	int		m_id=-1;	//表记录id
	QString m_strFile;

	QByteArray m_jpg;	//图片二进制数据
	void on_Bclick();//保存固定翼，b表
public slots:
	void on_okBtn_click();  //保存多旋翼，A表
	void on_addImageBtn_click();
private:
public:
	Ui::DlgModifyEquipment	 ui;
};
