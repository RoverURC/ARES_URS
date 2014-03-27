#ifndef MANIPULATOR_H
#define MANIPULATOR_H
#include <QObject>
#include "modbusclient.h"

#include "qmath.h"
#include <limits>

class Manipulator : public ModbusClient
{
  Q_OBJECT
public:
  explicit Manipulator(QObject *parent = 0);
  ~Manipulator();

public slots:

  void sendManipulatorData();
  void interpretJoypadButton(int id, bool status);
  void interpretJoypadAxis(int id, qint16 value);

private:

  quint16 firstMotorDirection;
  quint16 secondMotorDirection;
  quint16 firstMotorSpeed;
  quint16 secondMotorSpeed;
  quint16 turningMotorDirection;
  quint16 turningMotorSpeed;
  quint16 connectionIndicator;

};

#endif // MANIPULATOR_H
