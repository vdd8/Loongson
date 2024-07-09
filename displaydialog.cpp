#include "displaydialog.h"
#include "ui_displaydialog.h"

DisplayDialog::DisplayDialog(QStringList nameList, QList<QList<QPointF>> pointData, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DisplayDialog)
{
    ui->setupUi(this);
    this->setWindowTitle("数据展示");
    this->resize(800,400);
    ui->treeWidget->setStyleSheet("background-color:transparent;color:white");

    int max_column = 0;
    for(int i=0;i<pointData.size();i++)
    {
        if(pointData[i].size()>max_column)
        {
            max_column=pointData[i].size();
        }
    }
    QStringList headerList;
    headerList << "名称";
    for (int i=0;i<max_column;i++)
    {
        headerList<<QString("数据%1").arg(i+1);
    }
    ui->treeWidget->setHeaderLabels(headerList);

    if(pointData.size()>0)
    {
        for (int i=0;i<nameList.size();i++)
        {
            QStringList tree_data;
            tree_data << nameList[i];
            QList<QPointF> list = pointData[i];
            foreach (QPointF point, list)
            {
                QDateTime dateTime = QDateTime::fromSecsSinceEpoch(point.x());
                QString data="[";
                if(dateTime.isValid())
                {
                    data.append(dateTime.toString("hh:m:ss"));
                }
                else
                {
                    data.append("invalid");
                }
                data.append(",").append(QString::number(point.y())).append("]");
                tree_data << data;
            }
            new QTreeWidgetItem(ui->treeWidget,tree_data);
        }
    }

    QList<QPushButton*> list = this->findChildren<QPushButton*>();
    foreach(QPushButton * btn, list)
    {
        btn->setStyleSheet(m_button_style);
    }
    QPalette pal = window()->palette();
    pal.setColor(QPalette::Window, QRgb(0x121218));
    pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
    window()->setPalette(pal);
}

DisplayDialog::~DisplayDialog()
{
    delete ui;
}
