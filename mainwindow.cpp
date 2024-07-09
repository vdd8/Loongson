#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    this->setWindowTitle("工业无线物联网传感系统");
    ui->label->hide();
    ui->label_battery->hide();

    m_nameList << "温度" << "湿度" << "二氧化碳浓度" << "烟雾浓度" << "光照强度" << "电量";
    m_unitList << "℃" << "%RH" << "PPM" << "PPM" << "LUX" << "%";
    m_rangeList << QPair<int,int>(-10,50) << QPair<int,int>(0,100)<< QPair<int,int>(400,10000)
            << QPair<int,int>(200,10000)<< QPair<int,int>(0,65535)<< QPair<int,int>(0,100);

    updateStyle();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::initData()
{
    m_pointData.clear();

    int pointCount = 20;
    QDateTime currentTime = QDateTime::currentDateTime();
    for (int i = 0; i < m_nameList.size(); i++)
    {
        QList<QPointF> pointList;

        for (int j = 0; j < pointCount; j++)
        {
            qint64 x = currentTime.addSecs(qrand() % 60).toSecsSinceEpoch();\
            qint64 y = 0;
            if (m_nameList[i] == "温度")
            {
                y = qrand() % 61 - 10;
            }
            else if (m_nameList[i] == "湿度")
            {
                y = qrand() % 101;
            }
            else if (m_nameList[i] == "二氧化碳浓度")
            {
                y = qrand() % 9601 + 400;
            }
            else if (m_nameList[i] == "烟雾浓度")
            {
                y = qrand() % 8801 + 200;
            }
            else if (m_nameList[i] == "光照强度")
            {
                y = qrand() % 65536;
            }
            else if (m_nameList[i] == "电量")
            {
                y = qrand() % 101;
            }
            pointList.append(QPointF(x, y));
        }
        std::sort(pointList.begin(), pointList.end(), [=](const QPointF a, const QPointF b)->bool {return a.x() < b.x(); });
        m_pointData.append(pointList);
    }
}

void MainWindow::clearWidgetOfLayout(QLayout* layout)
{
    QLayoutItem* child;
    while ((child = layout->takeAt(0)) != 0)
    {
        if (child->widget())
        {
            child->widget()->setParent(NULL);
        }
        delete child;
        child = NULL;
    }
}

QChart* MainWindow::createLineChart(QString title) const
{
    QChart* chart = new QChart();
    chart->setTitle(title);

    qint64 min = INT_MAX;
    qint64 max = INT_MIN;
    for (int i = 0; i < m_pointData.size(); i++)
    {
        for (int j = 0; j < m_pointData[i].size(); j++)
        {
            if (m_pointData[i][j].x() > max)
            {
                max = m_pointData[i][j].x();
            }
            if (m_pointData[i][j].x() < min)
            {
                min = m_pointData[i][j].x();
            }
        }
    }

    //创建X轴
    QDateTimeAxis* AxisX = new QDateTimeAxis();
    AxisX->setTitleText("时间(时:分:秒)");                                // x轴显示标题
    AxisX->setGridLineVisible(false);                                   // 隐藏背景网格X轴框线
    AxisX->setFormat("hh:mm:ss");                                       // x轴格式
    AxisX->setLabelsAngle(0);                                           // x轴显示的文字倾斜角度
    AxisX->setTickCount(10);                                            // 轴上点的个数
    AxisX->setRange(QDateTime::fromSecsSinceEpoch(min - 1), QDateTime::fromSecsSinceEpoch(max + 1));  // 范围

    // y轴
    QValueAxis* AxisY = new QValueAxis();
    AxisY->setTitleText("");                                            // y轴显示标题
    AxisY->setRange(0, 25);                                             // 范围
    AxisY->setTickCount(11);                                            // 轴上点的个数
    AxisY->setGridLineVisible(false);                                   // 隐藏背景网格Y轴框线

    for (int i = 0; i < m_pointData.size(); i++)
    {
        QLineSeries* series = new QLineSeries(chart);
        //QSplineSeries *series = new QSplineSeries(chart);
        series->append(m_pointData[i]);
        series->setName(m_nameList[i]);
        series->attachAxis(AxisX);                                         // 线段依附的x轴
        series->attachAxis(AxisY);                                         // 线段依附的y轴
        chart->addSeries(series);
    }


    //    chart->createDefaultAxes();
    //    chart->axes(Qt::Horizontal).first()->setRange(QDateTime::currentDateTime().toSecsSinceEpoch() - 1, QDateTime::currentDateTime().toSecsSinceEpoch() + 60);
    //    chart->axes(Qt::Vertical).first()->setRange(0, 25);

    //    QValueAxis *axisY = qobject_cast<QValueAxis*>(chart->axes(Qt::Vertical).first());
    //    Q_ASSERT(axisY);
    //    axisY->setLabelFormat("%.1f  ");

    chart->addAxis(AxisX, Qt::AlignBottom);                // 设置x轴位置
    chart->addAxis(AxisY, Qt::AlignLeft);                  // 设置y轴位置

    return chart;
}

QChart *MainWindow::createLineChart(QString title, QString unit, QString name, QList<QPointF> list,int minY,int maxY) const
{
    QChart* chart = new QChart();
    chart->setTitle(title);

    qint64 min = INT_MAX;
    qint64 max = INT_MIN;
    for (int j = 0; j < list.size(); j++)
    {
        if (list[j].x() > max)
        {
            max = list[j].x();
        }
        if (list[j].x() < min)
        {
            min = list[j].x();
        }
    }

    //创建X轴
    QDateTimeAxis* AxisX = new QDateTimeAxis();
    AxisX->setTitleText("时间(时:分:秒)");                                // x轴显示标题
    AxisX->setGridLineVisible(false);                                   // 隐藏背景网格X轴框线
    AxisX->setFormat("hh:mm:ss");                                       // x轴格式
    AxisX->setLabelsAngle(0);                                           // x轴显示的文字倾斜角度
    AxisX->setTickCount(10);                                            // 轴上点的个数
    AxisX->setRange(QDateTime::fromSecsSinceEpoch(min - 1), QDateTime::fromSecsSinceEpoch(max + 1));  // 范围

    // y轴
    QValueAxis* AxisY = new QValueAxis();
    AxisY->setTitleText(unit);                                          // y轴显示标题
    AxisY->setRange(minY, maxY);                                        // 范围
    AxisY->setTickCount(10);                                            // 轴上点的个数
    AxisY->setGridLineVisible(false);                                   // 隐藏背景网格Y轴框线

    QLineSeries* series = new QLineSeries(chart);
    series->append(list);
    series->setName(name);
    series->attachAxis(AxisX);                                         // 线段依附的x轴
    series->attachAxis(AxisY);                                         // 线段依附的y轴
    chart->addSeries(series);

    chart->addAxis(AxisX, Qt::AlignBottom);                // 设置x轴位置
    chart->addAxis(AxisY, Qt::AlignLeft);                  // 设置y轴位置

    return chart;
}

void MainWindow::createView()
{
    initData();
    clearWidgetOfLayout(ui->gridLayout);

    for(int i=0;i<m_nameList.size();i++)
    {
        QChartView* chartView=new QChartView(createLineChart(m_nameList[i]+"折线图",m_unitList[i],m_nameList[i],
                                                             m_pointData[i],m_rangeList[i].first,m_rangeList[i].second));
        chartView->setRenderHint(QPainter::Antialiasing);
        chartView->chart()->setTheme(m_theme);
        ui->gridLayout->addWidget(chartView,i / 3, i % 3, 1, 1);
        ui->gridLayout->setRowStretch(i / 3, 1);
        ui->gridLayout->setColumnStretch(i % 3, 1);
    }
}

void MainWindow::updateStyle()
{
    QList<QPushButton*> list = this->findChildren<QPushButton*>();
    foreach(QPushButton * btn, list)
    {
        btn->setStyleSheet(m_button_style);
    }

    m_theme = QChart::ChartThemeDark;
    QPalette pal = window()->palette();
    if (m_theme == QChart::ChartThemeLight)
    {
        pal.setColor(QPalette::Window, QRgb(0xf0f0f0));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
    }
    else if (m_theme == QChart::ChartThemeDark)
    {
        pal.setColor(QPalette::Window, QRgb(0x121218));
        pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
    }
    else if (m_theme == QChart::ChartThemeBlueCerulean)
    {
        pal.setColor(QPalette::Window, QRgb(0x40434a));
        pal.setColor(QPalette::WindowText, QRgb(0xd6d6d6));
    }
    else if (m_theme == QChart::ChartThemeBrownSand)
    {
        pal.setColor(QPalette::Window, QRgb(0x9e8965));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
    }
    else if (m_theme == QChart::ChartThemeBlueNcs)
    {
        pal.setColor(QPalette::Window, QRgb(0x018bba));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
    }
    else if (m_theme == QChart::ChartThemeHighContrast)
    {
        pal.setColor(QPalette::Window, QRgb(0xffab03));
        pal.setColor(QPalette::WindowText, QRgb(0x181818));
    }
    else if (m_theme == QChart::ChartThemeBlueIcy)
    {
        pal.setColor(QPalette::Window, QRgb(0xcee7f0));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
    }
    else
    {
        pal.setColor(QPalette::Window, QRgb(0xe6f3d9));
        pal.setColor(QPalette::WindowText, QRgb(0x404044));
    }
    window()->setPalette(pal);
}

void MainWindow::on_pushButton_clicked()
{
    createView();
}

void MainWindow::on_pushButton_2_clicked()
{
    createView();
}

void MainWindow::on_pushButton_3_clicked()
{
    createView();
}

void MainWindow::on_pushButton_4_clicked()
{
    createView();
}

void MainWindow::on_pushButton_display_clicked()
{
    DisplayDialog dialog(m_nameList,m_pointData,this);
    dialog.exec();
}

void MainWindow::on_pushButton_export_clicked()
{

}
