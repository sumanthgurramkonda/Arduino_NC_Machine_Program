  /*TITLE: Development of algorithms for Numerical Control System
* PURPOSE: This project demonstrates motion in two demensional axis for different geometries
* NBKR Institute of Science and Technology
* Mechanical Engineering
* ARDUINO : https://www.arduino.cc/en/Reference
* AUTHOR: Sumanth Gurramkonda 
* Contact details: sumanthgrk@gmail.com
* DATE: 17/06/2022
*/
#include<Servo.h>

#define x_step_pin 22
#define x_dir_pin 24
#define x_enb_pin 26

#define y_step_pin 28
#define y_dir_pin 30
#define y_enb_pin 32

#define Z_pin 34

#define x_start_pin 36
#define x_end_pin 38

#define y_start_pin 40
#define y_end_pin 42

#define X_Dir_off  digitalWrite(x_dir_pin,0);
#define X_Dir_on   digitalWrite(x_dir_pin,1);

#define Y_Dir_off  digitalWrite(y_dir_pin,0);
#define Y_Dir_on   digitalWrite(y_dir_pin,1);

Servo Z_axis;

#define Z_up    Z_axis.write(120)
#define Z_down  Z_axis.write(180)

const int X=24,Y=15;
float l,b,h,k,m,r,n,conf;
int s;
float x,y;
String c;
float data;
float x_steps,y_steps;
long x_r,y_r;


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void setup() {
  
Serial.begin(9600);

Intro();
symbols();
pinMode(x_step_pin,OUTPUT);
pinMode(y_step_pin,OUTPUT);
pinMode(x_dir_pin,OUTPUT);
pinMode(y_dir_pin,OUTPUT);

pinMode(x_start_pin,INPUT);
pinMode(x_end_pin,INPUT);
pinMode(y_start_pin,INPUT);
pinMode(y_end_pin,INPUT);

Z_axis.attach(Z_pin);

}


////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void loop() {
  x_r=0,y_r=0;
 Z_up;
 Z_axis.write(90);
 X_Dir_off;
 Y_Dir_on;
 x_steps = 0;
 y_steps = 0;
Serial_Interface();
delay(2000);
return_zero();
delayMicroseconds(1000);
 
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Serial_Interface(){
  
if(Serial.available()>0) data = Serial.read();                  // Initialising the Serial monitor to Zero 
data = 0;
_delay_ms(2000);
Serial.println("");
Serial.println("Enter any one command to execute the operation");
Serial.println("");

while(Serial.available()==0);                                  // This loop Keeps running until the data is avaialable on serial monitor
//c = Serial.readString();
data = Serial.read();
//Serial.println(char(data));
delay(1000);

/////////////////////////////////////////////////////////////////////////////////////////////// 

if (data == 'l'|| data == 'L' ){                                            // It takes the line input values (Starting point, Length, Slope)
  Serial.println("You Selected line");
  while(Serial.available()==0);
  Serial.println("Enter the starting point of the line");
  Serial.print("h = ");
  h = serial_read();
  Serial.println(h);
  
  Serial.print("k = ");
  k = serial_read();
  Serial.println(k);
  
  Serial.println("Enter the length and slope of the line : ");
  Serial.print("Length = ");
  l = serial_read();
  Serial.println(l);
  
  Serial.print("slope = ");
  m = serial_read();
  Serial.println(m);
  Serial.println("");
  data = 'l';
  line(h,k,l,m);           // pass the parameters to line function 

}
////////////////////////////////////////////////////////////////////////////////////////////////////
else if(data == 'c' || data == 'C'){
   Serial.println(F("You selected circle"));
   Serial.println("Enter center point of the curve : ");
   Serial.print("h = ");

   h = serial_read();
   Serial.println(h);

   Serial.print("k = ");
   k = serial_read();
   Serial.println(k);
   
   Serial.println("Enter the radius of the curve");
   Serial.print("R = ");
   r = serial_read();
   Serial.println(r);
  data = 'c';
  circle(h,k,r);           // pass the parameters to circle function
  
}///////////////////////////////////////////////////////////////////////////////////////////////////
else if(data == 'r' || data == 'R'){
   Serial.println(F("You selected Rectangle"));
   Serial.println("Enter starting point of the Rectangle : ");
   Serial.print("h = ");
   h = serial_read();
   Serial.println(h);

   Serial.print("k = ");
   k = serial_read();
   Serial.println(k);
   
   Serial.println("Enter the Length and Width of the rectangle");
   Serial.print("L = ");
   l = serial_read();
   Serial.println(l);

   Serial.print("B = ");
   b = serial_read();
   Serial.println(b);
   data ='r';
   
   rectangle(h,k,l,b);
}
else if(data == 't' || data == 'T'){

   Serial.println(F("You selected Traingle"));
   Serial.println("Enter Starting point of the Traingle : ");
   Serial.print("h = ");
   h = serial_read();
   Serial.println(h);

   Serial.print("k = ");
   k = serial_read();
   Serial.println(k);
   
   Serial.println("Enter Length of the Traingle");
   Serial.print("L = ");
   l = serial_read();
   Serial.println(l);
   data = 't';
  
  traingle(h,k,l);
}
else Serial.println("Input data is not valid");
 }
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int serial_read(){

  if(Serial.available()>=0) data = Serial.read();
  data = 0;
  while(Serial.available()==0);
  return Serial.parseInt();
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void line(float h,float k,float l,float theta){                    // Line algorithm
  
  x=0,y=0;
  x =  l*cos((PI/180)*theta);
  y =  l*sin((PI/180)*theta);
 
  if(configure(x+h,y+k))  Serial.println("Input parameters are not within the limit");
  else {
   if(h>0 || k>0) calculate_L_steps(h,k);
  
   if(x>0 || y>0){
    Z_down;
    calculate_L_steps(x,y);
   }
   } 
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void circle(float h,float k, float r){     // circle algorithm
  if(h-r>=0 && k-r >=0) {
    
   // calculate_L_steps(h,k-r);
    delay(1000);
    calculate_C_steps(r); 
  }
  else Serial.println("Input parameters are not within the limit");
  return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void rectangle(float h,float k, float l, float b){                // Rectangle algorithm
    calculate_L_steps(h,k);
    delay(1000);
   Y_Dir_on;
   delay(500);
   for(int i=0;i<b*254;i++) run_y();
   X_Dir_off;
   delay(500);
   for(int i=0;i<l*254;i++) run_x();
   Y_Dir_off;
   delay(500);
   for(int i=0;i<b*254;i++) run_y();
   X_Dir_on;
   delay(500);
   for(int i=0;i<l*254;i++) run_x();
   delay(1500);  
    return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void traingle (float h,float k, float l){              // Traingle algorithm
  delay(1000);
  X_Dir_off;
  Y_Dir_on;
  calculate_L_steps(h,k);
  delay(1000);
  x=0,y=0;
  x =  l*cos((PI/180)*60)+1;
  y =  l*sin((PI/180)*60)+1;
  calculate_L_steps(l,0);
   X_Dir_on;
   Y_Dir_on;
   delay(500);
   calculate_L_steps(x,y);
   X_Dir_on;
   Y_Dir_off;
   delay(500);
   calculate_L_steps(x,y);
   x_r = h*250,y_r = k*250;
   delay(500);
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool configure(float x,float y){
  if(x<=X&&y<=Y)  return 0;
  else if(x>X && y>Y || x==0 && y==0) return 1;
  }

////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void calculate_L_steps(float x, float y){
 
    int Xtotsteps = round(x*254);
    int Ytotsteps = round(y*254);
    if(data!='t' || data != 'T'){
    x_r=x_r+Xtotsteps-6*x;
    y_r=y_r+Ytotsteps-6*y;
    }
    delay(2000);
    
   int da = Xtotsteps; // converting x distance in terms of steps
   int db = Ytotsteps; // converting y distance in terms of steps
   int diff = 2*db - da;
   long xp[da]={0},yp[da]={0};
 //  Serial.println( Xtotsteps);
   byte a,b;
    long timer0 = millis();
    if(da>db){
   for (int k = 0; k < Xtotsteps; k++)
   {
           if (diff < 0)
          {
             run_x();
             xp[k]=1;             
             yp[k]=0;
             diff = diff + 2*db; 
           }
           else
          {
            run_x();
            run_y();
            xp[k]=1; 
            yp[k]=1;
            diff = diff + 2*db - 2*da; 
          }
  
   }
    }
    else{
      diff = 2*da - db;
         for (int k = 0; k < Xtotsteps; k++)
   {
           if (diff < 0)
          {
             run_y();
             xp[k]=1;             
             yp[k]=0;
             diff = diff + 2*da; 
           }
           else
          {
            run_x();
            run_y();
            xp[k]=1; 
            yp[k]=1;
            diff = diff + 2*da - 2*db; 
          }
   }
    }
 
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void calculate_C_steps(float r){

    int x = 0, y = r*254;
    int x_steps[y];
    int y_steps[y];
    int d = 3 - 2 * r;
    long int i=0;
    while (y >= x)
    {   i++;
        x++;
        x_steps[i]=1;
        y_steps[i]=0;
        run_x();
        _delay_ms(2);
    
        if (d > 0)
        {
            y--;
            y_steps[i]=1;
            x_steps[i]=0;
            run_y();
         _delay_ms(2);
            d = d + 4 * (x - y) + 10;
        }
        else d = d + 4 * x + 6;
    }
    /*
    for(i=0;i<=r*100;i++){
      if(x_steps[i]==1)run_x();
      _delay_ms(2);
      if(y_steps[i]==1)run_y();
      _delay_ms(2);
    }
    for(i;i>=0;i--){
      if(y_steps[i]==1)run_x();
      _delay_ms(2);
      if(x_steps[i]==1)run_y();
      _delay_ms(2);
    }
    */

}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void return_zero(){
 X_Dir_on;
 Y_Dir_off;
   
  while(x_r !=0 && y_r != 0){
    if(x_r)run_x();
    if(y_r)run_y();
    x_r --;
    y_r --;
  }
  return 0;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void run_x(){

  digitalWrite(x_step_pin,1);
  _delay_ms(2);
  digitalWrite(x_step_pin,0);
  _delay_ms(2);
  
}
void run_y(){
  
  digitalWrite(y_step_pin,1);
  _delay_ms(2);
  digitalWrite(y_step_pin,0);
  _delay_ms(2);
  
}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Intro(){
  delay(1500);
  Serial.println(" ");
  Serial.println(" ");
  Serial.println("                                                                                               Welcome ");
  Serial.println(" ");
  Serial.println("                                                                                         Project Work 2022 ");
  Serial.println(" ");
  Serial.println(" ");
  delay(1500);
  Serial.println("                                                                                           Project Title");
  Serial.println("                                                                             Development of Algorithms for NC Machines ");
  Serial.println(" ");
  Serial.println(" ");
  delay(1500);
  Serial.println("                                                                               Department of Mechanical Engineering ");
  Serial.println(" ");
  Serial.println(" ");
  delay(1000);
  Serial.println("                                                                        Dr.CH.R.  Vikram Kumar ( Head of Mechanical Engineering ) ");
  Serial.println(" ");
  Serial.println(" ");
  delay(1000);
  Serial.println("                                                                                       Under the Guidance of ");
  Serial.println("                                                                              Dr.B. Satheesh Kumar( Associate Professor )");
  Serial.println(" ");
  Serial.println(" ");
  Serial.println(" ");
  delay(1500);
  Serial.println("                                                                     Team Members: ");
  Serial.println("                                                                                            G.Sumanth ");
  Serial.println(" ");
  Serial.println("                                                                                            Y.Sai ");
  Serial.println(" ");
  Serial.println("                                                                                            B.Ajay Reddy ");
  Serial.println(" ");
  Serial.println("                                                                                            A.Nitish ");

return 0;
}
void symbols(){
  delay(1500);
  Serial.println( "                For Line Enter      'l' or 'L'");
  Serial.println(" ");
  Serial.println( "                For Rectangle Enter 'r' or 'R'");
  Serial.println(" ");
  Serial.println( "                For Curve Enter     'c' or 'C'");
  Serial.println(" ");
  Serial.println( "                For Traingle Enter  't' or 'T'");
  Serial.println(" ");
}
