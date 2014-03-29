#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  myJoystickRover = new QJoystick(this);
  myJoystickManipulator = new QJoystick(this);
  joypadRoverRefresh = new QTimer(this);
  joypadManipulatorRefresh = new QTimer(this);

  roverRefresh = new QTimer(this);
  manipulatorRefresh = new QTimer(this);

  mySettingsWindow = new SettingsWindow();

  ui->setupUi(this);

  //myModbusServer = new ModbusServer(1502,this);
  //myModbusServer->startListening();

  myRover = new Rover(this);
  myManipulator = new Manipulator(this);

  //Rover
  connect(this->mySettingsWindow,SIGNAL(roverJoypadSelected(int)),this,SLOT(createRoverJoystick(int)));
  connect(this->myRover,SIGNAL(statusConnectedChanged(bool)),this->mySettingsWindow,SLOT(setStatusDiodeRover(bool)));
  connect(this->mySettingsWindow,SIGNAL(connectToHostRover(QString,int)),this->myRover,SLOT(connectToModbusServer(QString,int)));
  connect(this->mySettingsWindow,SIGNAL(disconnectFromHostRover()),this->myRover,SLOT(disconnectFromModbusServer()));

  //Manipulator
  connect(this->mySettingsWindow,SIGNAL(manipulatorJoypadSelected(int)),this,SLOT(createManipulatorJoystick(int)));
  connect(this->myManipulator,SIGNAL(statusConnectedChanged(bool)),this->mySettingsWindow,SLOT(setStatusDiodeManipulator(bool)));
  connect(this->mySettingsWindow,SIGNAL(connectToHostManipulator(QString,int)),this->myManipulator,SLOT(connectToModbusServer(QString,int)));
  connect(this->mySettingsWindow,SIGNAL(disconnectFromHostManipulator()),this->myManipulator,SLOT(disconnectFromModbusServer()));

  connect(this->ui->actionSettings,SIGNAL(triggered()),this,SLOT(showSettingsWindow()));

  connect(joypadRoverRefresh,SIGNAL(timeout()),myJoystickRover,SLOT(getData()));
  connect(joypadManipulatorRefresh,SIGNAL(timeout()),myJoystickManipulator,SLOT(getData()));

  connect(myJoystickRover,SIGNAL(axisChanged(int,qint16)),this->ui->widgetJoypadRover,SLOT(changeAxisStat(int,qint16)));
  connect(myJoystickRover,SIGNAL(buttonChanged(int,bool)),this->ui->widgetJoypadRover,SLOT(changeButtonState(int,bool)));

  connect(myJoystickManipulator,SIGNAL(axisChanged(int,qint16)),this->ui->widgetJoypadRover,SLOT(changeAxisStat(int,qint16)));
  connect(myJoystickManipulator,SIGNAL(buttonChanged(int,qint16)),this->ui->widgetJoypadRover,SLOT(changeButtonState(int,bool)));
  //roverRefresh = new QTimer(this);
  //roverRefresh->start(50);

  //manipulatorRefresh = new QTimer(this);
  //manipulatorRefresh->start(2000);

  //connect(roverRefresh,SIGNAL(timeout()),myRover,SLOT(sendRoverData()));
  //connect(manipulatorRefresh,SIGNAL(timeout()),myManipulator,SLOT(sendManipulatorData()));

  //connect(myJoystick,SIGNAL(axisChanged(int,qint16)),myRover,SLOT(interpretJoypadAxis(int,qint16)));
  //connect(myJoystick,SIGNAL(axisChanged(int,qint16)),myManipulator,SLOT(interpretJoypadAxis(int,qint16)));
}
void MainWindow::showSettingsWindow(){
  mySettingsWindow->show();
}

MainWindow::~MainWindow(){
  delete ui;
}

void MainWindow::on_pushButton_clicked(){
  for (u_int8_t i = 0; i < 5; i++){
    this->myRover->setRegister(i,i+5);
  }
  this->myRover->writeMultipleRegisters(0,5);
}

//Only once
void MainWindow::createManipulatorJoystick(int id){
  myJoystickManipulator->setJoystick(id);
  joypadManipulatorRefresh->start(20);
}
//Only once
void MainWindow::createRoverJoystick(int id){
  myJoystickRover->setJoystick(id);
  joypadRoverRefresh->start(20);
}
