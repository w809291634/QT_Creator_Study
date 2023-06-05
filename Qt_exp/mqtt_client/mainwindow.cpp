#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QDateTime>
#include "QtMqtt/qmqttclient.h"
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    m_client = new QMqttClient(this);
    m_client->setHostname(ui->host_cbox->currentText());
    m_client->setPort(static_cast<quint16>(ui->spinBoxPort->value()));

    connect(m_client, SIGNAL(stateChanged(ClientState)),
            this, SLOT(updateLogStateChange()));
    connect(m_client, SIGNAL(disconnected()),
            this, SLOT(brokerDisconnected()));


    connect(m_client, SIGNAL(messageReceived(const QByteArray &, const QMqttTopicName &)),
            this, SLOT(client_messageReceived(const QByteArray &, const QMqttTopicName &)));
    connect(m_client, SIGNAL(pingResponseReceived()),
            this,SLOT(client_ping_response()));
    connect(ui->host_cbox, SIGNAL(textChanged(const QString)),
            m_client, SLOT(setHostname(const QString)));
    connect(ui->spinBoxPort, SIGNAL(valueChanged(int)),
            this, SLOT(setClientPort(int)));
    updateLogStateChange();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::client_messageReceived(
        const QByteArray &message,
        const QMqttTopicName &topic)
{
    const QString content = QDateTime::currentDateTime().toString()
                + QLatin1String(" Received Topic: ")
                + topic.name()
                + QLatin1String(" Message: ")
                + message
                + QLatin1Char('\n');
}

void MainWindow::client_ping_response()
{
    ui->buttonPing->setEnabled(true);
    const QString content = QDateTime::currentDateTime().toString()
                + QLatin1String(" PingResponse")
                + QLatin1Char('\n');
}


void MainWindow::on_buttonConnect_clicked()
{
    if (m_client->state() == QMqttClient::Disconnected) {
        ui->host_cbox->setEnabled(false);
        ui->spinBoxPort->setEnabled(false);
        ui->buttonConnect->setText(tr("Disconnect"));
        m_client->connectToHost();
    } else {
        ui->host_cbox->setEnabled(true);
        ui->spinBoxPort->setEnabled(true);
        ui->buttonConnect->setText(tr("Connect"));
        m_client->disconnectFromHost();
    }
}

void MainWindow::on_buttonQuit_clicked()
{
    QApplication::quit();
}

void MainWindow::updateLogStateChange()
{
    const QString content = QDateTime::currentDateTime().toString()
                    + QLatin1String(": State Change")
                    + QString::number(m_client->state())
                    + QLatin1Char('\n');
//    ui->editLog->insertPlainText(content);
}

void MainWindow::brokerDisconnected()
{
    ui->host_cbox->setEnabled(true);
    ui->spinBoxPort->setEnabled(true);
    ui->buttonConnect->setText(tr("Connect"));
}

void MainWindow::setClientPort(int p)
{
    m_client->setPort(static_cast<quint16>(p));
}

void MainWindow::on_buttonPublish_clicked()
{
    if (m_client->publish(ui->pub_topic_cbox->currentText(), ui->lineEditMessage->text().toUtf8()) == -1)
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not publish message"));
}

void MainWindow::on_buttonSubscribe_clicked()
{
    auto subscription = m_client->subscribe(ui->sub_topic_cbox->currentText());
    if (!subscription) {
        QMessageBox::critical(this, QLatin1String("Error"), QLatin1String("Could not subscribe. Is there a valid connection?"));
        return;
    }
}

void MainWindow::on_buttonPing_clicked()
{
    ui->buttonPing->setEnabled(false);
    m_client->requestPing();
}
