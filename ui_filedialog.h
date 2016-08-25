/********************************************************************************
** Form generated from reading UI file 'filedialog.ui'
**
** Created: Fri Aug 5 15:28:51 2016
**      by: Qt User Interface Compiler version 4.8.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_FILEDIALOG_H
#define UI_FILEDIALOG_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtGui/QApplication>
#include <QtGui/QButtonGroup>
#include <QtGui/QDoubleSpinBox>
#include <QtGui/QHeaderView>
#include <QtGui/QLabel>
#include <QtGui/QLineEdit>
#include <QtGui/QMainWindow>
#include <QtGui/QMenuBar>
#include <QtGui/QPushButton>
#include <QtGui/QSplitter>
#include <QtGui/QStatusBar>
#include <QtGui/QToolBar>
#include <QtGui/QWidget>

QT_BEGIN_NAMESPACE

class Ui_FileDialog
{
public:
    QWidget *centralWidget;
    QPushButton *pushButton_2;
    QPushButton *pushButton_3;
    QSplitter *splitter;
    QLineEdit *lineEdit;
    QPushButton *pushButton;
    QSplitter *splitter_2;
    QLabel *label;
    QLabel *label_2;
    QSplitter *splitter_3;
    QLabel *label_3;
    QDoubleSpinBox *doubleSpinBox;
    QSplitter *splitter_4;
    QLabel *label_4;
    QLineEdit *lineEdit_2;
    QMenuBar *menuBar;
    QToolBar *mainToolBar;
    QStatusBar *statusBar;

    void setupUi(QMainWindow *FileDialog)
    {
        if (FileDialog->objectName().isEmpty())
            FileDialog->setObjectName(QString::fromUtf8("FileDialog"));
        FileDialog->resize(719, 681);
        centralWidget = new QWidget(FileDialog);
        centralWidget->setObjectName(QString::fromUtf8("centralWidget"));
        pushButton_2 = new QPushButton(centralWidget);
        pushButton_2->setObjectName(QString::fromUtf8("pushButton_2"));
        pushButton_2->setGeometry(QRect(500, 540, 141, 41));
        pushButton_3 = new QPushButton(centralWidget);
        pushButton_3->setObjectName(QString::fromUtf8("pushButton_3"));
        pushButton_3->setGeometry(QRect(110, 540, 131, 41));
        splitter = new QSplitter(centralWidget);
        splitter->setObjectName(QString::fromUtf8("splitter"));
        splitter->setGeometry(QRect(10, 330, 691, 31));
        splitter->setOrientation(Qt::Horizontal);
        lineEdit = new QLineEdit(splitter);
        lineEdit->setObjectName(QString::fromUtf8("lineEdit"));
        splitter->addWidget(lineEdit);
        pushButton = new QPushButton(splitter);
        pushButton->setObjectName(QString::fromUtf8("pushButton"));
        splitter->addWidget(pushButton);
        splitter_2 = new QSplitter(centralWidget);
        splitter_2->setObjectName(QString::fromUtf8("splitter_2"));
        splitter_2->setGeometry(QRect(9, 9, 691, 291));
        splitter_2->setOrientation(Qt::Horizontal);
        label = new QLabel(splitter_2);
        label->setObjectName(QString::fromUtf8("label"));
        splitter_2->addWidget(label);
        label_2 = new QLabel(splitter_2);
        label_2->setObjectName(QString::fromUtf8("label_2"));
        splitter_2->addWidget(label_2);
        splitter_3 = new QSplitter(centralWidget);
        splitter_3->setObjectName(QString::fromUtf8("splitter_3"));
        splitter_3->setGeometry(QRect(20, 390, 671, 31));
        splitter_3->setOrientation(Qt::Horizontal);
        label_3 = new QLabel(splitter_3);
        label_3->setObjectName(QString::fromUtf8("label_3"));
        splitter_3->addWidget(label_3);
        doubleSpinBox = new QDoubleSpinBox(splitter_3);
        doubleSpinBox->setObjectName(QString::fromUtf8("doubleSpinBox"));
        doubleSpinBox->setDecimals(3);
        doubleSpinBox->setMaximum(1);
        doubleSpinBox->setSingleStep(0.001);
        doubleSpinBox->setValue(0);
        splitter_3->addWidget(doubleSpinBox);
        splitter_4 = new QSplitter(centralWidget);
        splitter_4->setObjectName(QString::fromUtf8("splitter_4"));
        splitter_4->setGeometry(QRect(20, 440, 671, 31));
        splitter_4->setOrientation(Qt::Horizontal);
        label_4 = new QLabel(splitter_4);
        label_4->setObjectName(QString::fromUtf8("label_4"));
        splitter_4->addWidget(label_4);
        lineEdit_2 = new QLineEdit(splitter_4);
        lineEdit_2->setObjectName(QString::fromUtf8("lineEdit_2"));
        splitter_4->addWidget(lineEdit_2);
        FileDialog->setCentralWidget(centralWidget);
        menuBar = new QMenuBar(FileDialog);
        menuBar->setObjectName(QString::fromUtf8("menuBar"));
        menuBar->setGeometry(QRect(0, 0, 719, 25));
        FileDialog->setMenuBar(menuBar);
        mainToolBar = new QToolBar(FileDialog);
        mainToolBar->setObjectName(QString::fromUtf8("mainToolBar"));
        FileDialog->addToolBar(Qt::TopToolBarArea, mainToolBar);
        statusBar = new QStatusBar(FileDialog);
        statusBar->setObjectName(QString::fromUtf8("statusBar"));
        FileDialog->setStatusBar(statusBar);

        retranslateUi(FileDialog);

        QMetaObject::connectSlotsByName(FileDialog);
    } // setupUi

    void retranslateUi(QMainWindow *FileDialog)
    {
        FileDialog->setWindowTitle(QApplication::translate("FileDialog", "FileDialog", 0, QApplication::UnicodeUTF8));
        pushButton_2->setText(QApplication::translate("FileDialog", "Close Application", 0, QApplication::UnicodeUTF8));
        pushButton_3->setText(QApplication::translate("FileDialog", "Apply Filter", 0, QApplication::UnicodeUTF8));
        pushButton->setText(QApplication::translate("FileDialog", "Browse", 0, QApplication::UnicodeUTF8));
        label->setText(QString());
        label_2->setText(QString());
        label_3->setText(QApplication::translate("FileDialog", "Input threshold for Watershed Segmentation", 0, QApplication::UnicodeUTF8));
        label_4->setText(QApplication::translate("FileDialog", "SetLevel for Watershed Segmentation", 0, QApplication::UnicodeUTF8));
    } // retranslateUi

};

namespace Ui {
    class FileDialog: public Ui_FileDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_FILEDIALOG_H
