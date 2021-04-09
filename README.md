# Telemetry

The telemetry code repo contains all the code to gather and transmit sensor data for the BYU baja vehicle. This is a platform io project. PlatformIO is just way to write code that is much easier to modify than Arduino's ino. Also it sets up much better with vscode.

## Structure

```
Main.cpp{

  instantiate classes

  void setup()
  {
    Run setup() functions for all classes
      - This will intialize the CAN bus, radio module etc.
  }

  void loop()
  {
    Run each class's update() function.
      - CANbus.update() will check for incoming can messages and handle appropriately
      - RadioModule.update() will check for incoming message from the radio and handle appropriately
      - Status.update() updates the state machine that controls the status LEDs on the PCB.
  }

}
```

learn more about Arduino and CAN bus at https://create.arduino.cc/projecthub/maurizfa-13216008-arthur-jogy-13216037-agha-maretha-13216095/can-bus-using-arduino-9ce7ba
