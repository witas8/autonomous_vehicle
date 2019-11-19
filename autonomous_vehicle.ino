//przypisanie pinow  
  int LFmotor = 5;  
  int LBmotor = 6; 
  int RFmotor = 9;
  int RBmotor = 10;
  int Rsensor = A0; 
  int Csensor = A1; 
  int Lsensor = A2; 
  int echo_front = 3;
  int trig_front = 2;
  int echo_back = 11;
  int trig_back = 12;
  int buzzer = 7;
  int analogSensor = A6;
  int go_forward = 4;
  int go_backward = 13;
//inicjalizacja zmiennych globalnych  
  int error;
  float time1,time2;
  float distance1, distance2;
  int LFspeed;
  int RFspeed;
  int LBspeed;
  int RBspeed;
  
void setup()
{ 
  pinMode(LFmotor, OUTPUT);
  pinMode(LBmotor, OUTPUT);
  pinMode(RFmotor, OUTPUT);
  pinMode(RBmotor, OUTPUT);
  pinMode(Lsensor, INPUT);
  pinMode(Csensor, INPUT);
  pinMode(Rsensor, INPUT);
  analogWrite(LFmotor, 0);
  analogWrite(LBmotor, 0);
  analogWrite(RFmotor, 0);
  analogWrite(RBmotor, 0);
  pinMode(trig_front, OUTPUT);
  pinMode(echo_front, INPUT);
  pinMode(trig_back, OUTPUT);
  pinMode(echo_back, INPUT);
  pinMode(buzzer, OUTPUT);
  pinMode(analogSensor, INPUT);
  digitalWrite(buzzer, LOW);
  pinMode(go_forward, INPUT);
  pinMode(go_backward, INPUT);
  Serial.begin(9600);
}

int read_sensor(){ 
  int Sensor0;
  int Sensor1; //optyczny
  int Sensor2;
  Sensor0 = analogRead(Lsensor);
  Sensor1 = analogRead(Csensor);
  Sensor2 = analogRead(Rsensor); 
  //Serial.println(Sensor1);                          

     if((Sensor0>=500)&&(Sensor1>=500)&&(Sensor2>=500)){
      return error = 0; 
     }
     else if((Sensor0<500)&&(Sensor1>500)&&(Sensor2>=500)){
      return error = 1; 
     }
     
     else if((Sensor0>=500)&&(Sensor1>500)&&(Sensor2<500)){
      return error = -1; 
     }
     else if((Sensor0<500)&&(Sensor1<500)&&(Sensor2>=500)){ 
      return error = 2; 
     }
     else if((Sensor0>=500)&&(Sensor1<500)&&(Sensor2<500)){ 
      return error = -2; 
     }  
     else if((Sensor0<500)&&(Sensor1<500)&&(Sensor2<500)){
      return error = 10; 
     }
} 

float ultra_front(){
  digitalWrite(trig_front,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_front,LOW);
  time1 = pulseIn(echo_front,HIGH);
  distance1 = time1 * 0.017; 
  return distance1;
}

float ultra_back(){
  digitalWrite(trig_back,HIGH);
  delayMicroseconds(10);
  digitalWrite(trig_back,LOW);
  time2 = pulseIn(echo_back,HIGH);
  distance2 = time2 * 0.017; 
  return distance2;
}

int check(){
  int loud;                         
  loud = analogRead(analogSensor);
  Serial.println(loud);
  if(loud<300){
    digitalWrite(buzzer, LOW);
  }
  else{
    digitalWrite(buzzer, HIGH);
   }
}

void motors_speed(int LFspeed,int RFspeed,int LBspeed,int RBspeed){
  analogWrite(LFmotor,LFspeed);
  analogWrite(RFmotor,RFspeed); 
  analogWrite(LBmotor,LBspeed);
  analogWrite(RBmotor,RBspeed);   
}

void forward (int error){
 switch (error){
  case 10:  
   motors_speed(240, 240, 0, 0);
  break;
  case 0: 
   motors_speed(255, 255, 0, 0);
  break;
  case 1: 
   motors_speed(250, 0, 0, 0);
  break;
  case -1: 
   motors_speed(0, 250, 0, 0); 
  break;
  case 2: 
   motors_speed(255, 0, 0, 0);
  break;
  case -2: 
   motors_speed(0, 255, 0, 0);
  break;
  }
 }

void backward (int error){
 switch (error){
  case 10: 
   motors_speed(0, 0, 240, 240);
  break;
  case 0: 
   motors_speed(0, 0, 255, 255);
  break;
  case 1: 
   motors_speed(0, 0, 245, 0);
  break;
  case -1: 
   motors_speed(0, 0, 0, 245); 
  break;
  case 2: 
   motors_speed(0, 0, 250, 0);
  break;
  case -2: 
   motors_speed(0, 0, 0, 250);
  break;
  }
 }

void loop() {
 if(digitalRead(go_forward)==0 && digitalRead(go_backward)==0){
  motors_speed(0, 0, 0, 0);
 }
 else if(digitalRead(go_forward)==1){
  check();
  ultra_front();
   if (distance1 < 10){
     motors_speed(0, 0, 0, 0); 
   }
   else{
     read_sensor();
     forward(error);
   }
 }
 else if(digitalRead(go_backward)==1){
   check();
   ultra_back();
    if (distance2 < 10){
      motors_speed(0, 0, 0, 0); 
    }
    else{
      read_sensor();
      backward(error);
    }  
   }
}
