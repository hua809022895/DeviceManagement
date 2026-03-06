#ifndef DDSTHREAD_H
#define DDSTHREAD_H

#include <QThread>
#include "comm.h"
//#include "./mainwindow.h"
#include <QDateTime>
#include <qdebug.h>

class DDSThreadHandler:public QThread
{
    Q_OBJECT
public:
	DDSThreadHandler();
	//virtual ~DDSThreadHandler();    
signals:
    void receviePictureMessage(double leftupY, double leftupX,
                                   double rightupY, double rightupX,
                                   double leftdownY, double leftdownX,
                                   double rightdownY, double rightdownX, QString filename);
    void recevieLittlePictureMessage(const QByteArray);
public:
	void run();
};

#endif // DDSTHREAD_H




