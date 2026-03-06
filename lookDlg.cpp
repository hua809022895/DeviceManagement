#include "stdafx.h"
#include "lookDialog.h"
#include "ui_lookdialog.h"
#include <QCheckBox>
#include <QComboBox>
#include <QLabel>

#include "mainWindow.h"
#include <QSpacerItem>
#include <QMessageBox>
#include <QFileDialog>

#include "comm.h"

#include <qgsMarkersymbollayer.h>
#include <qgsExpressioncontextutils.h>
#include <qgsExpressioncontext.h>
#include <qgsFilterlineedit.h>
#include <qgsHeatmaprenderer.h>
#include <qgsLinesymbollayer.h>
#include <qgsSinglesymbolrenderer.h>
#include <qgsSymbol.h>
#include "Xlsx-src/xlsx/xlsxdocument.h"       //
#include <qgsAnnotationLayer.h>
#include "qgsAnnotationMarkerItem2.h"

//定位
void LookDialog::locateCurrent()
{
	int rowindex = ui->mTableWidgetHistory->currentRow();
	if (rowindex > -1) {
		Mappoint* mappoint = (Mappoint*)(qlonglong)ui->mTableWidgetHistory->item(rowindex, 0)->data(Qt::UserRole).value<qlonglong>();
		QgsPointXY point(mappoint->x, mappoint->y);
		mCanvas->setCenter(point);
		mCanvas->refresh();
	}
}

//删除g_sqliteDbase数据库，单个记录按钮
void LookDialog::deleteHistory(int rowid)
{
	//int rowIndex = ui->mTableWidgetPicture->currentRow();
	//ui->mTableWidgetPicture->removeCellWidget(rowIndex, 4);
	//ui->mTableWidgetPicture->removeRow(rowIndex);
	int rowindex = ui->mTableWidgetHistory->currentRow();
	if (rowindex > -1) {
		Mappoint* mappoint = (Mappoint*)(qlonglong)ui->mTableWidgetHistory->item(rowindex, 0)->data(Qt::UserRole).value<qlonglong>();
		ui->mTableWidgetHistory->removeRow(rowindex);

		QString deletesql = "delete from objectImage where messageId = %1";
		deletesql = deletesql.arg(mappoint->messageId);
		QSqlQuery sql_query(g_sqliteDbase);
		sql_query.exec(deletesql);
		g_sqliteDbase.commit();
	}
}

//按条件查询
void LookDialog::contionQuery()
{
	while (ui->mTableWidgetHistory->rowCount() > 0)
	{
		ui->mTableWidgetHistory->removeRow(0);
	}

	QString sql = buildsql();
	QSqlQuery sql_query(g_sqliteDbase);
	sql_query.exec(sql);

	ui->mProgressBar->setVisible(true);
	showhistory(sql_query);
	ui->mProgressBar->setVisible(false);
}

//查询全部
void LookDialog::historyPicture()
{
	while (ui->mTableWidgetHistory->rowCount() > 0)
	{
		ui->mTableWidgetHistory->removeRow(0);
	}

	QSqlQuery query(g_sqliteDbase);
	QString sql = "select * from objectImage";
	query.exec(sql);
	showhistory(query);
}

//删除全部
void LookDialog::deleteAll()
{
	//createDatabase();
	QString deletesql = "delete from objectImage";
	QSqlQuery sql_query(g_sqliteDbase);
	sql_query.exec(deletesql);
	g_sqliteDbase.commit();

	QMessageBox msgBox;
	msgBox.setWindowTitle(QString::fromLocal8Bit("提示"));
	msgBox.setText(QString::fromLocal8Bit("        全部删除成功.       "));
	msgBox.exec();
}

//导出到excel表格中
void LookDialog::exportExcel()
{
	QString sql = buildsql();	//构造sql语句;
	if (ui->mTableWidgetHistory->rowCount() == 0)
	{
		QMessageBox msgBox;
		msgBox.setWindowTitle(QString::fromLocal8Bit("提示"));
		msgBox.setText(QString::fromLocal8Bit("           请先查询.         "));
		msgBox.exec();
		return;
	}

	QString fileName = QFileDialog::getSaveFileName(this, tr("Save File"), "~/", "xlsx (*.xlsx)");
	if (fileName == "") {
		return;
	}

	ui->mProgressBar->setVisible(true);

	QXlsx::Document xlsx;
	xlsx.write("A1", QString::fromLocal8Bit("序号"));
	xlsx.write("B1", QString::fromLocal8Bit("经度"));
	xlsx.write("C1", QString::fromLocal8Bit("纬度"));
	xlsx.write("D1", QString::fromLocal8Bit("时间"));
	xlsx.write("E1", QString::fromLocal8Bit("机器判定"));
	xlsx.write("F1", QString::fromLocal8Bit("任务"));
	xlsx.write("G1", QString::fromLocal8Bit("人工确定"));
	xlsx.write("H1", QString::fromLocal8Bit("图片"));

	xlsx.setColumnWidth(1, 10);
	xlsx.setColumnWidth(2, 12);
	xlsx.setColumnWidth(3, 12);
	xlsx.setColumnWidth(4, 18);
	xlsx.setColumnWidth(5, 15);
	xlsx.setColumnWidth(6, 15);
	xlsx.setColumnWidth(7, 15);
	xlsx.setColumnWidth(8, 0);

	QXlsx::Format header;
	header.setVerticalAlignment(QXlsx::Format::AlignVCenter);

	QSqlQuery query(g_sqliteDbase);
	query.exec(sql);
	int count = query.record().count();
	ui->mProgressBar->setMinimum(0);
	ui->mProgressBar->setMaximum(count + 10);
	

	int i = 2; //从第二行开始算
	Mappoint *pt = new Mappoint;

	while (query.next())
	{
		ui->mProgressBar->setValue(i);
		double dProgress = (ui->mProgressBar->value() - ui->mProgressBar->minimum()) * 100.0
			/ (ui->mProgressBar->maximum() - ui->mProgressBar->minimum());
		ui->mProgressBar->setFormat(QString::fromLocal8Bit("当前进度为：%1%").arg(QString::number(dProgress, 'f', 1)));


		QByteArray jpg = query.value("jpg").toByteArray();
		//pt->messageId	= query.value("messageId").toString();
		//pt->planeId	= query.value("planeId").toString();
		pt->taskname = query.value("taskname").toString();
		pt->weaponname = query.value("typename").toString();
		pt->ishanded = query.value("ishanded").toString();
		pt->x = query.value("planePositionX").toString().toDouble();
		pt->y = query.value("planePostionY").toString().toDouble();
		pt->planeTime = query.value("planeTime").toString();

		QImage image;
		image.loadFromData(jpg, "jpg");      // int w = image.width();
		int h = image.height();
		xlsx.setRowHeight(i, 30);  //设置每行表格的高度为h

		QString sRow = QString::number(i - 1);
		xlsx.write(i, 1, sRow, header);			//序号
		xlsx.write(i, 2, pt->x, header);		//经度
		xlsx.write(i, 3, pt->y, header);		//纬度
		xlsx.write(i, 4, pt->planeTime, header); //时间
		xlsx.write(i, 5, pt->weaponname, header);//武器类型
		xlsx.write(i, 6, pt->taskname, header);		//任务
		xlsx.write(i, 7, pt->ishanded, header);
		xlsx.insertImage(i-1,8, image);			//图片
		i++;
		Sleep(10);
	}

	delete pt;
	xlsx.saveAs(fileName);
	ui->mProgressBar->setValue(count + 10);
	ui->mProgressBar->setVisible(false);

	QMessageBox msgBox;
	msgBox.setWindowTitle(QString::fromLocal8Bit("提示"));
	msgBox.setText(QString::fromLocal8Bit("  导出完成了！ "));
	msgBox.exec();
}

//构造sql语句
QString LookDialog::buildsql()
{
	QString timesection = " and ( (messageId >= %1) and (messageId <= %2) )";
	QString postionsetion = " and ( (planePositionX >=%1) and (planePositionX <= %2) and (planePostionY >= %3) and (planePostionY <= %4) )";
	QString handlesection = " and (taskname =%1)  ";
	QString begintime = ui->mDateTimeEditbegin->dateTime().toString("yyyyMMddhhmmss");
	QString endtime = ui->mDateTimeEditEnd->dateTime().toString("yyyyMMddhhmmss");

	QString xmin = QString::number(mCurrentExtent.xMinimum(), 'f', 6);
	QString xmax = QString::number(mCurrentExtent.xMaximum(), 'f', 6);
	QString ymin = QString::number(mCurrentExtent.yMinimum(), 'f', 6);
	QString ymax = QString::number(mCurrentExtent.yMaximum(), 'f', 6);
	QString weapon = " and ( typename = %1 )";

	QString s = ui->mComboBoxWeapon->currentText();
	if (s == QString::fromLocal8Bit("全部")) {
		weapon = " and ( 1 = 1)";
	}
	else {
		weapon = weapon.arg(QString("'") + ui->mComboBoxWeapon->currentText() + QString("'"));
	}

	if (ui->mComboBoxHandletype->currentText() == QString::fromLocal8Bit("全部")) {
		handlesection = " and ( 1 = 1)";
	}
	else if (ui->mComboBoxHandletype->currentText() == QString::fromLocal8Bit("已放弃"))
	{
		handlesection = handlesection.arg(QString("'") + "放弃" + QString("'"));
	}
	else if (ui->mComboBoxHandletype->currentText() == QString::fromLocal8Bit("未处理")) {
		handlesection = handlesection.arg("''");
	}
	else if (ui->mComboBoxHandletype->currentText() == QString::fromLocal8Bit("已人工确认")) {
		handlesection = handlesection.arg(QString("'") + "确认" + QString("'"));
	}
	else if (ui->mComboBoxHandletype->currentText() == QString::fromLocal8Bit("已监视")) {
		handlesection = handlesection.arg(QString("'") + "监视" + QString("'"));
	}
	else if (ui->mComboBoxHandletype->currentText() == QString::fromLocal8Bit("已打击")) {
		handlesection = handlesection.arg(QString("'") + "打击" + QString("'"));
	}

	if (ui->mLineEditExtend->text() == "") {
		postionsetion = " and ( 1 = 1)";
	}
	else {
		postionsetion = postionsetion.arg(xmin).arg(xmax).arg(ymin).arg(ymax);
	}

	timesection = timesection.arg(begintime).arg(endtime);
	QString sql = "select * from objectImage where (1 = 1)" + postionsetion + timesection + weapon + handlesection;
	sql += " order by messageId";
	return  sql;
}
//根据记录集，向表格中添加记录
void LookDialog::showhistory(QSqlQuery &query)
{
	int id = 0;
	while (query.next())
	{
		int row = ui->mTableWidgetHistory->rowCount();
		Mappoint *pUser = new Mappoint;
		QByteArray jpg = query.value("jpg").toByteArray();
		pUser->messageId = query.value("messageId").toString();
		//pUser->rowid = row.toString();		//序号
		pUser->planeId = query.value("planeId").toString();		//
		pUser->taskname = query.value("taskName").toString();
		pUser->weaponname = query.value("typeName").toString();
		pUser->x = query.value("planePositionX").toString().toDouble();
		pUser->y = query.value("planePostionY").toString().toDouble();
		pUser->planeTime = query.value("planetime").toString();
		pUser->xmin = query.value("minJpgX").toString().toDouble();
		pUser->ymin = query.value("minJpgX").toString().toDouble();
		pUser->xmax = query.value("maxJpgX").toString().toDouble();
		pUser->ymax = query.value("maxJpgY").toString().toDouble();
		pUser->angleX = query.value("angleJpgX").toString().toDouble();
		pUser->angleY = query.value("angleJpgY").toString().toDouble();
		pUser->ishanded = query.value("ishanded").toString();

		QPixmap pixmap;
		pixmap.loadFromData(jpg, "jpg");

		int w = pixmap.width();
		int h = pixmap.height();

		ui->mTableWidgetHistory->insertRow(row);

		ui->mTableWidgetHistory->setItem(row, 0, new QTableWidgetItem(QString("%1").arg(row + 1)));//序号
		ui->mTableWidgetHistory->item(row, 0)->setTextAlignment(Qt::AlignTop);

		ui->mTableWidgetHistory->setRowHeight(row, h+10);  //设置行高度
		//ui->mTableWidgetHistory->setColumnWidth(1, w );

		QLabel *label = new QLabel("");
		label->setPixmap(pixmap);
		pUser->picture = label;
		ui->mTableWidgetHistory->setCellWidget(row, 1, label);		//图片

		//经纬度
		ui->mTableWidgetHistory->setItem(row, 2, new QTableWidgetItem(QString("%1,\n%2").arg(QString::number(pUser->x, 'f', 5)).arg(QString::number(pUser->y, 'f', 5))));
		ui->mTableWidgetHistory->item(row, 2)->setTextAlignment(Qt::AlignTop);
		ui->mTableWidgetHistory->setItem(row, 3, new QTableWidgetItem(pUser->planeTime.mid(0, 8) + " " + pUser->planeTime.mid(8)));//时间
		ui->mTableWidgetHistory->item(row, 3)->setTextAlignment(Qt::AlignTop);

		//以下是机器判断，任务，人工
		qlonglong vaiid = reinterpret_cast<qlonglong>(pUser);
		ui->mTableWidgetHistory->itemAt(row, 0)->setData(Qt::UserRole, QVariant(vaiid));

		QStringList typelist = WeaponTypeList.split("|");
		QStringList tasklist = g_taskList.split("|");

		QComboBox *typebox = new QComboBox;
		typebox->setEditable(true);
		typebox->addItems(typelist);
		typebox->setCurrentText(pUser->weaponname);

		QComboBox *taskbox = new QComboBox;
		taskbox->setEditable(true);
		taskbox->addItems(tasklist);
		taskbox->setCurrentText(pUser->taskname);

		QComboBox *okbox = new QComboBox;
		okbox->setEditable(true);
		okbox->addItems(typelist);
		okbox->setCurrentText(pUser->ishanded);

		QWidget *widget3 = new QWidget();
		QVBoxLayout *vboxlayout3 = new QVBoxLayout;
		vboxlayout3->setSpacing(0);
		vboxlayout3->minimumHeightForWidth(1);

		widget3->setLayout(vboxlayout3);
		vboxlayout3->addWidget(typebox);
		vboxlayout3->addStretch();

		QWidget *widget4 = new QWidget();
		QVBoxLayout *vboxlayout4 = new QVBoxLayout;
		vboxlayout4->setSpacing(0);
		widget4->setLayout(vboxlayout4);
		vboxlayout4->addWidget(taskbox);
		vboxlayout4->addStretch();

		QWidget *widget5 = new QWidget();
		QVBoxLayout *vboxlayout5 = new QVBoxLayout;
		vboxlayout5->setSpacing(0);
		widget5->setLayout(vboxlayout5);
		vboxlayout5->addWidget(okbox);
		vboxlayout5->addStretch();

		ui->mTableWidgetHistory->setCellWidget(row, 4, widget3);
		ui->mTableWidgetHistory->setCellWidget(row, 5, widget4);
		ui->mTableWidgetHistory->setCellWidget(row, 6, widget5);

		ui->mProgressBar->setValue(row %100);
	}
}
