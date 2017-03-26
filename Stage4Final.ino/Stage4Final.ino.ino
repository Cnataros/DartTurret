//made by: Chris Nataros

#include <Servo.h>

Servo FlyWheels;
Servo Feeder;
Servo Pan;
Servo Tilt;

int potiPin = A0; //connect this pin with the middle pin of a potentiometer
int armUp = 24;
int armUpTimeMs = 1000;
int TimeToShootMS = 5000;


int panPos = 60;
int panShoot = 165;

int tiltPos = 45;
int tiltShoot = 60;

int feederSpeed = 120;

bool shotsFired = false;
bool Initialized = true;  //change this to false to wait for byte

void setup(){
  Serial.begin(9600);
  
  FlyWheels.attach(9);
  
  Pan.attach(11);
  Tilt.attach(12);
  
  Pan.write(panPos);
  Tilt.write(tiltPos);
  
  FlyWheels.write(armUp);
}

  void PanToShoot()
  {
      if(panPos < 170){
        for(panPos = 60; panPos < 165; panPos++){
          Pan.write(panPos);
          delay(15);
        }
      }
  }     

  void TiltToShoot()
  {
      if(tiltPos > 30){
        for(tiltPos = 45; tiltPos > tiltShoot; tiltPos--){
          Tilt.write(tiltPos);
          delay(30);
        }
      }
  }

  void PanToRest()
  {
      if(panPos > 60){
        for(panPos = 170; panPos > 60; panPos--){
          Pan.write(panPos);
          delay(30);
        }
      }
  }     

  void TiltToRest()
  {
      if(tiltPos < 45){
        for(tiltPos = 30; tiltPos < 45; tiltPos++){
          Tilt.write(tiltPos);
          delay(30);
        }
      }
  }

  void StartShooterMotors()
  {
    int poti = analogRead(potiPin);
    Serial.println(poti);
    poti = map(poti, 0, 1023, 52, 179);
    FlyWheels.write(poti);
  }

  void StopShooterMotors(){
    FlyWheels.write(0);
  }

  void ArmShooter(){
    int armUpStart = millis();
    if ((millis() - armUpStart) <= armUpTimeMs){
        FlyWheels.write(armUp);
        return;
    }
  }

  void Shoot(){
    Feeder.attach(10);
    Feeder.write(feederSpeed);
  }

  void CeaseFire(){
    Feeder.detach();
  }

  void loop(){

    if(shotsFired)
    {
      return;
    }

    if(!Initialized){
      // send data only when you receive data:
      if (Serial.available() > 0) {
          // read the incoming byte:
          char incomingByte = Serial.read();
          if(incomingByte == 'x')
          {
            Initialized = true;
          }
      }
      return;
    }
  
    //TODO: Will need to wait on a signal to start code past this line
    PanToShoot();
    TiltToShoot(); 
    ArmShooter();

    StartShooterMotors();

    //shoot for x time
    int shootTimer = millis();
    while(millis() - shootTimer <= TimeToShootMS){ 
      Shoot();
    }
    shotsFired = true;

    //Stop feeder
    CeaseFire();
    
    //stop flywheel
    StopShooterMotors();
    
    //return to position
    PanToRest();
    TiltToRest();
  }


