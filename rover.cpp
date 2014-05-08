#include "rover.h"

const int leftAxis = 1;
const int rightAxis = 4;
const int gpsOffset = 40;

Rover::Rover(QObject *parent ) :
  ModbusClient(parent)
{
  goodResponseCounter = 0;
  requestCounter = 0;

  connect(this,SIGNAL(transactionFinished(bool,qint8)),this,SLOT(proceedResponse(bool,qint8)));
}
void Rover::sendRoverData(){

  setRegister(10, 15);
  setRegister(11, 16);
  writeMultipleRegisters(10,2);
}
void Rover::readRoverData(){

  //Read Voltage and Current from motor drivers

  readHoldingRegisters(1,30);

}
void Rover::readGPSData(){

  readHoldingRegisters(40,21);
}

void Rover::updateRoverData(){
  if(isWaitingForResponse)
    return;
  requestCounter ++;
  if(requestCounter % 3 == 0){
    sendRoverData();
  }
  else if (requestCounter % 3 == 1){
    readRoverData();
  }
  else
    readGPSData();
}

void Rover::interpretJoypadButton(int id, bool status){

}
void Rover::setLeftPWM(qint16 value){
  setRegister(32, value);
  qDebug()<<"LEFT"<<value;

}
void Rover::setRightPWM(qint16 value){
  setRegister(33, value);
  qDebug()<<"RIGHT"<<value;
}
//Set speed and direction of each engine
void Rover::interpretJoypadAxis(int id, qint16 value){
  if(id==leftAxis){
    qint16 valuePWM = ((double)value/std::numeric_limits<quint16>::max()) * 1000;
    setLeftPWM(valuePWM);
  }
  if(id==rightAxis){
    qint16 valuePWM = ((double)value/std::numeric_limits<quint16>::max()) * 1000;
    setRightPWM(valuePWM);
  }
}
void Rover::proceedResponse(bool status, qint8 errorCode){
  if(status){
    goodResponseCounter++;
  }
  else
    qDebug()<<"BAD RESPONSE"; //We could add errorCode message

  emit roverDataUpdated();
}
