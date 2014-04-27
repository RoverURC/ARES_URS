#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <QString>
#include <QPushButton>
#include <QMainWindow>
#include <QAction>
#include <QActionEvent>
#include "modbusclient.h"
#include "modbusserver.h"
#include "qjoystick.h"
#include "rover.h"
#include "manipulator.h"

class GPSCoordinates
{
public:
  GPSCoordinates();
  GPSCoordinates(float latitudeDegrees, float latitudeMinutes, float latitudeSeconds, QChar latitudeDesignator,
                 float longnitudeDegrees, float longnitudeMinutes, float longnitudeSeconds, QChar longnitudeeDesignator);

  float latitudeDegrees;
  float latitudeMinutes;
  float latitudeSeconds;
  QChar latitudeDesignator;

  float longnitudeDegrees;
  float longnitudeMinutes;
  float longnitudeSeconds;
  QChar longnitudeeDesignator;
};

namespace Ui {
  class MainWindow;

}

class MainWindow : public QMainWindow
{
  Q_OBJECT

public:
  explicit MainWindow(QWidget *parent = 0);
  ~MainWindow();
  QTimer *tempTimer;
public slots:
  void createManipulatorJoystick();
  void createRoverJoystick();
  void updateRoverDisplayData();
  void updateManipulatorDisplayData();
private slots:
  void connectToHostManipulator();
  void connectToHostRover();
  void disconnectFromHostManipulator();
  void disconnectFromHostRover();

private:
  Ui::MainWindow *ui;

  ModbusServer *myModbusServer;
  Rover *myRover;
  //ModbusClient *myModbusClient;

  Manipulator *myManipulator;

  QJoystick *myJoystickRover;
  QJoystick *myJoystickManipulator;

  QTimer *joypadRoverRefresh;
  QTimer *joypadManipulatorRefresh;

  QTimer *roverRefresh;
  QTimer *manipulatorRefresh;

  void setDisplayStyle();
  void checkTelemetryValues();
  void initManipulatorAxisDisplayData();
  //void updateGPSWidget();
  GPSCoordinates targetGPSCoordinates;
  GPSCoordinates actualGPSCoordinates;

public slots:
  void setStatusDiodeRover(bool status);

  void setStatusDiodeManipulator(bool status);
};



#endif // MAINWINDOW_H
