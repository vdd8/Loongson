#ifndef DISPLAYDIALOG_H
#define DISPLAYDIALOG_H

#include <QDialog>
#include <QPushButton>
#include <QDateTime>

namespace Ui {
class DisplayDialog;
}

class DisplayDialog : public QDialog
{
    Q_OBJECT

public:
    explicit DisplayDialog(QStringList nameList, QList<QList<QPointF>> pointData, QWidget *parent = nullptr);
    ~DisplayDialog();

private:
    Ui::DisplayDialog *ui;

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

#endif // DISPLAYDIALOG_H
