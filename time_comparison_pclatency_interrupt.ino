#include <Mouse.h>

const int out = 8;
volatile int count = 0;
volatile int interval = 150; 
volatile bool outon = false;

//volatile int time2 = 0;

void setup() {
  pinMode(out, OUTPUT);

  /////////////////////////////////////////////////////////
  //TC 1 Channel 0 = TC3_IRQn 2つ目のTimerの1つ目のチャンネル
  pmc_set_writeprotect(false); //レジスタ書き込み許可
  pmc_enable_periph_clk(TC3_IRQn); //Clockを有効化?
  TC1->TC_CHANNEL[0].TC_CMR = TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC | TC_CMR_TCCLKS_TIMER_CLOCK1; //tc1 ch0 
  //uint32_t rc = VARIANT_MCK/2/50000; //rc = フレーム数？  mck/2 = 84000000/2 = 42000000 fps
  TC1->TC_CHANNEL[0].TC_RC = 42000000;  //42 * 1000000
  TC_Start(TC1, 0);
  TC1->TC_CHANNEL[0].TC_IER = TC_IER_CPCS;
  TC1->TC_CHANNEL[0].TC_IDR = ~TC_IER_CPCS;
  NVIC_EnableIRQ(TC3_IRQn);
  /////////////////////////////////////////////////////////
  
  Mouse.begin();
  //Serial.begin(115200);
}

void loop() {
  exit(0);
}

void TC3_Handler(){
  TC_GetStatus(TC1, 0);
  if(count < 200){        //move left for 100000us
    
    //time2 = micros();  //デバッグ用 現在時刻の記録
    //Serial.print("left ");
    //Serial.println(time2);
    
    Mouse.move(-1,0);
    if(outon == false){
      digitalWrite(out, HIGH);
      outon = true;
    }
    count ++;
    //interval = 250;  //125us + 25us
    TC1->TC_CHANNEL[0].TC_RC = 10500;  //42 * 250
  }
  else{                   //Move right for 8 times

    //time2 = micros();  //デバッグ用 現在時刻の記録
    //Serial.print("right ");
    //Serial.println(time2);
    
    Mouse.move(1,0);
    if(outon == true){
      digitalWrite(out, LOW);
      outon = false;
    }
    count ++;
    if(count > 207){
      count = 0;
    }
    //interval = 141;  //187.5us - 2.5us  ポーリング間隔より若干大きいほうが、タイムスタンプが遅くなっていくから、右肩下がり
    TC1->TC_CHANNEL[0].TC_RC = 5922; //42 * 141
  }

  //time2 = micros();  //デバッグ用 現在時刻の記録  
  //割り込み処理は約36usで終わるけど、おそらくここにシリアル通信の処理時間は含まれない
  //Serial.print("Done ");
  //Serial.println(time2);
}
