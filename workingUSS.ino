double wallRight[], wallLeft[];
//US Left
int trigPin2 = 8;
int echoPin2 = 9;


//US Right
int trigPin3 = 13;
int echoPin3 = 12;


//motor
int enA = 5;
int in1 = 4;
int in2 = 14;

int enB = 6;
int in4 = 7;
int in3;

void setup() {
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);


  Serial.begin(9600);// initialize serial communication of UltraSS

  pinMode(trigPin2, OUTPUT);
  pinMode(echoPin2, INPUT);

  pinMode(trigPin3, OUTPUT);
  pinMode(echoPin3, INPUT);

  Serial.begin(9600);// initialize serial communication of UltraSS
}

//motion
void RunF(int limit) {
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
  delay(2);
  digitalWrite(trigPin2, HIGH);// pulse is sent
  delayMicroseconds(10);        // for 10 microsec
  digitalWrite(trigPin2, LOW); // low pulse
  durationL = pulseIn(echoPin2, HIGH);
  distanceL = durationL * 0.034 / 2;

  return distanceL;
}

double UltraSSR() {//RIGHT USS
  long durationR;
  int distanceR;
  digitalWrite(trigPin3, LOW);// low signal to ensure clean high pulse
  delay(2);
  digitalWrite(trigPin3, HIGH);// pulse is sent
  delayMicroseconds(10);      // for 10 microsec
  digitalWrite(trigPin3, LOW);
  durationR = pulseIn(echoPin3, HIGH);
  distanceR = durationR * 0.034 / 2;

  distanceR = 0 - distanceR; //invert to plot graph more accurately
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
// function to calculate the median of the array
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
  L[0] = UltraSSL();
  R[0] = UltraSSR();
  
  for (int i = 1; i < sample; i++) {
    
    L[i] = UltraSSL();
    if (L[i] > 100) {
      L[i] = L[i-1];
    }
    
    R[i] = UltraSSR();
    if (R[i] > 100) {
      R[i] = R[i-1];
    }
  }
  
  Array_sort(L, sample);
  plotL = Find_median(L, sample);
  
  Array_sort(R, sample);
  plotR = Find_median(R , sample);

  Serial.print(plotL);
  Serial.print(" ");
  Serial.println(plotR);

  //RunF(70);
}

