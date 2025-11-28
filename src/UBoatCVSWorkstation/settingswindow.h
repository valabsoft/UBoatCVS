#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QDialog>
#include "applicationsettings.h"

namespace Ui {
class SettingsWindow;
}

class SettingsWindow : public QDialog
{
    Q_OBJECT

public:
    explicit SettingsWindow(QWidget *parent = nullptr);
    ~SettingsWindow();

private:
    Ui::SettingsWindow *ui;
    ApplicationSettings _appSet;

private slots:
    void onOKButtonClicked();
    void onCancelButtonClicked();
};

#endif // SETTINGSWINDOW_H
