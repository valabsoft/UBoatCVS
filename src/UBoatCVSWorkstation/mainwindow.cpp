#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Заголовок окна
    setWindowTitle("БЭК СТЗ :: AРМ Оператора :: " + _appSet.getAppVersion());

    setGeometry();

    ui->labelCameraView->setStyleSheet("QLabel {"
                                       "border-style: solid;"
                                       "border-width: 1px;"
                                       "border-color: blue;"
                                       "}");

    connect(ui->pbCamera, &QPushButton::clicked, this, &MainWindow::onCameraButtonClicked);
    connect(ui->pbPacket, &QPushButton::clicked, this, &MainWindow::onPacketButtonClicked);

    connect(this, &MainWindow::cameraStatusChanged, this, &MainWindow::onCameraStatusChanged);
    connect(this, &MainWindow::packetStatusChanged, this, &MainWindow::onPacketStatusChanged);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setGeometry()
{
    // Расчет размеров главного окна
    int mainWindowW = _appSet.BORDER_SIZE * 3 +
                      _appSet.CAMERA_WIDTH + _appSet.PANEL_SIZE;
    int mainWindowH = _appSet.BORDER_SIZE * 3 +
                      _appSet.CAMERA_HEIGHT + _appSet.PANEL_SIZE;

    // Фиксируем размер окна и убираем иконку ресайза
    setFixedSize(QSize(mainWindowW, mainWindowH));

    // Позиционирование панелей
    ui->groupBoxHorizontal->setGeometry(
        _appSet.BORDER_SIZE,
        _appSet.BORDER_SIZE * 2 + _appSet.CAMERA_HEIGHT,
        _appSet.CAMERA_WIDTH,
        _appSet.PANEL_SIZE);

    ui->groupBoxVertical->setGeometry(
        _appSet.BORDER_SIZE * 2 + _appSet.CAMERA_WIDTH,
        _appSet.BORDER_SIZE,
        _appSet.PANEL_SIZE,
        _appSet.CAMERA_HEIGHT + _appSet.BORDER_SIZE + _appSet.PANEL_SIZE);

    // Позиционируем лейбл для вывода изображения
    ui->labelCameraView->setGeometry(
        _appSet.BORDER_SIZE,
        _appSet.BORDER_SIZE,
        _appSet.CAMERA_WIDTH,
        _appSet.CAMERA_HEIGHT);

    // Позиционирование Layouts
    ui->horizontalLayout->setContentsMargins(10, 10, 10, 10);
    ui->horizontalLayout->setSpacing(_appSet.BORDER_SIZE);
    ui->groupBoxHorizontal->setLayout(ui->horizontalLayout);

    ui->verticalLayout->setContentsMargins(10, 10, 10, 10);
    ui->verticalLayout->setSpacing(_appSet.BORDER_SIZE);
    ui->groupBoxVertical->setLayout(ui->verticalLayout);

    moveWindowToCenter();
}

void MainWindow::moveWindowToCenter()
{
    auto primaryScreen = QGuiApplication::primaryScreen(); // Главный экран
    QRect primaryScreenRect = primaryScreen->availableGeometry(); // Размер главного экрана
    QPoint primaryScreenRectCenter = primaryScreenRect.center();
    primaryScreenRectCenter.setX(primaryScreenRectCenter.x() - (this->width()/2));
    primaryScreenRectCenter.setY(primaryScreenRectCenter.y() - (this->height()/2));
    move(primaryScreenRectCenter);
}

void MainWindow::setCameraSatatus(ConnectionStatus cameraStatus)
{
    if (_cameraStatus == cameraStatus)
        return;

    _cameraStatus = cameraStatus;
    emit cameraStatusChanged(cameraStatus);
}

void MainWindow::setPacketSatatus(ConnectionStatus packetStatus)
{
    if (_packetStatus == packetStatus)
        return;

    _packetStatus = packetStatus;
    emit packetStatusChanged(packetStatus);
}

MainWindow::ConnectionStatus MainWindow::CameraSatatus() const
{
    return _cameraStatus;
}

MainWindow::ConnectionStatus MainWindow::PacketSatatus() const
{
    return _packetStatus;
}

void MainWindow::onCameraButtonClicked()
{
    setCameraSatatus(ConnectionStatus::ON);
}

void MainWindow::onPacketButtonClicked()
{
    ;
}

void MainWindow::onCameraStatusChanged()
{
    ConnectionStatus status = this->CameraSatatus();

    if (status == ConnectionStatus::ON)
    {
        QMessageBox msgBox;
        msgBox.setText("ON");
        msgBox.exec();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("OFF");
        msgBox.exec();
    }
}

void MainWindow::onPacketStatusChanged()
{
    ConnectionStatus status = this->CameraSatatus();

    if (status == ConnectionStatus::ON)
    {
        QMessageBox msgBox;
        msgBox.setText("ON");
        msgBox.exec();
    }
    else
    {
        QMessageBox msgBox;
        msgBox.setText("OFF");
        msgBox.exec();
    }
}
