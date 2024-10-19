const int sv1 = 4;

int seed1;
int seed2;

volatile bool pulseon = false;
volatile int pulseinterval = 20000;
volatile int pulsewidth = 1100;

volatile int pulsecount = 0;
volatile int phasee = 0;
volatile int degree = 0;
volatile int prevdegree = 0;
volatile int avgdegree = 0;
volatile int randnumber = 0;
volatile bool lowspeed = false;
//volatile int lastphaseestart;

//volatile int time1 = 0;
//volatile int time2 = 0;


void setup() {
  pinMode(sv1, OUTPUT);
  seed1 = analogRead(A2);
  seed2 = analogRead(A4);
  randomSeed(seed1*seed2*100);
  /////////////////////////////////////////////////////////
  //TC 1 Channel 0 = TC3_IRQn 2つ目のTimerの1つ目のチャンネル
  pmc_set_writeprotect(false); //レジスタ書き込み許可
  pmc_enable_periph_clk(TC3_IRQn); //Clockを有効化?
  TC1->TC_CHANNEL[0].TC_CMR = TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK1; //tc1 ch0 
  //uint32_t rc = VARIANT_MCK/2/50000; //rc = フレーム数？  mck/2 = 84000000/2 = 42000000 fps
  TC1->TC_CHANNEL[0].TC_RC = 42 * 3000000;
  TC_Start(TC1, 0);
  TC1->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
  TC1->TC_CHANNEL[0].TC_IDR = ~TC_IER_CPCS;
  NVIC_EnableIRQ(TC3_IRQn);
  /////////////////////////////////////////////////////////
  //Serial.begin(115200);
  //time1 = micros();
  //Serial.println(time1);
}

void loop() {
  exit(0);
}

void TC3_Handler(){
  TC_GetStatus(TC1, 0);
  if(pulseon == false){       // START NEW PULSE
    //time2 = micros();  //デバッグ用 現在時刻の記録
    //Serial.print("ON ");
    //Serial.println(time2);
    digitalWrite(sv1, HIGH);
    pulseon = true;
    TC1->TC_CHANNEL[0].TC_RC = 42 * (pulsewidth);
  }
  else{                       // TURN OFF PULSE
    //time2 = micros();  //デバッグ用 現在時刻の記録
    //Serial.print("OFF ");
    //Serial.println(time2);
    digitalWrite(sv1, LOW);
    pulseon = false;
    randnumber = random(0, 200);  //乱数つけて少しだけパルス頻度を高くする  avg20.0ms→19.9005
    TC1->TC_CHANNEL[0].TC_RC = 42 * (pulseinterval - pulsewidth - randnumber);
    //random makes pulse interval 19801 ~ 20000 us

    switch(phasee){  // adjust the next pulse when time is ample
      case 0:  //速い回転
        pulsecount ++;
        if(avgdegree <= 740 && avgdegree > 640 && lowspeed == false){  //大きい値のほうが早い段階から減速
          pulsewidth = 1130;  //減速する
          lowspeed = true;
          phasee = 1;
          //Serial.println(prevdegree);       //エンコーダーの数値確認
          //Serial.println(degree);
        }
        break;

      case 1:  //減速
        pulsecount ++;
        pulsewidth = 1290;  //さらに減速する
        phasee = 2;
        //lastphaseestart = millis();    //回転時間確認
        //Serial.println(" phasee3");
        break;

      case 2:  //遅い回転
        pulsecount ++;
        if(pulsecount >= 90){
          randnumber = random(0,10);
          pulsewidth = 1095 + randnumber;  //加速する
          pulsecount = 0;
          lowspeed = false;
          phasee = 0;
          //Serial.print(millis() - lastphaseestart);    //回転時間確認
          //Serial.println(" phasee0");
        }
        break;
    }
  }
  prevdegree = degree;
  degree = analogRead(A0);
  avgdegree = (degree + prevdegree) / 2;

  //time2 = micros();
  //Serial.print("Done ");  //割り込みの処理時間はON時36us OFF時38us
  //Serial.println(avgdegree);
  //Serial.println(time2);
}
