#include <Arduino.h>
#include <EasyPCF8575.h>

EasyPCF8575 pcf;
/*
Se estiver configurado para 400 no dip switch (ON ON ON ON _ _ _ _) e o valor em sw_conf for mantido em 400, teremos 1 volta completa. 
Se a configuração for 800 (OFF ON ON ON _ _ _ _) e for mantido 400, será dada meia volta. Para uma volta completa na metade da velocidade inicial, use 800 e OFF ON ON ON _ _ _ _.
Lembre-se: A tabela mostra pulsos por revolução, então podemos controlar velocidade e avanço.


WIRING (UNO Leonardo etc) e Control Signal:
Arduino / Drive
D5     ---> ENA-
VCC    ---> ENA+
GND    ---> DIR-    
D6     ---> DIR+
D7     ---> PUL-
VCC    ---> PUL+

-------------- PCF8575 ----------------------
Bits   |   Driver
0      |   ENA-
1      |   DIR+
2      |   PUL-

3      | SIGNAL para EOC
4      | Leitura do EOC  - TODO: Deixar leitura no segundo byte?

Da MCU:
VCC    |   ENA+
GND    |   DIR-
VCC    |   PUL+
*/

/*
TODO: analisar a direcao para saber se deve ignorar o fim de curso, senão não voltará na direção oposta
*/
unsigned int sw_conf = 4000; //Configuração de pulsos, conforme tabela sobre o drive. No exemplo, estamos dando 1 volta completa.

String condition     = "stop";

enum {
    EN,
    DIR,
    PUL,
    EOC_VCC,
    EOC_READ
};

void setup(){
    Serial.begin(115200);
    pcf.startI2C(21,22,SEARCH);
    pcf.setAllBitsDown();
    //EN, DIR e PUL: motor
    //EOC_VCC: sinal para o EOC
    pcf.setNewLeftByteValue((1<<EN)|(1<<DIR)|(1<<PUL)|(1<<EOC_VCC));
}

void doStop(){
  if (Serial.available()){
    condition = Serial.readString();
  }
  //TODO: validar essa condição porque acho que é pra ser 0
  if (pcf.getBitValue(EOC_READ) == 1){
    condition = "ok";
  }

}

//Tutorial de biblioteca

void parameterizer(){
  if (Serial.available()){
      doStop();
      if (condition.indexOf("left") != -1){
        //for (int i=0; i<sw_conf; i++){
          while (condition != "stop"){ 
            doStop();
            pcf.setBitDown(DIR);
            Serial.println(pcf.getBitValue(DIR));
            pcf.setBitUp(PUL);
            delayMicroseconds(sw_conf);
            pcf.setBitDown(PUL);
            delayMicroseconds(sw_conf);
        }
      }
      else if (condition.indexOf("right") != -1){

        //for (int i=0; i<sw_conf; i++){
            while (condition != "stop"){ 
                doStop();
                pcf.setBitUp(DIR);
                Serial.println(pcf.getBitValue(DIR));
                pcf.setBitUp(PUL);
                delayMicroseconds(sw_conf);
                pcf.setBitDown(PUL);
                delayMicroseconds(sw_conf);
            }
        }
        else if (condition.indexOf("stop") != -1){
            Serial.println("stopped");
        }
    }
    delay(1000);
}

void loop(){
    parameterizer();
    if (pcf.getBitValue(EOC_READ) == 1){
      Serial.println("fim de curso");
      delay(1000);
    }
}