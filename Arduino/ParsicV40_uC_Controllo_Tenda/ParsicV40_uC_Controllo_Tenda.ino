//==============================================================================
#define VERSION_ "V1.190815.15b" // Release, bug primo comando WIND
//efine VERSION_ "V1.190814.23a" // Sviluppo, wind speed corretto
//efine VERSION_ "V1.190813.16b" // Sviluppo, modifiche messaggi
//efine VERSION_ "V1.190813.16a" // Sviluppo, ulteriori test
//efine VERSION_ "V1.190811.10a" // Sviluppo
//efine VERSION_ "V1.190707.19a" // Sviluppo
//efine VERSION_ "V1.190622.12a" // Sviluppo
//efine VERSION_ "V1.190614.19b" // Sviluppo
//efine VERSION_ "V1.190314.19b" // Versione iniziale
//
// === Messages ================================================================
const char VERSIONmsg[]   PROGMEM  = {"$ -- VoltageController -- Version: "};
//
// === Timer interrupt =========================================================
#include <Timer.h>
Timer Tmr;
//
const uint8_t InterruptTime = 5;
// -----------------------------------------------------------------------------
//
uint8_t PosTest;
uint8_t PositionCommand;
bool    SendMsg;
int16_t DebugReg[13];
uint16_t DispTim=1000;
bool cmdWINDflag=false;
//
// === DEBUG ===================================================================
#include <Debug.h>
//
// === Serial messages from PROGMEM ============================================
#include "ParsicV40_uC_Controllo_TendaMessaggi.h"
serialPROGMEM SPM;
//
// === Definizione funzioni pin ================================================
struct pinDefinition
{
  byte D00;               // D00
  byte D01;               // D01
  byte TestSignal;        // D02
  byte WindSwitch_int;    // D03
  byte PosReed;           // D04
  byte DHTio;             // D05
  byte OpenIn;            // D06
  byte DownIn;            // D07
  byte OpenOut;           // D08
  byte DownOut;           // D09
  byte D10;               // D10
  byte D11;               // D11
  byte D12;               // D12
  byte Debug;             // D13
};

/*******************************************************************************
Pin ControlloTenda:
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
*******************************************************************************/

const pinDefinition pin={0,1,2,3,4,5,6,7,8,9,10,11,12,13};
// === Timer ===================================================================
class UserTimer
{
private:
  long      mMilliSec;
  long      mMilliSec_set;
  uint16_t  mDecInterval;        
public:
//--- COSTRUTTORE di UserTimer ----------------------------------------------
  UserTimer(uint16_t DecInterval)
  {
      if(DecInterval<1000) mDecInterval=DecInterval;
      else                 mDecInterval=1;
      mMilliSec=0L;  
  }
// --- Decrement ------------------------------------------------------------
  bool Decrement(void) // SE true il timer è scaduto
  {
    if(mMilliSec<1L)
    {
      mMilliSec=mMilliSec_set;
      return(true); 
    }
    else
    {
      mMilliSec-=(long)mDecInterval;
      return(false);
    }
  }
  long Set(long TimerValue_mS)
  {
    if(TimerValue_mS<100000) mMilliSec_set=TimerValue_mS; 
    else                     mMilliSec_set=100000;
    return (mMilliSec_set);
  }
  long ReadRegister(uint8_t index)
  {
    switch(index)
    {
      case 1: return(mMilliSec); break;
      case 2: return(mMilliSec_set); break;
      case 3: return((long)mDecInterval); break;
      default: return(-1);
    }
  }
};
UserTimer TIMMSG(5);
UserTimer TIMcmdWIND(5);
// === SunBlind simulator ======================================================
#include "SunBlindSimulator.h"

byte pinref[6]={pin.PosReed, pin.OpenOut, pin.OpenIn, pin.DownOut, pin.DownIn, pin.WindSwitch_int};

SunBlindSimulator SBS(pinref, InterruptTime);

// === Prototipi ===============================================================
char    GetChar(void);       // Prendi comando
void    SerialPrintLabel(void); // Stampa etichette
// === SETUP ===================================================================
void setup() 
{
  PosTest=0;

  PositionCommand=0;
  
  pinMode(pin.D00,            INPUT);
  pinMode(pin.D01,            INPUT);
  pinMode(pin.TestSignal,     OUTPUT);
  pinMode(pin.WindSwitch_int, OUTPUT); // 3 Controllo software emulazione
  pinMode(pin.PosReed,        OUTPUT); // 4 Controllo software emulazione
  pinMode(pin.DHTio,          INPUT);  // 5 Dispositivo hardware
  pinMode(pin.OpenIn,         INPUT);  // 6 Controllo pulsante PARSECV40 hardware
  pinMode(pin.DownIn,         INPUT);  // 7 Controllo pulsante PARSECV40 hardware
  pinMode(pin.OpenOut,        INPUT);  // 8 Controllo da microcontroller
  pinMode(pin.DownOut,        INPUT);  // 9 Controllo da microcontroller
  pinMode(pin.D10,            INPUT);  // 10 display load
  pinMode(pin.D11,            INPUT);  // 11 display clock
  pinMode(pin.D12,            INPUT);  // 12 display data
  pinMode(pin.Debug,          OUTPUT);  // 13 LED (watchdog) 

  digitalWrite(pin.WindSwitch_int, LOW); // 3
  digitalWrite(pin.PosReed,        LOW); // 4
  
  // loop
  Tmr.every((long)InterruptTime, IntTim_ReadSwitch);

  TIMMSG.Set(DispTim);
  TIMcmdWIND.Set(1500);
  
  Serial.begin(115200);

  SPM.printPROGMEM(STARTmsg01a,false); Serial.println(VERSION_);
}

// === LOOP ====================================================================
void loop() 
{
  const char strCMD_BUFFER_SIZE=32;
  char strCMD[strCMD_BUFFER_SIZE];
  char inchr;
  uint8_t pointer = 0;
//Serial.print("> ");
  while (1)
  {
    Tmr.update();
//  Loop comandi
    inchr = GetChar();

    if (inchr > 0)
    {
      if (inchr != '\n')
      {
        strCMD[pointer] = inchr;
        if (pointer < (strCMD_BUFFER_SIZE - 1)) pointer++;
        strCMD[pointer] = '\0';
      }
      else if (pointer > 0)
      {
        Serial.print("> "); Serial.println(strCMD);
        if (strcmp(strCMD, "LABEL") == 0) // 
        {
          SerialPrintLabel();
        } else 
        if (strcmp(strCMD, "STAT") == 0) // 
        {
          for(int i=2;i<12;i++)
          {
            Serial.print(". PIN");
            SPM.DecRJ(i,2,'0',false);
            Serial.print(": ");
            Serial.println(digitalRead(i)); 
          }
        } else 
        if (strcmp(strCMD, "TURN+") == 0) // 
        {
          PosTest=SBS.TESTOPEN;
          SPM.printPROGMEM(SBSTOmsg01a,false); 
          Serial.print(" ["); Serial.print(PosTest); Serial.println("]");  
        } else 
        if (strcmp(strCMD, "TURN-") == 0)
        {
          PosTest=SBS.TESTDOWN;
          SPM.printPROGMEM(SBSTDmsg01a,false); 
          Serial.print(" ["); Serial.print(PosTest); Serial.println("]");  
        } else 
        if (strcmp(strCMD, "TURNX") == 0)
        {
          PosTest=SBS.TEST_OFF;
          SPM.printPROGMEM(SBSTXmsg01a,false); 
          Serial.print(" ["); Serial.print(PosTest); Serial.println("]");  
        } else 
        if (strncmp(strCMD, "TMSG", 4) == 0)
        {
          unsigned int spd;
          int success = sscanf(strCMD, "TMSG:%u", &spd);
          if(success==1) 
          {
            DispTim=spd;
            SPM.printPROGMEM(MSGTMmsg01a,false); 
            Serial.print(TIMMSG.Set(DispTim)); Serial.println("mS"); 
          }
        }else        
        if (strncmp(strCMD, "WIND", 4) == 0)
        {
          unsigned int spd;
          int success = sscanf(strCMD, "WIND:%u", &spd);
          if(success==1) 
          {
            SBS.WindSpeed((uint8_t)spd);
            TIMcmdWIND.Set(1500);
            cmdWINDflag=true;
          }
          else
          {
            success = strcmp(strCMD, "WIND");
            if(success==0) 
            {
              SPM.printPROGMEM(CWINDmsg02a,false); 
              Serial.print(SBS.ReadWindPlsNum()); Serial.println("mS");
            }
            else
            {
              SPM.printPROGMEM(ERPARmsg01a,true);
            }
          }
        }
        else
        {
          SPM.printPROGMEM(ERSTXmsg01a,true);    
          SPM.printPROGMEM(HELPmsg01,true); 
          SPM.printPROGMEM(HELPmsg02,true); 
          SPM.printPROGMEM(HELPmsg03,true); 
          SPM.printPROGMEM(HELPmsg04,true); 
          SPM.printPROGMEM(HELPmsg05,true); 
          SPM.printPROGMEM(HELPmsg06,true); 
          SPM.printPROGMEM(HELPmsg07,true); 
        };
        pointer = 0;
        strCMD[0] = '\0';
//      Serial.print("> ");
      }
    }
    if(SendMsg)
    {
      static uint8_t counter=0;
      uint16_t n;
      Serial.print(". "); 
      for(int i=0;i<13;i++)
      {
        SPM.DecRJ(DebugReg[i],6,'0',false); Serial.print(" ");
      }
//    Serial.print("SunBlindPos:");  n=SBS.ReadPosition();   Serial.print(n); Serial.print(" ");
//    Serial.print("ReedPos:");      n=SBS.ReadReedStatus(); Serial.print(n); Serial.print(" ");
//    Serial.print("PositionCommand:");  n=PositionCommand;  Serial.print(n); Serial.print(" ");
//    Serial.print("PosTest:");      n=PosTest;              Serial.print(n); Serial.print(" ");
      Serial.println("<");
      if(++counter==10) {counter=0; SerialPrintLabel();}
      SendMsg=false;
    }
  }
}

// === INTERRUPT ===============================================================
void IntTim_ReadSwitch() // Routine di servizio timer
{
  int status_debug_pin = digitalRead(pin.Debug);

  if(status_debug_pin==HIGH) digitalWrite(pin.Debug,LOW); else digitalWrite(pin.Debug,HIGH);
  
  SBS.WindHandler(); // Attivazione periodica simulatore anemometro
  
  int regPositionCommand=SBS.PositionHandler(PosTest); // Attivazione periodica posizione tenda

  SBS.DebugReadReg(DebugReg);

  if(cmdWINDflag) 
  {
    bool TIMcmdWINDflag=TIMcmdWIND.Decrement();
    
    if(TIMcmdWINDflag) 
    {
      SPM.printPROGMEM(CWINDmsg01a,false); 
      Serial.print(SBS.ReadWindPlsNum()); Serial.println("mS"); 
      cmdWINDflag=false;
    }
  } else
  if(TIMMSG.Decrement()) 
  {
    SendMsg=true; 
    PositionCommand=regPositionCommand;
  }  

  PosTest=SBS.TESTIDLE;
}

/*-----------------------------------------------------------------------*
 * Lettura comandi                                                       *
 *- ---------------------------------------------------------------------*/
char GetChar() // --§--
{
  char c = 0;
  if (Serial.available() > 0)
  {
    c = toupper((char)Serial.read());
  }
  return (c);
}
/*-----------------------------------------------------------------------*
 * Stampa etichette                                                        *
 *- ---------------------------------------------------------------------*/
void SerialPrintLabel(void)
{
  Serial.print(". ");
  Serial.print("POSITc "); // reg[00]=mPositionCnt;  
  Serial.print("REEDPc "); // reg[01]=mReedPosCnt;  
  Serial.print("POSCMD "); // reg[02]=mPositionCommand;
  Serial.print("WPLENc "); // reg[03]=mWindPlsLenCnt;
  Serial.print("WPULSc "); // reg[04]=mWindPlsCnt;
  Serial.print("WPLSLE "); // reg[05]=mWindPlsLen;
  Serial.print("POSINT "); // reg[06]=mPosCntInterval;
  Serial.print("POSPIN "); // reg[07]=mPosReedPin;
  Serial.print("oOPENP "); // reg[08]=mOpenOutPin;
  Serial.print("iOPENP "); // reg[09]=mOpenInPin;
  Serial.print("oDOWNP "); // reg[10]=mDownOutPin;
  Serial.print("iDOWNP "); // reg[11]=mDownInPin;
  Serial.print("WNDPIN "); // reg[12]=mWindSwitchPin;
  Serial.println(" ");
}
