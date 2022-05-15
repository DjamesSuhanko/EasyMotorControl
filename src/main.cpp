#include <Arduino.h>
#include <EasyPCF8575.h>
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
*/

/*
TODO: analisara direcao para saber se deve ignorar o fim de curso, senão não voltará na direção oposta
*/
unsigned int sw_conf = 4000; //Configuração de pulsos, conforme tabela sobre o drive. No exemplo, estamos dando 1 volta completa.

String condition = "stop";
struct pcon{
    uint8_t EN       = 5;
    uint8_t DIR      = 6;
    uint8_t PUL      = 7;
    uint8_t eoc      = 8; //end of curso
    uint8_t read_eoc = 10; //read end of curse
} pins_num;

void setup(){
    Serial.begin(115200);
    pinMode(8,OUTPUT); //sinal para o pino 10
    pinMode(10,OUTPUT); //le do fim de curso
    digitalWrite(8,HIGH); //ativa saida
    digitalWrite(10,LOW);

    for (uint8_t i = 5; i < 8; i++){
        pinMode(i, OUTPUT);
    }

    digitalWrite(pins_num.EN, HIGH);
}

void doStop(){
  if (Serial.available()){
    condition = Serial.readString();
  }
  if (digitalRead(pins_num.read_eoc) == 1){
    condition = "ok";
  }

}

void parameterizer(){
  if (Serial.available()){
      doStop();
      if (condition.indexOf("left") != -1){
        //for (int i=0; i<sw_conf; i++){
          while (condition != "stop"){ 
                                      doStop();
          digitalWrite(pins_num.DIR, LOW);
          Serial.println(digitalRead(pins_num.DIR));
          digitalWrite(pins_num.PUL, HIGH);
          delayMicroseconds(sw_conf);
          digitalWrite(pins_num.PUL, LOW);
          delayMicroseconds(sw_conf);
        }
      }
      else if (condition.indexOf("right") != -1){

        //for (int i=0; i<sw_conf; i++){
          while (condition != "stop"){ 
                                      doStop();
          digitalWrite(pins_num.DIR, HIGH);
          Serial.println(digitalRead(pins_num.DIR));
          digitalWrite(pins_num.PUL, HIGH);
          delayMicroseconds(sw_conf);
          digitalWrite(pins_num.PUL, LOW);
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
    if (digitalRead(10) == 1){
      Serial.println("fim de curso");
      delay(1000);
    }
}

/*
*/

/*
void setup(){}
void loop(){}
*/