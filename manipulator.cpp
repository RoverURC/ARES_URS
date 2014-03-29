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

  if (value<0) setRegister(id, 10000);
  else if (value==0) setRegister(id, 0);
  else setRegister(id,20000 );

  sendManipulatorData();

}

