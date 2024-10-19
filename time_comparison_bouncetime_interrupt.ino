const int sw1 = 2;
const int sw11 = 3;
const int sw2 = 5;
volatile int timeon;
volatile int timeoff;
volatile int timer = 1000000;
//volatile bool switchon = false;
volatile bool sendinfo = false;
volatile bool sendinfo2 = false;

void setup() {
  // put your setup code here, to run once:
  
  Serial.begin(115200);
  Serial.println("started");
  pinMode(sw1, INPUT);
  pinMode(sw11, INPUT);
  pinMode(sw2, INPUT);
  attachInterrupt(sw1, stamp1, RISING);       //接点のONを検知
  attachInterrupt(sw11, stamp11, FALLING);    //接点のOFFを検知
  attachInterrupt(sw2, stamp2, FALLING);      //フォトインタラプタのOFFを検知
}

void loop() {
  // put your main code here, to run repeatedly:
  if(sendinfo == true && sendinfo2 == true){
    Serial.print(timeon);
    Serial.print(" ON ");
    Serial.print(timeoff);
    Serial.print(" OFF, diff: ");
    Serial.println(timeoff - timeon);
    sendinfo = false;
    sendinfo2 = false;
  }
}

void stamp1(){
  timeon = micros();
  
}

void stamp11(){
  timeoff = micros();
  if(sendinfo2 == true && sendinfo == false){
    sendinfo = true;
  }
}

void stamp2(){
  if(sendinfo2 == false){
    sendinfo2 = true;
  }
}
