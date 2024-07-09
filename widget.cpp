
#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);

    this->tcpSocket = new QTcpSocket(this);

    connect(tcpSocket, &QTcpSocket::readyRead,this, &Widget::readyReadSlot);
}

Widget::~Widget()
{
    delete ui;

}


void Widget::readyReadSlot()
{
    QByteArray buffer = tcpSocket->readAll();
    qDebug()<< state <<"收到服务器回复："<<buffer;

    if(buffer.contains(this->expectedReply.toUtf8())){
        switch(state)
        {
        case 0:
            state += 1;
            sendHelo();
            break;
        case 1:
            state += 1;
            sendAuthLogin();
            break;
        case 2:
            state += 1;
            sendUser();
            break;
        case 3:
            state += 1;
             sendPassword();
            break;
        case 4:
             state += 1;
             sendMailFrom();
            break;
        case 5:
            state += 1;
            sendRcptTo();
            break;
        case 6:
            state += 1;
            sendData();
            break;
        case 7:
            state += 1;
            sendContent();
            break;
        case 8:
            state = 0;
            afterSendSuccess();
            break;
        }
    }
    else{
        qDebug() << "邮件发送失败!";
    }
}


//发送helo
void Widget::sendHelo()
{
    QString str = "helo 163.com\r\n";
    qDebug()<<"向服务器发送：　" + str;
    tcpSocket->write(str.toUtf8());
    this->expectedReply = "250 OK\r\n";
}

//发送auth login
void Widget::sendAuthLogin()
{
    QString str = "auth login\r\n";
    qDebug()<<"向服务器发送：　" + str;
    tcpSocket->write(str.toUtf8());
    this->expectedReply = "334 dXNlcm5hbWU6\r\n";

}

//发送用户名
void Widget::sendUser()
{
    QString str = QString("\r\n").prepend(senderMail.toLatin1().toBase64());
    qDebug()<<"向服务器发送：　" + str;
    tcpSocket->write(str.toUtf8());
    this->expectedReply = "334 UGFzc3dvcmQ6\r\n";

}

//发送密码
void Widget::sendPassword()
{
    QString str = QString("\r\n").prepend(authCode.toLatin1().toBase64());
    qDebug()<<"向服务器发送：　" + str;
    tcpSocket->write(str.toUtf8());
    this->expectedReply = "235 Authentication successful\r\n";

}

//发送mail from
void Widget::sendMailFrom()
{
    QString str = QString("mail from: <%1>\r\n").arg(senderMail);
    qDebug()<<"向服务器发送：　" + str;
    tcpSocket->write(str.toUtf8());
    this->expectedReply = "250 Mail OK\r\n";

}

//发送rcpt to
void Widget::sendRcptTo()
{
    QString str = QString("rcpt to: <%1>\r\n").arg(receiveMail);
    qDebug()<<"向服务器发送：　" + str;
    tcpSocket->write(str.toUtf8());
    this->expectedReply = "250 Mail OK\r\n";
}

//发送data
void Widget::sendData()
{
    QString str = "data\r\n";
    qDebug()<<"向服务器发送：　" + str;
    tcpSocket->write(str.toUtf8());
    this->expectedReply = "354 End data with <CR><LF>.<CR><LF>\r\n";
}

//发送邮件内容
void Widget::sendContent()
{
    QString str = QString("from:%1\r\n"
                          "to:%2\r\n"
                          "subject:=?UTF-8?B?%3?=\r\n"
                          "\r\n"
                          "%4\r\n"
                          "\r\n"
                          ".\r\n").arg(senderMail).arg(receiveMail).arg( QString("").append(title.toUtf8().toBase64()) ).arg(content);
    qDebug()<<"向服务器发送：　" + str;
    tcpSocket->write(str.toUtf8());
    this->expectedReply = "250 Mail OK queued as ";
}

//发送成功后续操作
void Widget::afterSendSuccess()
{
    this->expectedReply = " ";
    qDebug() << "邮件发送成功!";
}


//主要修改发件的邮箱和授权码
void Widget::on_pushButton_sendmail_clicked()
{
    title = "报警信息--邮件标题";
    content = ui->lineEdit->text(); //邮件正文
    senderMail = "18602500991@163.com";//发送方邮箱地址
    authCode = "HMRLIPLWPEAMDITE";//发送方邮箱授权码
    receiveMail = ui->lineEdit_mailbox->text(); //接收方邮箱地址

    tcpSocket->connectToHost("smtp.163.com", 25);
}

