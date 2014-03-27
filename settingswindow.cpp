#include "settingswindow.h"
#include "ui_settingswindow.h"

SettingsWindow::SettingsWindow(QWidget *parent) :
  QWidget(parent),
  ui(new Ui::SettingsWindow)
{
  ui->setupUi(this);
  //Fill comboBoxes with joypad names
  for(int i=0;i<QJoystick::availableJoysticks();i++){
    ui->comboBoxRoverJoystick->addItem(QJoystick::joystickName(i));
    ui->comboBoxManipulatorJoystick->addItem(QJoystick::joystickName(i));
    }

  connect(this->ui->pushButtonConnectRover,SIGNAL(clicked()),this,SLOT(connectButtonPressedRover()));
  connect(this->ui->pushButtonDisconnectRover,SIGNAL(clicked()),this,SLOT(disconnectButtonPressedRover()));

  connect(this->ui->pushButtonConnectManipulator,SIGNAL(clicked()),this,SLOT(connectButtonPressedManipulator()));
  connect(this->ui->pushButtonDisconnectManipulator,SIGNAL(clicked()),this,SLOT(disconnectButtonPressedManipulator()));
}

SettingsWindow::~SettingsWindow()
{
  delete ui;
}
//Rover
void SettingsWindow::setStatusDiodeRover(bool status){
  this->ui->statusDiodeRover->setStatus(status);
}
void SettingsWindow::connectButtonPressedRover(){
  emit connectToHostRover(this->ui->lineEditIPRover->text(),this->ui->lineEditPortRover->text().toInt());
}
void SettingsWindow::disconnectButtonPressedRover(){
  emit disconnectFromHostRover();
}

//Manipulator
void SettingsWindow::setStatusDiodeManipulator(bool status){
  this->ui->statusDiodeManipulator->setStatus(status);
}
void SettingsWindow::connectButtonPressedManipulator(){
  emit connectToHostManipulator(this->ui->lineEditIPManipulator->text(),this->ui->lineEditPortManipulator->text().toInt());
}
void SettingsWindow::disconnectButtonPressedManipulator(){
  emit disconnectFromHostManipulator();
}

void SettingsWindow::on_pushButtonSelectRoverJoypad_clicked()
{
  emit (roverJoypadSelected(ui->comboBoxRoverJoystick->currentIndex()));
  ui->pushButtonSelectRoverJoypad->setEnabled(false);
  ui->comboBoxRoverJoystick->setDisabled(true);
}

void SettingsWindow::on_pushButtonSelectManipulatorJoypad_clicked()
{
  emit(manipulatorJoypadSelected(ui->comboBoxManipulatorJoystick->currentIndex()));
  ui->pushButtonSelectManipulatorJoypad->setEnabled(false);
  ui->comboBoxManipulatorJoystick->setDisabled(true);
}
