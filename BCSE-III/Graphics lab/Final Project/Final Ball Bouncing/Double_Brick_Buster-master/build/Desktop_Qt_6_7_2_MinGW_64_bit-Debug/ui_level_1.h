/********************************************************************************
** Form generated from reading UI file 'level_1.ui'
**
** Created by: Qt User Interface Compiler version 6.7.2
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_LEVEL_1_H
#define UI_LEVEL_1_H

#include <QtCore/QVariant>
#include <QtWidgets/QApplication>
#include <QtWidgets/QGraphicsView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Level_1
{
public:
    QGraphicsView *level1_graphicsView;
    QLabel *level1_titleLable;
    QLabel *level1_pointsLabel;
    QPushButton *help_button;

    void setupUi(QWidget *Level_1)
    {
        if (Level_1->objectName().isEmpty())
            Level_1->setObjectName("Level_1");
        Level_1->resize(612, 601);
        level1_graphicsView = new QGraphicsView(Level_1);
        level1_graphicsView->setObjectName("level1_graphicsView");
        level1_graphicsView->setGeometry(QRect(25, 71, 561, 491));
        QFont font;
        font.setPointSize(27);
        level1_graphicsView->setFont(font);
        level1_graphicsView->setVerticalScrollBarPolicy(Qt::ScrollBarPolicy::ScrollBarAlwaysOff);
        level1_graphicsView->setRenderHints(QPainter::RenderHint::Antialiasing|QPainter::RenderHint::TextAntialiasing);
        level1_graphicsView->setCacheMode(QGraphicsView::CacheModeFlag::CacheBackground);
        level1_graphicsView->setViewportUpdateMode(QGraphicsView::ViewportUpdateMode::BoundingRectViewportUpdate);
        level1_titleLable = new QLabel(Level_1);
        level1_titleLable->setObjectName("level1_titleLable");
        level1_titleLable->setGeometry(QRect(30, 20, 351, 41));
        QFont font1;
        font1.setPointSize(20);
        level1_titleLable->setFont(font1);
        level1_pointsLabel = new QLabel(Level_1);
        level1_pointsLabel->setObjectName("level1_pointsLabel");
        level1_pointsLabel->setGeometry(QRect(30, 570, 67, 17));
        help_button = new QPushButton(Level_1);
        help_button->setObjectName("help_button");
        help_button->setGeometry(QRect(388, 20, 191, 41));

        retranslateUi(Level_1);

        QMetaObject::connectSlotsByName(Level_1);
    } // setupUi

    void retranslateUi(QWidget *Level_1)
    {
        Level_1->setWindowTitle(QCoreApplication::translate("Level_1", "Form", nullptr));
        level1_titleLable->setText(QCoreApplication::translate("Level_1", "Ball Bouncing Game", nullptr));
        level1_pointsLabel->setText(QString());
        help_button->setText(QCoreApplication::translate("Level_1", "Instructions / Controls", nullptr));
    } // retranslateUi

};

namespace Ui {
    class Level_1: public Ui_Level_1 {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_LEVEL_1_H
