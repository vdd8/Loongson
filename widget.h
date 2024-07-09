
#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QTcpSocket>
#include <QAbstractSocket>
#include <QMessageBox>
#include <QDebug>


QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget

{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();

    //发送helo
    void sendHelo();
    //发送auth login
    void sendAuthLogin();
    //发送用户名
    void sendUser();
    //发送密码
    void sendPassword();
    //发送mail from
    void sendMailFrom();
    //发送rcpt to
    void sendRcptTo();
    //发送data
    void sendData();
    //发送邮件内容
    void sendContent();
    //发送成功后续操作
    void afterSendSuccess();

private slots:
     void readyReadSlot();

    void on_pushButton_sendmail_clicked();

private:
    Ui::Widget *ui;
    QTcpSocket *tcpSocket;

    QString title;
    QString content; //邮件正文
    QString senderMail; //发送方邮箱地址
    QString authCode; //发送方邮箱授权码
    QString receiveMail; //接收方邮箱地址
    QString expectedReply; //期待收到的应答
    int state = 0;

};

#endif // WIDGET_H
