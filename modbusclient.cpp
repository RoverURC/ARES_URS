#include "modbusclient.h"

ModbusClient::ModbusClient(QObject *parent) :
  QObject(parent),
  statusConnected(false),
  isWaitingForResponse(false),
  responseTimer(new QTimer(this)),
  holdingRegistersSize(127),
  transactionID(1)
{
  holdingRegisters = new quint16[holdingRegistersSize];
  for(int i=0 ; i<holdingRegistersSize ; i++){
    holdingRegisters[i]=0;
  }
  setResponseTimerTimeout(1000);
  mySocket = new QTcpSocket(this);
  connect(mySocket, SIGNAL(disconnected()), this, SLOT(disconnected()));
  connect(mySocket, SIGNAL(connected()),this,SLOT(connected()));
  connect(mySocket, SIGNAL(readyRead()), this, SLOT(readDataAndCheck()));

  connect(responseTimer,SIGNAL(timeout()),this,SLOT(timeoutConfirmation()));
  //temp
  connect(this, SIGNAL(modbusDataReady()), this, SLOT(tempConfirmation()));
  connect(this, SIGNAL(modbusBadResponseFormat()), this, SLOT(tempBadReq()));
}

ModbusClient::~ModbusClient(){
  disconnectFromModbusServer();
  if(!mySocket->isOpen())
    mySocket->waitForDisconnected(2000);
  delete[] holdingRegisters;
}

bool ModbusClient::connectToModbusServer(QString ip, int port){
  qDebug()<<"connecting";
  qDebug()<<ip<< " port "<< port;
  if(!mySocket->isOpen()){
      mySocket->connectToHost(ip,port);
  }
  else{
    mySocket->disconnectFromHost();
    mySocket->waitForDisconnected(2000);
    mySocket->connectToHost(ip,port);
  }
  if(!mySocket->waitForConnected(2000))
    return false;
  return true;
}
bool ModbusClient::disconnectFromModbusServer(){
  if(mySocket->isOpen()){
    mySocket->disconnectFromHost();
    if(!mySocket->waitForDisconnected(2000))
      return false;
  }
  return true;
}

void ModbusClient::connected(){
  qDebug()<<"Connected";
  statusConnected = true;
  emit statusConnectedChanged(true);
}

void ModbusClient::disconnected(){
  qDebug()<<"Disconnected";
  statusConnected = false;
  emit statusConnectedChanged(false);
}
void ModbusClient::readDataAndCheck(){
  qDebug()<<"reading DATA";
  responseTimer->stop();

  byteArrayInput.clear();
  byteArrayInput = mySocket->readAll();

  if(!isWaitingForResponse)
    return;
  isWaitingForResponse = false;

  //Analyze Modbus frame
  if(byteArrayInput.size()<8){
    emit modbusBadResponseFormat();
    return ;
  }
  quint16 incomingTransactionID;
  getQInt16(byteArrayInput, 0, incomingTransactionID);

  if(incomingTransactionID!=transactionID){
    emit modbusBadResponseFormat();
    return ;
  }
  transactionID++;
  quint16 protocolIdentyfier;
  getQInt16(byteArrayInput, 2, protocolIdentyfier );

  if(protocolIdentyfier!= 0){
    emit modbusBadResponseFormat();
    return ;
  }

  quint16 length;
  getQInt16(byteArrayInput, 4, length);

  if(byteArrayInput.size()!=length+6){
    emit modbusBadResponseFormat();
    return ;
  }
  quint8 unitID;
  getQInt8(byteArrayInput, 6, unitID);

  if(unitID!=255){
    emit modbusBadResponseFormat();
    return ;
  }

  quint8 functionCode;
  getQInt8(byteArrayInput, 7, functionCode);

  switch(waitingFunctionCode){
    case MODBUS_FC_WRITE_SINGLE_REGISTER: {
      switch(functionCode){
        case MODBUS_FC_WRITE_SINGLE_REGISTER + 0x80:{
          proceedIncomingError();
          return;
        }
        case MODBUS_FC_WRITE_SINGLE_REGISTER:{
          proceedWriteSigngleRegisterResponse();
          return;
        }
        default:{
          emit modbusBadResponseFormat();
          return;
        }
      }
      return;
    }
    case MODBUS_FC_WRITE_MULTIPLE_REGISTERS: {
      switch(functionCode){
        case MODBUS_FC_WRITE_MULTIPLE_REGISTERS + 0x80:{
          proceedIncomingError();
          return;
        }
        case MODBUS_FC_WRITE_MULTIPLE_REGISTERS:{
          proceedWriteMultipleRegistersResponse();
          return;
        }
        default:{
          emit modbusBadResponseFormat();
          return ;
        }
      }
        return;
    }
    case MODBUS_FC_READ_HOLDING_REGISTERS: {
      switch(functionCode){
        case MODBUS_FC_READ_HOLDING_REGISTERS + 0x80:{
          proceedIncomingError();
          return;
        }
        case MODBUS_FC_READ_HOLDING_REGISTERS:{
          proceedReadHoldingRegistersResponse();
          return;
        }
        default:{
          emit modbusBadResponseFormat();
          return ;
        }
      }
        return;
    }
    default:{
      qDebug()<<"That should never happen";
      return;
    }
  }
}

bool ModbusClient::setRegister(int index, quint16 value){
  if(index<0 || index>=holdingRegistersSize)
    return false;
  holdingRegisters[index]=value;
  return true;
}
bool ModbusClient::getRegister(int index, quint16 &value){
  if(index<0 || index>=holdingRegistersSize)
    return false;
  value = holdingRegisters[index];
  return true;
}

bool ModbusClient::getQInt16(QByteArray array, int index, quint16 &output){
  if((index+1)>=array.size())
    return false;
  output = qFromBigEndian<quint16>(reinterpret_cast<uchar*>(array.data()+index));
    return true;
}
bool ModbusClient::setQInt16(QByteArray &array, int index, quint16 input){
  if((index+1)>=array.size())
    return false;
  qToBigEndian<quint16>(input,((uchar*)array.data())+index);
    return true;
}
bool ModbusClient::getQInt8(QByteArray array, int index, quint8 &output){
  if(index>=array.size())
    return false;
  output = *reinterpret_cast<quint8*>(array.data()+index);
    return true;
}
bool ModbusClient::setQInt8(QByteArray &array, int index, quint8 input){
  if(index>=array.size())
    return false;

  *((quint8*)array.data()+index)=input;
    return true;
}
bool ModbusClient::writeSingleRegister(quint16 registerAddress){
  if(!statusConnected)
    return false;
  if(isWaitingForResponse)
    return false;

  if(registerAddress>=holdingRegistersSize)
    return false;

  byteArrayOutput.clear();
  formatHeader(6);
  quint8 functionCode = MODBUS_FC_WRITE_SINGLE_REGISTER;
  byteArrayOutput.push_back(functionCode);

  char temp = 0;
  byteArrayOutput.push_back(temp);
  byteArrayOutput.push_back(temp);
  setQInt16(byteArrayOutput,8,registerAddress);

  byteArrayOutput.push_back(temp);
  byteArrayOutput.push_back(temp);
  setQInt16(byteArrayOutput,10,holdingRegisters[registerAddress]);

  if(mySocket->write(byteArrayOutput)){
      responseTimer->start(responseTimerTimeout);
      isWaitingForResponse = true;
      waitingFunctionCode = MODBUS_FC_WRITE_SINGLE_REGISTER;
      return true;
  }
    return false;
}
bool ModbusClient::writeMultipleRegisters(quint16 startingAddress, quint16 quantityOfRegisters){
  if(!statusConnected)
    return false;

  if(isWaitingForResponse)
    return false;
  if(startingAddress>=holdingRegistersSize)
    return false;
  if((startingAddress+quantityOfRegisters)>=holdingRegistersSize)
    return false;

  if(quantityOfRegisters>127)
    return false;

  byteArrayOutput.clear();

  formatHeader(7+2*quantityOfRegisters);

  quint8 functionCode = MODBUS_FC_WRITE_MULTIPLE_REGISTERS;
  byteArrayOutput.push_back(functionCode);

  char temp = 0;
  byteArrayOutput.push_back(temp);
  byteArrayOutput.push_back(temp);
  setQInt16(byteArrayOutput, 8, startingAddress);

  byteArrayOutput.push_back(temp);
  byteArrayOutput.push_back(temp);
  setQInt16(byteArrayOutput, 10, quantityOfRegisters);

  quint8 byteCount = quantityOfRegisters*2;
  byteArrayOutput.push_back(temp);
  setQInt8(byteArrayOutput, 12, byteCount);
  for(int i=0 ; i<quantityOfRegisters ; i++){
    byteArrayOutput.push_back(temp);
    byteArrayOutput.push_back(temp);
    setQInt16(byteArrayOutput,13+2*i,holdingRegisters[startingAddress+i]);
  }
  if(mySocket->write(byteArrayOutput, byteArrayOutput.size())){
    isWaitingForResponse = true;
    waitingFunctionCode = MODBUS_FC_WRITE_MULTIPLE_REGISTERS;
    responseTimer->start(responseTimerTimeout);
    return true;
  }
  return false;
}
bool ModbusClient::readHoldingRegisters(quint16 startingAddress, quint16 quantityOfRegisters){
  if(!statusConnected)
    return false;
  if(isWaitingForResponse)
    return false;
  if(startingAddress>=holdingRegistersSize)
    return false;
  if((startingAddress+quantityOfRegisters) >= holdingRegistersSize)
    return false;
  if(quantityOfRegisters>127)
    return false;

  byteArrayOutput.clear();
  formatHeader(6);
  quint8 functionCode = MODBUS_FC_READ_HOLDING_REGISTERS;
  byteArrayOutput.push_back(functionCode);

  char temp = 0;
  byteArrayOutput.push_back(temp);
  byteArrayOutput.push_back(temp);
  setQInt16(byteArrayOutput, 8, startingAddress);

  byteArrayOutput.push_back(temp);
  byteArrayOutput.push_back(temp);
  setQInt16(byteArrayOutput, 10, quantityOfRegisters);

  if(mySocket->write(byteArrayOutput)){
    responseTimer->start(responseTimerTimeout);
    isWaitingForResponse = true;
    responseTimer->start();
    waitingFunctionCode = MODBUS_FC_READ_HOLDING_REGISTERS;
    return true;
  }
  return false;
}
void ModbusClient::formatHeader(quint16 length){
  char temp=0;
  byteArrayOutput.push_back(temp);
  byteArrayOutput.push_back(temp);
  setQInt16(byteArrayOutput, 0, transactionID);

  byteArrayOutput.push_back(temp);
  byteArrayOutput.push_back(temp);
  quint16 modbusProtocolID=0;
  setQInt16(byteArrayOutput, 2, modbusProtocolID);

  byteArrayOutput.push_back(temp);
  byteArrayOutput.push_back(temp);
  setQInt16(byteArrayOutput, 4, length);

  byteArrayOutput.push_back(temp);
  quint8 unitID = 255;
  setQInt8(byteArrayOutput, 6, unitID);
}
bool ModbusClient::proceedIncomingError(){
  if(byteArrayInput.size()!=9)
    return false;
  quint8 errorCode;
  getQInt8(byteArrayInput,8,errorCode);

  switch(errorCode){
    case (0x01):{

      return true;
    }
    case(0x02):{

      return true;
    }
    case(0x03):{

      return true;
    }
    case(0x04):{

      return true;
    }
    default:{

      return false;
    }
  }
}
bool ModbusClient::proceedReadHoldingRegistersResponse(){
  quint16 startingAddress;
  getQInt16(byteArrayOutput, 8, startingAddress);
  quint16 quantityOfRegisters;
  getQInt16(byteArrayOutput, 10, quantityOfRegisters);
  qDebug()<<"byteArrayInputSize"<<byteArrayInput.size();
  qDebug()<<"quantity of registers"<<quantityOfRegisters;
  if(byteArrayInput.size() != (9+2*quantityOfRegisters)){
    emit modbusBadResponseFormat();
      qDebug()<<"1";
    return false;
  }
  quint8 bytesCount;
  getQInt8(byteArrayInput, 8 , bytesCount);
  if(bytesCount!=quantityOfRegisters*2){
    emit modbusBadResponseFormat();
      qDebug()<<"2";
    return false;
  }
  quint16 value;
  for(int i=0; i<quantityOfRegisters; i++){
    getQInt16(byteArrayInput, (9+i*2), value);
    holdingRegisters[startingAddress+i] = value;
    qDebug()<<"3";
  }
  emit modbusDataReady();
  return true;
}
bool ModbusClient::proceedWriteSigngleRegisterResponse(){
  if(byteArrayInput.size()!=12){
    emit modbusBadResponseFormat();
    return false;
  }
  for (int i=0 ; i<12 ; i++){
    if(byteArrayOutput[i]!=byteArrayInput[i]){
      emit modbusBadResponseFormat();
      return false;
      }
  }
  emit modbusDataReady();
  return true;
}
bool ModbusClient::proceedWriteMultipleRegistersResponse(){
  if(byteArrayInput.size()!=12){
    emit modbusBadResponseFormat();
    return false;
  }
  for (int i=0 ; i<12 ; i++){
    if(i==4 || i==5)
      i = 6;
    if(byteArrayOutput[i]!=byteArrayInput[i]){
      emit modbusBadResponseFormat();
      return false;
    }
  }
  emit modbusDataReady();
  return true;
}
void ModbusClient::tempConfirmation(){
  isWaitingForResponse = false;
  qDebug()<<"Confirmation";
}
void ModbusClient::timeoutConfirmation(){
  responseTimer->stop();
  emit modbusResponseTimeout();
  isWaitingForResponse = false;
  qDebug()<<"Timeout";
}
void ModbusClient::tempBadReq(){
  isWaitingForResponse = false;
  qDebug()<<"bad req";
}
