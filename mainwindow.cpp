#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connected = false;
    camera = new QCamera();

    qDebug() << "Number of cameras found:" << QCameraInfo::availableCameras().count();

    QList<QCameraInfo>  cameras = QCameraInfo::availableCameras();

    foreach (const QCameraInfo &cameraInfo, cameras)
    {
        qDebug() << "Camera info:" << cameraInfo.deviceName() << cameraInfo.description() << cameraInfo.position();

        ui->deviceSelection->addItem(cameraInfo.description());
    }
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_Connect_clicked()
{
    if(!connected)
    {
        connectCamera();
    }
    else
    {
        camera->stop();
        viewfinder->deleteLater();
         ui->pushButton_Connect->setText("Connect");
        connected = false;

    }

}

void MainWindow::connectCamera()
{
    QList<QCameraInfo> cameras = QCameraInfo::availableCameras();
    foreach (const QCameraInfo &cameraInfo, cameras)
    {
        qDebug() << cameraInfo.description() << ui->deviceSelection->currentText();

        if (cameraInfo.description() == ui->deviceSelection->currentText())
        {
            camera = new QCamera(cameraInfo);
            viewfinder = new QCameraViewfinder(this);
            camera->setViewfinder(viewfinder);
            ui->webcamLayout->addWidget(viewfinder);
            connected = true;
            ui->pushButton_Connect->setText("Disconnect");
            camera->start();

            return;

        }
    }
}



void MainWindow::on_pushButton_Capture_clicked()
{
    if (connected)
    {
        imageCapture = new QCameraImageCapture(camera);
        camera->setCaptureMode(QCamera::CaptureStillImage);
        camera->searchAndLock();
        imageCapture->capture(qApp->applicationDirPath());
        camera->unlock();
    }
}


void MainWindow::on_pushButton_Record_clicked()
{
    if (connected)
    {
        if(!recording)
        {
            recorder = new QMediaRecorder(camera);
            camera->setCaptureMode(QCamera::CaptureVideo);
            recorder->setOutputLocation(QUrl(qApp->applicationDirPath()));
            recorder->record();
            recording = true;
        }
        else
        {
            recorder->stop();
            recording = false;
        }
    }
}

