/********************************************************************************
** Form generated from reading UI file 'flyfi.ui'
**
** Created by: Qt User Interface Compiler version 5.4.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FLYFI_H
#define UI_FLYFI_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QComboBox>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QTextEdit>
#include <QtWidgets/QToolBar>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FlyFiClass
{
public:
    QAction *action_close;
    QAction *actionAbout;
    QWidget *centralWidget;
    QPushButton *btnRefreshPorts;
    QComboBox *cmbSerialPorts;
    QSlider *sldFreq;
    QLabel *lblFreq;
    QLabel *label;
    QLabel *label_2;
    QPushButton *btnPlay;
    QPushButton *btnStop;
    QPushButton *btnOpen;
    QTextEdit *edtDebug;
    QComboBox *cmbBaudrate;
    QLabel *label_3;
    QMenuBar *menuBar;
    QMenu *menu_File;
    QMenu *menuHelp;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FlyFiClass)
    {
        if (FlyFiClass->objectName().isEmpty())
            FlyFiClass->setObjectName(QStringLiteral("FlyFiClass"));
        FlyFiClass->resize(511, 503);
        action_close = new QAction(FlyFiClass);
        action_close->setObjectName(QStringLiteral("action_close"));
        actionAbout = new QAction(FlyFiClass);
        actionAbout->setObjectName(QStringLiteral("actionAbout"));
        actionAbout->setEnabled(false);
        centralWidget = new QWidget(FlyFiClass);
        centralWidget->setObjectName(QStringLiteral("centralWidget"));
        btnRefreshPorts = new QPushButton(centralWidget);
        btnRefreshPorts->setObjectName(QStringLiteral("btnRefreshPorts"));
        btnRefreshPorts->setGeometry(QRect(70, 60, 61, 23));
        cmbSerialPorts = new QComboBox(centralWidget);
        cmbSerialPorts->setObjectName(QStringLiteral("cmbSerialPorts"));
        cmbSerialPorts->setGeometry(QRect(70, 20, 421, 22));
        cmbSerialPorts->setMaxVisibleItems(10);
        sldFreq = new QSlider(centralWidget);
        sldFreq->setObjectName(QStringLiteral("sldFreq"));
        sldFreq->setGeometry(QRect(169, 110, 161, 20));
        sldFreq->setMaximum(60000);
        sldFreq->setOrientation(Qt::Horizontal);
        lblFreq = new QLabel(centralWidget);
        lblFreq->setObjectName(QStringLiteral("lblFreq"));
        lblFreq->setGeometry(QRect(350, 110, 51, 16));
        label = new QLabel(centralWidget);
        label->setObjectName(QStringLiteral("label"));
        label->setGeometry(QRect(90, 110, 61, 16));
        label_2 = new QLabel(centralWidget);
        label_2->setObjectName(QStringLiteral("label_2"));
        label_2->setGeometry(QRect(10, 20, 61, 16));
        btnPlay = new QPushButton(centralWidget);
        btnPlay->setObjectName(QStringLiteral("btnPlay"));
        btnPlay->setGeometry(QRect(90, 150, 331, 23));
        btnStop = new QPushButton(centralWidget);
        btnStop->setObjectName(QStringLiteral("btnStop"));
        btnStop->setGeometry(QRect(90, 180, 331, 23));
        btnOpen = new QPushButton(centralWidget);
        btnOpen->setObjectName(QStringLiteral("btnOpen"));
        btnOpen->setGeometry(QRect(430, 60, 61, 23));
        edtDebug = new QTextEdit(centralWidget);
        edtDebug->setObjectName(QStringLiteral("edtDebug"));
        edtDebug->setGeometry(QRect(90, 220, 331, 151));
        cmbBaudrate = new QComboBox(centralWidget);
        cmbBaudrate->setObjectName(QStringLiteral("cmbBaudrate"));
        cmbBaudrate->setGeometry(QRect(330, 60, 91, 22));
        label_3 = new QLabel(centralWidget);
        label_3->setObjectName(QStringLiteral("label_3"));
        label_3->setGeometry(QRect(270, 70, 47, 13));
        FlyFiClass->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(FlyFiClass);
        menuBar->setObjectName(QStringLiteral("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 511, 21));
        menu_File = new QMenu(menuBar);
        menu_File->setObjectName(QStringLiteral("menu_File"));
        menuHelp = new QMenu(menuBar);
        menuHelp->setObjectName(QStringLiteral("menuHelp"));
        FlyFiClass->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FlyFiClass);
        mainToolBar->setObjectName(QStringLiteral("mainToolBar"));
        FlyFiClass->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(FlyFiClass);
        statusBar->setObjectName(QStringLiteral("statusBar"));
        FlyFiClass->setStatusBar(statusBar);

        menuBar->addAction(menu_File->menuAction());
        menuBar->addAction(menuHelp->menuAction());
        menu_File->addAction(action_close);
        menuHelp->addAction(actionAbout);

        retranslateUi(FlyFiClass);

        cmbBaudrate->setCurrentIndex(-1);


        QMetaObject::connectSlotsByName(FlyFiClass);
    } // setupUi

    void retranslateUi(QMainWindow *FlyFiClass)
    {
        FlyFiClass->setWindowTitle(QApplication::translate("FlyFiClass", "Floppy Orgel 3.0 Controller", 0));
        action_close->setText(QApplication::translate("FlyFiClass", "&Close", 0));
        actionAbout->setText(QApplication::translate("FlyFiClass", "&About", 0));
        btnRefreshPorts->setText(QApplication::translate("FlyFiClass", "Refresh", 0));
        lblFreq->setText(QApplication::translate("FlyFiClass", "0.00 Hz", 0));
        label->setText(QApplication::translate("FlyFiClass", "Frequency:", 0));
        label_2->setText(QApplication::translate("FlyFiClass", "Serial Port:", 0));
        btnPlay->setText(QApplication::translate("FlyFiClass", "Play", 0));
        btnStop->setText(QApplication::translate("FlyFiClass", "Stop", 0));
        btnOpen->setText(QApplication::translate("FlyFiClass", "Open", 0));
        cmbBaudrate->setCurrentText(QString());
        label_3->setText(QApplication::translate("FlyFiClass", "Baudrate:", 0));
        menu_File->setTitle(QApplication::translate("FlyFiClass", "&File", 0));
        menuHelp->setTitle(QApplication::translate("FlyFiClass", "Help", 0));
    } // retranslateUi

};

namespace Ui {
    class FlyFiClass: public Ui_FlyFiClass {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FLYFI_H
