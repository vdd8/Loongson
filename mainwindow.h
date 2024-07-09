#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QDateTime>
#include <QtCharts>

#include "displaydialog.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void initData();
    void clearWidgetOfLayout(QLayout *layout);
    QChart* createLineChart(QString title) const;
    QChart* createLineChart(QString title, QString unit, QString name, QList<QPointF> list,int minY,int maxY) const;
    void createView();
    void updateStyle();

private slots:
    void on_pushButton_clicked();
    void on_pushButton_2_clicked();
    void on_pushButton_3_clicked();
    void on_pushButton_4_clicked();

    void on_pushButton_display_clicked();
    void on_pushButton_export_clicked();

private:
    Ui::MainWindow *ui;
    QStringList m_nameList;
    QStringList m_unitList;
    QList<QPair<int,int>> m_rangeList;
    QList<QList<QPointF>> m_pointData;//面积图以及线图展示的数据集
    QChart::ChartTheme m_theme;
    QString m_button_style = "QPushButton{\
            margin-right:3px;\
            margin-bottom:0px;\
            color: rgb(255, 255, 255);\
            background-color: rgba(165, 205, 255,125);\
            border:1px outset rgb(255, 255, 255);\
            border-radius:5px;\
            font: bold 14px \"Microsoft YaHei\";\
        }";
};

#endif // MAINWINDOW_H
