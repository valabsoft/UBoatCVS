#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _model = new UBoatModel();  // Дата-модель - не используется
    _cameraStatus = ConnectionStatus::OFF;
    _packetStatus = ConnectionStatus::OFF;

    // Заголовок окна
    setWindowTitle("БЭК СТЗ :: AРМ Оператора :: " + _appSet.getAppVersion());

    setGeometry();          // Геометрия окон
    setStyle(Theme::BLACK); // Установка темы приложения
    setButtonIcons();       // Установка иконок

    // Сигналы
    connect(ui->pbCamera, &QPushButton::clicked, this, &MainWindow::onCameraButtonClicked);
    connect(ui->pbPacket, &QPushButton::clicked, this, &MainWindow::onPacketButtonClicked);

    connect(this, &MainWindow::cameraStatusChanged, this, &MainWindow::onCameraStatusChanged);
    connect(this, &MainWindow::packetStatusChanged, this, &MainWindow::onPacketStatusChanged);    
}

MainWindow::~MainWindow()
{
    delete _model;
    delete ui;
}

void MainWindow::setButtonIcons()
{
    ui->pbCamera->setIcon(QIcon(":/img/button_off_icon.png"));
    ui->pbCamera->setIconSize(QSize(64, 64));

    ui->pbPacket->setIcon(QIcon(":/img/button_off_icon.png"));
    ui->pbPacket->setIconSize(QSize(64, 64));

    ui->pbSettings->setStyleSheet(
        ":active { border-image: url(:/img/button_settings_icon.png); }"
        ":hover { border-image: url(:/img/button_settings_mouseover_icon.png); }");
    ui->pbSetTargets->setStyleSheet(
        ":active { border-image: url(:/img/button_video_icon.png); }"
        ":hover { border-image: url(:/img/button_video_mouseover_icon.png); }");
    ui->pbResetTargets->setStyleSheet(
        ":active { border-image: url(:/img/button_trash_icon.png); }"
        ":hover { border-image: url(:/img/button_trash_mouseover_icon.png); }");
    ui->pbPhoto->setStyleSheet(
        ":active { border-image: url(:/img/button_camera_icon.png); }"
        ":hover { border-image: url(:/img/button_camera_mouseover_icon.png); }");
    ui->pbPaperplane->setStyleSheet(
        ":active { border-image: url(:/img/button_paperplane_paper plane_icon.png); }"
        ":hover { border-image: url(:/img/button_paperplane_paper plane_mouseover_icon.png); }");
    ui->pbEarth->setStyleSheet(
        ":active { border-image: url(:/img/button_earth_globe_internet_browser_world_icon.png); }"
        ":hover { border-image: url(:/img/button_earth_globe_internet_browser_world_mouse_over_icon.png); }");
    ui->pbLocation->setStyleSheet(
        ":active { border-image: url(:/img/button_location_map_pin_icon.png); }"
        ":hover { border-image: url(:/img/button_location_map_pin_mouseover_icon.png); }");
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

void MainWindow::setStyle(Theme theme) {
    QFont fontLabel("GOST type A", 18, QFont::Bold);

    switch(theme)
    {

    case Theme::WHITE:
    case Theme::BLACK:
        // Цвет фона главного окна приложения
        this->setStyleSheet("background-color: black;");

        ui->labelCameraView->setStyleSheet("QLabel {"
                                           "border-style: solid;"
                                           "border-width: 1px;"
                                           "border-color: silver;"
                                           "}");

        ui->lbCamera->setStyleSheet("background-color : black; color : silver;");
        ui->lbCamera->setFont(fontLabel);

        ui->lbPacket->setStyleSheet("background-color : black; color : silver;");
        ui->lbPacket->setFont(fontLabel);

        ui->lbSettings->setStyleSheet("background-color : black; color : silver;");
        ui->lbSettings->setFont(fontLabel);

        break;

    default:
        break;
    }


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
    if (_cameraStatus == ConnectionStatus::OFF)
    {
        _cameraStatus = ConnectionStatus::ON;
    }
    else
    {
        _cameraStatus = ConnectionStatus::OFF;
    }

    switch (_cameraStatus)
    {
    case ConnectionStatus::ON:
        //setCameraSatatus(ConnectionStatus::OFF);
        ui->pbCamera->setIcon(QIcon(":/img/button_on_icon.png"));
        ui->pbCamera->setIconSize(QSize(64, 64));
        break;
    case ConnectionStatus::OFF:
        //setCameraSatatus(ConnectionStatus::ON);
        ui->pbCamera->setIcon(QIcon(":/img/button_off_icon.png"));
        ui->pbCamera->setIconSize(QSize(64, 64));
        break;
    case ConnectionStatus::UNKNOWN:
        break;
    }
}

void MainWindow::onPacketButtonClicked()
{
    if (_packetStatus == ConnectionStatus::OFF)
    {
        _packetStatus = ConnectionStatus::ON;
    }
    else
    {
        _packetStatus = ConnectionStatus::OFF;
    }

    switch (_packetStatus)
    {
    case ConnectionStatus::ON:
        //setPacketSatatus(ConnectionStatus::ON);
        ui->pbPacket->setIcon(QIcon(":/img/button_on_icon.png"));
        ui->pbPacket->setIconSize(QSize(64, 64));
        break;
    case ConnectionStatus::OFF:
        //setPacketSatatus(ConnectionStatus::OFF);
        ui->pbPacket->setIcon(QIcon(":/img/button_off_icon.png"));
        ui->pbPacket->setIconSize(QSize(64, 64));
        break;
    case ConnectionStatus::UNKNOWN:
        break;
    }
}

void MainWindow::onCameraStatusChanged()
{
    ConnectionStatus status = this->CameraSatatus();

    //if (status == ConnectionStatus::ON)
    //{
    //    QMessageBox msgBox;
    //    msgBox.setText("ON");
    //    msgBox.exec();
    //}
    //else
    //{
    //    QMessageBox msgBox;
    //    msgBox.setText("OFF");
    //    msgBox.exec();
    //}
}

void MainWindow::onPacketStatusChanged()
{
    ConnectionStatus status = this->PacketSatatus();

    //if (status == ConnectionStatus::ON)
    //{
    //    QMessageBox msgBox;
    //    msgBox.setText("ON");
    //    msgBox.exec();
    //}
    //else
    //{
    //    QMessageBox msgBox;
    //    msgBox.setText("OFF");
    //    msgBox.exec();
    //}
}
