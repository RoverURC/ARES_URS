#include "manipulator.h"

// Axis 0 (Pin PWM 0,1), Axis 1 (Pin PWM 2), Axis 2 (Pin PWM 3), Axis 3 (Pin PWM 4)
int const Manipulator::axisNumber = 4;
int const Manipulator::servoInit[] = {250,250,250,250};
int const Manipulator::servoMax[] = { 500, 380, 475, 550};
int const Manipulator::servoMin[] = { 150, 100, 100, 100};
int const MotorSpeedButton = 6;
int const MotorSpeedIndex = 4;
Manipulator::Manipulator(QObject *parent ) :
  ModbusClient(parent)
{
  motorStatus = false;
  requestCounter = 0;
  goodResponseCounter = 0;
  axisValues = new quint16[axisNumber];

  for(int i=0; i<axisNumber;i++){
      axisValues[i] = servoInit[i];
    }

  axisStatus = new bool[axisNumber];
  actualAxisValue = 0;
  resetManipulator();

  incrementAxisTimer = new QTimer(this);
  incrementAxisTimer->start(50);
  connect(incrementAxisTimer,SIGNAL(timeout()),this,SLOT(incrementManipulatorAxisValues()));
}
Manipulator::~Manipulator(){
  delete[] axisValues;
  delete[] axisStatus;
}
//TO DO
void Manipulator::incrementManipulatorAxisValues(){

  for(int i=0; i<axisNumber; i++){
    if(axisStatus[i]==true){
      if(axisValues[i] +actualAxisValue<servoMin[i]){
        axisValues[i] = servoMin[i];
        setRegister(i, axisValues[i]);
        return;
      }
      if(axisValues[i] + actualAxisValue>servoMax[i]){
        axisValues[i] = servoMax[i];
        setRegister(i, axisValues[i]);
        return;
      }
      axisValues[i] += actualAxisValue;
      setRegister(i, axisValues[i]);
      return;
    }
  }
}
void Manipulator::resetManipulator(){
  for(int i=0;i<axisNumber; i++){
    axisValues[i] = servoMin[i];
    setRegister(i, servoMin[i]);
  }
  for(int i=0; i<axisNumber; i++)
    axisStatus[i] = false;
}
void Manipulator::updateManipulatorData(){
  if(isWaitingForResponse)
    return;
  requestCounter ++;

  sendManipulatorData();
  //For now we dont have any read functions
}

void Manipulator::sendManipulatorData(){

  writeMultipleRegisters(0,6);
}

void Manipulator::interpretJoypadButton(int id, bool status){
  if(status == true)
    for(int i=0; i<axisNumber; i++)
      if(i == id)
        axisStatus[i] = true;
      else
        axisStatus[i] = false;
  else
    axisStatus[id] = false;

  if(id == MotorSpeedButton){
    if(status == true){
      motorStatus = true;
    }
    else{
      motorStatus = false;
    }
  }
}

void Manipulator::interpretJoypadAxis(int id, qint16 value){
  if(id == 1){
    actualAxisValue = value/5000;
    if(motorStatus==true)
      setRegister(MotorSpeedIndex,value/35);
  }

  if(id == 3){
    qint16 uartAngle = (float)value/100;
    if(uartAngle>300)
      uartAngle = 300;
    if(uartAngle<0)
      uartAngle = 0;

    setRegister(5,uartAngle);
  }

}

void Manipulator::proceedResponse(bool status, qint8 errorCode){
  if(status){
    goodResponseCounter++;
    emit manipulatorDataUpdated();
  }
  else
    qDebug()<<"BAD RESPONSE Manipulator"; //We could add errorCode message
}

int Manipulator::getAxisValue(int index){
  if(index<0 || index>=axisNumber)
    return 0;
  else
    return axisValues[index];
}
