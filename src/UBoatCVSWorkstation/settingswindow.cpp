#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SettingsWindow)
{
    ui->setupUi(this);

    this->setStyleSheet(styleSheet());
    this->setStyleSheet("");

    // Иконка главного окна
    setWindowIcon(QIcon(":/img/sight.png"));

    _appSet.load();

    // Настройки камеры
    ui->leCameraID->setText( QString::number(_appSet.CAMERA_ID) );
    ui->leCameraFPS->setText( QString::number(_appSet.CAMERA_FPS) );
    ui->leCameraWidth->setText( QString::number(_appSet.CAMERA_WIDTH) );
    ui->leCameraHeight->setText( QString::number(_appSet.CAMERA_HEIGHT) );

    // Настройки экрана
    ui->cbGrid->setChecked(_appSet.DRAW_GRID);
    ui->cbSight->setChecked(_appSet.DRAW_SIGHT);
    ui->cbTimeStamp->setChecked(_appSet.DRAW_TIMESTAMP);

    // Настройки общие
    ui->cbTestMode->setChecked(_appSet.TEST_MODE);
    ui->lePortVideo->setText( QString::number(_appSet.PORT_VIDEO) );
    ui->lePortData->setText( QString::number(_appSet.PORT_DATA) );    
    ui->cbTheme->setCurrentIndex(_appSet.THEME);
    // Стили
    QFont fontLabel("GOST type A", 20, QFont::Bold);
    ui->lbTitle->setStyleSheet("color : black;");
    ui->lbTitle->setFont(fontLabel);

    connect(ui->pbOK, &QPushButton::clicked, this, &SettingsWindow::onOKButtonClicked);
    connect(ui->pbCancel, &QPushButton::clicked, this, &SettingsWindow::onCancelButtonClicked);
}

SettingsWindow::~SettingsWindow()
{
    delete ui;
}

void SettingsWindow::onOKButtonClicked()
{
    _appSet.CAMERA_ID = ui->leCameraID->text().toInt();
    _appSet.CAMERA_FPS = ui->leCameraFPS->text().toInt();
    _appSet.CAMERA_WIDTH = ui->leCameraWidth->text().toInt();
    _appSet.CAMERA_HEIGHT = ui->leCameraHeight->text().toInt();

    _appSet.DRAW_GRID = ui->cbGrid->isChecked();
    _appSet.DRAW_SIGHT = ui->cbSight->isChecked();
    _appSet.DRAW_TIMESTAMP = ui->cbTimeStamp->isChecked();

    _appSet.TEST_MODE = ui->cbTestMode->isChecked();
    _appSet.PORT_VIDEO = ui->lePortVideo->text().toInt();
    _appSet.PORT_DATA = ui->lePortData->text().toInt();
    _appSet.THEME = ui->cbTheme->currentIndex();

    // Сохранить настройки
    _appSet.save();

    // Закрыть форму
    this->accept();
}

void SettingsWindow::onCancelButtonClicked()
{
    // Закрыть форму
    this->reject();
}
