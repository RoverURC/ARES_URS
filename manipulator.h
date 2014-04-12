#ifndef MANIPULATOR_H
#define MANIPULATOR_H
#include <QObject>
#include "modbusclient.h"

#include "qmath.h"
#include <limits>
#define AXIS_NUMBER 5
class Manipulator : public ModbusClient
{
  Q_OBJECT
public:
  explicit Manipulator(QObject *parent = 0);
  ~Manipulator();
  void resetManipulator();
public slots:
  void sendManipulatorData();
  void interpretJoypadButton(int id, bool status);
  void interpretJoypadAxis(int id, qint16 value);
private slots:
  void incrementManipulatorAxisValues();
private:
  qint16 actualAxisValue;
  quint16 *axisValue;
  bool *axisStatus;
  QTimer *incrementAxisTimer;
};

#endif // MANIPULATOR_H
