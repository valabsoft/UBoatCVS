#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _model = new UBoatModel();

    // Заголовок окна
    setWindowTitle("БЭК СТЗ :: AРМ Оператора :: " + _appSet.getAppVersion());

    setGeometry();          // Геометрия окон
    setStyle(Theme::BLACK); // Установка темы приложения
    setButtonIcons();       // Установка иконок

    _videoTimer = new QTimer(this);
    connect(_videoTimer, &QTimer::timeout, this, &MainWindow::onVideoTimer);

    _capture = nullptr;

    // Сигналы
    connect(ui->pbCamera, &QPushButton::clicked, this, &MainWindow::onCameraButtonClicked);
    connect(ui->pbPacket, &QPushButton::clicked, this, &MainWindow::onPacketButtonClicked);

    connect(this, &MainWindow::cameraStatusChanged, this, &MainWindow::onCameraStatusChanged);
    connect(this, &MainWindow::packetStatusChanged, this, &MainWindow::onPacketStatusChanged);    
}

MainWindow::~MainWindow()
{
    if (!_videoTimer->isActive())
        _videoTimer->stop();

    if (_videoTimer)
        delete _videoTimer;

    if (_capture)
        delete _capture;

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
    int mainWindowW = _appSet.BORDER_SIZE * 4 +
                      _appSet.CAMERA_WIDTH + _appSet.PANEL_TOOL_SIZE + _appSet.PANEL_INFO_SIZE;
    int mainWindowH = _appSet.BORDER_SIZE * 3 +
                      _appSet.CAMERA_HEIGHT + _appSet.PANEL_TOOL_SIZE;

    // Фиксируем размер окна и убираем иконку ресайза
    setFixedSize(QSize(mainWindowW, mainWindowH));

    // Позиционирование панелей
    ui->groupBoxInfo->setGeometry(
        _appSet.BORDER_SIZE,
        _appSet.BORDER_SIZE,
        _appSet.PANEL_INFO_SIZE,
        _appSet.CAMERA_HEIGHT + _appSet.BORDER_SIZE + _appSet.PANEL_TOOL_SIZE);

    ui->groupBoxHorizontal->setGeometry(
        _appSet.BORDER_SIZE * 2 + _appSet.PANEL_INFO_SIZE,
        _appSet.BORDER_SIZE * 2 + _appSet.CAMERA_HEIGHT,
        _appSet.CAMERA_WIDTH,
        _appSet.PANEL_TOOL_SIZE);

    ui->groupBoxVertical->setGeometry(
        _appSet.BORDER_SIZE * 3 + _appSet.CAMERA_WIDTH + _appSet.PANEL_INFO_SIZE,
        _appSet.BORDER_SIZE,
        _appSet.PANEL_TOOL_SIZE,
        _appSet.CAMERA_HEIGHT + _appSet.BORDER_SIZE + _appSet.PANEL_TOOL_SIZE);

    // Позиционируем лейбл для вывода изображения
    ui->labelCameraView->setGeometry(
        _appSet.BORDER_SIZE * 2 + _appSet.PANEL_INFO_SIZE,
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
    QFont fontLabelTitle("GOST type A", 20, QFont::Bold);
    QFont fontLabelSmall("GOST type A", 18, QFont::Bold);
    QFont fontLabelBig("GOST type B", 54, QFont::Bold);

    switch(theme)
    {

    case Theme::WHITE:
    {
        ui->labelCameraView->setStyleSheet("QLabel {"
                                           "border-style: solid;"
                                           "border-width: 1px;"
                                           "border-color: dimgrey;"
                                           "color : dimgrey;"
                                           "}");
        ui->labelCameraView->setFont(fontLabel);

        ui->lbCamera->setStyleSheet("color : dimgrey;");
        ui->lbCamera->setFont(fontLabel);

        ui->lbPacket->setStyleSheet("color : dimgrey;");
        ui->lbPacket->setFont(fontLabel);

        ui->lbSettings->setStyleSheet("color : dimgrey;");
        ui->lbSettings->setFont(fontLabel);

        ui->lbReset->setStyleSheet("color : dimgrey;");
        ui->lbReset->setFont(fontLabel);
        break;
    }

    case Theme::BLACK:
        // Цвет фона главного окна приложения
        this->setStyleSheet("background-color: black;");

        ui->labelCameraView->setStyleSheet("QLabel {"
                                           "border-style: solid;"
                                           "border-width: 1px;"
                                           "border-color: silver;"
                                           "color : silver;"
                                           "}");        
        ui->labelCameraView->setFont(fontLabel);

        ui->lbCamera->setStyleSheet("background-color : black; color : silver;");
        ui->lbCamera->setFont(fontLabel);

        ui->lbPacket->setStyleSheet("background-color : black; color : silver;");
        ui->lbPacket->setFont(fontLabel);

        ui->lbSettings->setStyleSheet("background-color : black; color : silver;");
        ui->lbSettings->setFont(fontLabel);

        ui->lbReset->setStyleSheet("background-color : black; color : silver;");
        ui->lbReset->setFont(fontLabel);

        ui->lbInfoPanelTitle->setStyleSheet("background-color : black; color : silver;");
        ui->lbInfoPanelTitle->setFont(fontLabelTitle);

        ui->lbInfoPanelTargetTotal->setStyleSheet("background-color : black; color : dimgrey;");
        ui->lbInfoPanelTargetTotal->setFont(fontLabelSmall);

        ui->lbInfoPanelTargetTotalValue->setStyleSheet("background-color : black; color : '#3C66D9';");
        ui->lbInfoPanelTargetTotalValue->setFont(fontLabelBig);

        ui->lbInfoPanelTargetActive->setStyleSheet("background-color : black; color : dimgrey;");
        ui->lbInfoPanelTargetActive->setFont(fontLabelSmall);

        ui->lbInfoPanelTargetActiveValue->setStyleSheet("background-color : black; color : '#4CAF50';");
        ui->lbInfoPanelTargetActiveValue->setFont(fontLabelBig);

        ui->lbInfoPanelSpeed->setStyleSheet("background-color : black; color : dimgrey;");
        ui->lbInfoPanelSpeed->setFont(fontLabelSmall);

        ui->lbInfoPanelSpeedValue->setStyleSheet("background-color : black; color : silver;");
        ui->lbInfoPanelSpeedValue->setFont(fontLabelBig);

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
    if (_model->getCameraStatus() == cameraStatus)
        return;

    _model->setCameraStatus(cameraStatus);
    emit cameraStatusChanged(cameraStatus);
}

void MainWindow::setPacketSatatus(ConnectionStatus packetStatus)
{
    if (_model->getPacketStatus() == packetStatus)
        return;

    _model->setPacketStatus(packetStatus);
    emit packetStatusChanged(packetStatus);
}

void MainWindow::onCameraButtonClicked()
{
    switch (_model->getCameraStatus())
    {
    case ConnectionStatus::ON:
        setCameraSatatus(ConnectionStatus::OFF);
        ui->pbCamera->setIcon(QIcon(":/img/button_off_icon.png"));
        ui->pbCamera->setIconSize(QSize(64, 64));
        break;
    case ConnectionStatus::OFF:
        setCameraSatatus(ConnectionStatus::ON);
        ui->pbCamera->setIcon(QIcon(":/img/button_on_icon.png"));
        ui->pbCamera->setIconSize(QSize(64, 64));
        break;
    case ConnectionStatus::UNKNOWN:
        break;
    }
}

void MainWindow::onPacketButtonClicked()
{
    switch (_model->getPacketStatus())
    {
    case ConnectionStatus::ON:
        setPacketSatatus(ConnectionStatus::OFF);
        ui->pbPacket->setIcon(QIcon(":/img/button_off_icon.png"));
        ui->pbPacket->setIconSize(QSize(64, 64));
        break;
    case ConnectionStatus::OFF:
        setPacketSatatus(ConnectionStatus::ON);
        ui->pbPacket->setIcon(QIcon(":/img/button_on_icon.png"));
        ui->pbPacket->setIconSize(QSize(64, 64));
        break;
    case ConnectionStatus::UNKNOWN:
        break;
    }
}

void MainWindow::onCameraStatusChanged()
{
    switch (_model->getCameraStatus()) {
    case ConnectionStatus::OFF:
        qDebug() << "onCameraStatusChanged(): OFF";

        // Остановка таймера
        if (_videoTimer->isActive())
        {
            _videoTimer->stop();
            qDebug() << "Видеотаймер остановлен";
        }

        // Освобождение камеры
        if (_capture)
        {
            _capture->release();
            _capture = nullptr;
            qDebug() << "Дескриптор камеры освобожден";
        }

        break;
    case ConnectionStatus::ON:
        qDebug() << "onCameraStatusChanged(): ON";

        // Захват камеры
        if (!_capture)
        {
            _capture = new cv::VideoCapture(_appSet.CAMERA_ID, cv::CAP_DSHOW);
            // Настраиваем параметры камеры
            _capture->set(cv::CAP_PROP_FRAME_WIDTH, _appSet.CAMERA_WIDTH);
            _capture->set(cv::CAP_PROP_FRAME_HEIGHT, _appSet.CAMERA_HEIGHT);
            _capture->set(cv::CAP_PROP_FPS, _appSet.CAMERA_FPS);

            qDebug() << "Дескриптор камеры создан";

            if (!_capture->isOpened())
            {
                QMessageBox::critical(this, "Ошибка", "Не удалось открыть камеру!");
                delete _capture;
                _capture = nullptr;
                return;
            }
        }

        // Запускаем таймер
        if (!_videoTimer->isActive())
        {
            _videoTimer->start(_appSet.VIDEO_TIMER_INTERVAL);
            qDebug() << "Видеотаймер запущен";
        }

        break;
    default:
        break;
    }
}

void MainWindow::onPacketStatusChanged()
{
    switch (_model->getPacketStatus()) {
    case ConnectionStatus::OFF:
        qDebug() << "onPacketStatusChanged(): OFF";
        break;
    case ConnectionStatus::ON:
        qDebug() << "onPacketStatusChanged(): ON";
        break;
    default:
        break;
    }
}

void MainWindow::onVideoTimer()
{
    if (!_capture || !_capture->isOpened())
    {
        return;
    }

    cv::Mat frame;
    *_capture >> frame;

    if (frame.empty())
    {
        return;
    }

    // Конвертация BGR в RGB для Qt
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

    QImage image = cvMatToQImage(frame);
    ui->labelCameraView->setPixmap(QPixmap::fromImage(image));

}

QImage MainWindow::cvMatToQImage(const cv::Mat &mat)
{
    if (mat.type() == CV_8UC3) {
        return QImage(mat.data, mat.cols, mat.rows,
                      static_cast<int>(mat.step), QImage::Format_RGB888);
    }
    else if (mat.type() == CV_8UC1) {
        return QImage(mat.data, mat.cols, mat.rows,
                      static_cast<int>(mat.step), QImage::Format_Grayscale8);
    }

    return QImage();
}
