#define BLYNK_PRINT Serial
#include <ESP8266WiFi.h>
#include <BlynkSimpleEsp8266.h>
#include <ThingSpeak.h>;

char auth[] = "dF8BaHiYDmHu1txHEJSAQuwUZyCnYmOU";

char ssid[] = "k27"; //Your Network SSID
char password[]= "987654321"; //Your Network Password

unsigned long myChannelNumber = 884872; //Your Channel Number (Without Brackets)
const char * myWriteAPIKey = "UKHN0ZN8Z7VDYVNB"; //Your Write API Key

WiFiClient client;

int sensor = A0; //MQ2 Sensor Connected at A0 Pin

int motorR1= D0;
int motorR2=D1;
int motorL1=D2;
int motorL2=D3;
int EN1=D5;
int EN2=D6;

BLYNK_WRITE(V1)
{
  int x = param[0].asInt();
  Serial.println(x);
  int y= param[1].asInt();
  Serial.println(y);
 
  movecontrol(x,y);
}

void setup()
{
    
    pinMode(sensor,INPUT);
    
    pinMode(EN1,OUTPUT);
    pinMode(EN2,OUTPUT);
    pinMode(motorL1,OUTPUT);
    pinMode(motorL2,OUTPUT);
    pinMode(motorR1,OUTPUT);
    pinMode(motorR2,OUTPUT);
    
    digitalWrite(EN1,HIGH);
    digitalWrite(EN2,HIGH);
    
    Serial.begin(115200);
    
  // Connect to WiFi network
  Serial.println(); 
  Serial.print("Connecting to ");
  Serial.println(ssid);
 
  WiFi.begin(ssid, password);
 
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi connected");
 
  Blynk.begin(auth, ssid, password);
    
  ThingSpeak.begin(client);

  
}

void movecontrol(int x,int y)
{


    if(x > 400 && x < 600 && y ==1000 )
    {
      forward();
      Serial.println("Moving forward");
    }
    else if(x > 400 && x < 600 && y ==0 )
    {
      backward();
      Serial.println("Moving Backward");
    }
    else if(x ==1000 && y > 400 && y<600 )
    {
      right();
      Serial.println("Moving Right");
    }
    else if(x ==0 && y > 400 && y<600 )
    {
      left();
      Serial.println("Moving Left");
    }
    else if( (x==500) and (y==500) )
    {
      nomove();
      Serial.println("Movement Stopped");
    }

}

void loop()
{
  Blynk.run();

  int gas_sensor = analogRead(sensor); //Read Analog values and Store in val variable
  Serial.println(gas_sensor); //Print on Serial Monitor
 
  delay(1000);
 
  int h = ThingSpeak.writeField(myChannelNumber, 1,gas_sensor, myWriteAPIKey); //Update in ThingSpeak
 
  if(h == 200)
  {
    Serial.println("Channel update successful.");
  }
  else
  {
    Serial.println("Problem updating channel. HTTP error code " + String(h));
  }

  delay(3000);

}


void forward()
{
      digitalWrite(motorL1,LOW);  //Left part moving anticlockwise
      digitalWrite(motorL2,HIGH);
      digitalWrite(motorR1,HIGH); //Right part moving clockwise
      digitalWrite(motorR2,LOW);
}
void backward()
{
      digitalWrite(motorL1,HIGH);  //Left part moving clockwise
      digitalWrite(motorL2,LOW);
      digitalWrite(motorR1,LOW);   //Right part moving anticlockwise
      digitalWrite(motorR2,HIGH);
}
void right()
{
      digitalWrite(motorL1,LOW);   //Left part moving anticlockwise
      digitalWrite(motorL2,HIGH);
      digitalWrite(motorR1,LOW);  //Right part not moving
      digitalWrite(motorR2,LOW);
}
void left()
{
      digitalWrite(motorL1,LOW);   //Left part not moving
      digitalWrite(motorL2,LOW);
      digitalWrite(motorR1,HIGH);  //Right part moving clockwise
      digitalWrite(motorR2,LOW);
}
void nomove()
{
      digitalWrite(motorL1,LOW);   //Left part not moving
      digitalWrite(motorL2,LOW);
      digitalWrite(motorR1,LOW);   //Right part not moving
      digitalWrite(motorR2,LOW);
}
