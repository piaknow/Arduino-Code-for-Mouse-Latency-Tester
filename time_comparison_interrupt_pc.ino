volatile char key;
const int sw1 = 2;
const int sw2 = 5;
volatile bool measuresw = true;
volatile bool measureserial = false;
volatile unsigned int time1;
volatile unsigned int time2;
volatile int diff;
volatile unsigned int seconds;
volatile unsigned int timelastoff;
volatile unsigned int count = 1;

void setup() {
  // put your setup code here, to run once:
  //max 457500 i guess
  Serial.begin(115200);
  Serial.println("started");
  pinMode(sw1, INPUT);
  pinMode(sw2, INPUT);
  attachInterrupt(sw1, stamp1, RISING);     //接点のONを検知
  attachInterrupt(sw2, stamp2, FALLING);    //フォトインタラプタのOFFを検知
}

void loop() {
  if(Serial.available()){
    time2 = micros();
    key = Serial.read();
    if(measureserial == true){  //接触してからはじめてのシリアル通信だけ
      diff = time2 - time1;
      seconds = time1/1000;
      Serial.print(count);
      Serial.print(" Time; ");
      Serial.print(time1);
      Serial.print("; ");
      Serial.println(diff);
      measureserial = false;      //接触後2回目以降のシリアル通信は処理なし
      count ++;
    }
  }
}

void stamp1(){    //switch on
  if(measuresw == true){          //台車が上がってからはじめての接触だけ
    time1 = micros();
    measureserial = true;         //接触したらシリアル通信の監視を開始
    //measuresw = false;            //接触の検知は一度だけ
  }
}

void stamp2(){    //photo interrupter trigger
  //measuresw = true;
}
