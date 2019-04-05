#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

//radio comm
//https://howtomechatronics.com/tutorials/arduino/arduino-wireless-communication-nrf24l01-tutorial/
//https://makerfabs.com/Comunication?product_id=309

RF24 radio(48,49);// set pin CE, CSN
const byte address[6] = "00001";

//US Left
int trigPin2 = 3;
int echoPin2 = 2;


//US Right
int trigPin3 = 13;
int echoPin3 = 12;


//motors
int enA = 9;
int in1 = 8;
int in2 = 14;

int enB = 6;
int in4 = 7;
int in3;

void setup() {
  //motors
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);


  Serial.begin(9600);// initialize serial communication of UltraSS

  //Ultrasound
  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);

  //radio
  radio.begin();
  radio.openWritingPipe(address);
  radio.setPALevel(RF24_PA_MIN);
  radio.stopListening();
}



//motion
void RunF(int limit) {// limit = speed
  digitalWrite(in1, HIGH);//motor A at power 400, so moving
  digitalWrite(in2, LOW);
  analogWrite(enA, limit);
  digitalWrite(in3, LOW);//motor B
  digitalWrite(in4, HIGH);
  analogWrite(enB, limit);
}


//Ultrasound

double UltraSSL() {//LEFT USS
  long durationL;
  int distanceL;
  digitalWrite(trigPin2, LOW);// low signal to ensure clean high pulse
  delay(1);
  digitalWrite(trigPin2, HIGH);// set trig pin high
  delayMicroseconds(10);        // for 10 microsec, after this HIGH input, USS will send 8 small pulses
  digitalWrite(trigPin2, LOW); // turn off
  durationL = pulseIn(echoPin2, HIGH);
  distanceL = durationL * 0.034 / 2;//distance in cm

  return distanceL;
}

double UltraSSR() {//RIGHT USS
  long durationR;
  int distanceR;
  digitalWrite(trigPin3, LOW);// low signal to ensure clean high pulse
  delay(1);
  digitalWrite(trigPin3, HIGH);// set trig pin high
  delayMicroseconds(10);      // for 10 microsec, after this HIGH input, USS will send 8 small pulses
  digitalWrite(trigPin3, LOW);// turn off
  durationR = pulseIn(echoPin3, HIGH);
  distanceR = durationR * 0.034 / 2;//distance in cm

  distanceR = 0 - distanceR; //invert to plot graph
  return distanceR;
}

//sorting
void Array_sort(double *array , int n)
{
  int i = 0 , j = 0 , temp = 0;

  for (i = 0 ; i < n ; i++)
  {
    for (j = 0 ; j < n - 1 ; j++)
    {
      if (array[j] > array[j + 1])
      {
        temp        = array[j];
        array[j]    = array[j + 1];
        array[j + 1]  = temp;
      }
    }
  }
}
// find median of array
float Find_median(double array[] , int n)
{
  float median = 0;

  // if number of elements are even
  if (n % 2 == 0)
    median = (array[(n - 1) / 2] + array[n / 2]) / 2.0;
  // if number of elements are odd
  else
    median = array[n / 2];

  return median;
}

void loop() {
  int sample=10, plotL, plotR;
  double L[sample], R[sample];
  
  L[0] = UltraSSL();//initial reading
  R[0] = UltraSSR();//
  
  for (int i = 1; i < sample; i++) {
    
    L[i] = UltraSSL();// read L distance
    if (L[i] > 100) {//discard value if too large ~1m
      L[i] = L[i-1];//copy last reading
    }
    
    R[i] = UltraSSR();// read R distance
    if (R[i] > 100) {//discad value if too large ~1m
      R[i] = R[i-1];//copy last reading
    }
  }
  
  Array_sort(L, sample);// sort samples
  plotL = Find_median(L, sample);//return median from samples
  
  Array_sort(R, sample);// sort samples
  plotR = Find_median(R , sample);//return median from samples

  Serial.print(plotL); //plot L distance afer adjustments
  Serial.print(" ");
  Serial.println(plotR);// plot R distance afer adjustments

  //send radio signal
  const char text[] = "Hello World";
  radio.write(&text, sizeof(text));
  delay(1000);
  
  //RunF(70);
}
