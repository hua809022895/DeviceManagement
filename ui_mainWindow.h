/********************************************************************************
** Form generated from reading UI file 'mainWindow.ui'
**
** Created by: Qt User Interface Compiler version 5.12.4
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MAINWINDOW_H
#define UI_MAINWINDOW_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_MainWindow
{
public:
    QAction *mActionLoadShp;
    QAction *mActionLoadRaster;
    QAction *mActionDrawPoint;
    QAction *mActionDrawLine;
    QAction *mActionDrawPolygon;
    QAction *mActionSaveLayer;
    QAction *mActionMoveView;
    QAction *mActionSelectElementDrawed;
    QAction *mActionVertext;
    QAction *mActionToggleEditing;
    QAction *mActionDeleteSelected;
    QAction *mActionMoveFeature;
    QAction *mActionSetFeature;
    QAction *mActionEditingLine;
    QAction *mActionEditStyle;
    QAction *mActionEditPoint;
    QAction *mActionAirCount;
    QAction *mActionAbundance;
    QAction *mActionGlobalPlan;
    QAction *mActionHistoryTask;
    QAction *mActionInitalAbundance;
    QAction *mActionSelectPoint;
    QAction *mActionSelectLine;
    QAction *mActionSelectPolygon;
    QAction *mActionSetLayers;
    QAction *mActionCreateGrid;
    QAction *mActionLook;
    QAction *mActionCircle2point;
    QAction *mActionMeaureLength1;
    QAction *mActionMeasureArea;
    QAction *mActiontesttest;
    QAction *mActionSelectPicture;
    QAction *mActionHidePictureTable;
    QAction *mActionTaskSelect;
    QAction *mActionHideTarge;
    QAction *mActionHideBigPicture;
    QAction *mActionMeasureArea1;
    QAction *mActionCircle;
    QAction *mActionHideSmallJpg;
    QAction *mActionMiddle;
    QAction *mActionDTback;
    QAction *mActionHideTable;
    QAction *mActionStartStop;
    QAction *mActionRefreshGJ;
    QAction *actionRadarGo;
    QAction *mActionSelectRadarLayer;
    QAction *mActionSelectRadarDev;
    QAction *mActionMoveRadarDev;
    QAction *mActionDelRadarDev;
    QAction *mActionFixRadar;
    QAction *mActionOutfitMove;
    QAction *mActionAddTaskRect;
    QAction *mActionRadarTestAirList;
    QAction *mActionSelTaskArea;
    QAction *mActionFixTaskArea;
    QAction *mActionMoveTaskArea;
    QAction *mActionDelTaskArea;
    QAction *mActionSelTaskLayer;
    QAction *mActionRadarNo;
    QAction *mActionRadarOverList;
    QAction *mActionConnect;
    QAction *mActionEquipment;
    QAction *mActionTaskGuihua;
    QAction *mActionTuili;
    QAction *mAction3D;
    QWidget *centralwidget;
    QTableWidget *mTableWidgetMarkAttribute;
    QTableWidget *mPlaneWidget;
    QTableWidget *mTableWidget;
    QMenuBar *menubar;
    QMenu *menu;
    QMenu *menu_2;
    QMenu *menu_3;
    QStatusBar *mStatusbar;
    QToolBar *mToolBarDraw;
    QToolBar *mToolBarSetting;
    QToolBar *mToolBarSchedule;
    QToolBar *toolBar;
    QToolBar *toolBar_2;
    QToolBar *toolBar_3;
    QToolBar *toolBar_4;

    void setupUi(QMainWindow *MainWindow)
    {
        if (MainWindow->objectName().isEmpty())
            MainWindow->setObjectName(QString::fromUtf8("MainWindow"));
        MainWindow->resize(1336, 852);
        mActionLoadShp = new QAction(MainWindow);
        mActionLoadShp->setObjectName(QString::fromUtf8("mActionLoadShp"));
        mActionLoadRaster = new QAction(MainWindow);
        mActionLoadRaster->setObjectName(QString::fromUtf8("mActionLoadRaster"));
        mActionDrawPoint = new QAction(MainWindow);
        mActionDrawPoint->setObjectName(QString::fromUtf8("mActionDrawPoint"));
        mActionDrawLine = new QAction(MainWindow);
        mActionDrawLine->setObjectName(QString::fromUtf8("mActionDrawLine"));
        mActionDrawPolygon = new QAction(MainWindow);
        mActionDrawPolygon->setObjectName(QString::fromUtf8("mActionDrawPolygon"));
        mActionSaveLayer = new QAction(MainWindow);
        mActionSaveLayer->setObjectName(QString::fromUtf8("mActionSaveLayer"));
        mActionMoveView = new QAction(MainWindow);
        mActionMoveView->setObjectName(QString::fromUtf8("mActionMoveView"));
        mActionSelectElementDrawed = new QAction(MainWindow);
        mActionSelectElementDrawed->setObjectName(QString::fromUtf8("mActionSelectElementDrawed"));
        mActionVertext = new QAction(MainWindow);
        mActionVertext->setObjectName(QString::fromUtf8("mActionVertext"));
        mActionToggleEditing = new QAction(MainWindow);
        mActionToggleEditing->setObjectName(QString::fromUtf8("mActionToggleEditing"));
        mActionDeleteSelected = new QAction(MainWindow);
        mActionDeleteSelected->setObjectName(QString::fromUtf8("mActionDeleteSelected"));
        mActionMoveFeature = new QAction(MainWindow);
        mActionMoveFeature->setObjectName(QString::fromUtf8("mActionMoveFeature"));
        mActionSetFeature = new QAction(MainWindow);
        mActionSetFeature->setObjectName(QString::fromUtf8("mActionSetFeature"));
        mActionEditingLine = new QAction(MainWindow);
        mActionEditingLine->setObjectName(QString::fromUtf8("mActionEditingLine"));
        mActionEditingLine->setEnabled(true);
        mActionEditStyle = new QAction(MainWindow);
        mActionEditStyle->setObjectName(QString::fromUtf8("mActionEditStyle"));
        mActionEditPoint = new QAction(MainWindow);
        mActionEditPoint->setObjectName(QString::fromUtf8("mActionEditPoint"));
        mActionAirCount = new QAction(MainWindow);
        mActionAirCount->setObjectName(QString::fromUtf8("mActionAirCount"));
        mActionAbundance = new QAction(MainWindow);
        mActionAbundance->setObjectName(QString::fromUtf8("mActionAbundance"));
        mActionGlobalPlan = new QAction(MainWindow);
        mActionGlobalPlan->setObjectName(QString::fromUtf8("mActionGlobalPlan"));
        mActionHistoryTask = new QAction(MainWindow);
        mActionHistoryTask->setObjectName(QString::fromUtf8("mActionHistoryTask"));
        mActionInitalAbundance = new QAction(MainWindow);
        mActionInitalAbundance->setObjectName(QString::fromUtf8("mActionInitalAbundance"));
        mActionSelectPoint = new QAction(MainWindow);
        mActionSelectPoint->setObjectName(QString::fromUtf8("mActionSelectPoint"));
        mActionSelectLine = new QAction(MainWindow);
        mActionSelectLine->setObjectName(QString::fromUtf8("mActionSelectLine"));
        mActionSelectPolygon = new QAction(MainWindow);
        mActionSelectPolygon->setObjectName(QString::fromUtf8("mActionSelectPolygon"));
        mActionSetLayers = new QAction(MainWindow);
        mActionSetLayers->setObjectName(QString::fromUtf8("mActionSetLayers"));
        mActionCreateGrid = new QAction(MainWindow);
        mActionCreateGrid->setObjectName(QString::fromUtf8("mActionCreateGrid"));
        mActionLook = new QAction(MainWindow);
        mActionLook->setObjectName(QString::fromUtf8("mActionLook"));
        mActionCircle2point = new QAction(MainWindow);
        mActionCircle2point->setObjectName(QString::fromUtf8("mActionCircle2point"));
        mActionMeaureLength1 = new QAction(MainWindow);
        mActionMeaureLength1->setObjectName(QString::fromUtf8("mActionMeaureLength1"));
        mActionMeasureArea = new QAction(MainWindow);
        mActionMeasureArea->setObjectName(QString::fromUtf8("mActionMeasureArea"));
        mActionMeasureArea->setCheckable(true);
        mActiontesttest = new QAction(MainWindow);
        mActiontesttest->setObjectName(QString::fromUtf8("mActiontesttest"));
        mActionSelectPicture = new QAction(MainWindow);
        mActionSelectPicture->setObjectName(QString::fromUtf8("mActionSelectPicture"));
        mActionHidePictureTable = new QAction(MainWindow);
        mActionHidePictureTable->setObjectName(QString::fromUtf8("mActionHidePictureTable"));
        mActionTaskSelect = new QAction(MainWindow);
        mActionTaskSelect->setObjectName(QString::fromUtf8("mActionTaskSelect"));
        mActionHideTarge = new QAction(MainWindow);
        mActionHideTarge->setObjectName(QString::fromUtf8("mActionHideTarge"));
        mActionHideBigPicture = new QAction(MainWindow);
        mActionHideBigPicture->setObjectName(QString::fromUtf8("mActionHideBigPicture"));
        mActionMeasureArea1 = new QAction(MainWindow);
        mActionMeasureArea1->setObjectName(QString::fromUtf8("mActionMeasureArea1"));
        mActionCircle = new QAction(MainWindow);
        mActionCircle->setObjectName(QString::fromUtf8("mActionCircle"));
        mActionHideSmallJpg = new QAction(MainWindow);
        mActionHideSmallJpg->setObjectName(QString::fromUtf8("mActionHideSmallJpg"));
        mActionMiddle = new QAction(MainWindow);
        mActionMiddle->setObjectName(QString::fromUtf8("mActionMiddle"));
        mActionDTback = new QAction(MainWindow);
        mActionDTback->setObjectName(QString::fromUtf8("mActionDTback"));
        mActionHideTable = new QAction(MainWindow);
        mActionHideTable->setObjectName(QString::fromUtf8("mActionHideTable"));
        mActionStartStop = new QAction(MainWindow);
        mActionStartStop->setObjectName(QString::fromUtf8("mActionStartStop"));
        mActionRefreshGJ = new QAction(MainWindow);
        mActionRefreshGJ->setObjectName(QString::fromUtf8("mActionRefreshGJ"));
        actionRadarGo = new QAction(MainWindow);
        actionRadarGo->setObjectName(QString::fromUtf8("actionRadarGo"));
        actionRadarGo->setCheckable(true);
        mActionSelectRadarLayer = new QAction(MainWindow);
        mActionSelectRadarLayer->setObjectName(QString::fromUtf8("mActionSelectRadarLayer"));
        mActionSelectRadarDev = new QAction(MainWindow);
        mActionSelectRadarDev->setObjectName(QString::fromUtf8("mActionSelectRadarDev"));
        mActionMoveRadarDev = new QAction(MainWindow);
        mActionMoveRadarDev->setObjectName(QString::fromUtf8("mActionMoveRadarDev"));
        mActionMoveRadarDev->setCheckable(true);
        mActionMoveRadarDev->setChecked(false);
        mActionDelRadarDev = new QAction(MainWindow);
        mActionDelRadarDev->setObjectName(QString::fromUtf8("mActionDelRadarDev"));
        mActionFixRadar = new QAction(MainWindow);
        mActionFixRadar->setObjectName(QString::fromUtf8("mActionFixRadar"));
        mActionOutfitMove = new QAction(MainWindow);
        mActionOutfitMove->setObjectName(QString::fromUtf8("mActionOutfitMove"));
        mActionAddTaskRect = new QAction(MainWindow);
        mActionAddTaskRect->setObjectName(QString::fromUtf8("mActionAddTaskRect"));
        mActionRadarTestAirList = new QAction(MainWindow);
        mActionRadarTestAirList->setObjectName(QString::fromUtf8("mActionRadarTestAirList"));
        mActionSelTaskArea = new QAction(MainWindow);
        mActionSelTaskArea->setObjectName(QString::fromUtf8("mActionSelTaskArea"));
        mActionFixTaskArea = new QAction(MainWindow);
        mActionFixTaskArea->setObjectName(QString::fromUtf8("mActionFixTaskArea"));
        mActionMoveTaskArea = new QAction(MainWindow);
        mActionMoveTaskArea->setObjectName(QString::fromUtf8("mActionMoveTaskArea"));
        mActionMoveTaskArea->setCheckable(true);
        mActionDelTaskArea = new QAction(MainWindow);
        mActionDelTaskArea->setObjectName(QString::fromUtf8("mActionDelTaskArea"));
        mActionSelTaskLayer = new QAction(MainWindow);
        mActionSelTaskLayer->setObjectName(QString::fromUtf8("mActionSelTaskLayer"));
        mActionRadarNo = new QAction(MainWindow);
        mActionRadarNo->setObjectName(QString::fromUtf8("mActionRadarNo"));
        mActionRadarOverList = new QAction(MainWindow);
        mActionRadarOverList->setObjectName(QString::fromUtf8("mActionRadarOverList"));
        mActionConnect = new QAction(MainWindow);
        mActionConnect->setObjectName(QString::fromUtf8("mActionConnect"));
        mActionEquipment = new QAction(MainWindow);
        mActionEquipment->setObjectName(QString::fromUtf8("mActionEquipment"));
        mActionTaskGuihua = new QAction(MainWindow);
        mActionTaskGuihua->setObjectName(QString::fromUtf8("mActionTaskGuihua"));
        mActionTuili = new QAction(MainWindow);
        mActionTuili->setObjectName(QString::fromUtf8("mActionTuili"));
        mAction3D = new QAction(MainWindow);
        mAction3D->setObjectName(QString::fromUtf8("mAction3D"));
        centralwidget = new QWidget(MainWindow);
        centralwidget->setObjectName(QString::fromUtf8("centralwidget"));
        mTableWidgetMarkAttribute = new QTableWidget(centralwidget);
        mTableWidgetMarkAttribute->setObjectName(QString::fromUtf8("mTableWidgetMarkAttribute"));
        mTableWidgetMarkAttribute->setGeometry(QRect(10, 0, 161, 491));
        mPlaneWidget = new QTableWidget(centralwidget);
        mPlaneWidget->setObjectName(QString::fromUtf8("mPlaneWidget"));
        mPlaneWidget->setGeometry(QRect(0, 690, 1181, 51));
        mTableWidget = new QTableWidget(centralwidget);
        mTableWidget->setObjectName(QString::fromUtf8("mTableWidget"));
        mTableWidget->setGeometry(QRect(10, 495, 161, 191));
        mTableWidget->setVisible(false);
        MainWindow->setCentralWidget(centralwidget);
        menubar = new QMenuBar(MainWindow);
        menubar->setObjectName(QString::fromUtf8("menubar"));
        menubar->setGeometry(QRect(0, 0, 1336, 23));
        menu = new QMenu(menubar);
        menu->setObjectName(QString::fromUtf8("menu"));
        menu_2 = new QMenu(menubar);
        menu_2->setObjectName(QString::fromUtf8("menu_2"));
        menu_3 = new QMenu(menubar);
        menu_3->setObjectName(QString::fromUtf8("menu_3"));
        MainWindow->setMenuBar(menubar);
        mStatusbar = new QStatusBar(MainWindow);
        mStatusbar->setObjectName(QString::fromUtf8("mStatusbar"));
        MainWindow->setStatusBar(mStatusbar);
        mToolBarDraw = new QToolBar(MainWindow);
        mToolBarDraw->setObjectName(QString::fromUtf8("mToolBarDraw"));
        mToolBarDraw->setAllowedAreas(Qt::TopToolBarArea);
        mToolBarDraw->setIconSize(QSize(24, 24));
        MainWindow->addToolBar(Qt::TopToolBarArea, mToolBarDraw);
        mToolBarSetting = new QToolBar(MainWindow);
        mToolBarSetting->setObjectName(QString::fromUtf8("mToolBarSetting"));
        mToolBarSetting->setAllowedAreas(Qt::TopToolBarArea);
        MainWindow->addToolBar(Qt::TopToolBarArea, mToolBarSetting);
        mToolBarSchedule = new QToolBar(MainWindow);
        mToolBarSchedule->setObjectName(QString::fromUtf8("mToolBarSchedule"));
        mToolBarSchedule->setMovable(true);
        mToolBarSchedule->setAllowedAreas(Qt::TopToolBarArea);
        mToolBarSchedule->setFloatable(false);
        MainWindow->addToolBar(Qt::TopToolBarArea, mToolBarSchedule);
        toolBar = new QToolBar(MainWindow);
        toolBar->setObjectName(QString::fromUtf8("toolBar"));
        toolBar->setAllowedAreas(Qt::TopToolBarArea);
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar);
        MainWindow->insertToolBarBreak(toolBar);
        toolBar_2 = new QToolBar(MainWindow);
        toolBar_2->setObjectName(QString::fromUtf8("toolBar_2"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar_2);
        MainWindow->insertToolBarBreak(toolBar_2);
        toolBar_3 = new QToolBar(MainWindow);
        toolBar_3->setObjectName(QString::fromUtf8("toolBar_3"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar_3);
        toolBar_4 = new QToolBar(MainWindow);
        toolBar_4->setObjectName(QString::fromUtf8("toolBar_4"));
        MainWindow->addToolBar(Qt::TopToolBarArea, toolBar_4);
        MainWindow->insertToolBarBreak(toolBar_4);

        menubar->addAction(menu->menuAction());
        menubar->addAction(menu_2->menuAction());
        menubar->addAction(menu_3->menuAction());
        menu->addAction(mActionLoadShp);
        menu->addAction(mActionSetLayers);
        menu->addAction(mActionStartStop);
        menu->addAction(mActionRefreshGJ);
        menu_2->addAction(mActionMoveView);
        menu_2->addAction(mActionMeaureLength1);
        menu_2->addAction(mActionMeasureArea1);
        menu_2->addAction(mActionDTback);
        menu_2->addAction(mAction3D);
        menu_3->addAction(mActionAirCount);
        menu_3->addAction(mActionLook);
        menu_3->addAction(mActionTaskSelect);
        menu_3->addAction(mActionMiddle);
        mToolBarDraw->addAction(mActionMoveView);
        mToolBarDraw->addAction(mActionMeaureLength1);
        mToolBarDraw->addAction(mActionMeasureArea1);
        mToolBarDraw->addAction(mActionDTback);
        mToolBarDraw->addAction(mAction3D);
        mToolBarSetting->addAction(mActionConnect);
        mToolBarSetting->addAction(mActionEquipment);
        mToolBarSetting->addAction(mActionLook);
        mToolBarSchedule->addAction(mActionAirCount);
        mToolBarSchedule->addAction(mActionTaskSelect);
        mToolBarSchedule->addAction(mActionTuili);
        mToolBarSchedule->addSeparator();
        toolBar_3->addAction(actionRadarGo);
        toolBar_3->addAction(mActionSelectRadarLayer);
        toolBar_3->addAction(mActionMoveRadarDev);
        toolBar_3->addAction(mActionDelRadarDev);
        toolBar_3->addAction(mActionFixRadar);
        toolBar_3->addAction(mActionOutfitMove);
        toolBar_3->addAction(mActionRadarTestAirList);
        toolBar_4->addAction(mActionSelTaskLayer);
        toolBar_4->addAction(mActionSelTaskArea);
        toolBar_4->addAction(mActionFixTaskArea);
        toolBar_4->addAction(mActionMoveTaskArea);
        toolBar_4->addAction(mActionDelTaskArea);
        toolBar_4->addAction(mActionRadarNo);
        toolBar_4->addAction(mActionRadarOverList);
        toolBar_4->addAction(mActionTaskGuihua);

        retranslateUi(MainWindow);

        QMetaObject::connectSlotsByName(MainWindow);
    } // setupUi

    void retranslateUi(QMainWindow *MainWindow)
    {
        MainWindow->setWindowTitle(QApplication::translate("MainWindow", "\346\227\217\347\276\244\344\273\273\345\212\241\346\214\207\346\216\247\347\263\273\347\273\237", nullptr));
        mActionLoadShp->setText(QApplication::translate("MainWindow", "\345\212\240\350\275\275Shp", nullptr));
        mActionLoadRaster->setText(QApplication::translate("MainWindow", "\345\212\240\350\275\275Raster", nullptr));
        mActionDrawPoint->setText(QApplication::translate("MainWindow", "\346\267\273\345\212\240\344\277\256\346\224\271\347\233\256\346\240\207\347\202\271", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionDrawPoint->setToolTip(QApplication::translate("MainWindow", "\346\267\273\345\212\240\346\210\226\344\277\256\346\224\271\347\233\256\346\240\207\347\202\271", nullptr));
#endif // QT_NO_TOOLTIP
        mActionDrawLine->setText(QApplication::translate("MainWindow", "\346\267\273\345\212\240\347\272\277\346\240\207\346\263\250", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionDrawLine->setToolTip(QApplication::translate("MainWindow", "\347\272\277\346\240\207\346\263\250", nullptr));
#endif // QT_NO_TOOLTIP
        mActionDrawPolygon->setText(QApplication::translate("MainWindow", "\346\267\273\345\212\240\351\235\242\346\240\207\346\263\250", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionDrawPolygon->setToolTip(QApplication::translate("MainWindow", "\351\235\242\346\240\207\346\263\250", nullptr));
#endif // QT_NO_TOOLTIP
        mActionSaveLayer->setText(QApplication::translate("MainWindow", "\344\277\235\345\255\230", nullptr));
        mActionMoveView->setText(QApplication::translate("MainWindow", "\345\271\263\347\247\273\350\247\206\347\202\271", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionMoveView->setToolTip(QApplication::translate("MainWindow", "\345\271\263\347\247\273\350\247\206\347\202\271", nullptr));
#endif // QT_NO_TOOLTIP
        mActionSelectElementDrawed->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\347\233\256\346\240\207", nullptr));
        mActionVertext->setText(QApplication::translate("MainWindow", "\347\274\226\350\276\221\351\235\242", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionVertext->setToolTip(QApplication::translate("MainWindow", "\347\274\226\350\276\221\351\235\242\346\240\207\346\263\250", nullptr));
#endif // QT_NO_TOOLTIP
        mActionToggleEditing->setText(QApplication::translate("MainWindow", "\345\274\200\345\247\213\347\274\226\350\276\221", nullptr));
        mActionDeleteSelected->setText(QApplication::translate("MainWindow", "\345\210\240\351\231\244\346\240\207\346\263\250", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionDeleteSelected->setToolTip(QApplication::translate("MainWindow", "\345\210\240\351\231\244\346\240\207\346\263\250", nullptr));
#endif // QT_NO_TOOLTIP
        mActionMoveFeature->setText(QApplication::translate("MainWindow", "\347\247\273\345\212\250\346\240\207\346\263\250", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionMoveFeature->setToolTip(QApplication::translate("MainWindow", "\347\247\273\345\212\250\346\240\207\346\263\250", nullptr));
#endif // QT_NO_TOOLTIP
        mActionSetFeature->setText(QApplication::translate("MainWindow", "\350\256\276\347\275\256\346\240\207\346\263\250", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionSetFeature->setToolTip(QApplication::translate("MainWindow", "\350\256\276\347\275\256\346\240\207\346\263\250", nullptr));
#endif // QT_NO_TOOLTIP
        mActionEditingLine->setText(QApplication::translate("MainWindow", "\347\274\226\350\276\221\347\272\277", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionEditingLine->setToolTip(QApplication::translate("MainWindow", "\347\274\226\350\276\221\347\272\277\346\240\207\346\263\250", nullptr));
#endif // QT_NO_TOOLTIP
        mActionEditStyle->setText(QApplication::translate("MainWindow", "\350\256\276\347\275\256\351\243\216\346\240\274", nullptr));
        mActionEditPoint->setText(QApplication::translate("MainWindow", "\347\274\226\350\276\221\347\202\271", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionEditPoint->setToolTip(QApplication::translate("MainWindow", "\347\274\226\350\276\221\347\202\271\346\240\207\346\263\250", nullptr));
#endif // QT_NO_TOOLTIP
        mActionAirCount->setText(QApplication::translate("MainWindow", "\350\216\267\345\217\226\351\243\236\346\234\272\344\277\241\346\201\257", nullptr));
        mActionAbundance->setText(QApplication::translate("MainWindow", "\345\217\221\351\200\201\346\203\205\346\212\245\344\270\260\345\272\246", nullptr));
        mActionGlobalPlan->setText(QApplication::translate("MainWindow", "\345\205\250\345\261\200\350\247\204\345\210\222", nullptr));
        mActionHistoryTask->setText(QApplication::translate("MainWindow", "\346\211\223\345\274\200\345\267\262\345\217\221\351\200\201\344\273\273\345\212\241", nullptr));
        mActionInitalAbundance->setText(QApplication::translate("MainWindow", "\346\210\230\345\234\272\350\256\276\347\275\256", nullptr));
        mActionSelectPoint->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\347\233\256\346\240\207\347\202\271", nullptr));
        mActionSelectLine->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\347\272\277", nullptr));
        mActionSelectPolygon->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\351\235\242", nullptr));
        mActionSetLayers->setText(QApplication::translate("MainWindow", "\350\256\276\347\275\256\345\233\276\345\261\202", nullptr));
        mActionCreateGrid->setText(QApplication::translate("MainWindow", "\347\275\221\346\240\274\345\214\226", nullptr));
        mActionLook->setText(QApplication::translate("MainWindow", "\346\225\260\346\215\256\345\257\274\345\207\272", nullptr));
        mActionCircle2point->setText(QApplication::translate("MainWindow", "\345\234\206\346\240\207\346\263\250", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionCircle2point->setToolTip(QApplication::translate("MainWindow", "\345\234\206\346\240\207\346\263\250", nullptr));
#endif // QT_NO_TOOLTIP
        mActionMeaureLength1->setText(QApplication::translate("MainWindow", "\346\265\213\351\207\217\351\225\277\345\272\246", nullptr));
        mActionMeasureArea->setText(QApplication::translate("MainWindow", "\346\265\213\351\207\217\351\235\242\347\247\257", nullptr));
        mActiontesttest->setText(QApplication::translate("MainWindow", "testtest", nullptr));
        mActionSelectPicture->setText(QApplication::translate("MainWindow", "\351\200\211\346\213\251\345\233\276", nullptr));
        mActionHidePictureTable->setText(QApplication::translate("MainWindow", "\351\232\220\350\227\217/\346\230\276\347\244\272\345\233\276\347\211\207\346\240\210", nullptr));
        mActionHidePictureTable->setIconText(QApplication::translate("MainWindow", "\351\232\220\350\227\217/\346\230\276\347\244\272\345\233\276\347\211\207\346\240\210", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionHidePictureTable->setToolTip(QApplication::translate("MainWindow", "\351\232\220\350\227\217/\346\230\276\347\244\272\345\233\276\347\211\207\346\240\210", nullptr));
#endif // QT_NO_TOOLTIP
        mActionTaskSelect->setText(QApplication::translate("MainWindow", "\351\207\215\346\226\260\350\256\241\346\225\260", nullptr));
        mActionTaskSelect->setIconText(QApplication::translate("MainWindow", "\351\207\215\346\226\260\350\256\241\346\225\260", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionTaskSelect->setToolTip(QApplication::translate("MainWindow", "\344\273\273\345\212\241\351\207\215\346\226\260\350\256\241\346\225\260", nullptr));
#endif // QT_NO_TOOLTIP
        mActionHideTarge->setText(QApplication::translate("MainWindow", "\351\232\220\350\227\217\347\233\256\346\240\207", nullptr));
        mActionHideBigPicture->setText(QApplication::translate("MainWindow", "\351\232\220\350\227\217/\346\230\276\347\244\272\345\244\247\345\233\276", nullptr));
        mActionMeasureArea1->setText(QApplication::translate("MainWindow", "\346\265\213\351\207\217\351\235\242\347\247\257", nullptr));
        mActionCircle->setText(QApplication::translate("MainWindow", "\346\267\273\345\212\240\345\234\206\346\240\207\346\263\250", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionCircle->setToolTip(QApplication::translate("MainWindow", "\345\234\206\346\240\207\346\263\250", nullptr));
#endif // QT_NO_TOOLTIP
        mActionHideSmallJpg->setText(QApplication::translate("MainWindow", "\351\232\220\350\227\217/\346\230\276\347\244\272\345\260\217\345\233\276", nullptr));
        mActionMiddle->setText(QApplication::translate("MainWindow", "\345\234\260\345\233\276\345\261\205\344\270\255", nullptr));
        mActionDTback->setText(QApplication::translate("MainWindow", "\346\225\260\346\215\256\345\233\236\346\224\276", nullptr));
        mActionHideTable->setText(QApplication::translate("MainWindow", "\351\232\220\350\227\217/\346\230\276\347\244\272\345\256\236\346\227\266\350\241\250\346\240\274", nullptr));
        mActionStartStop->setText(QApplication::translate("MainWindow", "\345\220\257\345\212\250/\345\201\234\346\255\242\346\216\245\346\224\266", nullptr));
        mActionRefreshGJ->setText(QApplication::translate("MainWindow", "\345\210\267\346\226\260\350\275\250\350\277\271\347\272\277", nullptr));
        actionRadarGo->setText(QApplication::translate("MainWindow", "\351\233\267\350\276\276\346\212\225\345\275\261", nullptr));
#ifndef QT_NO_TOOLTIP
        actionRadarGo->setToolTip(QApplication::translate("MainWindow", "\351\233\267\350\276\276\346\212\225\345\275\261", nullptr));
#endif // QT_NO_TOOLTIP
        mActionSelectRadarLayer->setText(QApplication::translate("MainWindow", "\350\243\205\345\244\207\345\233\276\345\261\202", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionSelectRadarLayer->setToolTip(QApplication::translate("MainWindow", "\351\200\211\346\213\251\350\243\205\345\244\207\345\233\276\345\261\202", nullptr));
#endif // QT_NO_TOOLTIP
        mActionSelectRadarDev->setText(QApplication::translate("MainWindow", "\351\200\211\351\233\267\350\276\276", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionSelectRadarDev->setToolTip(QApplication::translate("MainWindow", "\351\200\211\344\270\255\351\233\267\350\276\276", nullptr));
#endif // QT_NO_TOOLTIP
        mActionMoveRadarDev->setText(QApplication::translate("MainWindow", "\347\247\273\345\212\250\350\243\205\345\244\207", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionMoveRadarDev->setToolTip(QApplication::translate("MainWindow", "\347\247\273\345\212\250\350\243\205\345\244\207", nullptr));
#endif // QT_NO_TOOLTIP
        mActionDelRadarDev->setText(QApplication::translate("MainWindow", "\345\210\240\351\231\244\350\243\205\345\244\207", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionDelRadarDev->setToolTip(QApplication::translate("MainWindow", "\345\210\240\351\231\244\350\243\205\345\244\207\350\256\276\345\244\207", nullptr));
#endif // QT_NO_TOOLTIP
        mActionFixRadar->setText(QApplication::translate("MainWindow", "\344\277\256\346\224\271\350\243\205\345\244\207\345\217\202\346\225\260", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionFixRadar->setToolTip(QApplication::translate("MainWindow", "\344\277\256\346\224\271\350\243\205\345\244\207\345\217\202\346\225\260", nullptr));
#endif // QT_NO_TOOLTIP
        mActionOutfitMove->setText(QApplication::translate("MainWindow", "\347\247\273\345\212\250\350\243\205\345\244\207", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionOutfitMove->setToolTip(QApplication::translate("MainWindow", "\347\247\273\345\212\250\350\243\205\345\244\207", nullptr));
#endif // QT_NO_TOOLTIP
        mActionAddTaskRect->setText(QApplication::translate("MainWindow", "\346\267\273\345\212\240\344\273\273\345\212\241\345\214\272\345\237\237", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionAddTaskRect->setToolTip(QApplication::translate("MainWindow", "\346\267\273\345\212\240\346\227\240\344\272\272\346\234\272\344\273\273\345\212\241\345\214\272\345\237\237", nullptr));
#endif // QT_NO_TOOLTIP
        mActionRadarTestAirList->setText(QApplication::translate("MainWindow", "\350\243\205\345\244\207\346\216\242\346\265\213\345\210\227\350\241\250", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionRadarTestAirList->setToolTip(QApplication::translate("MainWindow", "\350\243\205\345\244\207\346\216\242\346\265\213\346\227\240\344\272\272\346\234\272\345\210\227\350\241\250", nullptr));
#endif // QT_NO_TOOLTIP
        mActionSelTaskArea->setText(QApplication::translate("MainWindow", "\351\200\211\344\273\273\345\212\241\345\214\272\345\237\237", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionSelTaskArea->setToolTip(QApplication::translate("MainWindow", "\351\200\211\346\227\240\344\272\272\346\234\272\344\273\273\345\212\241\345\214\272\345\237\237", nullptr));
#endif // QT_NO_TOOLTIP
        mActionFixTaskArea->setText(QApplication::translate("MainWindow", "\344\277\256\346\224\271\344\273\273\345\212\241\345\214\272\345\237\237", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionFixTaskArea->setToolTip(QApplication::translate("MainWindow", "\344\277\256\346\224\271\346\227\240\344\272\272\346\234\272\344\273\273\345\212\241\345\214\272\345\237\237", nullptr));
#endif // QT_NO_TOOLTIP
        mActionMoveTaskArea->setText(QApplication::translate("MainWindow", "\347\247\273\345\212\250\344\273\273\345\212\241\345\214\272\345\237\237", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionMoveTaskArea->setToolTip(QApplication::translate("MainWindow", "\347\247\273\345\212\250\344\273\273\345\212\241\345\214\272\345\237\237", nullptr));
#endif // QT_NO_TOOLTIP
        mActionDelTaskArea->setText(QApplication::translate("MainWindow", "\345\210\240\351\231\244\344\273\273\345\212\241\345\214\272\345\237\237", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionDelTaskArea->setToolTip(QApplication::translate("MainWindow", "\345\210\240\351\231\244\344\273\273\345\212\241\345\214\272\345\237\237", nullptr));
#endif // QT_NO_TOOLTIP
        mActionSelTaskLayer->setText(QApplication::translate("MainWindow", "\344\273\273\345\212\241\345\233\276\345\261\202", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionSelTaskLayer->setToolTip(QApplication::translate("MainWindow", "\351\200\211\346\213\251\346\227\240\344\272\272\346\234\272\344\273\273\345\212\241\345\233\276\345\261\202", nullptr));
#endif // QT_NO_TOOLTIP
        mActionRadarNo->setText(QApplication::translate("MainWindow", "\350\243\205\345\244\207\345\267\262\350\246\206\347\233\226", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionRadarNo->setToolTip(QApplication::translate("MainWindow", "\350\243\205\345\244\207\345\267\262\350\246\206\347\233\226\345\214\272\345\237\237", nullptr));
#endif // QT_NO_TOOLTIP
        mActionRadarOverList->setText(QApplication::translate("MainWindow", "\350\243\205\345\244\207\350\246\206\347\233\226\345\210\227\350\241\250", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionRadarOverList->setToolTip(QApplication::translate("MainWindow", "\350\243\205\345\244\207\350\246\206\347\233\226\345\210\227\350\241\250", nullptr));
#endif // QT_NO_TOOLTIP
        mActionConnect->setText(QApplication::translate("MainWindow", "\350\277\236\346\216\245", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionConnect->setToolTip(QApplication::translate("MainWindow", "\351\205\215\347\275\256UDP\346\216\245\346\224\266\347\253\257\345\217\243\345\271\266\350\277\236\346\216\245", nullptr));
#endif // QT_NO_TOOLTIP
        mActionEquipment->setText(QApplication::translate("MainWindow", "\350\243\205\345\244\207\345\272\223\347\256\241\347\220\206", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionEquipment->setToolTip(QApplication::translate("MainWindow", "\350\243\205\345\244\207\345\272\223\347\256\241\347\220\206", nullptr));
#endif // QT_NO_TOOLTIP
        mActionTaskGuihua->setText(QApplication::translate("MainWindow", "\344\273\273\345\212\241\350\247\204\345\210\222", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionTaskGuihua->setToolTip(QApplication::translate("MainWindow", "\344\273\273\345\212\241\350\247\204\345\210\222", nullptr));
#endif // QT_NO_TOOLTIP
        mActionTuili->setText(QApplication::translate("MainWindow", "\351\233\206\347\276\244\346\216\250\347\220\206", nullptr));
#ifndef QT_NO_TOOLTIP
        mActionTuili->setToolTip(QApplication::translate("MainWindow", "\346\227\240\344\272\272\346\234\272\351\233\206\347\276\244\346\216\250\347\220\206", nullptr));
#endif // QT_NO_TOOLTIP
        mAction3D->setText(QApplication::translate("MainWindow", "2D3D\347\252\227\345\217\243\345\210\207\346\215\242", nullptr));
        menu->setTitle(QApplication::translate("MainWindow", "\350\256\276\347\275\256", nullptr));
        menu_2->setTitle(QApplication::translate("MainWindow", "\345\267\245\345\205\267", nullptr));
        menu_3->setTitle(QApplication::translate("MainWindow", "\345\205\266\344\273\226", nullptr));
        mToolBarDraw->setWindowTitle(QApplication::translate("MainWindow", "toolBar", nullptr));
        mToolBarSetting->setWindowTitle(QApplication::translate("MainWindow", "toolBar_2", nullptr));
        mToolBarSchedule->setWindowTitle(QApplication::translate("MainWindow", "toolBar", nullptr));
        toolBar->setWindowTitle(QApplication::translate("MainWindow", "toolBar", nullptr));
        toolBar_2->setWindowTitle(QApplication::translate("MainWindow", "toolBar_2", nullptr));
        toolBar_3->setWindowTitle(QApplication::translate("MainWindow", "toolBar_3", nullptr));
        toolBar_4->setWindowTitle(QApplication::translate("MainWindow", "toolBar_4", nullptr));
    } // retranslateUi

};

namespace Ui {
    class MainWindow: public Ui_MainWindow {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MAINWINDOW_H
