#ifndef SETTINGSWINDOW_H
#define SETTINGSWINDOW_H

#include <QWidget>
#include <qjoystick.h>
#include <QDebug>
namespace Ui {
  class SettingsWindow;
}

class SettingsWindow : public QWidget
{
  Q_OBJECT

public:
  explicit SettingsWindow(QWidget *parent = 0);
  ~SettingsWindow();

signals:
  void connectToHostRover(QString ip, int port);
  void disconnectFromHostRover();

  void connectToHostManipulator(QString ip, int port);
  void disconnectFromHostManipulator();

  void roverJoypadSelected(int id);
  void manipulatorJoypadSelected(int id);
public slots:
  void setStatusDiodeRover(bool status);
  void connectButtonPressedRover();
  void disconnectButtonPressedRover();

  void setStatusDiodeManipulator(bool status);
  void connectButtonPressedManipulator();
  void disconnectButtonPressedManipulator();

private slots:
  void on_pushButtonSelectRoverJoypad_clicked();

  void on_pushButtonSelectManipulatorJoypad_clicked();

private:

  Ui::SettingsWindow *ui;
};

#endif // SETTINGSWINDOW_H
