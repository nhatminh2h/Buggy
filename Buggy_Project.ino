
int turn;
const int encoderInA = 11, encoderInB =10;

int detectState=0, count=0, lastState;

//US1
int trigPin1= 2;
int echoPin1= 3;
long duration1;
int distance1;

//US2-left
int trigPin2= 8;
int echoPin2= 9;
long duration2;
int distance2;

//US3-right
int trigPin3= 13;
int echoPin3= 12;
long duration3;
int distance3;


int enA = 5;
int in1 = 4;
int in2 = 14;

int enB = 6;
int in4 = 7;
int in3;


//Fuction I/O
void setup() {

pinMode(enA, OUTPUT);
pinMode(enB, OUTPUT);
pinMode(in1, OUTPUT);
pinMode(in2, OUTPUT);
pinMode(in3, OUTPUT);
pinMode(in4, OUTPUT);

pinMode(trigPin1, OUTPUT); 
pinMode(echoPin1, INPUT);

pinMode(trigPin2, OUTPUT); 
pinMode(echoPin2, INPUT);

pinMode(trigPin3, OUTPUT); 
pinMode(echoPin3, INPUT);

Serial.begin(9600);// initialize serial communication of UltraSS
pinMode(encoderInA, INPUT);
pinMode(encoderInB, INPUT);
}


void UltraSS1() {
digitalWrite(trigPin1, LOW);// short high pulse to ensure clean high pulse
delay(2);
digitalWrite(trigPin1, HIGH);// pulse is sent
delay(10);
digitalWrite(trigPin1, LOW);
duration1 = pulseIn(echoPin1, HIGH);
distance1= duration1*0.034/2;
}

void UltraSS2() {//LEFT USS
digitalWrite(trigPin2, LOW);// short high pulse to ensure clean high pulse
delay(2);
digitalWrite(trigPin2, HIGH);// pulse is sent
delay(10);
digitalWrite(trigPin2, LOW);
duration2 = pulseIn(echoPin2, HIGH);
distance2= duration2*0.034/2;
}

void UltraSS3() {//RIGHT USS
digitalWrite(trigPin3, LOW);// short high pulse to ensure clean high pulse
delay(2);
digitalWrite(trigPin3, HIGH);// pulse is sent
delay(10);
digitalWrite(trigPin3, LOW);
duration3 = pulseIn(echoPin3, HIGH);
distance3= duration3*0.034/2;
}




void RunF(int limit){
digitalWrite(in1, HIGH);//motor A at power 400, so moving
digitalWrite(in2, LOW);
analogWrite(enA, limit);
digitalWrite(in3, LOW);//motor B
digitalWrite(in4, HIGH);
analogWrite(enB, limit);
}






void Stop(){
digitalWrite(in1, HIGH);//motor B at power 0,so stop
digitalWrite(in2, LOW);
analogWrite(enA, 0);
digitalWrite(in3, LOW);
digitalWrite(in4, LOW);
analogWrite(enB, 0); 
}

void LEFT(){//motor B
digitalWrite(in1, HIGH);
digitalWrite(in2, LOW);
analogWrite(enA, 0);
digitalWrite(in3, LOW);
digitalWrite(in4, HIGH);
analogWrite(enB, 80);
}
void RIGHT(){//motor A
digitalWrite(in1, HIGH);
digitalWrite(in2, LOW);
analogWrite(enA, 80);
digitalWrite(in3, LOW);
digitalWrite(in4, HIGH);
analogWrite(enB, 0); 
}

void Turnright(int limit){//motor A
while (count <limit) {
  LEFT();
  lastState = detectState;
  detectState=digitalRead(encoderInA);
  if (detectState != lastState){
    count++;
    //Serial.println(count);
}
}
count = 0;
}
void Turnleft(int limit){//motor B
while (count <limit) {
  RIGHT();
  lastState = detectState;
  detectState=digitalRead(encoderInB);
  if (detectState != lastState){
    count++;
    //Serial.println(count);
}
}
count = 0;
}


void loop() {
  name_servo.write(10);
  UltraSS1();
  UltraSS2();
  UltraSS3();
if(distance2 < 10){
 Turnleft(2);
 delay(200);
}
else if(distance3 < 10){
 Turnright(2);
 delay(200);
}
else if(distance1 < 44){
    Stop();
    delay(200);
    UltraSS2();
    UltraSS3();
    Serial.println(distance2);
    Serial.print("-");
    Serial.println(distance3);
    if (distance2 < distance3) {
        Turnleft(29);
    }
    else {
        Turnright(29);
    }
    Stop();
    delay(200);
}
else {
  RunF(80);
}
}
