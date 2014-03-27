#include "manipulator.h"

Manipulator::Manipulator(QObject *parent ) :
  ModbusClient(parent)
{
}
Manipulator::~Manipulator(){

}

void Manipulator::sendManipulatorData(){
  qDebug()<<"Sending data";
  qDebug()<<writeMultipleRegisters(0,5);
}

void Manipulator::interpretJoypadButton(int id, bool status){

}

void Manipulator::interpretJoypadAxis(int id, qint16 value){
  qDebug()<<"Axis ID:"<<id<<" Axis Value"<<value;
  if (value<0) setRegister(0, 3);
  else if (value==0) setRegister(0, 0);
  else setRegister(0, 12);

  sendManipulatorData();

}

