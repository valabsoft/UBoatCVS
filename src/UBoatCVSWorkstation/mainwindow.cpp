#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    _model = new UBoatModel();
    _appSet.load(); // Загрузка файла настроек

    // Заголовок окна
    setWindowTitle("БЭК СТЗ :: AРМ Оператора :: " + _appSet.getAppVersion());

    // Иконка главного окна
    setWindowIcon(QIcon(":/img/sight.png"));

    setGeometry();          // Геометрия окон
    setStyle(Theme::WHITE); // Установка темы приложения
    setButtonIcons();       // Установка иконок

    _videoTimer = new QTimer(this);
    connect(_videoTimer, &QTimer::timeout, this, &MainWindow::onVideoTimer);

    _capture = nullptr;

    // SetupUDP
    _udpSocket = nullptr;
    _videoCaptureThread = nullptr;


    // Сигналы
    connect(ui->pbCamera, &QPushButton::clicked, this, &MainWindow::onCameraButtonClicked);
    connect(ui->pbPacket, &QPushButton::clicked, this, &MainWindow::onPacketButtonClicked);
    connect(ui->pbResetTargets, &QPushButton::clicked, this, &MainWindow::onResetButtonClicked);
    connect(ui->pbSettings, &QPushButton::clicked, this, &MainWindow::onSettingsButtonClicked);

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
    ui->groupBoxVerticalInfo->setGeometry(
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

    ui->verticalLayoutInfo->setContentsMargins(10, 10, 10, 10);
    ui->verticalLayoutInfo->setSpacing(_appSet.BORDER_SIZE);
    ui->groupBoxVerticalInfo->setLayout(ui->verticalLayoutInfo);

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

        ui->lbInfoPanelTitle->setStyleSheet("color : dimgrey;");
        ui->lbInfoPanelTitle->setFont(fontLabelTitle);

        ui->lbInfoPanelTargetTotal->setStyleSheet("color : dimgrey;");
        ui->lbInfoPanelTargetTotal->setFont(fontLabelSmall);

        ui->lbInfoPanelTargetTotalValue->setStyleSheet("color : '#3C66D9';");
        ui->lbInfoPanelTargetTotalValue->setFont(fontLabelBig);

        ui->lbInfoPanelTargetActive->setStyleSheet("color : dimgrey;");
        ui->lbInfoPanelTargetActive->setFont(fontLabelSmall);

        ui->lbInfoPanelTargetActiveValue->setStyleSheet("color : '#4CAF50';");
        ui->lbInfoPanelTargetActiveValue->setFont(fontLabelBig);

        ui->lbInfoPanelSpeed->setStyleSheet("color : dimgrey;");
        ui->lbInfoPanelSpeed->setFont(fontLabelSmall);

        ui->lbInfoPanelSpeedValue->setStyleSheet("color : silver;");
        ui->lbInfoPanelSpeedValue->setFont(fontLabelBig);

        // Стиль окна терминала
        ui->plainTextEdit->setReadOnly(true);
        ui->plainTextEdit->setFont(QFont("Consolas", 10)); // Моноширинный шрифт
        //ui->plainTextEdit->setStyleSheet(
        //    "QPlainTextEdit {"
        //    "    background-color: #000000;"
        //    "    color: #00FF00;"
        //    "    border: 1px solid #333;"
        //    "    font-family: 'Courier New', monospace;"
        //    "    selection-background-color: #555;"
        //    "}"
        //    );

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

        // Стиль окна терминала
        ui->plainTextEdit->setReadOnly(true);
        ui->plainTextEdit->setFont(QFont("Consolas", 10)); // Моноширинный шрифт
        ui->plainTextEdit->setStyleSheet(
            "QPlainTextEdit {"
            "    background-color: #000000;"
            "    color: #00FF00;"
            "    border: 1px solid #333;"
            "    font-family: 'Courier New', monospace;"
            "    selection-background-color: #555;"
            "}"
            );

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
        setUDPConnection(ConnectionStatus::OFF);
        ui->pbPacket->setIcon(QIcon(":/img/button_off_icon.png"));
        ui->pbPacket->setIconSize(QSize(64, 64));
        break;
    case ConnectionStatus::OFF:
        setPacketSatatus(ConnectionStatus::ON);
        setUDPConnection(ConnectionStatus::ON);
        ui->pbPacket->setIcon(QIcon(":/img/button_on_icon.png"));
        ui->pbPacket->setIconSize(QSize(64, 64));
        break;
    case ConnectionStatus::UNKNOWN:
        break;
    }
}

void MainWindow::onResetButtonClicked()
{
    clearTerminal();
    terminalError("Выполнен сброс");
}

bool MainWindow::tryOpenVideoCapture()
{
    try
    {
        if (_videoCaptureThread && _videoCaptureThread->isRunning())
        {
            return false;
        }

        int port = _appSet.PORT_VIDEO;
        std::string pipeline = "udpsrc port=" + std::to_string(port) + " buffer-size=524288 ! "
                                                                       "application/x-rtp,encoding-name=H264,payload=96 ! "
                                                                       "rtph264depay ! h264parse ! avdec_h264 ! videoconvert ! "
                                                                       "video/x-raw,format=BGR ! appsink drop=1 max-buffers=1 sync=false drop=true";

        _videoCaptureThread = new VideoCaptureThread(pipeline, this);
        connect(_videoCaptureThread, &VideoCaptureThread::frameReady, this, &MainWindow::updateImage);
        connect(_videoCaptureThread, &VideoCaptureThread::captureError, this, &MainWindow::handleCaptureError);
        connect(_videoCaptureThread, &VideoCaptureThread::captureOpened, this, &MainWindow::handleCaptureOpened);
        connect(_videoCaptureThread, &VideoCaptureThread::finished, _videoCaptureThread, &QObject::deleteLater);

        _videoCaptureThread->start();
        return true;
    }
    catch (...)
    {
        return false;
    }
    return false;
}

void MainWindow::handleCaptureError(const QString &error)
{
    terminalError(error);
    qDebug() << error;

    if (_videoCaptureThread)
    {
        _videoCaptureThread->stop();
        _videoCaptureThread->deleteLater();
        _videoCaptureThread = nullptr;

        qDebug() << "Дескриптор видеопотока освобожден";
        terminalError("Дескриптор видеопотока освобожден");
    }
}

void MainWindow::handleCaptureOpened()
{
    terminalInfo("Получение видеопотока на порту " + QString::number(_appSet.PORT_VIDEO));
    terminalWarning("Видеопоток успешно открыт");
    qDebug() << "Получение видеопотока на порту " + QString::number(_appSet.PORT_VIDEO);
    qDebug() << "Видеопоток успешно открыт";
}

void MainWindow::onCameraStatusChanged()
{
    if (_appSet.TEST_MODE)
    {
        switch (_model->getCameraStatus()) {
        case ConnectionStatus::OFF:
            qDebug() << "onCameraStatusChanged(): OFF";
            terminalInfo("onCameraStatusChanged(): OFF");

            // Остановка таймера
            if (_videoTimer->isActive())
            {
                _videoTimer->stop();
                qDebug() << "Видеотаймер остановлен";
                terminalWarning("Видеотаймер остановлен");
            }

            // Освобождение камеры
            if (_capture)
            {
                _capture->release();
                _capture = nullptr;
                qDebug() << "Дескриптор камеры освобожден";
                terminalWarning("Дескриптор камеры освобожден");
            }

            break;
        case ConnectionStatus::ON:
            qDebug() << "onCameraStatusChanged(): ON";
            terminalInfo("onCameraStatusChanged(): ON");

            // Захват камеры
            if (!_capture)
            {
                if (!_appSet.TEST_MODE)
                {
                    _capture = new cv::VideoCapture(_appSet.CAMERA_ID, cv::CAP_DSHOW);
                    // Настраиваем параметры камеры
                    _capture->set(cv::CAP_PROP_FRAME_WIDTH, _appSet.CAMERA_WIDTH);
                    _capture->set(cv::CAP_PROP_FRAME_HEIGHT, _appSet.CAMERA_HEIGHT);
                    _capture->set(cv::CAP_PROP_FPS, _appSet.CAMERA_FPS);
                    _fps = _capture->get(cv::CAP_PROP_FPS);
                }
                else
                {
                    _capture = new cv::VideoCapture("C:\\VID_20250501_082326_854_480.mp4", cv::CAP_FFMPEG);
                    _fps = _capture->get(cv::CAP_PROP_FPS);
                    _totalFrames = _capture->get(cv::CAP_PROP_FRAME_COUNT);
                }


                qDebug() << "Дескриптор камеры создан";
                terminalWarning("Дескриптор камеры создан");

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
                if (!_appSet.TEST_MODE)
                {
                    // _videoTimer->start(_appSet.VIDEO_TIMER_INTERVAL);
                    _videoTimer->start((int)(1000 / _fps));
                }
                else
                {
                    _videoTimer->start((int)(1000 / _fps));
                }
                qDebug() << "Видеотаймер запущен";
                terminalWarning("Видеотаймер запущен");
            }

            break;
        default:
            break;
        }
    }
    else
    {
        switch (_model->getCameraStatus())
        {
        case ConnectionStatus::ON:
            if (!tryOpenVideoCapture())
            {
                qDebug() << "Дескриптор видеопотока НЕ создан";
                terminalError("Дескриптор видеопотока НЕ создан");
            }
            else
            {
                if (_videoCaptureThread)
                {
                    qDebug() << "Дескриптор видеопотока создан";
                    terminalWarning("Дескриптор видеопотока создан");
                }
            }
            break;
        case ConnectionStatus::OFF:
            if (_videoCaptureThread)
            {
                _videoCaptureThread->stop();
                delete _videoCaptureThread;

                ////////////////////////////////////////////////////////////////
                // Почему не так?
                // _videoCaptureThread->stop();
                // _videoCaptureThread->deleteLater();
                // _videoCaptureThread = nullptr;
                ////////////////////////////////////////////////////////////////

                qDebug() << "Дескриптор видеопотока освобожден";
                terminalWarning("Дескриптор видеопотока освобожден");
            }
            break;
        default:
            break;
        }
    }
}

void MainWindow::onPacketStatusChanged()
{
    switch (_model->getPacketStatus()) {
    case ConnectionStatus::OFF:
        qDebug() << "onPacketStatusChanged(): OFF";
        terminalInfo("onPacketStatusChanged(): OFF");
        break;
    case ConnectionStatus::ON:
        qDebug() << "onPacketStatusChanged(): ON";
        terminalInfo("onPacketStatusChanged(): ON");
        break;
    default:
        break;
    }
}

void MainWindow::drawGraphicalObjects(cv::Mat &frame)
{
    #pragma region Draw Graphical Objects
    cv::Mat overlayImage;
    cv::Mat transparencyiImage;

    // Конвертация BGR в RGB для Qt
    cv::cvtColor(frame, frame, cv::COLOR_BGR2RGB);

    frame.copyTo(overlayImage);

    int X0 = _appSet.CAMERA_WIDTH / 2;
    int Y0 = _appSet.CAMERA_HEIGHT / 2;

    if (_appSet.DRAW_SIGHT)
    {
        // Внешний контур прицела
        roundedRectangle(frame,
                         cv::Point(X0 - _appSet.SIGHT_SIZE, Y0 - _appSet.SIGHT_SIZE),
                         cv::Point(X0 + _appSet.SIGHT_SIZE, Y0 + _appSet.SIGHT_SIZE),
                         CV_RGB(0, 255, 255),
                         2,
                         cv::LINE_8,
                         10);

        // Рисочки внешнего контура
        cv::line(frame,
                 cv::Point(X0, Y0 - _appSet.SIGHT_SIZE),
                 cv::Point(X0, Y0 - _appSet.SIGHT_SIZE + _appSet.SIGHT_TICK),
                 CV_RGB(0, 255, 255),
                 1,
                 cv::LINE_8);
        cv::line(frame,
                 cv::Point(X0, Y0 + _appSet.SIGHT_SIZE),
                 cv::Point(X0, Y0 + _appSet.SIGHT_SIZE - _appSet.SIGHT_TICK),
                 CV_RGB(0, 255, 255),
                 1,
                 cv::LINE_8);
        cv::line(frame,
                 cv::Point(X0 - _appSet.SIGHT_SIZE, Y0 ),
                 cv::Point(X0 - _appSet.SIGHT_SIZE + _appSet.SIGHT_TICK, Y0),
                 CV_RGB(0, 255, 255),
                 1,
                 cv::LINE_8);
        cv::line(frame,
                 cv::Point(X0 + _appSet.SIGHT_SIZE, Y0 ),
                 cv::Point(X0 + _appSet.SIGHT_SIZE - _appSet.SIGHT_TICK, Y0),
                 CV_RGB(0, 255, 255),
                 1,
                 cv::LINE_8);

        // Рисочки внутреннего прицела
        cv::line(frame,
                 cv::Point(X0 - _appSet.SIGHT_DELTA, Y0),
                 cv::Point(X0 - _appSet.SIGHT_DELTA - _appSet.SIGHT_CROSS, Y0),
                 CV_RGB(255, 255, 255),
                 1,
                 cv::LINE_8);
        cv::line(frame,
                 cv::Point(X0 + _appSet.SIGHT_DELTA, Y0),
                 cv::Point(X0 + _appSet.SIGHT_DELTA + _appSet.SIGHT_CROSS, Y0),
                 CV_RGB(255, 255, 255),
                 1,
                 cv::LINE_8);

        cv::line(frame,
                 cv::Point(X0, Y0 - _appSet.SIGHT_DELTA),
                 cv::Point(X0, Y0 - _appSet.SIGHT_DELTA - _appSet.SIGHT_CROSS),
                 CV_RGB(255, 255, 255),
                 1,
                 cv::LINE_8);
        cv::line(frame,
                 cv::Point(X0, Y0 + _appSet.SIGHT_DELTA),
                 cv::Point(X0, Y0 + _appSet.SIGHT_DELTA + _appSet.SIGHT_CROSS),
                 CV_RGB(255, 255, 255),
                 1,
                 cv::LINE_8);
    }

    if (_appSet.DRAW_GRID)
    {
        ///////////////////////////////////////////////////////////////////////
        // Риски вертикальные (левые)
        ///////////////////////////////////////////////////////////////////////
        for (int i = 1; i < _appSet.GRID_V_MAX; i++)
        {
            cv::line(frame,
                     cv::Point(_appSet.XV0, _appSet.YV0 + _appSet.GRID_V_DELTA * 10 * (i - 1)),
                     cv::Point(_appSet.XV0 + _appSet.GRID_BIG_SIZE, _appSet.YV0 + _appSet.GRID_V_DELTA * 10 * (i - 1)),
                     CV_RGB(255, 255, 255),
                     2,
                     cv::LINE_8);

            for (int j = 1; j < 10; j++)
            {
                cv::line(frame,
                         cv::Point(_appSet.XV0 + _appSet.GRID_SMALL_SIZE, _appSet.YV0 + _appSet.GRID_V_DELTA * 10 * (i - 1) + j * _appSet.GRID_V_DELTA),
                         cv::Point(_appSet.XV0 + 2 * _appSet.GRID_SMALL_SIZE, _appSet.YV0 + _appSet.GRID_V_DELTA * 10 * (i - 1) + j * _appSet.GRID_V_DELTA),
                         CV_RGB(255, 255, 255),
                         1,
                         cv::LINE_8);
            }
        }
        // Завершающая
        cv::line(frame,
                 cv::Point(_appSet.XV0, _appSet.YV0 + _appSet.GRID_V_DELTA * 10 * (_appSet.GRID_V_MAX - 1)),
                 cv::Point(_appSet.XV0 + _appSet.GRID_BIG_SIZE, _appSet.YV0 + _appSet.GRID_V_DELTA * 10 * (_appSet.GRID_V_MAX - 1)),
                 CV_RGB(255, 255, 255),
                 2,
                 cv::LINE_8);


        ///////////////////////////////////////////////////////////////////////
        // Риски вертикальные (правые)
        ///////////////////////////////////////////////////////////////////////
        for (int i = 1; i < _appSet.GRID_V_MAX; i++)
        {
            cv::line(frame,
                     cv::Point(X0 + (X0 - _appSet.XV0) - 30, _appSet.YV0 + _appSet.GRID_V_DELTA * 10 * (i - 1)),
                     cv::Point(X0 + (X0 - _appSet.XV0) + _appSet.GRID_BIG_SIZE - 30, _appSet.YV0 + _appSet.GRID_V_DELTA * 10 * (i - 1)),
                     CV_RGB(255, 255, 255),
                     2,
                     cv::LINE_8);

            for (int j = 1; j < 10; j++)
            {
                cv::line(frame,
                         cv::Point(X0 + (X0 - _appSet.XV0) - 30, _appSet.YV0 + _appSet.GRID_V_DELTA * 10 * (i - 1) + j * _appSet.GRID_V_DELTA),
                         cv::Point(X0 + (X0 - _appSet.XV0) + _appSet.GRID_SMALL_SIZE - 30, _appSet.YV0 + _appSet.GRID_V_DELTA * 10 * (i - 1) + j * _appSet.GRID_V_DELTA),
                         CV_RGB(255, 255, 255),
                         1,
                         cv::LINE_8);
            }
        }
        // Завершающая
        cv::line(frame,
                 cv::Point(X0 + (X0 - _appSet.XV0) - 30, _appSet.YV0 + _appSet.GRID_V_DELTA * 10 * (_appSet.GRID_V_MAX - 1)),
                 cv::Point(X0 + (X0 - _appSet.XV0) + _appSet.GRID_BIG_SIZE - 30, _appSet.YV0 + _appSet.GRID_V_DELTA * 10 * (_appSet.GRID_V_MAX - 1)),
                 CV_RGB(255, 255, 255),
                 2,
                 cv::LINE_8);


        ///////////////////////////////////////////////////////////////////////
        // Риски горизонтальные (верх)
        ///////////////////////////////////////////////////////////////////////
        for (int i = 1; i < _appSet.GRID_H_MAX; i++)
        {
            cv::line(frame,
                     cv::Point(_appSet.XH0 + _appSet.GRID_H_DELTA * 10 * (i - 1), _appSet.YH0),
                     cv::Point(_appSet.XH0 + _appSet.GRID_H_DELTA * 10 * (i - 1), _appSet.YH0 + _appSet.GRID_BIG_SIZE),
                     CV_RGB(255, 255, 255),
                     2,
                     cv::LINE_8);

            for (int j = 1; j < 10; j++)
            {
                cv::line(frame,
                         cv::Point(_appSet.XH0 + _appSet.GRID_H_DELTA * 10 * (i - 1) + j * _appSet.GRID_H_DELTA, _appSet.YH0 + _appSet.GRID_SMALL_SIZE),
                         cv::Point(_appSet.XH0 + _appSet.GRID_H_DELTA * 10 * (i - 1) + j * _appSet.GRID_H_DELTA , _appSet.YH0 + 2 * _appSet.GRID_SMALL_SIZE),
                         CV_RGB(255, 255, 255),
                         1,
                         cv::LINE_8);
            }
        }
        // Завершающая
        cv::line(frame,
                 cv::Point(_appSet.XH0 + _appSet.GRID_H_DELTA * 10 * (_appSet.GRID_H_MAX - 1), _appSet.YH0),
                 cv::Point(_appSet.XH0 + _appSet.GRID_H_DELTA * 10 * (_appSet.GRID_H_MAX - 1), _appSet.YH0 + _appSet.GRID_BIG_SIZE),
                 CV_RGB(255, 255, 255),
                 2,
                 cv::LINE_8);

        ///////////////////////////////////////////////////////////////////////
        // Риски горизонтальные (низ)
        ///////////////////////////////////////////////////////////////////////
        for (int i = 1; i < _appSet.GRID_H_MAX; i++)
        {
            cv::line(frame,
                     cv::Point(_appSet.XH0 + _appSet.GRID_H_DELTA * 10 * (i - 1), Y0 + (Y0 - _appSet.YV0) + 30),
                     cv::Point(_appSet.XH0 + _appSet.GRID_H_DELTA * 10 * (i - 1), Y0 + (Y0 - _appSet.YV0) + _appSet.GRID_BIG_SIZE + 30),
                     CV_RGB(255, 255, 255),
                     2,
                     cv::LINE_8);

            for (int j = 1; j < 10; j++)
            {
                cv::line(frame,
                         cv::Point(_appSet.XH0 + _appSet.GRID_H_DELTA * 10 * (i - 1) + j * _appSet.GRID_H_DELTA, Y0 + (Y0 - _appSet.YV0) + 30),
                         cv::Point(_appSet.XH0 + _appSet.GRID_H_DELTA * 10 * (i - 1) + j * _appSet.GRID_H_DELTA , Y0 + (Y0 - _appSet.YV0) + _appSet.GRID_SMALL_SIZE + 30),
                         CV_RGB(255, 255, 255),
                         1,
                         cv::LINE_8);
            }
        }
        // Завершающая
        cv::line(frame,
                 cv::Point(_appSet.XH0 + _appSet.GRID_H_DELTA * 10 * (_appSet.GRID_H_MAX - 1), Y0 + (Y0 - _appSet.YV0) + 30),
                 cv::Point(_appSet.XH0 + _appSet.GRID_H_DELTA * 10 * (_appSet.GRID_H_MAX - 1), Y0 + (Y0 - _appSet.YV0) + _appSet.GRID_BIG_SIZE + 30),
                 CV_RGB(255, 255, 255),
                 2,
                 cv::LINE_8);
    }

    // Отрисовка BoundingBoxes
    for (const auto &box :  std::as_const(_boxesMap))
    {
        cv::Scalar color = box.selected ? cv::Scalar(0, 0, 255) : cv::Scalar(0, 255, 0);
        int thickness = box.selected ? 3 : 2;

        cv::rectangle(frame,
                      cv::Point(box.x, box.y),
                      cv::Point(box.x + box.w, box.y + box.h),
                      color, thickness);

        //std::string text = "ID:" + std::to_string(box.track_id) +
        //                   " Conf:" + std::to_string(box.confidence).substr(0, 4);

        //cv::putText(frame, text,
        //            cv::Point(box.x, box.y - 5),
        //            cv::FONT_HERSHEY_SIMPLEX, 0.5, color, 1);
    }

    // Склейка
    cv::addWeighted(overlayImage, _appSet.ALPHA, frame, 1 - _appSet.ALPHA, 0, transparencyiImage);

    _image = QImage((uchar*) transparencyiImage.data,
                    transparencyiImage.cols,
                    transparencyiImage.rows,
                    transparencyiImage.step,
                    QImage::Format_RGB888);
    #pragma endregion

    ui->labelCameraView->setPixmap(QPixmap::fromImage(_image));
    //QImage image = cvMatToQImage(frame);
    //ui->labelCameraView->setPixmap(QPixmap::fromImage(image));
}

void MainWindow::onVideoTimer()
{
    if (_appSet.TEST_MODE && _model->getPacketStatus() == ConnectionStatus::ON)
        return;

    if (!_capture || !_capture->isOpened())
    {
        return;
    }

    cv::Mat frame;
    *_capture >> frame; // Читаем следующий фрейм

    if (frame.empty())
    {
        return;
    }


    drawGraphicalObjects(frame);

}

void MainWindow::updateImage(cv::Mat &frame)
{
    if (frame.empty())
    {
        return;
    }

    drawGraphicalObjects(frame);

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

// Команды для работы с терминалом
void MainWindow::terminalMessage(const QString &message, const QString &color)
{
    QString formatted = QString("<span style='color: %1'>%2</span>")
    .arg(color, message.toHtmlEscaped());

    // Для большого объема текста лучше использовать QPlainTextEdit
    ui->plainTextEdit->appendHtml(formatted);

    // Автопрокрутка
    QTextCursor cursor = ui->plainTextEdit->textCursor();
    cursor.movePosition(QTextCursor::End);
    ui->plainTextEdit->setTextCursor(cursor);
}

void MainWindow::terminalWarning(const QString &command)
{
    terminalMessage("$ " + command, "#4CAF50"); // Зеленый для команд
}

void MainWindow::terminalInfo(const QString &output)
{
    terminalMessage("$ " + output, "#696969"); // Белый для вывода
}

void MainWindow::terminalError(const QString &error)
{
    terminalMessage("$ ERROR: " + error, "#990000"); // Красный для ошибок
}

void MainWindow::clearTerminal()
{
    ui->plainTextEdit->clear();
}

// Отрисовка графических элементов

void MainWindow::roundedRectangle(
    cv::Mat& src,
    cv::Point topLeft,
    cv::Point bottomRight,
    const cv::Scalar lineColor,
    const int thickness,
    const int lineType,
    const int cornerRadius)
{
    // Сorners:
    // p1 - p2
    // |     |
    // p4 - p3

    cv::Point p1 = topLeft;
    cv::Point p2 = cv::Point(bottomRight.x, topLeft.y);
    cv::Point p3 = bottomRight;
    cv::Point p4 = cv::Point(topLeft.x, bottomRight.y);

    // Draw Straight Lines
    cv::line(src, cv::Point(p1.x + cornerRadius, p1.y), cv::Point(p2.x - cornerRadius, p2.y), lineColor, thickness, lineType);
    cv::line(src, cv::Point(p2.x, p2.y + cornerRadius), cv::Point(p3.x, p3.y - cornerRadius), lineColor, thickness, lineType);
    cv::line(src, cv::Point(p4.x + cornerRadius, p4.y), cv::Point(p3.x-cornerRadius, p3.y), lineColor, thickness, lineType);
    cv::line(src, cv::Point(p1.x, p1.y + cornerRadius), cv::Point(p4.x, p4.y - cornerRadius), lineColor, thickness, lineType);

    // Draw Arcs
    cv::ellipse(src, p1 + cv::Point(cornerRadius, cornerRadius), cv::Size(cornerRadius, cornerRadius), 180.0, 0, 90, lineColor, thickness, lineType);
    cv::ellipse(src, p2 + cv::Point(-cornerRadius, cornerRadius), cv::Size(cornerRadius, cornerRadius), 270.0, 0, 90, lineColor, thickness, lineType);
    cv::ellipse(src, p3 + cv::Point(-cornerRadius, -cornerRadius), cv::Size(cornerRadius, cornerRadius), 0.0, 0, 90, lineColor, thickness, lineType);
    cv::ellipse(src, p4 + cv::Point(cornerRadius, -cornerRadius), cv::Size(cornerRadius, cornerRadius), 90.0, 0, 90, lineColor, thickness, lineType);
}

void MainWindow::onSettingsButtonClicked()
{
    // Если вызвать конструктор SettingsWindow(this),
    // то копируются стили главного окна, поэтому вызываем с NULL
    _settingsWindow = new SettingsWindow(NULL);

    // Центрировать инструментальную панель
    QRect screenGeometry = QGuiApplication::screens()[0]->geometry();
    int x = (screenGeometry.width() - _settingsWindow->width()) / 2;
    int y = (screenGeometry.height() - _settingsWindow->height()) / 2;

    _settingsWindow->setWindowTitle("БЭК СТЗ :: Настройки :: " + _appSet.getAppVersion());

    _settingsWindow->move(x, y);

    if (_settingsWindow->exec() == QDialog::Accepted)
    {
        _appSet.load();
    }

    delete _settingsWindow;
}

void MainWindow::setUDPConnection(ConnectionStatus connectionStatus)
{
    switch (connectionStatus)
    {

    case ON:
    {
        try
        {
            _udpSocket = new QUdpSocket(this);
        }
        catch (...)
        {
            qDebug() << "ERROR: Создание UDP сокета";
            terminalError("Создание UDP сокета");
        }

        if (_udpSocket)
        {
            qDebug() << "UDP сокет создан";
            terminalWarning("UDP сокет создан");

            try
            {
                if (!_udpSocket->bind(_appSet.PORT_DATA))
                {
                    terminalError("Не удалось выполнить привязку UDP сокета");
                }
                else
                {
                    qDebug() << "Привязка UDP сокета -- OK";
                    terminalInfo("Привязка UDP сокета -- OK");

                    connect(_udpSocket, &QUdpSocket::readyRead, this, &MainWindow::readPendingDatagrams);
                }
            } catch (...)
            {
                qDebug() << "ERROR: Привязка UDP сокета";
                terminalError("Привязка UDP сокета");
            }
        }

        break;
    }
    case OFF:
    {
        if (_udpSocket)
        {
            disconnect(_udpSocket, &QUdpSocket::readyRead, nullptr, nullptr);
            _udpSocket->close();

            qDebug() << "UDP сокет освобожден -- OK";
            terminalInfo("UDP сокет освобожден -- OK");

            delete _udpSocket;

            qDebug() << "UDP сокет уничтожен";
            terminalWarning("UDP сокет уничтожен");
        }

        break;
    }

    case UNKNOWN:
        break;
    }
}

void MainWindow::readPendingDatagrams()
{
    while (_udpSocket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(_udpSocket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;

        _udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        processUDPData(datagram);
    }
}

void MainWindow::processUDPData(const QByteArray &data)
{
    if (data.size() == _appSet.DATA_PACKET_SIZE)
    {
        // Передача пакета данных

        float x, y, w, h, confidence;
        int32_t index, track_id;
        QDataStream stream(data);
        stream.setByteOrder(QDataStream::LittleEndian);
        stream.setFloatingPointPrecision(QDataStream::SinglePrecision);
        stream >> x >> y >> w >> h >> confidence >> index >> track_id;

        QMutexLocker locker(&_boxesMutex);
        BoundingBox box;
        box.x = x;
        box.y = y;
        box.w = w;
        box.h = h;
        box.confidence = confidence;
        box.track_id = track_id;
        box.selected = true;

        QString box_info = QString::number(track_id) + ": (" + QString::number(x) + ";" + QString::number(y) + ") [" + QString::number(w) + ";" + QString::number(h) + "]";
        terminalInfo(box_info);
        qDebug() << box_info;

        _boxesMap[track_id] = box; // Добавляем бокс в карту
    }
    else if (data.size() == 1)
    {
        // Окончание передачи
        QMutexLocker locker(&_boxesMutex);
        emit updateTargetInfo((int)_boxesMap.count());
        _boxesMap.clear();
    }
    else
    {
        QString packetContent;
        for (char byte : data) {
            packetContent += QString("0x%1 ").arg((unsigned char)byte, 2, 16, QChar('0'));
        }
        qDebug() << "Неверный размер UDP-данных:" << data.size() << "байт, содержимое:" << packetContent;
        terminalError(QString("Неверный размер UDP-данных: ") +  QString::number(data.size()));
    }
}

void MainWindow::updateTargetInfo(int targetcount)
{
    ui->lbInfoPanelTargetTotalValue->setText(QString::number(targetcount));
    ui->lbInfoPanelTargetActiveValue->setText(QString::number(targetcount));
}
