#include <Servo.h>
Servo rackPinServo;  // create servo object to control a servo
Servo gripperServo;
Servo baseServo;

// base servo important angles
int initBase = 40;
int baseRight = 13;
int baseLeft = 63;

// rack and pinion servo important angles
int bottomRack = 0;
int midRack = 30;
int topRack = 70;

// gripper servo important angles
int closedGrip = 0;
int openGrip = 10;

void setup() {
  // initialize the rack and pinion servo to 20 deg
  rackPinServo.attach(9);  
  rackPinServo.write(bottomRack);
  // initialize gripper in slightly open position
  gripperServo.attach(8);
  gripperServo.write(closedGrip);
  // initialize base servo to middle position
  baseServo.attach(10);
  baseServo.write(initBase);
}

void moveServo(Servo servo, int start, int finish, int stepSize, int delayTime, int finalDelay) {
  // loop from the start position to end position with appropriate middle position
  for(int pos = start; (stepSize > 0) ? pos <= finish : pos >= finish; pos += stepSize) {
    servo.write(pos);
    delay(delayTime);
  }
  delay(finalDelay);
}

int moveLeft(int initPos, int endPos) {
  // move servo from init pos to 85 assuming arm is to the right of the ball/can
  moveServo(baseServo, initPos, endPos, 1, 40, 500);
  return endPos;
}

int moveRight(int initPos, int endPos) {
  // move servo from init pos to 40 assuming arm is to the right of the ball/can
  moveServo(baseServo, initPos, endPos, -1, 40, 100);
  return endPos;
}

int centreBase(int initPos) {
  // move servo back to centre from current position assuming arm is to the right of ball/can
  moveServo(baseServo, initPos, initBase,(initPos > initBase) ? -1: 1, 40, 100);
  return initBase;
}

void resetAll(int basePos, int rackHeight, int grip) {
  // centre the base position
  centreBase(basePos);
  // drop rack and pinion to ground
  moveServo(rackPinServo, rackHeight, 0, -1, 100, 500);
  // reset gripper servo
  moveServo(gripperServo, grip, closedGrip, -1, 50, 500);
}

void loop() {
  // move rack pinion up to overcome ball
  moveServo(rackPinServo, bottomRack, midRack, 1, 100, 500);
  // move base servo in line with ball
  int basePos = moveLeft(initBase, baseLeft);
  // open servo to pick up ball
  moveServo(gripperServo, closedGrip, openGrip, 1, 50, 500);
  // drop servo to ball
  moveServo(rackPinServo, midRack, bottomRack, -1, 100, 500);
  // close servo on ball
  moveServo(gripperServo, openGrip, closedGrip, -1, 50, 500);

  // move servo all the way up
  moveServo(rackPinServo, bottomRack, topRack, 1, 100, 500);
  // turn base servo all the way left
  basePos = moveRight(basePos, baseRight);

  // drop servo to top of pop can
  moveServo(rackPinServo, topRack, topRack-3, -1, 100, 500);
  // open servo hand to release ball
  moveServo(gripperServo, closedGrip, openGrip, 1, 50, 500);
  // move servo up to clear the ball
  moveServo(rackPinServo, topRack-3, topRack, 1, 100, 500);

  // reset back to starting position
  resetAll(basePos, topRack, openGrip);
  // stop the program
  exit(0);  
}
