#include "mainwindow.h"
#include "ui_mainwindow.h"
/*
 * MainWindow to główne okno programu, znajdują sie w nim ...
 * Opis:
 *  Na początku tworzone są obiekty : 2 Joypady, którym nie są przypisane żadne joypady w systemie,
 *  Przypisać joypad można tylko raz wchodząc w settings window
 *
 *  W kolejnym kroku tworzone są timery które odliczają czas po którym następuje uaktualnienie stanu
 *  guzików i osi w każdym z joypadów,
 *  Joypad wykrywa, że jakaś oś/guzik uległa/uległ zmianie i emitują sygnały axisChanged, buttonChanged
 *
 *  Następnie tworzone są timery króre po przepełnieniu uaktywniają wysłanie danych przez sieć do manipulatora
 *  i Łazika
 *
 *  Następnie tworzone są obiekty Manipulaotra i Łazika dziedzicące po ModbusClient, które interpretują komendy
 *  z joypadów, obiekty te odpowiedzalne są również za połączenie.
 *
 *  Następnie łączone są sygnały z settingsWindow które uaktywniają połączenie, oraz wyświetlają jego status
 *
 *  Następnie łączone są sygnały zmiany osi/przycisku z elementami w UI odpowiedzialnymi za wyświetlanie stanu
 *  joypada
 *
 */
MainWindow::MainWindow(QWidget *parent) :
  QMainWindow(parent),
  ui(new Ui::MainWindow)
{
  myJoystickRover = new QJoystick(this);
  myJoystickManipulator = new QJoystick(this);

  //Timer for refreshing joypadData inside this program
  joypadRoverRefresh = new QTimer(this);
  joypadManipulatorRefresh = new QTimer(this);
  connect(joypadRoverRefresh,SIGNAL(timeout()),myJoystickRover,SLOT(getData()));
  connect(joypadManipulatorRefresh,SIGNAL(timeout()),myJoystickManipulator,SLOT(getData()));

  //Timer to send data to rover and manipulator
  roverRefresh = new QTimer(this);
  manipulatorRefresh = new QTimer(this);

  mySettingsWindow = new SettingsWindow();

  ui->setupUi(this);

  //Manipulator and rover objects that inherits from ModbusServer
  myRover = new Rover(this);
  myManipulator = new Manipulator(this);

  //Rover
  connect(this->mySettingsWindow,SIGNAL(roverJoypadSelected(int)),this,SLOT(createRoverJoystick(int)));
  connect(this->myRover,SIGNAL(statusConnectedChanged(bool)),this->mySettingsWindow,SLOT(setStatusDiodeRover(bool)));
  connect(this->mySettingsWindow,SIGNAL(connectToHostRover(QString,int)),this,SLOT(connectToHostRover(QString,int)));
  connect(this->mySettingsWindow,SIGNAL(disconnectFromHostRover()),this,SLOT(disconnectFromHostRover()));

  //Manipulator
  connect(this->mySettingsWindow,SIGNAL(manipulatorJoypadSelected(int)),this,SLOT(createManipulatorJoystick(int)));
  connect(this->myManipulator,SIGNAL(statusConnectedChanged(bool)),this->mySettingsWindow,SLOT(setStatusDiodeManipulator(bool)));
  connect(this->mySettingsWindow,SIGNAL(connectToHostManipulator(QString,int)),this,SLOT(connectToHostManipulator(QString,int)));
  connect(this->mySettingsWindow,SIGNAL(disconnectFromHostManipulator()),this,SLOT(disconnectFromHostManipulator()));

  //Open settings window
  connect(this->ui->actionSettings,SIGNAL(triggered()),this,SLOT(showSettingsWindow()));

  //Show data on joypadRoverWidget
  connect(myJoystickRover,SIGNAL(axisChanged(int,qint16)),this->ui->widgetJoypadRover,SLOT(changeAxisStat(int,qint16)));
  connect(myJoystickRover,SIGNAL(buttonChanged(int,bool)),this->ui->widgetJoypadRover,SLOT(changeButtonState(int,bool)));

  //Show data on joypadRoverWidget (temp rover, couse we will have different graphics)
  connect(myJoystickManipulator,SIGNAL(axisChanged(int,qint16)),this->ui->widgetJoypadRover,SLOT(changeAxisStat(int,qint16)));
  connect(myJoystickManipulator,SIGNAL(buttonChanged(int, bool)),this->ui->widgetJoypadRover,SLOT(changeButtonState(int,bool)));

  //Rover sending over modbus interval
  roverRefresh = new QTimer(this);
  connect(roverRefresh,SIGNAL(timeout()),myRover,SLOT(sendRoverData()));

  //Manipulator sending over modbus interval
  manipulatorRefresh = new QTimer(this);
  connect(manipulatorRefresh,SIGNAL(timeout()),myManipulator,SLOT(sendManipulatorData()));

  //Send data about changed Axis/Button state from joypad to Rover
  connect(myJoystickRover,SIGNAL(axisChanged(int,qint16)),myRover,SLOT(interpretJoypadAxis(int,qint16)));
  connect(myJoystickRover,SIGNAL(buttonChanged(int,bool)),myRover,SLOT(interpretJoypadButton(int,bool)));

  //Send data about changed Axis/Button state from joypad to Manipulator
  connect(myJoystickManipulator,SIGNAL(axisChanged(int,qint16)),myManipulator,SLOT(interpretJoypadAxis(int,qint16)));
  connect(myJoystickManipulator,SIGNAL(buttonChanged(int,bool)),myManipulator,SLOT(interpretJoypadButton(int,bool)));

}

void MainWindow::connectToHostManipulator(QString ip,int port){
  if(myManipulator->connectToModbusServer(ip, port))
    manipulatorRefresh->start(50);
}

void MainWindow::connectToHostRover(QString ip,int port){
  if(myRover->connectToModbusServer(ip, port))
    roverRefresh->start(50);
}

void MainWindow::disconnectFromHostManipulator(){
  myManipulator->disconnectFromModbusServer();
  manipulatorRefresh->stop();
}

void MainWindow::disconnectFromHostRover(){
  myRover->disconnectFromModbusServer();
  roverRefresh->stop();
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
