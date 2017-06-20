/*Projeto final de 076
Agradecimento ao professor Tiago F. Tavares e ao PAD Guilherme Tahara pelo apoio e suporte durante o semestre e por terem transformado EA076 na melhor matéria da graduação.
Muito obrigado!
Autores:
Guilherme Sato - RA: 155639
Marco Aurélio de Oliveira Nona - RA: 156544
*/
#include <stdio.h> 
#include <LiquidCrystal.h>
#include "TimerOne.h"

/*-----------------------------------------------Declarações de variáveis-----------------------------------------------*/ 
int i = 0, warning = 0, alarm = 0, lcd_timer = 0, alarm_trigger = 0, counter_tone =0, set_tone = 0, siren = 0;
int livingroom = 0, kitchen = 0, bathroom = 0, bedroom = 0, all = 0, wrongpw_chances = 3;
int flag_check_command = 0, flag_serial_command = 0, control = 0;
int aux = 1;
char readBluetooth; 
char commands;
 
/*---------------------------------------Rotina auxiliar para comparacao de strings---------------------------------------*/

int str_cmp(char *s1, char *s2, int len) {
  /* Compare two strings up to length len. Return 1 if they are
   *  equal, and 0 otherwise.
   */
  int i;
  for (i=0; i<len; i++) {
    if (s1[i] != s2[i]) return 0;
    if (s1[i] == '\0') return 1;
  }
  return 1;
}

LiquidCrystal lcd(12, 11, 10, 8, 7, 6); // Definição de pinos do LCD

/*---------------------------------------Inicializações---------------------------------------*/ 
void setup(){
  /*Inicialização da Serial e da Serial1, esta utilizada para o bluetooth além de definições de pinmodes e inicialização da interrupção de tempo*/
  Serial.begin(9600);
  Serial1.begin(9600);//Inicia Comunicação Serial
  lcd.begin(16, 2);
  lcd.setCursor(0, 0);
  lcd.print("System is online");
  Timer1.initialize(2500);
  Timer1.attachInterrupt(temporizador);
  pinMode(16,OUTPUT);
  pinMode(15,OUTPUT);
  pinMode(14,OUTPUT);
  pinMode(20,OUTPUT);
  pinMode(21, INPUT);    
}
/*---------------------------------------Interrupção de tempo---------------------------------------*/ 
void temporizador() {
  /* Esta interrupção fará a contagem para a realização de um clear no timer, além de um incremento no tom simulando o som de uma sirene */
  counter_tone++;
  siren = counter_tone+2000;
  lcd_timer++;
  if(lcd_timer == 2000){
    lcd.clear();
  }
}

/*---------------------------------------Loop principal---------------------------------------*/ 
void loop(){
  /* Declarações de variáveis iniciais */
  char buffer_bluetooth[3];
  char buffer_clear[3] = "000"; 
  int door = digitalRead(21); // Leitura do pino do infravermelho
  /* Leitura de recebimento de dados provenientes do Bluetooth através de uma porta Serial. 
  Os dados sao armazenados em um buffer de tamanho 3, uma vez que os comandos dados sao de 3 digitos */
   if(Serial1.available()){
    readBluetooth = Serial1.read(); // Valor lido na Serial será guardado na variável readBluetooth
      for(i=0; i<2;i++){
        buffer_bluetooth [i] = buffer_bluetooth[i+1];
      }
    buffer_bluetooth[2] = readBluetooth;
    flag_check_command = 1;
  }
  /*Leitura da Serial via PC, sem buffer, para a simplificação do projeto, uma vez que é possível controlar todas as variáveis através de 4 letras representativas
  Q - Quarto
  S - Sala 
  B - Banheiro 
  C - Cozinha  */
  if(Serial.available()){
    commands = Serial.read(); // Valor lido na Serial será guardado na variável readBluetooth
    flag_serial_command  = 1;
  }
  if(flag_serial_command == 1){
    buffer_bluetooth[0] = buffer_clear[0];
    if(commands == 'Q'){
      /*Inversao da variavel do quarto e print das informações no LCD*/
      bedroom = !bedroom;
      lcd.clear();
      lcd.print("Alarm is ");
      lcd.setCursor(10,0);
      control = 1;
      flag_check_command = 1;
      if(alarm == 1){
        lcd.print("on");
      }
      else{
        lcd.print("off");
      }
      lcd.setCursor(0,1);
      lcd.print("Bedroom ");
      lcd.setCursor(9,1);
       if(bedroom == 1){
        digitalWrite(15, HIGH);
        lcd.print("on");
      }
      else{
        digitalWrite(15, LOW);
        lcd.print("off");
      }
    }
    /*Inversao da variavel da sala e print das informações no LCD*/
      if(commands == 'S'){
      livingroom = !livingroom;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Alarm is ");
      lcd.setCursor(10,0);
      control = 1;
      flag_check_command = 1;
      if(alarm == 1){
        lcd.print("on");
      }
      else{
        lcd.print("off");
      }
      lcd.setCursor(0,1);
      lcd.print("Living room ");
      lcd.setCursor(12,1);
       if(livingroom == 1){
        digitalWrite(20, HIGH);
        lcd.print("on");
      }
      else{
        digitalWrite(20, LOW);
        lcd.print("off");
      }
      lcd_timer = 0;
    }
    /*Inversao da variavel da cozinha e print das informações no LCD*/
    if(commands == 'C'){
      kitchen = !kitchen;
      lcd.clear();
      lcd.print("Alarm is ");
      lcd.setCursor(10,0);
      control = 1;
      flag_check_command = 1;
      if(alarm == 1){
        lcd.print("on");
      }
      else{
        lcd.print("off");
      }
      lcd.setCursor(0,1);
      lcd.print("Kitchen ");
      lcd.setCursor(8,1);
       if(kitchen == 1){
        digitalWrite(14, HIGH);
        lcd.print("on");
      }
      else{
        digitalWrite(14, LOW);
        lcd.print("off");
      }
    }
    /*Inversao da variavel do banheiro e print das informações no LCD*/
    if(commands == 'B'){
      bathroom = !bathroom;
      lcd.clear();
      lcd.print("Alarm is ");
      lcd.setCursor(10,0);
      control = 1;
      flag_check_command = 1;
      if(alarm == 1){
        lcd.print("on");
      }
      else{
        lcd.print("off");
      }
      lcd.setCursor(0,1);
      lcd.print("Bathroom ");
      lcd.setCursor(9,1);
       if(bathroom == 1){
        digitalWrite(16, HIGH);
        lcd.print("on");
      }
      else{
        digitalWrite(16, LOW);
        lcd.print("off");
      }
    }
    flag_serial_command = 0;
  }

  /*Condicional simples para o caso da leitura do sensor infravermelho for para zero (pull up)*/
  if(door == 0 && alarm == 1){
    alarm_trigger = 1;
    if (aux==1){
    lcd.setCursor(0, 0);
    lcd.print("Alarm TRIGGERED!");
    lcd.setCursor(0, 1);
    lcd.print("Door alarm!");
    lcd_timer = 0;
    aux = 0;
  }
  }
  /* Caso haja leitura de recebimento via Bluetooth entra nesta condicional */
  if(flag_check_command == 1){
    /*Mapeamento das variáveis recebidas para a senha
    PCO - Password Correct
    PIN - Password Incorrect
    Caso PCO seja inserido uma vez, seta a variável de alarme e desativa o trigger do alarme para interromper a sirene. Se a senha estiver incorreta 3 vezes, seta trigger do alarme.
    Se o alarme estiver desativado e a senha for inserida incorretamente, imprime "Try again" e nao dispara o alarme trigger em momento algum */
    if(str_cmp(buffer_bluetooth,"PCO", 3)){
      alarm_trigger = 0;
      if (alarm == 0){
        alarm = 1;
        wrongpw_chances = 3;
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Alarm is on");
        flag_check_command = 0;
        lcd_timer = 0;
      }
      else {
      alarm = 0;
      wrongpw_chances = 3;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Alarm is off");
      aux = 1;
      flag_check_command = 0;
      lcd_timer = 0;
      }
    }
     if(str_cmp(buffer_bluetooth,"PIN", 3) && alarm == 0){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Wrong Password");
      lcd.setCursor(0, 1);
      lcd.print("Try again");
      flag_check_command = 0;
      lcd_timer = 0;
    }
    if(str_cmp(buffer_bluetooth,"PIN", 3) && alarm == 1){
      wrongpw_chances--;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Wrong Password");
      lcd.setCursor(0, 1);
      if(wrongpw_chances > 0){
        lcd.print(wrongpw_chances);
        lcd.setCursor(3, 1);
        lcd.print("chances left");

      }
      if(wrongpw_chances <= 0){
        lcd.print("Alarm TRIGGERED!");
        alarm_trigger = 1;
      }
      flag_check_command = 0;
      lcd_timer = 0;
    }

    /*Mapeamento da iluminação da casa
    2 primeiras letras
    IS - Iluminação da Sala 
    IC - Iluminação da Cozinha
    IQ - Iluminação do Quarto
    IB - Iluminação do Banheiro
    IT - Iluminação de Tudo
    Terminações:
    B - Toggle simples (botão no app)
    L - Liga (comando de voz)
    D - Desliga (comando de voz)
    */
    if(str_cmp(buffer_bluetooth,"ISB", 3)){
      livingroom = !livingroom;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Alarm is ");
      lcd.setCursor(10,0);
      if(alarm == 1){
        lcd.print("on");
      }
      else{
        lcd.print("off");
      }
      lcd.setCursor(0,1);
      lcd.print("Living room ");
      lcd.setCursor(12,1);
      if(livingroom == 1){
        digitalWrite(20, HIGH);
        lcd.print("on");
      }
      else{
        digitalWrite(20, LOW);
        lcd.print("off");
      }
      
      flag_check_command = 0;
      lcd_timer = 0;
    }
    if(str_cmp(buffer_bluetooth,"ICB", 3)){
      kitchen = !kitchen;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Alarm is ");
      lcd.setCursor(10,0);
      if(alarm == 1){
        lcd.print("on");
      }
      else{
        lcd.print("off");
      }
      lcd.setCursor(0,1);
      lcd.print("Kitchen ");
      lcd.setCursor(8,1);
      if(kitchen == 1){
        digitalWrite(14, HIGH);
        lcd.print("on");
      }
      else{
        digitalWrite(14, LOW);
        lcd.print("off");
      }
      flag_check_command = 0;
      lcd_timer = 0;
    }
    if(str_cmp(buffer_bluetooth,"IBB", 3)){
      bathroom = !bathroom;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Alarm is ");
      lcd.setCursor(10,0);
      if(alarm == 1){
        lcd.print("on");
      }
      else{
        lcd.print("off");
      }
      lcd.setCursor(0,1);
      lcd.print("Bathroom ");
      lcd.setCursor(9,1);
      if(bathroom == 1){
        digitalWrite(16, HIGH);
        lcd.print("on");
      }
      else{
        digitalWrite(16, LOW);
        lcd.print("off");
      }
      flag_check_command = 0;
      lcd_timer = 0;
    }
    if(str_cmp(buffer_bluetooth,"IQB", 3)){
      bedroom = !bedroom;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Alarm is ");
      lcd.setCursor(10,0);
      if(alarm == 1){
        lcd.print("on");
      }
      else{
        lcd.print("off");
      }
      lcd.setCursor(0,1);
      lcd.print("Bedroom ");
      lcd.setCursor(9,1);
      if(bedroom == 1){
        digitalWrite(15, HIGH);
        lcd.print("on");
      }
      else{
        digitalWrite(15, LOW);
        lcd.print("off");
      }
      flag_check_command = 0;
      lcd_timer = 0;
    }
    if(str_cmp(buffer_bluetooth,"ITB", 3)){
      all = !all;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Alarm is ");
      lcd.setCursor(10,0);
      if(alarm == 1){
        lcd.print("on");
      }
      else{
        lcd.print("off");
      }
      lcd.setCursor(0,1);
      lcd.print("All is ");
      lcd.setCursor(7,1);
      if(all == 1){
        digitalWrite(14, HIGH);
        digitalWrite(15, HIGH);
        digitalWrite(16, HIGH);
        digitalWrite(20, HIGH);
        lcd.print("on");
      }
      else{
        digitalWrite(14, LOW);
        digitalWrite(15, LOW);
        digitalWrite(16, LOW);
        digitalWrite(20, LOW);
        lcd.print("off");
      }
      flag_check_command = 0;
      lcd_timer = 0;
    }

  if(str_cmp(buffer_bluetooth,"ISL", 3)){
      livingroom = 1;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Alarm is ");
      lcd.setCursor(10,0);
      if(alarm == 1){
        lcd.print("on");
      }
      else{
        lcd.print("off");
      }
      lcd.setCursor(0,1);
      lcd.print("Living room ");
      lcd.setCursor(12,1);
      digitalWrite(20, HIGH);
      lcd.print("on");
      flag_check_command = 0;
      lcd_timer = 0;
   }
   if(str_cmp(buffer_bluetooth,"ISD", 3)){
      livingroom = 0;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Alarm is ");
      lcd.setCursor(10,0);
      if(alarm == 1){
        lcd.print("on");
      }
      else{
        lcd.print("off");
      }
      lcd.setCursor(0,1);
      lcd.print("Living room ");
      lcd.setCursor(13,1);
      digitalWrite(20, LOW);
      lcd.print("off");
      flag_check_command = 0;
      lcd_timer = 0;
   }
   if(str_cmp(buffer_bluetooth,"ICL", 3)){
      kitchen = 1;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Alarm is ");
      lcd.setCursor(10,0);
      if(alarm == 1){
        lcd.print("on");
      }
      else{
        lcd.print("off");
      }
      lcd.setCursor(0,1);
      lcd.print("Kitchen ");
      lcd.setCursor(8,1);
      digitalWrite(14, HIGH);
      lcd.print("on");
      flag_check_command = 0;
      lcd_timer = 0;
    }
    if(str_cmp(buffer_bluetooth,"ICD", 3)){
      kitchen = 0;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Alarm is ");
      lcd.setCursor(10,0);
      if(alarm == 1){
        lcd.print("on");
      }
      else{
        lcd.print("off");
      }
      lcd.setCursor(0,1);
      lcd.print("Kitchen ");
      lcd.setCursor(8,1);
      digitalWrite(14, LOW);
      lcd.print("off");
      flag_check_command = 0;
      lcd_timer = 0;
    }
    if(str_cmp(buffer_bluetooth,"IBL", 3)){
      bathroom = 1;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Alarm is ");
      lcd.setCursor(10,0);
      if(alarm == 1){
        lcd.print("on");
      }
      else{
        lcd.print("off");
      }
      lcd.setCursor(0,1);
      lcd.print("Bathroom ");
      lcd.setCursor(9,1);
      digitalWrite(16, HIGH);
      lcd.print("on");
      flag_check_command = 0;
      lcd_timer = 0;
    }
    if(str_cmp(buffer_bluetooth,"IBD", 3)){
      bathroom = 1;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Alarm is ");
      lcd.setCursor(10,0);
      if(alarm == 1){
        lcd.print("on");
      }
      else{
        lcd.print("off");
      }
      lcd.setCursor(0,1);
      lcd.print("Bathroom ");
      lcd.setCursor(9,1);
      digitalWrite(16, LOW);
      lcd.print("off");
      flag_check_command = 0;
      lcd_timer = 0;
    }
    if(str_cmp(buffer_bluetooth,"IQL", 3)){
      bedroom = 1;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Alarm is ");
      lcd.setCursor(10,0);
      if(alarm == 1){
        lcd.print("on");
      }
      else{
        lcd.print("off");
      }
      lcd.setCursor(0,1);
      lcd.print("Bedroom ");
      lcd.setCursor(9,1);
      digitalWrite(15, HIGH);
      lcd.print("on");
      flag_check_command = 0;
      lcd_timer = 0;
   }
   if(str_cmp(buffer_bluetooth,"IQD", 3)){
      bedroom = 0;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Alarm is ");
      lcd.setCursor(10,0);
      if(alarm == 1){
        lcd.print("on");
      }
      else{
        lcd.print("off");
      }
      lcd.setCursor(0,1);
      lcd.print("Bedroom ");
      lcd.setCursor(9,1);
      digitalWrite(15, LOW);
      lcd.print("off");
      flag_check_command = 0;
      lcd_timer = 0;
   }
   if(str_cmp(buffer_bluetooth,"ITL", 3)){
      livingroom = 1;
      kitchen = 1;
      bedroom = 1;
      bathroom = 1;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Alarm is ");
      lcd.setCursor(10,0);
      if(alarm == 1){
        lcd.print("on");
      }
      else{
        lcd.print("off");
      }
      lcd.setCursor(0,1);
      lcd.print("All is ");
      lcd.setCursor(7,1);
      digitalWrite(14, HIGH);
      digitalWrite(15, HIGH);
      digitalWrite(16, HIGH);
      digitalWrite(20, HIGH);
      lcd.print("on");
      flag_check_command = 0;
      lcd_timer = 0;
    }
    if(str_cmp(buffer_bluetooth,"ITD", 3)){
      livingroom = 0;
      kitchen = 0;
      bedroom = 0;
      bathroom = 0;
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print("Alarm is ");
      lcd.setCursor(10,0);
      if(alarm == 1){
        lcd.print("on");
      }
      else{
        lcd.print("off");
      }
      lcd.setCursor(0,1);
      lcd.print("All is ");
      lcd.setCursor(7,1);
      digitalWrite(14, LOW);
      digitalWrite(15, LOW);
      digitalWrite(16, LOW);
      digitalWrite(20, LOW);
      lcd.print("off");
      flag_check_command = 0;
      lcd_timer = 0;
    }
    /*Envio da máscara para o app que faz o controle de recebimento de dados
    Será ativo em 2 situaçõaes:
    Quando houver uma conexão o app enviará o comando CON via Serial1.
    Caso digitado na Serial do PC, o comando CON será forçado através da variável de controle que envia a máscara da atual situação da iluminação
    A separação é dada por vírgulas e o primeiro bit e é de paridade que reconhecerá o recebimento para a entrada de dados*/
    if(str_cmp(buffer_bluetooth,"CON", 3) || control == 1){
        
        Serial1.write("1");
        Serial1.write(",");
      
      if(livingroom == 1){
      Serial1.write("1");
      Serial1.write(",");
      }
      else{
      Serial1.write("0");
      Serial1.write(",");
      }
      if(kitchen == 1){
      Serial1.write("1");
      Serial1.write(",");
      }
      else{
      Serial1.write("0");
      Serial1.write(",");
      }
      if(bedroom == 1){
      Serial1.write("1");
      Serial1.write(",");
      }
      else{
      Serial1.write("0");
      Serial1.write(",");
      }
      if(bathroom == 1){
      Serial1.write("1");
      Serial1.write(",");
      }
      else{
      Serial1.write("0");
      Serial1.write(",");
      }
      Serial1.write("0");   
      if(alarm == 1){
      Serial1.write("1");
      Serial1.write(",");
      }
      else{
      Serial1.write("0");
      Serial1.write(",");
      }
      Serial1.write("0"); 
      flag_check_command = 0;
      control = 0;
      lcd_timer = 0;  
    }
    /*Caso o recebimento for PAN, o botão de panico foi pressionado e realizará um trigger no alarme*/
    if(str_cmp(buffer_bluetooth,"PAN", 3)){
      lcd.setCursor(0,0);
      lcd.print("Alarm triggered");
      alarm_trigger = !alarm_trigger;
      flag_check_command = 0;
      lcd_timer = 0;
    }
    
    
  }
  /* Condicionais do alarme - Alternancia de som quando o trigger etá ativo e o desligamento forçado caso a variável seja alterada para 0 */
  if(alarm_trigger == 1){
    if(counter_tone == 700){
      counter_tone = 0;
      set_tone = !set_tone;
    }
    if (set_tone == 0){
      noTone(2);
    }
    if(set_tone == 1){
      tone(2, siren);
    }
  }
  if(alarm_trigger == 0){
    counter_tone = 0;
    noTone(2);
  }
}
