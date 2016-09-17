/*
 * IRremote: IRrecvDump - dump details of IR codes with IRrecv
 * An IR detector/demodulator must be connected to the input RECV_PIN.
 * Version 0.1 July, 2009
 * Copyright 2009 Ken Shirriff
 * http://arcfn.com
 * JVC and Panasonic protocol added by Kristian Lauszus (Thanks to zenwheel and other people at the original blog post)
 * LG added by Darryl Smith (based on the JVC protocol)
 */
#include <IRremote.h>

/* 
*  Default is Arduino pin D11. 
*  You can change this to another available Arduino Pin.
*  Your IR receiver should be connected to the pin defined here
*/
int RECV_PIN = 11; 

IRrecv irrecv(RECV_PIN);
IRsend irsend;

decode_results results;

void setup() {
    Serial.begin(9600);
    irrecv.enableIRIn(); // Start the receiver

    pinMode(2,INPUT) ;    //スイッチに接続ピンをデジタル入力に設定
    pinMode(13,OUTPUT) ;  //ＬＥＤに接続ピンをデジタル出力に設定
}


// IRrecvDump
void dump(decode_results *results) {
    // Dumps out the decode_results structure.
    // Call this after IRrecv::decode()
    int count = results->rawlen;
    if      (results->decode_type == UNKNOWN)   {    Serial.print("Unknown encoding: "); }
    else if (results->decode_type == NEC)           { Serial.print("Decoded NEC: "); }
    else if (results->decode_type == SONY)          { Serial.print("Decoded SONY: "); }
    else if (results->decode_type == RC5)           { Serial.print("Decoded RC5: "); }
    else if (results->decode_type == RC6)           { Serial.print("Decoded RC6: "); }
    else if (results->decode_type == PANASONIC)     { Serial.print("Decoded PANASONIC - Address: ");
                                                      Serial.print(results->address, HEX);
                                                      Serial.print(" Value: "); }
    else if (results->decode_type == LG)            { Serial.print("Decoded LG: "); }
    else if (results->decode_type == JVC)           { Serial.print("Decoded JVC: "); }
    else if (results->decode_type == AIWA_RC_T501)  { Serial.print("Decoded AIWA RC T501: "); }
    else if (results->decode_type == WHYNTER)       {  Serial.print("Decoded Whynter: "); }

    Serial.print(results->value, HEX);
    Serial.print(" (");
    Serial.print(results->bits, DEC);
    Serial.println(" bits)");
    Serial.print("Raw (");
    Serial.print(count, DEC);
    Serial.print("): ");

    for (int i = 1; i < count; i++) {
        if (i & 1) {
            Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
        } else {
            Serial.write('-');
            Serial.print((unsigned long) results->rawbuf[i]*USECPERTICK, DEC);
        }
        Serial.print(" ");
    }
    Serial.println();
}

void loop() {
    //==========================
    // IR解析
    //==========================
    if (irrecv.decode(&results)) {
        Serial.println(results.value, HEX);
        dump(&results);
        irrecv.resume(); // Receive the next value
    }

    //==========================
    // IR送信
    //==========================
    if (digitalRead(2) == HIGH) {     //スイッチの状態を調べる
        digitalWrite(13,HIGH) ;      //スイッチが押されているならLEDを点灯
        // NECデータを送ってみる
        for (int i = 0; i < 3; i++) {
//            irsend.sendNEC(0x1D00D926, 32);   // volume up
            irsend.sendNEC(0x1D00B946, 32);     // power on/off
            delay(40);
        }
        delay(1000); //5 second delay between each signal burst
     } else {
        digitalWrite(13,LOW) ;       //スイッチが押されていないならLEDを消灯
     }
}
