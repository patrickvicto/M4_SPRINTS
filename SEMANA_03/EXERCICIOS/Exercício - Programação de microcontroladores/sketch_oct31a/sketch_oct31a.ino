//Declaração de variaveis
int LEDS[4] = {14,13,11,9};
int button_save = 6;
int estado_save = LOW;
int button_clean = 18;
int estado_clean = LOW;
int VCLUZ = 0;
int VCLUZES[30] = {};
int position = 0;
int BUZZER_PIN = 10;

//função para converter o valor recbido no intervalo de 0 a 15
  int converteSensor(int valor, int minimo, int maximo) {
	// realiza o calculo para ajustar valor no intervalo
	int convertido = float(((valor - minimo) / float(maximo - minimo))*15);
	// retorna o valor convertido;
	return convertido; 
}


  

void setup() {
  //declaração dos pinos
  Serial.begin(115200);
  for (int i = 0; i<4; i++){
  pinMode(LEDS[i], OUTPUT);
  }
  pinMode(button_save, INPUT_PULLUP);
  pinMode(button_clean, INPUT_PULLUP);
  pinMode(BUZZER_PIN, OUTPUT);
}

void loop() {

  //roda em loop para o funcionamento do sistema.
  //confere se o botão foi precionado
  estado_save = digitalRead(button_save);
  // se ele foi precionado
  if (estado_save == LOW){
    VCLUZ = converteSensor(analogRead(4),64,4095);
    Serial.println(VCLUZ);
    //acender as leds referentes ao valor lido
    int acender[4]= {0,0,0,0};
    for (int i = 0; i<4; i++){
      acender[i] = (1&VCLUZ>>i);
      if (acender[i] == 1){
        digitalWrite(LEDS[i],HIGH);
      }
    }
  //faz o barulho
  tone(BUZZER_PIN,(100+(150*VCLUZ)),3000);
  //salva o valor
  VCLUZES[position] = VCLUZ;
  position += 1;
  delay(2500);
  // desliga os leds
    for (int i = 0; i<4; i++){
      digitalWrite(LEDS[i],LOW);
    }
  }


 // confere se o botão foi precionado
  estado_clean = digitalRead(button_clean);
  // se ele foi precionado
  if (estado_clean == LOW){
    // acende todos os leds e faz os braulhos referentes aos valores salvos do ultimo ao primeiro
    int acender_2[4]= {0,0,0,0};
    for (int h = 0; h< position; h++){
      VCLUZ = VCLUZES[h];
      Serial.println(VCLUZ);
      for (int i = 0; i<4; i++){
        acender_2[i] = (1&VCLUZ>>i);
        if (acender_2[i] == 1){
          digitalWrite(LEDS[i],HIGH);
        }
      }
      tone(BUZZER_PIN,(100+(150*VCLUZES[h])),3000);
      VCLUZES[h] = 0;
      delay(5000);
      for (int i = 0; i<4; i++){
        //desliga as leds
        digitalWrite(LEDS[i],LOW);
          } 
    }
    position = 0;
  }
}
