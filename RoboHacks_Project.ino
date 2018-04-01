#include <rgb_lcd.h>

#include <SoftwareSerial.h>

rgb_lcd lcd;

//Motor Definition
#define LEFT_POSITIVE 6 //motor 1
#define LEFT_NEGATIVE 4 //motor 1
#define RIGHT_POSITIVE 10 //motor 2
#define RIGHT_NEGATIVE 5 //motor 2

//Sonar Definition
#define TRIG_PIN 2
#define ECHO_PIN 3
long duration;
int distance;

//Warning and Clear Signal
#define WARNING 8 //red
#define CLEAR 9 //green

//Seconds Offset
#define leftOffset 4.3
#define rightOffset 4.0




void setup() {
  // put your setup code here, to run once:
  //Baudrate Initializing
  Serial.begin(115200);

  lcd.begin(16,2);

  //Motor Initialization
  pinMode(LEFT_POSITIVE, OUTPUT);
  pinMode(LEFT_NEGATIVE, OUTPUT);
  pinMode(RIGHT_POSITIVE, OUTPUT);
  pinMode(RIGHT_NEGATIVE, OUTPUT);

  //Sonar Initialization
  pinMode(TRIG_PIN, OUTPUT);
  pinMode(ECHO_PIN, INPUT);

  //Warning and Clear Signal Initialization
  pinMode(WARNING, OUTPUT);
  pinMode(CLEAR, OUTPUT);

  //LCD Initialization
  lcd.clear();
  lcd.setRGB(255, 255, 255);

  //Initial Display on the screen
  lcd.setCursor(3,0);
  lcd.print("WAITING FOR");
  lcd.setCursor(5,1);
  lcd.print("ORDER");
  
//  
}

void loop() {
  if (Serial.available()>0){
    char input = Serial.read();
    if (input=='1'){ 
      //Go to Table # 1
      pathToTable1();
    }

    if (input =='2'){ 
      //Go to Table # 2
      pathToTable2();
      
    }
  }

  lcd.clear();
  lcd.setRGB(255, 255, 255);

  lcd.setCursor(0,0);
  lcd.print("WAITING    FOR");
  lcd.setCursor(5,1);
  lcd.print("ORDER");

}





void pathToTable1(){
      goForward();
      setPause(2);
      goLeft(90);
      
      goForward();
      setPause(1);
      goRight(90);

      goForward();
      setPause(1);
      brake();

      lcd.clear();
      lcd.setRGB(0,0,255);
      lcd.setCursor(0,0);
      lcd.print("ARRIVED   AT");

      lcd.setCursor(0,1);
      lcd.print("TABLE 1");
      
      delay(5000);

      lcd.clear();
      lcd.setRGB(0,255,255);
      lcd.setCursor(0,0);
      lcd.print("RETURN TO BASE");

      //Wait for 2 seconds and then trace step backwards

      turnAround();
      goForward();
      setPause(1);
      goLeft(90);

      goForward();
      setPause(1);
      goRight(90);
      
      goForward();
      setPause(2);
      brake();

      turnAround();
      brake();
  
}

void pathToTable2(){
      goForward();
      setPause(2);
      goRight(90);

      goForward();
      setPause(1);
      goLeft(90);

      goForward();
      setPause(1);
      brake();

      lcd.clear();
      lcd.setRGB(0,0,255);
      lcd.setCursor(0,0);
      lcd.print("ARRIVED   AT");

      lcd.setCursor(0,1);
      lcd.print("TABLE 2");
      
      delay(5000);

      lcd.clear();
      lcd.setRGB(0,255,255);
      lcd.setCursor(0,0);
      lcd.print("RETURN TO BASE");

     


      

//      Wait for 2 seconds and then trace steps backwards
      turnAround();
      goForward();
      setPause(1);
      goRight(90);

      goForward();
      setPause(1);
      goLeft(90);

      goForward();
      setPause(2);
      brake();

      turnAround();
      brake();
}

//Movements for Mars Rover
void goForward(){
  digitalWrite(CLEAR, HIGH);
  digitalWrite(LEFT_POSITIVE, HIGH);
  digitalWrite(LEFT_NEGATIVE, LOW);
  
  digitalWrite(RIGHT_POSITIVE, HIGH);
  digitalWrite(RIGHT_NEGATIVE, LOW);
  
}

void brake(){
  digitalWrite(LEFT_POSITIVE, LOW);
  digitalWrite(LEFT_NEGATIVE, LOW);
  
  digitalWrite(RIGHT_POSITIVE, LOW);
  digitalWrite(RIGHT_NEGATIVE, LOW);
}

void goLeft(double angle){
  
  double time = (leftOffset*angle*1000)/360; //s --> ms
  
  digitalWrite(CLEAR, HIGH);
  
  digitalWrite(LEFT_POSITIVE, LOW);
  digitalWrite(LEFT_NEGATIVE, HIGH);

  digitalWrite(RIGHT_POSITIVE, HIGH);
  digitalWrite(RIGHT_NEGATIVE, LOW);

  delay(time);
  brake();
  
}

void goRight(double angle){
  
  double time = (rightOffset* angle* 1000)/360;

  digitalWrite(CLEAR, HIGH);

  digitalWrite(LEFT_POSITIVE, HIGH);
  digitalWrite(LEFT_NEGATIVE, LOW);

  digitalWrite(RIGHT_POSITIVE, LOW);
  digitalWrite(RIGHT_NEGATIVE, HIGH);

  delay(time);
  brake();
}


void turnAround(){
  
goRight(180);
}



//Pause in place for delay
//timePaused in seconds NOT MILLI-SECONDS
void setPause(int timePaused){
  unsigned long startTime = millis();
  lcd.clear();
  while ((millis() - startTime) < (timePaused*1000)) {
    //Execute code for detection
    if (detect()){
      digitalWrite(WARNING, HIGH);
      digitalWrite(CLEAR, LOW);
      unsigned long startDetect = millis();

      lcd.setRGB(255, 0, 0);
      lcd.setCursor(5,0);
      lcd.print("OBJECT");

      lcd.setCursor(4,1);
      lcd.print("DETECTED");

      while (detect()){
      brake();
      
      }
    startTime = startTime + (millis() - startDetect);//Compensate the lost seconds
    lcd.clear();
    lcd.setRGB(0, 255, 0);
      lcd.setCursor(5,0);
      lcd.print("CLEAR");

      lcd.setCursor(6,1);
      lcd.print("PATH");
    
    digitalWrite(WARNING, LOW);
    digitalWrite(CLEAR, HIGH);
      
    }
    goForward();
    
  }
}

//Obstacle comes infront of the rover
boolean detect() {
  digitalWrite (TRIG_PIN, LOW);
  delayMicroseconds (2);
  digitalWrite (TRIG_PIN, HIGH);
  delayMicroseconds(10);
  digitalWrite (TRIG_PIN, LOW);
  duration = pulseIn (ECHO_PIN, HIGH);
  distance = (duration / 2) * 0.034;
  return (distance < 30);
}
