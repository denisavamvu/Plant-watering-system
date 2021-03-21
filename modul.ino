// codul generat necesar pentru conectarea si configurarea modulului wifi
//////////////////////////////////////////////
//        RemoteXY include library          //
//////////////////////////////////////////////

// RemoteXY select connection mode and include library 
#define REMOTEXY_MODE__ESP8266_HARDSERIAL_POINT

#include <RemoteXY.h>

// RemoteXY connection settings 
#define REMOTEXY_SERIAL Serial
#define REMOTEXY_SERIAL_SPEED 115200
#define REMOTEXY_WIFI_SSID "RemoteXY"
#define REMOTEXY_WIFI_PASSWORD "12345678"
#define REMOTEXY_SERVER_PORT 6377


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,1,0,0,0,20,0,10,13,1,
  2,0,20,34,22,11,2,26,31,31,
  79,78,0,79,70,70,0 };
  
// this structure defines all the variables and events of your control interface 
struct {

    // input variables
  uint8_t switch_1; // =1 if switch ON and =0 if OFF 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop)

/////////////////////////////////////////////
//           END RemoteXY include          //
/////////////////////////////////////////////

//pin-ul pentru releu
#define PIN_SWITCH_1 12
//intrerupere interna pentru numararea secundelor
#include <TimerOne.h>

//pin pentru senzorul de umiditate
int SensorPin = A0; 

float sensorValue = 0;
int seconds=0;

//un interval reprezentand 10 secunde
int interval=10000;
long prevT=0, currT=0;
int flag = 0;

void setup() 
{

  RemoteXY_Init (); 
  
  pinMode (PIN_SWITCH_1, OUTPUT);

  Timer1.initialize(1000000);
   Timer1.attachInterrupt(count);
  
}

//numararea secundelor
void count()
{
  seconds++;
}

void loop() 
{ 
  RemoteXY_Handler ();

  sensorValue = analogRead(SensorPin);
  sensorValue = map(sensorValue, 0, 1023, 0, 100);

  //flag = 0 reprezinta faptul ca pompa nu a pornit din cauza senzorului
  currT=millis();
  if (RemoteXY.switch_1==0 && flag == 0)
         digitalWrite(PIN_SWITCH_1,HIGH);
           else 
  // daca s-a activat switch-ul se porneste releul (de mentionat ca este low level)
  if (RemoteXY.switch_1==1 && flag == 0)
         digitalWrite(PIN_SWITCH_1,LOW);
    if (seconds % 60 == 0 && sensorValue < 50 )
      { 
          digitalWrite(PIN_SWITCH_1,LOW);
          flag = 1;
      }
    // in caz ca a pornit pompa se tine 10 secunde si apoi se opreste 
    if(flag == 1 && currT-prevT>=interval)
      {
        prevT=currT;
        digitalWrite(PIN_SWITCH_1,HIGH);
        flag = 0;
       }
}
