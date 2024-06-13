#include <Wire.h>
#include <Sim800L.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 20, 4);
Sim800L GSM(3, 1);

void setup() {
  // put your setup code here, to run once:
  Wire.begin(14, 15);
  lcd.begin(20, 4);
  GSM.begin(115200);
  GSM.listen()
  lcd.init();
  lcd.backlight();
  lcd.noCursor();
  pinMode(4, OUTPUT);
  analogWrite(4, 1);
  delay(100);
  analogWrite(4, 0);
  delay(7000);
  lcd.setCursor(15, 3);
  lcd.print("Done!");

  SMSsnd("+258833501120", "Arduino ON, lendo mensagens...");
}
  

  
void loop() {
  // put your main code here, to run repeatedly:
  SMSrd();//Verifica se há novas mensagens para serem lidas a cada 10seg
  delay(5000);
}

String gsmWrite(String snd, String rcv)//Funcao que envia dados pro GSM e espera a resposta de ate 30seg
{
  GSM.println(snd);

  if (rcv.indexOf("+CMGS") > -1)
  {
    delay(150);
    GSM.write(0x1A);
  }

  for (uint16_t i = 0; i < 1200; i++)
  {
    delay(25);
    if (GSM.available())
    {
      delay(50);
      String a = GSM.readString();


      if (a.indexOf(rcv) > -1 || rcv.length() == 0)
      {
        return a;
      }
    }
  }

  return "FAIL";
}

void SMSsnd(String nm, String msg)//Funcao que envia o SMS
{
  String snd = "AT+CMGS=\"";  snd += nm;  snd += "\"";

  gsmWrite("AT+CMGF=1", "OK");
  gsmWrite("AT+CSMP=17,255,0,0", "OK");
  gsmWrite(snd, ">");
  gsmWrite(msg, "+CMGS:");
}

void SMSrd()//Funcao que le se ha SMS nao lido
{
  String c = "";
  gsmWrite("AT+CMGF=1", "OK");
  c = gsmWrite("AT+CMGL=\"REC UNREAD\"", "");

  if (c.indexOf("+CMGL:") > -1)
  {
    if    (c.indexOf("ON") > -1)//ON
    {
      analogWrite(4, 10);//se o SMS conter o texto ON, liga o LED
      lcd.setCursor(0, 1);
      lcd.print("LED LIGADO");
    }
    else if (c.indexOf("OFF") > -1)//OFF
    {
      analogWrite(4, 0);//se o SMS conter o texto OFF, desliga o LED
      lcd.setCursor(0, 2);
      lcd.print("LED Desligado");
    }

    gsmWrite("AT+CMGD=1,1", "OK");//apaga todas mensagens (SIM card ha pouca memoria)
  }
}

