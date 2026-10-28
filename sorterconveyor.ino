/*In this project the goal is to sort objects in the conveyor belt.
 since we dont have the colour sensor, the project uses a UV sensor
 this object is sorted in 3 steps.
 A)the motor (bottom) which is powered by the 9V battery is the motor
 of the conveyor belt. it brings the objects to the edge of the convayor 
 belts and drops it.
 B)the UV sensor calculates the length of the object (by calculating the distance
 of the object such the other side of the object are on the same horizontal level
 we divide them into 3 groups, in this case 0cm to 100cm,100 to 200cm,200 to 333 cm
 C) Based on the reaading, I programmed the servo motor to be at 0,90,180 degrees when
 different groups are detected.a right triangular prism hence guiding objects to their
 resprctive bins using grvavity
*/



// importing libraries for servo and lcd screen
#include <Servo.h> 		
#include <Adafruit_LiquidCrystal.h>
// A,B,C are counter variables for the 3 groups
int A = 0;
int B = 0;
int C = 0; 
int pos =0;
int lastZone = 0;          // 0 = nothing, 1 = A, 2 = B, 3 = C
int uvPin = A0;			   // pin for uv sensor
// creating objects for servo and lcd
Servo myServo;
Adafruit_LiquidCrystal lcd(0); 
// this function makes sure that no repeated results are there in the continuous loop
//without this function it was going crazy
//it is making sure that each object is scanned only once

double readDistanceCm()
{
  pinMode(uvPin, OUTPUT);
  digitalWrite(uvPin, LOW);
  delayMicroseconds(2);
  digitalWrite(uvPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(uvPin, LOW);

  pinMode(uvPin, INPUT);
  double duration = pulseIn(uvPin, HIGH, 30000);
  if (duration == 0) return -1;
  // this calculates the distance using the speed on sound
  return duration * 0.0343 / 2;
  // sound travels at 0,0343 cm per micro second, by 2 is because sound covered twice distance
 }
// setup
void setup() {
  myServo.attach(9);
  myServo.write(0);

  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("A:0 B:0 C:0");
}
//loop
void loop() {
  double reading = readDistanceCm();
  int zone = 0;
// categorize into groups
  if (reading > 0 && reading <= 100)
  {zone = 1;
  }
  else if (reading > 100 && reading <= 200) 
  {zone = 2;
  }
  else if (reading > 200 && reading <= 333)
  {zone = 3;
  }
  // Act only when the zone changes
  if (zone != lastZone && zone != 0) {
    if (zone == 1)    
    { pos=0;   
     A++;
    }
    else if (zone == 2) 
    { pos=90;
     B++;
    }
    else                
    { pos=180;
     C++;
    }
  }  
  
  myServo.write(pos);
  lastZone = zone;
  // print on the lcd
  lcd.setCursor(0, 0);
  lcd.print("A:");
  lcd.print(A);
  lcd.print(" B:");
  lcd.print(B);
  lcd.print(" C:");
  lcd.print(C);
  lcd.print("   ");

  lcd.setCursor(0, 1);
  lcd.print("Dist:");
  if (reading < 0) 
  {lcd.print("--");
  }
  else 
  {lcd.print((int)reading);
  }
  lcd.print("cm     ");
  
  delay(300);
}