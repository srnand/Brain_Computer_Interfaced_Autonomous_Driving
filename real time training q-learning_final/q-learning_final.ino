#include <Servo.h>

//Distance Front Right
int FR_Trig = 4;
int FR_Echo = 5;

int FR1_Trig = 8;
int FR1_Echo = 9;

//Distance Front Middle
int FM_Trig = 2;
int FM_Echo = 3;

//Distance Front Left
int FL_Trig = 6;
int FL_Echo = 7;

int FL1_Trig = 10;
int FL1_Echo = 11;

float fr_p;
float fm_p;
float fl_p;

float fr_s;
float fm_s;
float fl_s;

//float arr_x[7];

//Servo right; 
//Servo left;

//Variables for Self-Learning
float learn_rate = 0.0001;

//float weights[] = 
//  { 0.0018, 0.0017, 0.0019, 0.0011, 0.0013, 0.0012, 0.0011 };

float weights[] = 
  { 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0 };

float Pred_Reward(float arr_x[]){
  float total = 0.0;
  //Serial.println();
  for(int i = 0; i < 7; i++)
  {
    total = total + (arr_x[i] * weights[i]);
    /*Serial.print("arr: ");
    Serial.print(arr_x[i]);
    Serial.print("------");
    Serial.print("weight: ");
    Serial.print(weights[i]);*/
    
  }
  //Serial.println();
  return total;
}

void Update_Weights(float y_actual, float y_pred, float arr_x[])
{
  for(int i = 0; i < 7; i++){
    float pDelta = (y_pred - y_actual) * arr_x[i];
    weights[i] = weights[i] - (learn_rate * pDelta);
    Serial.print(weights[i]);
    Serial.print(" ");
  }
}

float Dist_Read_CM(int trigPin, int EchoPin){
  // The sensor is triggered by a HIGH pulse of 
  //10 or more microseconds.
  // Give a short LOW pulse beforehand to 
  //ensure a clean HIGH pulse:
  digitalWrite(trigPin, LOW);
  delayMicroseconds(5);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  // Read the signal from the sensor: a HIGH pulse whose
  // duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(EchoPin, INPUT);
  float duration = pulseIn(EchoPin, HIGH);

  return (duration/2) / 29.1;
}

int Dist(float d){
if(d>=0 && d<5)
d=0;
else if(d>=5 && d<10)
d=5;
else if(d>=10 && d<20)
d=10;
else if(d>=20)
d=20;
  return d;
}

void fullForward(){
  digitalWrite(17,LOW);
 digitalWrite(16,HIGH);
 digitalWrite(15,LOW);
 digitalWrite(14,HIGH);
}

void fullLeft(){
  digitalWrite(17,LOW);
 digitalWrite(16,LOW);
 digitalWrite(15,HIGH);
 digitalWrite(14,LOW);
}

void fullRight(){
   digitalWrite(17,HIGH);
 digitalWrite(16,LOW);
 digitalWrite(15,LOW);
 digitalWrite(14,LOW);
}

void setup()
{
  Serial.begin(9600);
  pinMode(17,OUTPUT);
  pinMode(16,OUTPUT);
  pinMode(15,OUTPUT);
  pinMode(14,OUTPUT);
  pinMode(FR_Trig, OUTPUT);
  pinMode(FR1_Trig, OUTPUT);
  pinMode(FM_Trig, OUTPUT);
  pinMode(FL_Trig, OUTPUT);
  pinMode(FL1_Trig, OUTPUT);
  pinMode(FR_Echo, INPUT);
  pinMode(FR1_Echo, INPUT);
  pinMode(FM_Echo, INPUT);
  pinMode(FL_Echo, INPUT);
  pinMode(FL1_Echo, INPUT);
  float fr1_p = Dist_Read_CM(FR_Trig, FR_Echo);
  delay(10);
  float fr2_p = Dist_Read_CM(FR1_Trig, FR1_Echo);
  delay(10);
  fr_p = (fr1_p + fr2_p) / 2;
  
  float fm_p = Dist_Read_CM(FM_Trig, FM_Echo);
  delay(10);
  
  float fl1_p = Dist_Read_CM(FL_Trig, FL_Echo);
  delay(10);
  float fl2_p = Dist_Read_CM(FL1_Trig, FL1_Echo);
  delay(10);
  fl_p = (fl1_p + fl2_p) / 2;
  
  fullForward();
}

float Receive_Reward(float fr, float fl, float fm, boolean wentForward){
  float reward = 0.0;
  if(fr < 12.0 && fl < 12.0 && fm < 12.0){
    reward = reward - 3.0;
  }else if(fr < 6.0 || fl < 6.0 || fm < 6.0){
    reward = reward - 2.0;
  } 
  if(wentForward){
    reward = reward + 1.0;
  }
  else{
    reward = reward + 0.65;
  }
  return reward;
}

void loop()
{  
  
 
  //Scale Distances
  //Use previous distances
  float fr_s1 = fr_p; // 10.0;
   fr_s = Dist(fr_s1);
  float fm_s1 = fm_p; // 10.0;
   fm_s = Dist(fm_s1);
  float fl_s1 = fl_p; // 10.0;
   fl_s = Dist(fl_s1);

    Serial.println();
  //Serial.print("FR Dist: ");
  Serial.print(fr_s);
  Serial.print(" ");
  //Serial.print("FM Dist: ");
  Serial.print(fm_s);
  Serial.print(" ");
  //Serial.print("FL Dist: ");
  Serial.print(fl_s);
  Serial.print("\n");
while (!Serial.available()){
  }
Serial.print("\n");

/*for(int i=0;i<10;i++){  
int v=Serial.read();
//Serial.write(Serial.read());
if(i>=3){
weights[i]=v;}
}*/
//Serial.flush();
//for(int i=0;i<10;i++){  
int cnt=0;
int j=0;
String s;
String v=Serial.readString();
//Serial.write(Serial.read());
 for(int i=0;i<v.length();i++){
    if(v.charAt(i)==' '){
      if(cnt>=3){
      weights[j]=s.toFloat();
      j++;
      s="";
      }
      cnt++;
      }
      else{
        s+=v.charAt(i);
        }
        
    } 
//}
//Serial.print("\n");
  /*String s;
  int j=0;
  for(int i=0;i<v.length;i++){
    
    if(v.charAt(i)==' '){
      weights[j]=s.toFloat();
      j++;
      s=null;
      }
      else{
        s+=v.charAt(i);
        }
    }*/
  /*int found = 0;
  char separator = ' ';
    int strIndex[] = { 0, -1 };
    int maxIndex = v.length() - 1;

    for (int i = 0; i <= maxIndex && found <= index; i++) {
        if (data.charAt(i) == separator || i == maxIndex) {
            found++;
            strIndex[0] = strIndex[1] + 1;
            strIndex[1] = (i == maxIndex) ? i+1 : i;
        }
    }

    if(found>index){
      data.substring(strIndex[0], strIndex[1])
      }*/
  //Create Current State Representation
  //using prevoius loops distances
  //Notice indices 3,4 and 5 are left, forward, right.
  //final index is the bias unit.
  float arr_x[7] = {fr_s, fm_s, fl_s, 1, 0, 0, 1};
  //what is predicted for left?
  float left_pred = Pred_Reward(arr_x);
  //re-encode state for forward
//   arr_x[7] = {fr_s, fm_s, fl_s, 0, 1, 0, 1};
  arr_x[3] = 0; arr_x[4] = 1;
  //what is forward reward prediction?
  float for_pred = Pred_Reward(arr_x);
  //re-encode state for right
//   arr_x[7] = {fr_s, fm_s, fl_s, 0, 0, 1, 1};
  arr_x[4] = 0; arr_x[5] = 1;
  //what is right prediction?
  float right_pred = Pred_Reward(arr_x);
  
  /*Serial.println("Predicted Rewards:");
  Serial.print(left_pred);
  Serial.print(":::");
  Serial.print(for_pred);
  Serial.print(":::");
  Serial.print(right_pred);
  Serial.println();*/
  
  //Made a decision, lets see what the current state is.
  //Read Distances
  float fr1_d = Dist_Read_CM(FR_Trig, FR_Echo);
  delay(10);
  float fr2_d = Dist_Read_CM(FR1_Trig, FR1_Echo);
  delay(10);
  float fr_d = (fr1_d + fr2_d) / 2;
  //float fr_d = Dist(fr_d1);
  
  float fm_d = Dist_Read_CM(FM_Trig, FM_Echo);
  //float fm_d = Dist(fm_d1);
  delay(10);
  
  float fl1_d = Dist_Read_CM(FL_Trig, FL_Echo);
  delay(10);
  float fl2_d = Dist_Read_CM(FL1_Trig, FL1_Echo);
  delay(10);
  float fl_d = (fl1_d + fl2_d) / 2;
  //float fl_d = Dist(fl_d1);
  

  
  //actual reward
  float a_reward;
  //predicted reward for action actually taken.
  float y_reward;
 /* if(y_reward > 1000 || y_reward < -1000)
  {
    for(int i = 0; i < 7; i++){
      weights[i] = 0.0001 * i;
    }
  }*/
  if(for_pred > left_pred && for_pred > right_pred)
  {
    //Serial.println("Chose Forward");
    fullForward();
    y_reward = for_pred;
    a_reward = Receive_Reward(fr_d, fl_d, fm_d, true);
    arr_x[3] = 0;
    arr_x[4] = 1;
    arr_x[5] = 0;
  }
  else if(left_pred > right_pred)
  {
    //Serial.println("Chose Left");
    fullLeft();
    y_reward = left_pred;
    a_reward = Receive_Reward(fr_d, fl_d, fm_d, false);
    arr_x[3] = 1;
    arr_x[4] = 0;
    arr_x[5] = 0;
  }
  else if(for_pred == right_pred && for_pred == left_pred){
    //Serial.println("All Predictions Same - Chose Forward");
    fullForward();
    y_reward = for_pred;
    a_reward = Receive_Reward(fr_d, fl_d, fm_d, true);
    arr_x[3] = 0;
    arr_x[4] = 1;
    arr_x[5] = 0;
  }else{
    //Serial.println("Chose Right");
    fullRight();
    y_reward = right_pred;
    a_reward = Receive_Reward(fr_d, fl_d, fm_d, false);
    arr_x[3] = 0;
    arr_x[4] = 0;
    arr_x[5] = 1;
  }
  /*Serial.println(".....");
  Serial.print("Actual Reward: ");
  Serial.println(a_reward);
  Serial.print("Predicted Reward: ");
  Serial.print(y_reward);
  Serial.println(".....");*/
  
  //update the model based on experience
  Update_Weights(a_reward, y_reward, arr_x);
  
  //set previous distances to current distances.
  fr_p = fr_d;
  fm_p = fm_d;  
  fl_p = fl_d;
  
}

