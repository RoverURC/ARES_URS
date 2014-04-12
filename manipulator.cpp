#include "manipulator.h"

Manipulator::Manipulator(QObject *parent ) :
  ModbusClient(parent)
{
  axisValue = new quint16[AXIS_NUMBER];

  axisStatus = new bool[AXIS_NUMBER];
  actualAxisValue = 0;
  resetManipulator();

  incrementAxisTimer = new QTimer(this);
  incrementAxisTimer->start(50);
  connect(incrementAxisTimer,SIGNAL(timeout()),this,SLOT(incrementManipulatorAxisValues()));
}
Manipulator::~Manipulator(){
  delete[] axisValue;
  delete[] axisStatus;
}
//TO DO
void Manipulator::incrementManipulatorAxisValues(){
  for(int i=0;i<AXIS_NUMBER;i++)
    if(axisStatus[i]==true){
      if((-1)*actualAxisValue>axisValue[i]){
        axisValue[i] = 0;
        return;
      }
      axisValue[i] += actualAxisValue;
      if(axisValue[i]>615)
        axisValue[i] = 615;
      return; // We can control only one axis at time
    }
}
void Manipulator::resetManipulator(){
  for(int i=0;i<AXIS_NUMBER; i++)
    axisValue[i] = 0;  //TO DO, Maybe 0 PWM shouldnt be default value
  for(int i=0; i<AXIS_NUMBER; i++)
    axisStatus[i] = false;
}
void Manipulator::sendManipulatorData(){
  for(int i=0; i<AXIS_NUMBER; i++){
    setRegister(i, axisValue[i]);
  }

  qDebug()<<"Sending data";
  qDebug()<<"Writing status " << writeMultipleRegisters(0,AXIS_NUMBER);
}

void Manipulator::interpretJoypadButton(int id, bool status){
  if(status == true)
    for(int i=0; i<AXIS_NUMBER; i++)
      if(i == id)
        axisStatus[i] = true;
      else
        axisStatus[i] = false;
  else
    axisStatus[id] = false;
}

void Manipulator::interpretJoypadAxis(int id, qint16 value){
  if(id == 1)
    actualAxisValue = value/5000;
}

