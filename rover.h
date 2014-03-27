#ifndef ROVER_H
#define ROVER_H

#define MOTOR_FORWARD 1
#define MOTOR_BACKWARD 2
#define MOTOR_BREAK 3
#define MOTOR_IDLE 4
#define MOTOR_RIGHT 5
#define MOTOR_LEFT 6
#define CONNECTION_INDICATOR 7

#include <QObject>
#include "modbusclient.h"
#include "qmath.h"
#include <limits>
/*How data is organized in HoldingRegisters for Rover Unit
 *
 * 0 - Right Motor Direction
 * 1 - Left Motor Direction
 * 2 - Right Motor Speed
 * 3 - Left Motor Speed
 * 4 - Turning Motor Direction
 * 5 - Connection Indicator
 * 6 -
 */

class Rover : public ModbusClient
{
  Q_OBJECT
public:

  explicit Rover(QObject *parent = 0);

public slots:

  void sendRoverData();
  void interpretJoypadButton(int id, bool status);
  void interpretJoypadAxis(int id, qint16 value);

private slots:
  void setRightMotorDirection(quint16 value);
  void setLeftMotorDirection(quint16 value);
  void setRightMotorSpeed(quint16 value);
  void setLeftMotorSpeed(quint16 value);

  void setTurningMotorDirection(quint16 value);

private:

  quint16 firstMotorDirection;
  quint16 secondMotorDirection;
  quint16 firstMotorSpeed;
  quint16 secondMotorSpeed;
  quint16 turningMotorDirection;
  quint16 turningMotorSpeed;
  quint16 connectionIndicator;

};

#endif // ROVER_H
