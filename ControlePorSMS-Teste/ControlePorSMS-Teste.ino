#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Sim800L.h>

LiquidCrystal_I2C lcd(0x27, 20, 4);
Sim800L GSM(3, 1); // rx tx
int contador = 0;
bool temSMS = false;
String telefoneSMS;
String dataHoraSMS;
String messagemSMS;
String comandoGSM = "";
String ultimoGSM = "";
bool status = false;
int callS=0;
bool callStatus = false;
byte character[8] = {
  B00000,
  B00001,
  B00001,
  B00101,
  B00101,
  B10101,
  B10101,
  B10101,
};

//byte WIFI[8] = {  B01110,  B10001,  B00000,  B01110,  B10001,  B00100,  B00100,  B00100,};


byte olhos[8] = {
  B00000,
  B00000,
  B01110,
  B11111,
  B10001,
  B01110,
  B00000,
  B00000,
};

byte olhando_esquerda[8] = {
  B00000,
  B00000,
  B01110,
  B11001,
  B11001,
  B01110,
  B00000,
  B00000,
};
byte olhando_direita[8] = {
  B00000,
  B00000,
  B01110,
  B10011,
  B10011,
  B01110,
  B00000,
  B00000,
};

byte pisca_olhos[8] = {
  B01110,
  B11011,
  B10001,
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
};
byte boca[8] = {
  B00000,
  B00000,
  B00000,
  B10001,
  B01110,
  B00000,
  B00000,
  B00000,
};
byte boca_direita[8] = {
  B00000,
  B00000,
  B00001,
  B00001,
  B01110,
  B00000,
  B00000,
  B00000,
};
byte boca_esquerda[8] = {
  B00000,
  B00000,
  B10000,
  B10000,
  B01110,
  B00000,
  B00000,
  B00000,
};
int x=0;

#define numeroCall "+258833501120"//"+258832001120"
#define senhaGSM "Odon"
#define pinBotaoCall 12
#define pinBotaoSMS 13
#define led 0
#define SMS 2
void leGSM();
void configuraGSM();
void enviaSMS(String telefone, String Messagem);
void animacao(int time, short coluna, short linha, char caractere);
void tamagotchi();


void setup() {
  pinMode(pinBotaoCall, INPUT_PULLUP);
  pinMode(led, OUTPUT);
  pinMode(SMS, INPUT_PULLUP);
  digitalWrite(led, LOW);
  Wire.begin(14, 15);
  lcd.begin(20, 4);
  lcd.init();
  lcd.createChar(0, character);
  lcd.createChar(1, olhos);
  lcd.createChar(2, boca);
  lcd.createChar(3, pisca_olhos);
  lcd.createChar(4, olhando_direita);
  lcd.createChar(5, olhando_esquerda);
  lcd.createChar(6, boca_direita);
  lcd.createChar(7, boca_esquerda);
  //lcd.createChar(8, WIFI);
  lcd.backlight();
  lcd.setCursor(13, 3);
  lcd.print("HACK-21");
  GSM.begin(115200);
  delay(3000);
  //GSM.listen();
  lcd.setCursor(0,0);
  lcd.print("Searching network");
  delay(1500);
  while (!GSM) {
    lcd.setCursor(0,0);
    lcd.print("Trying GSM Conect...");
    delay(500);
  }
  //configuraGSM();
  lcd.setCursor(0, 1);
  lcd.print("Configurando a rede");
  GSM.print("AT\n");
  delay(1000);
  //GSM.print("\nATZ\n");
  delay(1000);
  //GSM.print("ATH0\n");
  delay(1000);
  lcd.clear();
  lcd.setCursor(0, 2);
  lcd.print("All system it's [OK]");
  delay(200);
  //GSM.print("AT+GSN\n");
  //GSM.sendSms(numeroCall, "System online");
  delay(1200);
  lcd.clear();
}

void loop() {
  lcd.clear();
  tamagotchi();
  lcd.setCursor(0, 3);
  lcd.print("Sensor 1");
  lcd.setCursor(10, 3);
  lcd.print(analogRead(pinBotaoCall));
  lcd.setCursor(17, 3);
  lcd.print(contador);
  for (int x = 0; x<15; x++){
    tamagotchi();

    if ((digitalRead(SMS) == LOW) && (digitalRead(led) == LOW)){
      digitalWrite(led, HIGH);
      delay(250);
      }
    if ((digitalRead(SMS) == LOW) && (digitalRead(led) == HIGH)){
      digitalWrite(led, LOW);
      delay(250);
    }
    
    
    leGSM();
    if (comandoGSM !=""){
      lcd.clear();
      lcd.setCursor(0, 1);
      lcd.print(comandoGSM);
      ultimoGSM = comandoGSM;
      comandoGSM ="";
      delay(3000);
    }
    if (temSMS){
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("chegou SMS de:");
      lcd.setCursor(0, 1);
      lcd.print(telefoneSMS);
      lcd.setCursor(0, 2);
      lcd.setCursor(5, 1);
      lcd.print("Data ");
      lcd.print(dataHoraSMS);
      delay(5000);
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print(messagemSMS);
      delay(10000);

      messagemSMS.trim();
      if (messagemSMS == senhaGSM){
        lcd.clear();
        lcd.print("Enviando SMS de Resp");
        enviaSMS(telefoneSMS, "SMS recebido ok");
        delay(4000);
      }
      temSMS=false;
    }
    
    if (analogRead(pinBotaoCall)>=3400  && !callStatus){
      fazLigacao(numeroCall);
      callStatus = true;
      lcd.clear();
      contador++; 
      for(int i=0; i<3; i++){
        lcd.setCursor(10, 3);
        lcd.print(analogRead(pinBotaoCall));
        lcd.setCursor(0, 0);
        lcd.print("Alvo detectado");
        lcd.setCursor(0, 1);
        lcd.print("Enviando alerta   ");
        animacao(300, 15, 1, '.');
      }
      if (ultimoGSM.indexOf("ERROR")>-1){
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Erro! Ligacao no efe");
        lcd.setCursor(0, 1);
        lcd.print("tuada");
        callStatus = false;
        callS=20;
        delay(3000);
      }
      else{
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Chamada iniciada..!");
        delay(2000);
        lcd.clear();
      }
    }
    if (callStatus == true){
      lcd.setCursor(4, 0);
      lcd.print("Calling");
      animacao(500, 11, 0, '.');
      //digitalWrite(led, HIGH);
      callS++;
      if (callS >= 20){
        lcd.clear();
        lcd.setCursor(4, 0);
        //digitalWrite(led, LOW);
        lcd.print("Call done.");
        callStatus = false;
        callS=0;
        //delay(1000);
      }
      //
    }

    if (ultimoGSM.indexOf("+COLP:") > -1){
      
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("A ligacao foi iniciada...");
      GSM.print("ATL");
      ultimoGSM = "";
      delay(1000);
    }
    
    if (ultimoGSM.indexOf("NO CARRIER") > -1){
      callS = 20;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Ligacao Terminada...");
      lcd.setCursor(0, 1);
      lcd.print("Nao Atendeu");
      ultimoGSM = "";
      callStatus = false;
      delay(1500);
      lcd.clear();
    }

    if (ultimoGSM.indexOf("BUSY") > -1){
      callS = 20;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Linha Ocupada");
      ultimoGSM = "";
      //digitalWrite(led, LOW);
      callStatus = false;
      delay(1000);
    }

    if (ultimoGSM.indexOf("NO DIALTONE")> -1){
      callS = 20;
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Sm Linha");
      ultimoGSM = "";
      callStatus = false;
      delay(1000);
    }

    if (ultimoGSM.indexOf("NO ANSWER") > -1){
      lcd.clear();
      callS = 20;
      lcd.setCursor(0, 0);
      lcd.print("Nao atende...");
      ultimoGSM = "";
      callStatus = false;
      delay(3000);
    }
    //delay(50);
  }
  
  //
  //
  //
  //
}
void leGSM(){
  //lcd.clear();
  static String textoRec ="";
  static unsigned long delay1= 0;
  static int count=0;
  static unsigned char buffer[64];
  if (GSM.available()){
    while (GSM.available()) {
      buffer[count++] = GSM.read();
      if(count == 64)break;
    }
    textoRec +=(char*)buffer;
    delay1 = millis();
    for (int i=0 ; i< count; i++){
      buffer[i]=NULL;
    }
    count =0;
  }
  if (((millis()- delay1) > 100) && textoRec !=""){
    if (textoRec.substring(2,7)=="+CMT:"){
      temSMS = true;
    }
    if (temSMS){
      telefoneSMS = "";
      dataHoraSMS = "";
      messagemSMS = "";

      byte linha =0;
      byte aspas =0;
      for (int nL=1; nL < textoRec.length(); nL++){
        if (textoRec.charAt(nL)=='"'){
          aspas++;
          continue;
        }
        if ((linha ==1) && (aspas ==1)){
          telefoneSMS += textoRec.charAt(nL);
        }
        if ((linha ==1) && (aspas ==5)){
          dataHoraSMS += textoRec.charAt(nL);
        }
        if (linha ==2){
          messagemSMS +=textoRec.charAt(nL);
        }
        if (textoRec.substring(nL -1, nL +1)=="\r\n"){
          linha++;
        }
      }
    }
    else {
      comandoGSM = textoRec;
    }
    textoRec = "";
  }
}

void enviaSMS(String telefone, String Messagem){
  lcd.clear();
  lcd.setCursor(0, 0);
  GSM.print("AT+CMGS=\""+ telefone + "\"\n");
  GSM.print(Messagem + "\n");
  GSM.print((char)26);
}
/////
void fazLigacao(String telefone){
  lcd.clear();
  lcd.setCursor(0, 0);
  GSM.println("ATH0\n");
  GSM.print((char)26);
  GSM.println("ATD "+ telefone +";\n");
  GSM.print((char)26);
}

void configuraGSM(){
  GSM.print("AT+CMGF=1\n;AT+CNMI=2,2,0,0,0\n;ATX4\n;AT+COLP=1"); //AT+COLP=1
}

void animacao(int time, short coluna, short linha, char caractere){
  lcd.setCursor(coluna, linha);
  lcd.print(caractere);
  delay(time);
  lcd.setCursor(coluna+1, linha);
  lcd.print(caractere);
  delay(time);
  lcd.setCursor(coluna+2, linha);
  lcd.print(caractere);
  delay(time);
  lcd.setCursor(coluna, linha);
  lcd.print("   ");
}
void tamagotchi(){
  lcd.clear();
  x++;
  if (x %2==0){
    lcd.setCursor(8, 1);
    lcd.write(byte(4)); //  olhando direita
    lcd.setCursor(10, 1);
    lcd.write(byte(4));
    //SIREN
    //lcd.setCursor(0,0);
    //lcd.write(byte(8));

    lcd.setCursor(19, 0);
    lcd.write(byte(0));
    //BOCA
    lcd.setCursor(9, 2);
    lcd.write(byte(7));
    delay(2000);
  }
  if (x %5==1){
    lcd.setCursor(8, 1);
    lcd.write(byte(5)); // olhando esquerda
    lcd.setCursor(10, 1);
    lcd.write(byte(5));
    //SIREN
    //lcd.setCursor(0,0);
    //lcd.write(byte(8));
    
    lcd.setCursor(19, 0);
    lcd.write(byte(0));
    //BOCA
    lcd.setCursor(9, 2);
    lcd.write(byte(6));
    delay(2000);
  }
  else{
    lcd.setCursor(8, 1);
    lcd.write(byte(1)); // olhos normais

    lcd.setCursor(10, 1);
    lcd.write(byte(1));
    // SIREN
    
    lcd.setCursor(19, 0);
    lcd.write(byte(0));
    // BOCA
    lcd.setCursor(13,1);
    lcd.print("zzz");

    lcd.setCursor(9, 2);
    lcd.write(byte(2));
    delay(1000);
  }
  delay(300);
  if (x>1000) x=0;

}