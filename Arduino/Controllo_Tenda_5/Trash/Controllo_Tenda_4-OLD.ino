// ---------------012345678901234567890-----------------------------------------------------------------------------
// VERSION display: Vx.amggn x=versione a=anno da 0=2019 m=mese(hex, da 1 a C) gg=giorno n=sottoversione giornaliera 
#define VERSION_ "190824.13.0 V4.08241" // Corretto DHT 
//efine VERSION_ "190818.17.0 V4.08181" // Tolto serial_EEprint, inserito conto alla rovescia wait iniziale 
//efine VERSION_ "190815.17.0 V4.08151" // Corretto firmware 
//
// -----------------------------------------------------------------
// Riassunto

// Interrupt pin 3 --- Anemometro
// - Set swWindAntiBump

// Interrupt timer

// # Routine 1/1000 di secondo
// - Routine antirimbalzo wind antibump
//   . inc cntWind se swWindAntibump=1
// - Test reed posizione
//  . set/reset impulso di reed

// # Routine 1/10 di secondo
// - Gestione pulsanti di controllo
//  . Gestione pulsante open,  CntSwOpen
//  . Gestione pulsante down CntSwDown
// - Gestione pulsanti
//  . Call SUNBSwitchElab
// - Controllo tenda
//  . Call SUNBCommandCtrl

// # Routine 1 secondo
// - Gestione messaggi di controllo su linea seriale
// - Gestione velocitï¿½ vento
//  . Aggorna WindSpeed
// - Gestione controllo vento
//  . Call WindControl
// - Gestione display

//#include <Adafruit_SleepyDog.h>
//#include <EEPROM.h>

#ifndef STOP_PROG
  #define STOP_PROG 1 
#endif
#define DISPLAYTEMP    1
#define DISPLAYHUMID   2
#define DISPLAYPOS     3
#define DISPLAYWSPEED  4
#define DISPLAYREALTIM 5

//#define TESTMODE 0 // Se = 1, test mode, mettere a 0 se NON test
//#define MonitorMODE 0 // Se = 1, test del vento, mettere a 0 se NON test
//#define PlotterMODE 0 // Se = 1, test del vento, mettere a 0 se NON test

//#if TESTMODE == 1
//#define WINDTIMER    13   // Secondi. Valore timer di ripristino in caso di vento forte
//#define WINDTIMERSTEP 1   // Secondi. Valore timer di ripristino in caso di vento forte
//#define WINDALARM     2   // Velocitï¿½ del vento per sollevare la tenda
//#define RE_TIMEOUT 4000   // milliSeconds
//#define BARDISPTIME   5   // secondi
//#define DISP_INTERVAL 3590
//#else
#define WINDTIMER     90  // Secondi. Valore timer di ripristino in caso di vento forte
#define WINDTIMERSTEP 10  // Secondi. Valore timer di ripristino in caso di vento forte
#define WINDALARM     14  // Velocitï¿½ del vento per sollevare la tenda con P > 4
#define RE_TIMEOUT  4000  // milliSeconds
#define BARDISPTIME 3600  // secondi
#define DISP_INTERVAL 0
//#endif

#define RE_BUMPFILTER_ACT    2
#define RE_BUMPFILTER_NOTACT 5

#define SWITCHTIMEFILTER 2   // *50mS
#define SWITCHTIMELONG   20  // *50mS
#define SWITCHTIMERESET  120 // *50mS

#define PIN_ACTIVE LOW
#define PIN_NOTACT HIGH
#define FLG_ACTIVE true
#define FLG_NOTACT false

#define pMD_UPSMALL 0
#define pMD_DWSMALL 1
#define pMD_UPLARGE 2
#define pMD_DWLARGE 3
#define pMD_DWwind  4
#define pMD_UPwind  5

const byte pinWindSwitch_int =  3; // Anemometro rosso, giallo a massa
const byte pinPosReed        =  4;
const byte pinDHT            =  5; // what digital pin we're connected to
const byte pinOpenIn         =  6;
const byte pinDownIn         =  7;
const byte pinOpenOut        =  8; // bianco
const byte pinDownOut        =  9; // verde
const byte pinLCLoad         = 10; // LedControl load pin
const byte pinLClock         = 11; // LedControl clock pin
const byte pinLCData         = 12; // LedControl data pin

char BarSet;

#include "SerialPROGMEM.h"
#include "LedMatrixLib.h"

#include "SerialGetString.h"
#define inString_MAXLEN 30
CSerialGetString SGS(inString_MAXLEN,false);
char inString[inString_MAXLEN];
char sCommand[inString_MAXLEN];

#include "DHT.h"
#define DHTTYPE DHT22   // DHT 22  (AM2302), AM2321
//efine DHTTYPE DHT11   // DHT 11  
DHT dht(pinDHT, DHTTYPE);

#include <Timer.h>
Timer Tmr;

// Prototipi di funzioni
// -------------------------------------------------------------------------------------------------
//void    printPROGMEMDecRJ(int, int, char, bool); // Stampa numero decimale    giustificato a destra
//void    printPROGMEMHexRJ(int, int, char, bool); // Stampa numero esadecimale giustificato a destra
//void    printPROGMEMBinRJ(int, int, char, bool); // Stampa numero binario     giustificato a destra
// -------------------------------------------------------------------------------------------------
//void    printPROGMEM(char*, bool);        // Stampa una stringa memorizzata in ProgMEM
//void    printPROGMEMYES(bool);            // Stampa YES with or not NL
//void    printPROGMEMNo(bool);             // Stampa NO with or not NL
//void    printPROGMEMSPACE(uint8_t, bool); // Stampa n SPACE with or not NL
// -------------------------------------------------------------------------------------------------
void Int3_MeasureWind(void);
void SUNBSwitchElab(void);
void SUNBCommandCtrl (void);
void PinSwitch(byte);

void ReadTempHigro(void);

void IntTim_ReadSwitch(void);

// uint8_t Version2Code(char[],char[]);


bool enMonitorMode = true;

const char BARTEMP  ='T'; 
const char BARHUMID ='H';
const char BARPOSIT ='P';
const char BARWIND  ='W';

serialPROGMEM SPM; // === Serial messages from PROGMEM


const int arrayDim = 15;
const int arraySize =14;
MatrixDisplay MD(arraySize, arrayDim);

uint8_t  timerWeeks   = 0;
uint8_t  timerDays    = 0;
uint8_t  timerHours   = 0;
uint8_t  timerMinutes = 0;
uint8_t  timerSeconds = 0;
uint16_t timerWindDelta = 0;
uint16_t timerHourTick      = 3600;
bool     tickHour       = true;

uint8_t CntSwOpen   = 0;
uint8_t CntSwDown   = 0;
uint8_t CntWdt      = 0;

uint8_t pinOpenOutHold = 1;
uint8_t pinDownOutHold = 1;

bool SendMsg = false;
bool SendPlot = false;
bool RoutineInLoop1sec = true;

int timWind;       // Timer di ripristino in caso di vento forte
int timWindMult = 0;

uint16_t ResetRequest = 0;
bool ResetRequestOnError = false;

#define CO_STARTUP_00         0 // Startup
#define CO_STARTUP_01         1 // Startup
#define CO_WAIT_02            2 // Idle, nulla attivo
#define CO_SUNBOPENON_03      3 // Apri tenda, comandi OPEN=0 DOWN=1 (0=ON)
#define CO_SUNBDOWNON_04      4 // Chiudi tenda, comandi OPEN=1 DOWN=0 (0=ON)
#define CO_SUNBOPENACT_05     5 // Resetta comando open, comandi OPEN=1 DOWN=1 (0=ON)
#define CO_SUNBDOWNACT_06     6 // Resetta comando down, comandi OPEN=1 DOWN=1 (0=ON)
#define CO_STOPMOTOR_07       7 // Blocca tenda, comandi OPEN=0 DOWN=0 (0=ON)
#define CO_CLEARCOMMAND_08    8 // Resetta, comandi OPEN=1 DOWN=1 (0=ON)
#define CO_OVERHEATRESET_09   9 // Errore!

uint8_t smSUNBCommand = CO_STARTUP_00;

int8_t SUNBstep    = 0;
int8_t SUNBpos, meanSUNBpos;;
int8_t SUNBposHold = 0;

bool flgSUNBStop = false;
bool flgSUNBWait = false;
bool flgSUNBRun = false;

#define TIMEROVERHEAT 10*60*20;           // 10 minuti
uint16_t timeOutOverheat = TIMEROVERHEAT; // 1/20 sec, 20=1 secondo

bool StatusLED = false;

#define WA_NOALARM_00        0
#define WA_ALARM_10         10
#define WA_MOVETOZERO_01     1
#define WA_MOVETO1STSTEP_01
#define WA_MOVETO1STSTEP_02  2
#define WA_WAITENDALARM_03   3
#define WA_WAITENDRESTORE_04 4
#define WA_BUMPFILTER         3 // Valore antibump

uint8_t   smWindAlarm   = WA_NOALARM_00;
bool Stop_smWindAlarm   = false;

bool     flgReedStatus  = false;
bool     flgReedPulse   = false;
bool     flgReedPrintOnPulse = false;

uint16_t timReedOn      = 0;
uint16_t timReedOff     = 0;

uint16_t timReedLastOn  = 0;
uint16_t timReedLastOff = 0;
uint8_t  swReedAntiBumpACT  = RE_BUMPFILTER_ACT;
uint8_t  swReedAntiBumpNACT = RE_BUMPFILTER_NOTACT;

uint16_t cntRoutine1000ms = 1000;

uint8_t swWindAntiBump = WA_BUMPFILTER;

float Temperature, meanTemperature;
float Humidity,    meanHumidity;

uint8_t cntWind = 0;
uint16_t WindSpeed = 0, meanWindSpeed;

uint8_t  InitLoops = 100;

// -------------------------------------------------------------------------------------------------

#include <avr/pgmspace.h>

int8_t  arrayTemp[arrayDim]  ={0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0}; // Controllati puntatori
int8_t  arrayHumid[arrayDim] ={0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0}; // Controllati puntatori
int8_t  arrayWind[arrayDim]  ={0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0}; // Controllati puntatori
int8_t  arrayPosit[arrayDim] ={0,0,0,0,0, 0,0,0,0,0, 0,0,0,0,0}; // Controllati puntatori

const byte pinLED = 13; // LED connected to digital pin 13
#define SUNBMIN       0   // Giri
#define SUNBMAX       16  // Giri
#define SUNBOVER      20  // Giri
#define SUNBSTEP      4   // Giri
#define SUNBWINDPOS   4   // Giri -- Posizione tenda con vento.
#define SUNBMAXHOLD   15  // Giri -- Posizione tenda max se c'ï¿½ un allarme vento
#define SUNBSTEPEXTRA 1   // Giri -- Extra step per errore ricarica

uint32_t MatrixArea[8];

// int arrayStart;
bool SUNBStart;
// int BarToDisplay;

void setup()
{
  //  digitalWrite(pinLED, LOW);

//  int countdownMS = Watchdog.enable(4000);

  timerHourTick=5000;

//  BarToDisplay=0;

  swReedAntiBumpACT = RE_BUMPFILTER_ACT;        // Reset antibump di ACTIVE
  swReedAntiBumpNACT = RE_BUMPFILTER_NOTACT;    // Reset antibump di NOTACT

  flgReedStatus = false;
  flgReedPulse  = false;
  timReedOn  = 0;
  timReedOff = 0;
  timReedLastOn  = 0;
  timReedLastOff = 0;
  flgSUNBRun = false;
  Stop_smWindAlarm = false;     // Reset flag

  ResetRequest = 0;
  ResetRequestOnError = true;

  SUNBpos  = 0;
  SUNBstep = 0;

  // put your setup code here, to run once:
  pinMode(pinWindSwitch_int, INPUT_PULLUP); // 3
  pinMode(pinPosReed,        INPUT);        // 4
  pinMode(pinOpenIn,         INPUT_PULLUP); // 6
  pinMode(pinDownIn,         INPUT_PULLUP); // 7
  pinMode(pinOpenOut,        OUTPUT);       // 8
  pinMode(pinDownOut,        OUTPUT);       // 9
  pinMode(pinLED,            OUTPUT);       // 13 sets the digital pin as output

  // 100mS loop per lettura switch
  Tmr.every(1L, IntTim_ReadSwitch);

  Serial.begin(115200);

  Serial.println("$ ...");

  SPM.printPROGMEM(SETUPmsg1,false); // (string to print (pointer!), Linefeed se TRUE)

  Serial.println(VERSION_);

  SPM.printPROGMEM(SETUPmsg3,true); // SPM.printPROGMEM(string to print (pointer!), Linefeed se TRUE)
  
  char ver[40]=VERSION_;
  MD.MatrixWriteString("Sun",      9,true); PinSwitch(pinLED); delay(1500);
  MD.MatrixWriteString("blind" ,   5,true); PinSwitch(pinLED); delay(1500);
  MD.MatrixWriteString("control",  0,true); PinSwitch(pinLED); delay(1500);
  MD.MatrixWriteString(ver+13,0,true);      PinSwitch(pinLED); delay(4000);

  int showPin;
  digitalWrite(pinDownOut, HIGH); // showPin = digitalRead(pinDownOut);  Serial.print(showPin);
  digitalWrite(pinOpenOut, HIGH); // showPin = digitalRead(pinOpenOut);  Serial.println(showPin);
  PinSwitch(pinLED); delay(1500);
  digitalWrite(pinDownOut, LOW);  // showPin = digitalRead(pinDownOut);  Serial.print(showPin);
  digitalWrite(pinOpenOut, HIGH); // showPin = digitalRead(pinOpenOut);  Serial.println(showPin);
  PinSwitch(pinLED); delay(1500);
  digitalWrite(pinDownOut, HIGH); // showPin = digitalRead(pinDownOut);  Serial.print(showPin);
  digitalWrite(pinOpenOut, HIGH); // showPin = digitalRead(pinOpenOut);  Serial.println(showPin);

  Serial.print("$ Wait");
 
  for(uint8_t cnt=60; cnt>0; cnt--)
  {
    if((cnt%10)==0) 
    {
      Serial.println(" ");
      Serial.print("$ -");
      Serial.print(cnt);
    }
    Serial.print(".");
    
    char str[8];
    PinSwitch(pinLED); delay(1000);

    MD.MatrixWriteString("Wait:", 0,true);
    sprintf(str,"%d", cnt);
    MD.MatrixWriteString(str,23,false);
    if (digitalRead(pinDownIn) == LOW) cnt = 1;
    
  }

  Serial.println(" ");

  Serial.println("$ Ready!");
  delay(1000);
  
  while(digitalRead(pinDownIn) == LOW);

  MD.MatrixWriteString("Run...",1,true); delay(1500);

  attachInterrupt(digitalPinToInterrupt(pinWindSwitch_int), Int3_MeasureWind, FALLING);

  dht.begin();
//  arrayStart=arraySize;
  SUNBStart=true;
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void loop()
{
  static bool DisplayChange=false;
  static byte DisplaySel=1;
  static int  MediaNumber;
  static bool  enMonitorMode_flag=true;

  // put your main code here, to run repeatedly:


// MD.MatrixWriteString("blind",0,true); delay(1000);
// MD.MatrixWriteString("ctrl" ,0,true); delay(1000);

  Tmr.update();
// Get string. 
// Se result <= 0 errore o nessun comando completo

  if (SendMsg && enMonitorMode) 
  {
    enMonitorMode_flag=true;
    Serial.print("$");
//  SPM.printPROGMEM(MONITmsgTemper,false); Serial.print(Temperature);
//  SPM.printPROGMEM(MONITmsgHumidy,false); Serial.print(Humidity);
//  SPM.printPROGMEM(MONITmsgOpenIn,false); Serial.print(digitalRead(pinOpenIn));
//  SPM.printPROGMEM(MONITmsgDownIn,false); Serial.print(digitalRead(pinDownIn));
  SPM.printPROGMEM(MONITmsgOpenOu,false); Serial.print(pinOpenOutHold); pinOpenOutHold = 1;
  SPM.printPROGMEM(MONITmsgDownOu,false); Serial.print(pinDownOutHold); pinDownOutHold = 1;
  SPM.printPROGMEM(MONITmsgCntOpn,false); Serial.print(CntSwOpen);
  SPM.printPROGMEM(MONITmsgCntDow,false); Serial.print(CntSwDown);
//  SPM.printPROGMEM(MONITmsgCntWdt,false); Serial.print(CntWdt);
    SPM.printPROGMEM(MONITmsgTenSte,false); Serial.print(SUNBstep);
    SPM.printPROGMEM(MONITmsgTenPos,false); Serial.print(SUNBpos);
    SPM.printPROGMEM(MONITmsgRstReq,false); Serial.print(ResetRequest);
//  SPM.printPROGMEM(MONITmsgTenPoH,false); Serial.print(SUNBposHold/10);
//  SPM.printPROGMEM(MONITmsgsmSUNB,false); Serial.print(smSUNBCommand);
//  SPM.printPROGMEM(MONITmsgfPulse,false); Serial.print(flgReedPulse);
//  SPM.printPROGMEM(MONITmsgtimROf,false); Serial.print(timReedOff);
//  SPM.printPROGMEM(MONITmsgtimROn,false); Serial.print(timReedOn);
//  SPM.printPROGMEM(MONITmsgswReed,false); Serial.print(flgReedStatus);
//  SPM.printPROGMEM(MONITmsgtimRof,false); Serial.print(timReedLastOff);
//  SPM.printPROGMEM(MONITmsgtimRon,false); Serial.print(timReedLastOn);
//  SPM.printPROGMEM(MONITmsgtimRee,false); Serial.print(timReedOn+timReedOff);
    SPM.printPROGMEM(MONITmsgWndSpd,false); Serial.print(WindSpeed);
//  SPM.printPROGMEM(MONITmsgsmWind,false); Serial.print(smWindAlarm);
//  SPM.printPROGMEM(MONITmsgtimWnd,false); Serial.print(timWind);
    SPM.printPROGMEM(MONITmsgtmHour,false); Serial.print(timerHours);
    SPM.printPROGMEM(MONITmsgtmMinu,false); Serial.print(timerMinutes);
    SPM.printPROGMEM(MONITmsgtmSeco,false); Serial.print(timerSeconds);
//  SPM.printPROGMEM(MONITmsgstaLED,false); Serial.print(StatusLED);
//  SPM.printPROGMEM(MONITmsginChar,false); Serial.print(inString[0]);
//  SPM.printPROGMEM(MONITmsgmTempr,false); Serial.print(meanTemperature);
//  SPM.printPROGMEM(MONITmsgmHumid,false); Serial.print(meanHumidity);
//  SPM.printPROGMEM(MONITmsgmPosit,false); Serial.print(meanSUNBpos);
//  SPM.printPROGMEM(MONITmsgmWindy,false); Serial.print(meanWindSpeed);
    Serial.println("");
    SendMsg = false;
  }
  else
  {
    if(enMonitorMode_flag && !enMonitorMode) 
    {
      Serial.println("> ");
      enMonitorMode_flag=false;
    }
  }
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++ Display messages ++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  if (RoutineInLoop1sec) // && (arrayStart==0) && (SUNBStart==false))
  {
    PinSwitch(pinLED);
    int8_t result=SGS.GetString(inString);
    if(result>0) 
    {
      strcpy(sCommand,inString);
      Serial.print("> "); Serial.print(inString);
      inString[0]='\0';  
    }
    if (sCommand[0]=='m')
    {
      if (enMonitorMode) enMonitorMode = false;
      else               enMonitorMode = true;
      sCommand[0]='\0';
      SPM.printPROGMEM(LOOPmsgCMD_m,false);
      Serial.println(enMonitorMode);
    } 
//  else if (strcmp(sCommand,"VERSION")==0)
//  {
//    char version[11];
//  Serial.print("$ "); Serial.print(VERSION);
//  } else
//  if (strncmp(sCommand,"VERCODE",7)==0)
//  {
//  char version[11];
//    string="VERCODE aammgg.hh.n"
//            00000000001111111111
//            01234567890123456789
//  uint8_t sCommand_len=strlen(sCommand);
//  if(sCommand_len>=19)
//  {
//    char sCommand_operand[sCommand_len-7];
//    uint8_t indexout=0;
//    for(int index=7;index<sCommand_len;index++)
//    {
//      if(isdigit(sCommand[index])
//        sCommand_operand[indexout++]=sCommand[index];
//    } 
//    strcpy(sCommand+9,version); version[11]="\0";
//    uint8_t result=Version2Code(version,versioncode);
//    if(result)
//    {
//      SPM.printPROGMEM(VERmsg01,false);
//      Serial.print(versioncode);
//    }
//    else
//    {
//      SPM.printPROGMEM(VERmsgERROR,true);
//    }
//  }
//  } else
    if(timeOutOverheat == 0) MD.MatrixWriteString("Errore!", 0,true); // Motor error
    else
    {
      if(DisplayChange)
      {
        switch (DisplaySel)                                         // Display switcher
        {
          int intTemp;
          case (DISPLAYTEMP): // ---- Temperature -----------        // Display temperatura
          char str[10];
          MD.MatrixWriteString("t", 0,true);                           // Stampa 't'

          intTemp=(int)(Temperature*10);                            //
          if((intTemp%10)>5) intTemp=intTemp/10+1;                  // Temperatura in formato integer
          else               intTemp=intTemp/10;

          sprintf(str,"%d",abs(intTemp));                           // Stampa valore temperatura
          if (Temperature<0)  { MD.MatrixWriteString("-",4,false);     // Stampa '-' se negativa
                                MD.MatrixWriteString(str,7,false);}    // Stampa valore di temp negativa
          else                  MD.MatrixWriteString(str,4,false);     // Stampa valore di temp positiva
    //DBG Serial.print("T "); // ----------------- DEBUG
          MD.MatrixWriteBar(arrayTemp, 16, BARTEMP);                   // Stampa barra temperatura
          break;

          case (DISPLAYHUMID): // ---- Humidity --------------
          MD.MatrixWriteString("h",0,true);
          sprintf(str,"%d",(int)Humidity);
          MD.MatrixWriteString(str,5,false);
    //DBG Serial.print("H "); // ----------------- DEBUG
          MD.MatrixWriteBar(arrayHumid,16, BARHUMID);
          break;

          case (DISPLAYPOS): // ---- Sun Blind position
          MD.MatrixWriteString("p", 0,true);
          sprintf(str,"%d",SUNBpos);
          MD.MatrixWriteString(str,5,false);
    //DBG Serial.print("P "); // ----------------- DEBUG
          if      (SUNBstep>12)
              MD.MatrixWriteString("<<<<<",15,false);
          else if (SUNBstep>6 )
              MD.MatrixWriteString("<<<<", 15,false);
          else if (SUNBstep>4 )
              MD.MatrixWriteString("<<<",  15,false);
           else if (SUNBstep>0)
              MD.MatrixWriteString("<<",   15,false);
          else if (SUNBstep<-12)
              MD.MatrixWriteString(">>>>>",15,false);
          else if (SUNBstep<-6)
              MD.MatrixWriteString(">>>>", 15,false);
          else if (SUNBstep<-4)
              MD.MatrixWriteString(">>>",  15,false);
          else if (SUNBstep<0)
              MD.MatrixWriteString(">>",   15,false);
          else
            MD.MatrixWriteBar(arrayPosit,16, BARPOSIT);
          break;

          case (DISPLAYWSPEED): // ---- Wind Speed
          MD.MatrixWriteString("w",0,true);
          sprintf(str,"%d",WindSpeed);
          MD.MatrixWriteString(str,6,false);
    //DBG Serial.print("W "); // ----------------- DEBUG

          switch(smWindAlarm)
          {
            case WA_NOALARM_00:
              MD.MatrixWriteBar(arrayWind,16, BARWIND);
              break;
            case WA_ALARM_10:
            case WA_MOVETOZERO_01:
            case WA_MOVETO1STSTEP_02:
              MD.MatrixWriteString(">>>",16,false);
              break;
            case WA_WAITENDALARM_03:
              sprintf(str,"-%d",timWind);
              MD.MatrixWriteString(str,16,false);
              break;
            case WA_WAITENDRESTORE_04:
              MD.MatrixWriteString("<<<",16,false);
              break;
            default:
              break;
          }
          break;
          default:
//        case (DISPLAYREALTIM):
            MD.MatrixWriteString("Rt",0,true);
            if(timerWeeks>0)
            {
              sprintf(str,"%d", timerWeeks);
              if(timerWeeks=999)  strcat(str,"w+"); 
              else                strcat(str,"w");              
            } else
            if(timerDays>0)
            {
              sprintf(str,"%d", timerDays);
              strcat(str,"d"); 
            } else
            if(timerHours>0)
            {
              sprintf(str,"%d", timerHours);
              strcat(str,"h");              
            } else
            if(timerMinutes>0)
            {
              sprintf(str,"%d", timerMinutes);
              strcat(str,"m");
            }
            else
            {
              sprintf(str,"%d", timerSeconds);
              strcat(str,"s");
            }
//          if(strlen(str)>4) str[4]='\0';
            MD.MatrixWriteString(str,10,false);
//          break;
//        default:
            DisplaySel=0;
            break;
        }

        MD.RefreshBarDisplay();

        if (smWindAlarm!=WA_NOALARM_00) DisplaySel=DISPLAYWSPEED;
        else if (SUNBstep!=0)           DisplaySel=DISPLAYPOS;
        else
        {
          ++DisplaySel;
          DisplayChange=false;
        }
      }
      else
        DisplayChange=true;

  // Qui nuove routines
      if ((flgSUNBRun==false) && (smWindAlarm==WA_NOALARM_00)) ReadTempHigro();
      if (timerWindDelta < 36000) timerWindDelta++;

      if (sCommand[0] =='H') {timerHourTick=3595; sCommand[0]='\0';}

      if (++timerSeconds>=60)
      {
        timerSeconds = 0;
        if(++timerMinutes>=60)
        {
          timerMinutes = 0;
          if (++timerHours>=24)
          {
            timerHours = 0;
            if(++timerDays>=7)
            {
              timerDays=0;
              if(++timerWeeks>999)
              {
                timerWeeks=999;
              }
            }
          }
        }
      };

      if (tickHour)
      {
        MediaNumber = 1;

        arrayHumid[0]= (int)meanHumidity;
        arrayTemp[0] = (int)(meanTemperature);
        arrayPosit[0]= meanSUNBpos;
        arrayWind[0] = meanWindSpeed;

        for(uint8_t p=14;p>0;p--)
        {
          arrayHumid[p]=arrayHumid[p-1];
          arrayPosit[p]=arrayPosit[p-1];
          arrayTemp[p] =arrayTemp[p-1];
          arrayWind[p] =arrayWind[p-1];

          meanHumidity    = Humidity;
          meanSUNBpos     = 0;
          meanTemperature = Temperature;
          meanWindSpeed   = 0;
        }
        tickHour = false;
      }
      else
      {
        if(MediaNumber==1)
        {
          meanHumidity=(Humidity+meanHumidity)/2;
          meanTemperature=(Temperature+meanTemperature)/2;
        }
        else
        {
          meanHumidity   =((meanHumidity   *MediaNumber)+Humidity)   /(MediaNumber+1);
          meanTemperature=((meanTemperature*MediaNumber)+Temperature)/(MediaNumber+1);
        }
        MediaNumber++;
        if (meanSUNBpos<SUNBpos)     meanSUNBpos = SUNBpos;
        if (meanWindSpeed<WindSpeed) meanWindSpeed = WindSpeed;
      }

      arrayHumid[0]=(int)Humidity;
      arrayTemp[0] =(int)Temperature;
      arrayPosit[0]=SUNBpos;
      arrayWind[0] =meanWindSpeed;

      RoutineInLoop1sec=false;
    }
  }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void Int3_MeasureWind()
{
  swWindAntiBump = WA_BUMPFILTER; // Interrupt anemometro attiva il test dei pin con antibup
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
void IntTim_ReadSwitch() // Routine di servizio timer
{
  static bool flgReadTemp;
  static uint8_t cntRoutine50ms;
// -------------------------------------------------------------------------------------------------
// ## Routine 1/1000 di secondo --------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// -- Incremento contatore vento con antibump ------------------------------------------------------
  if (swWindAntiBump  > 1)                                         // Switch antibump non scaduto
  {
    if (digitalRead(pinWindSwitch_int) == LOW) swWindAntiBump--;   // Se switch wind attivo decrementa contatore antibump
    else                                       swWindAntiBump = 0; // Disattiva contatore
  }
  if (swWindAntiBump == 1)                                         // Switch antibump scaduto
  {
    if (digitalRead(pinWindSwitch_int) == LOW)
      {if (cntWind<0xFF) cntWind++;};                              // Se switch wind attivo Incrementa contatore vento...
    swWindAntiBump = 0;                                            // ...e disattiva antibump
  }
// -- Contatore giri (reed) ------------------------------------------------------------------------
  if (digitalRead(pinPosReed) == PIN_NOTACT)      // Reed non attivo
  {
    swReedAntiBumpACT = RE_BUMPFILTER_ACT;        // Reset antibump di ACTIVE
    if (swReedAntiBumpNACT > 0)                   // Contatore antibump NOTACT non scaduto
    {
      swReedAntiBumpNACT--;                       // Decrementa contatore NOTACT antibump
    }
    else
    {
      if (flgReedStatus)                          // Se Reed era attivo (TRUE)
      {
        flgReedPulse         = true;              // Impulso di giro
        flgReedPrintOnPulse  = true;              // Impulso di giro
        timReedLastOn = timReedOn;                // Salva tempo HIGH
      }
      flgReedStatus = false;                      // Set Reed non attivo
      timReedOn = 0;                              // Contatore HIGH azzerato
      if (flgSUNBRun) timReedOff++;                 // Se motore attivo incrementa contatore LOW
      else            timReedOff = 0;               // Altrimenti azzera
    }
  }
  else if (digitalRead(pinPosReed) == PIN_ACTIVE) // Reed attivo
  {
    swReedAntiBumpNACT = RE_BUMPFILTER_NOTACT;    // Reset antibump di NOTACT
    if (swReedAntiBumpACT > 0)                    // Contatore antibump ACTIVE non scaduto
    {
      swReedAntiBumpACT--;                        // Decrementa contatore ACTIVE antibump
    }
    else
    {
      if (!flgReedStatus)                         // Se Reed non era era attivo (FALSE)
      {
        flgReedPulse = false;                     // Cancella flag pulse (per sicurezza)
        timReedLastOff = timReedOff;              // Salva tempo LOW
      }
      flgReedStatus = true;                       // Set Reed HIGH
      timReedOff = 0;                             // Contatore LOW azzerato
      if (flgSUNBRun) timReedOn++;                // Se motore attivo incrementa contatore HIGH
      else            timReedOn = 0;              // Altrimenti azzera
    }
  }
// -------------------------------------------------------------------------------------------------
// ## Routine 1/20 di secondo --------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
  if ((--cntRoutine50ms) == 0)
  {
    cntRoutine50ms = 50;

  // -- Incremento contatori switch --------------------------------------------------------------
    if (digitalRead(pinOpenIn) == HIGH)
    {
      if(sCommand[0]!='\0')
      {
        if      (sCommand[0]=='u' || sCommand[0]=='s') { CntSwOpen = SWITCHTIMEFILTER+1; sCommand[0]='\0'; Serial.println("CntSwOpen++");}
        else if (sCommand[0]=='U')                     { CntSwOpen = SWITCHTIMELONG+1;   sCommand[0]='\0'; Serial.println("CntSwOpen+");}
      }
      else
      {
        CntSwOpen = 0; CntWdt = 0;
      }
    }
    else if (CntSwOpen < 0xFF) { CntSwOpen++;}

    if (digitalRead(pinDownIn) == HIGH)
    {
      if(sCommand[0]!='\0')
      {
        if      (sCommand[0]=='d' || sCommand[0]=='s') { CntSwDown = SWITCHTIMEFILTER+1; sCommand[0]='\0'; Serial.println("CntSwOpen++");}
        else if (sCommand[0]=='D')                     { CntSwDown = SWITCHTIMELONG+1;   sCommand[0]='\0'; Serial.println("CntSwOpen+");}
      }
      else
      {
      CntSwDown = 0; CntWdt = 0;
      }
    }
    else if (CntSwDown < 0xFF) { CntSwDown++; CntWdt++;}

    if ((digitalRead(pinOpenIn) == LOW) &&
      (digitalRead(pinDownIn) == LOW) ){ if (ResetRequest < 0xFFFF) ResetRequest++;} else ResetRequest = 0;

    // ------------------------------------------------------------------------------------------------
    // -- Movimento tenda -----------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------
    // -- SUNBCommandCtrl -----------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------
    switch (smSUNBCommand)                    // State machine SUNB Command
    {
    case CO_STARTUP_00:                      // Startup
      digitalWrite(pinDownOut, HIGH);        // Azzera comando Close
      digitalWrite(pinOpenOut, HIGH);        // Azzera comando Open
      smSUNBCommand = CO_WAIT_02;            // Next
      break;
    // ---------------------------------------------------------------------------------------------
    case CO_STARTUP_01:                      // Spare state
      break;
    // ---------------------------------------------------------------------------------------------
    case CO_WAIT_02:                                      // Idle, nulla attivo
      if (SUNBstep > 0) smSUNBCommand = CO_SUNBOPENON_03; // Se SUNB step > 0 OPEN
      if (SUNBstep < 0) smSUNBCommand = CO_SUNBDOWNON_04; // Se SUNB step < 0 CLOSE
      flgSUNBRun = false;                                 // Motore non attivo
      timeOutOverheat = TIMEROVERHEAT; // 10 minuti
      break;
    // ---------------------------------------------------------------------------------------------
    case CO_SUNBOPENON_03:                   // Apri tenda, comandi OPEN=0 DOWN=1 (0=ON)
      digitalWrite(pinDownOut, HIGH);        // Disable close
      digitalWrite(pinOpenOut, LOW);         // Enable open
      flgSUNBRun = false;                    // Motore non attivo
      smSUNBCommand = CO_SUNBOPENACT_05;     // Next
      break;
    // ---------------------------------------------------------------------------------------------
    case CO_SUNBDOWNON_04:                   // Chiudi tenda, comandi OPEN=1 DOWN=0 (0=ON)
      digitalWrite(pinDownOut, LOW);         // Enable close
      digitalWrite(pinOpenOut, HIGH);        // Disable open
      flgSUNBRun = false;                    // Motore non attivo
      smSUNBCommand = CO_SUNBDOWNACT_06;  // Next
      break;
    // ---------------------------------------------------------------------------------------------
    case CO_SUNBOPENACT_05:               // Resetta comando open, comandi OPEN=1 DOWN=1 (0=ON)
      digitalWrite(pinDownOut, HIGH);        // Disable close
      digitalWrite(pinOpenOut, HIGH);        // Disable open
      flgSUNBRun = true;                     // Motore attivo
      if (flgReedPulse)                      // IMpulso da reed posizione
      {
        SUNBpos++;                           // Incrementa contatore posizione
        if (SUNBpos >= SUNBMAX)              // Se limite superiore...
        {
          SUNBstep = 0;                      // ...azzera contatore step,
          SUNBpos = SUNBMAX;
        }
        else                                 // altrimenti...
          SUNBstep--;                        // ...decrementa contatore step
  
        if (SUNBstep<=0)                     // Controllo se contatore step = 0 (o minore)
        {
          smSUNBCommand = CO_STOPMOTOR_07;   // Ferma motori
          SUNBstep = 0;                      // Azzera contatore step
        }
        flgReedPulse = false;                // Azzera flag reed posizione
      }
  
      if ((timReedOn+timReedOff)>RE_TIMEOUT)
      {
      --timeOutOverheat;
      if (timeOutOverheat<=0)
        smSUNBCommand = CO_OVERHEATRESET_09;   // Ferma motori
      else if (timeOutOverheat%40==0)
        {
          smSUNBCommand = CO_SUNBOPENON_03;  // Torna indietro
        }
      }
      else
      timeOutOverheat = TIMEROVERHEAT;          // 10 minuti

      break;
  // ---------------------
    case CO_SUNBDOWNACT_06:                  // Resetta comando down, comandi OPEN=1 DOWN=1 (0=ON)
      digitalWrite(pinDownOut, HIGH);        // Disable close
      digitalWrite(pinOpenOut, HIGH);        // Disable open
      flgSUNBRun = true;                     // Motore attivo
      if (flgReedPulse)                      // IMpulso da reed posizione
      {
        SUNBpos--;                           // Decrementa contatore posizione
        if (SUNBpos <= SUNBMIN)              // Se limite inferiore...
        {
          SUNBstep = 0;                      // ...azzera contatore step,
          SUNBpos = SUNBMIN;
        }
        else                                 // altrimenti...
          SUNBstep++;                        // ...incrementa contatore step.
        flgReedPulse = false;                // Azzera flag reed posizione
        if (SUNBstep >= 0)                   // Controllo se contatore step = 0 (o maggiore)
        {
          smSUNBCommand = CO_STOPMOTOR_07;   // Ferma motori
          SUNBstep = 0;                      // Azzera contatore step
        }
      }
                         // Timeout, tenda in fondo
      if ((timReedOn+timReedOff)>RE_TIMEOUT)
      {
        --timeOutOverheat;
        if (timeOutOverheat<=0)
          smSUNBCommand = CO_OVERHEATRESET_09;   // Ferma motori
        else if (timeOutOverheat%40==0)
        {
          smSUNBCommand = CO_SUNBDOWNON_04;  // Torna indietro
        }
      }
      else
      timeOutOverheat = TIMEROVERHEAT;     // 10 minuti
      break;
    // ---------------------
    case CO_STOPMOTOR_07: //  Blocca tenda, comandi OPEN=0 DOWN=0 (0=ON)
      digitalWrite(pinDownOut, LOW);
      digitalWrite(pinOpenOut, LOW);
      flgSUNBRun = false;
      smSUNBCommand = CO_CLEARCOMMAND_08;
      break;
    // ---------------------
    case CO_CLEARCOMMAND_08: // Resetta, comandi OPEN=1 DOWN=1 (0=ON)
      digitalWrite(pinDownOut, HIGH);
      digitalWrite(pinOpenOut, HIGH);
      flgSUNBRun = false;
      smSUNBCommand = CO_WAIT_02;
      break;
    // ---------------------
    case CO_OVERHEATRESET_09: //  Blocca tenda, comandi OPEN=0 DOWN=0 (0=ON)
      digitalWrite(pinDownOut, LOW);
      digitalWrite(pinOpenOut, LOW);
      flgSUNBRun = false;
      ResetRequestOnError = true;
      break;
    }
    if (digitalRead(pinOpenOut) == LOW) pinOpenOutHold = 0;
    if (digitalRead(pinDownOut) == LOW) pinDownOutHold = 0;
    // ------------------------------------------------------------------------------------------------
    // -- SUNBSwitchElab -----------------------------------------------------------------------------
    // ------------------------------------------------------------------------------------------------
    if (SUNBpos>=17) SUNBpos = 17;
    if (SUNBpos<0)   SUNBpos = 0;

    if (CntSwOpen > SWITCHTIMEFILTER &&              // Stop SUNB, entrambii pulsanti attivi
      CntSwDown > SWITCHTIMEFILTER)
    {
      if (smWindAlarm == WA_NOALARM_00)              // Se non attivo allarme vento
      {
        CntSwOpen = 0; CntSwDown = 0;                // Azzera contatori
        SUNBstep =  0;                               // Stop tenda UP
      }
  //  ResetRequest++;
    }
    else if (CntSwOpen > SWITCHTIMEFILTER)           // Se open tenda
    {
      if (smWindAlarm == WA_NOALARM_00)              // Se non attivo allarme vento
      {
  //    Serial.print("+ SUNBstep up find:"); Serial.println(SUNBstep);
        if (CntSwOpen > SWITCHTIMELONG)              // Se tempo lungo
        {
          if(SUNBpos<16) SUNBstep = SUNBOVER;          // LONG up
        }
        else if (SUNBstep==0)                        // Solo se non attivo SUNBstep
        {
          if(SUNBpos<16) SUNBstep = 4-(SUNBpos%4);     // STEP up
           Serial.print("+ SUNBstep up set:"); Serial.println(SUNBstep); // -> Commenta 
        }
      }
      else
      {
        CntSwOpen = 0;  // Azzera contatore con vento
      }
    }
    else if (CntSwDown > SWITCHTIMEFILTER)           // Se down tenda
    {
      if (smWindAlarm == WA_NOALARM_00)              // Se non attivo allarme vento
      {
//    Serial.print("+ SUNBstep dw find:"); Serial.println(SUNBstep);
        if (CntSwDown > SWITCHTIMELONG)              // Se tempo lungo
        {
          if(SUNBpos>0) SUNBstep=-SUNBOVER;          // LONG dw
        }
        else if (SUNBstep==0)                        // Solo se non attivo SUNBstep
        {
          if     (SUNBpos<=0) SUNBstep = 0;          // STEP dw
          else if(SUNBpos>=13)SUNBstep = 12-SUNBpos;
          else                SUNBstep = 0-(((SUNBpos-1)%4)+1);
          Serial.print("+ SUNBstep dw set:"); Serial.println(SUNBstep); // -> Commenta
        }
      }
      else
      {
        Stop_smWindAlarm = true;                     // Ferma allarme vento, con vento
      }
    }
  // ------------------------------------------------------------------------------------------------
  }

  if ((--cntRoutine1000ms) <= 0)
  {
    cntRoutine1000ms = 1000;

//  valid inString[0] m|d|D|s|u|U|...|w|m|p

//  if((ResetRequest<SWITCHTIMERESET) && ResetRequestOnError == false) PinSwitch(pinLED);

    // -- Display enabler -----------------------------------------------------------------------------

    // -- Message enabler -----------------------------------------------------------------------------

//    if (smWindAlarm != 0)   SendMsg = true;  // ### Flag PULSE
//    if (smSUNBCommand != 2) SendMsg = true;  // ### Flag PULSE
//    if (flgReedPrintOnPulse == true)  SendMsg = true;  // ### Flag PULSE
//    if (flgReedPrintOnPulse == true)  SendPlot = true;  // ### Flag PULSE
//    if (SUNBstep != 0)      SendMsg = true;  // ### Flag PULSE
//    if (SUNBpos   < 0)      SendMsg = true;  // ### Flag PULSE
//    if (WindSpeed > 0)      SendMsg = true;  // ### Flag PULSE
                              SendMsg = true;  // ### Flag PULSE
                              SendPlot = true;  // ### Flag PULSE
      flgReedPrintOnPulse = false;

    // -- Aggornamento wind speed ---------------------------------------------------------------------

    if(sCommand[0]=='w')
    {
      WindSpeed = 20; cntWind = 0; sCommand[0]='\0';
    }
    else
    {
      WindSpeed = cntWind; cntWind = 0;
    }

    sCommand[0]='\0';

// ----------------------------------------------------------------------------
// -- WindControl -------------------------------------------------------------
// ----------------------------------------------------------------------------
    if (SUNBpos>=17) SUNBpos = 17;
    if (SUNBpos<0)   SUNBpos = 0;

    switch (smWindAlarm)
    {
      case WA_NOALARM_00:                           // State 00
        if ((WindSpeed > WINDALARM) &&              // Se wind speed maggiore di max safe...
            (SUNBpos   > SUNBWINDPOS))                // ...e posizione maggiore di max safe
            if      (SUNBstep == 0)                 // Se tenda non in movimento
              smWindAlarm = WA_ALARM_10;            // Attiva routine chiusura per vento
            else if (SUNBstep > 0)                  // Se la tenda si sta aprendo
              SUNBstep = 0;                           // Bloccala
        break;                                      // Se si sta chiudendo non fare nulla
      // ---------------------$$$
      case WA_ALARM_10:                             // State 10
        if(SUNBpos<=SUNBWINDPOS) SUNBstep = 0;                  // Setup movimento tenda con vento forte
        else                     SUNBstep = SUNBWINDPOS-SUNBpos;          // WINDALARM down

        if (SUNBpos < SUNBMAXHOLD)
          SUNBposHold = SUNBpos;
        else
          SUNBposHold = SUNBMAXHOLD;

        smWindAlarm = WA_MOVETOZERO_01;
        break;
      // ---------------------
      case WA_MOVETOZERO_01:                        // State 1
        if (SUNBpos == SUNBWINDPOS)                 // Attendi che tenda si metta in posizione safe
        {
          smWindAlarm = WA_MOVETO1STSTEP_02;
        }
        break;
      // ---------------------
      case WA_MOVETO1STSTEP_02:                     // State 2
        timerWindDelta = 0;                         // Set timer
        timWind = WINDTIMER;
        smWindAlarm = WA_WAITENDALARM_03;
        break;
      // ---------------------
      case WA_WAITENDALARM_03:                      // State 3
        if (WindSpeed > WINDALARM)                  // Se allarme vento azzera il timer
        {
          timWind = WINDTIMER;
        }
        else if (Stop_smWindAlarm)                  // Se premuto pulsante DOWN
        {
          Stop_smWindAlarm = false;                 // Cancella flag
          smWindAlarm = WA_NOALARM_00;              // Esci da smWindAlarm
        }
        else if (--timWind == 0)                    // Decrementa il timer
        {
          smWindAlarm = WA_WAITENDRESTORE_04;       // Ritorno in posizione
          SUNBstep = SUNBposHold - SUNBpos;               // WINDALARM up
        }
        break;
      // ---------------------
      case WA_WAITENDRESTORE_04:                    // State 4
        smWindAlarm = WA_NOALARM_00;                // Ritorna dall'inizio
        break;
      // ---------------------
      default:
        smWindAlarm = WA_NOALARM_00;
        break;
    }
// ----
// ----------------------------------------------------------------------------
    RoutineInLoop1sec = true;

    if (SUNBpos<=0) SUNBStart=false;

// --- Spostatate routine in loop ----------------------------------------------
    if (timerHourTick<3600)
    {
      timerHourTick++;
    }
    else
    {
      timerHourTick = DISP_INTERVAL;
      tickHour = true;
    }
  }
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ## Estrazione valori temperatura e umiditï¿½

void ReadTempHigro(void)
{
  // Reading temperature or humidity takes about 250 milliseconds!
  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  Humidity = dht.readHumidity();
  if (isnan(Humidity)) Humidity=40.0;
  // Read temperature as Celsius (the default)
  Temperature = dht.readTemperature();
  // Read temperature as Celsius (the default)
  if (isnan(Temperature)) Temperature=20.0;
   return;
}

// ## Elaborazione dello stato degli switch per attivare comandi tenda ----------
void PinSwitch(byte pin)
{
  if (StatusLED)
  {
    digitalWrite(pin, HIGH);  StatusLED = false;
  }
  else
  {
    digitalWrite(pin, LOW); StatusLED = true;
  }
}
// ## Crea codice versione ------------------------------------------------------
//bool Version2Code(version[],code[])
//{
//  uint8_t ptr=0;
//  uint8_t len=strlen(version);
//  if(len!=9) return(false);
//  char str[3]; str[2]='\0';
//  uint8_t num;
//  unsigned long ver;
//  do
//  {
//    str[0]=version[ptr++];
//    str[1]=version[ptr++];
//    sscanf(str;"%d";&num);
//    switch(ptr)
//    {
//      case 2: if(num>99) return(false); break; // anno
//      case 4: if(num>12) return(false); break; // mese
//      case 6: if(num>31) return(false); break; // giorno
//      case 8: if(num>23) return(false); break; // ora
//    }
//  }
//  while(++ptr<8);
//} 
//// ## End of program -------------------------------------------------------------------------------
