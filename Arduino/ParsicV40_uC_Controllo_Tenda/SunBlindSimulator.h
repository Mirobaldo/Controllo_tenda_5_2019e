// === Classe con simulatore tenda =============================================
#define PULSE_PER_REED_CYCLE 5
#define MAX_REED_CYCLE_PER_SECOND (200/PULSE_PER_REED_CYCLE)
class SunBlindSimulator
{
  private:
    int16_t  mPositionCnt=S_CNTSET;  
    int16_t  mReedPosCnt=0;  
    int16_t  mReedPosCntLast=0;  
    uint8_t  mPositionCommand=0;
    uint8_t  mWindPlsLenCnt=0;
    uint8_t  mWindPlsCnt=0;
    uint8_t  mWindPlsLen=0;
//  uint16_t mWindSimValue=3;
    
    int16_t  mPosCntInterval;
    
    byte     mPosReedPin;
    byte     mOpenOutPin;
    byte     mOpenInPin;
    byte     mDownOutPin;
    byte     mDownInPin;
    byte     mWindSwitchPin;
    byte     mWindPlsRead;
    byte     mWindPlsReadCnt;
    byte     PosReedPinHandler(int16_t);
  public:
/* --- Costruttore classe SunBeamSimulator--------------------------------------
   Trasferisce alla classe la posizione dei segnal sui pin

   L'argomento deve specificare, in sequenza il pin number dei segnali:
   1) Pos Reed
   2) Open Out
   3) Open In
   4) Down Out
   5) Down In
   6) Wind Speed

   SunBlindSimulator(byte pinref[], uint8_t tim);
   byte pinref[6]={pin.PosReed, pin.OpenOut, pin.OpenIn, pin.DownOut, pin.DownIn, pin.WindSwitch_int};
   ---------------------------------------------------------------------------*/
    SunBlindSimulator(byte* pinref, uint8_t tim) // {PosReedPin,OpenOutPin,OpenInPin,DownOutPin,DownInPin,WindSwitchPin}
    {
      mPosReedPin     = pinref[0]; 
      mOpenOutPin     = pinref[1];
      mOpenInPin      = pinref[2];
      mDownOutPin     = pinref[3];
      mDownInPin      = pinref[4];
      mWindSwitchPin  = pinref[5];
      mPositionCnt    = S_CNTSET;
      mPosCntInterval = (uint16_t) tim;
      mPositionCommand=S_IDLE;
      mReedPosCnt=0;  
      mReedPosCntLast=0;  
    } 
// --- Altri metodi
    uint16_t PositionHandler(uint8_t);
    bool     WindHandler(void);
    void     WindSpeed(uint8_t);
    uint8_t  ReadWindPlsNum(void);
    
    uint16_t ReadReedStatus(void);
    uint16_t ReadPosition(void);

    void     DebugReadReg(int16_t reg[13])
    {
      reg[0]=mPositionCnt;  
      reg[1]=mReedPosCnt;  
      reg[2]=mPositionCommand;
      reg[3]=mWindPlsLenCnt;
      reg[4]=mWindPlsCnt;
      reg[5]=mWindPlsLen;
      reg[6]=mPosCntInterval;
      reg[7]=mPosReedPin;
      reg[8]=mOpenOutPin;
      reg[9]=mOpenInPin;
      reg[10]=mDownOutPin;
      reg[11]=mDownInPin;
      reg[12]=mWindSwitchPin;
    }
    
    const uint8_t S_IDLE=0;
    const uint8_t T_OPEN=1;
    const uint8_t T_DOWN=2;
    const uint8_t T_OFF =3;
    const uint8_t R_OPEN=14;
    const uint8_t R_DOWN=24;
    const uint8_t R_OFF =34;
    const uint8_t S_OPEN=10;
    const uint8_t S_DOWN=20;
    const uint8_t S_OFF =30;
   
    const uint8_t S_CNTMAX =17;
    const uint8_t S_CNTMIN =0;
    const uint8_t S_CNTSET =0;
    
    const int16_t S_REEDPOSDOWN = 1000;
    const int16_t S_REEDPOSOPEN = 1200;
    const int16_t S_REEDPOS_MAX = 3000;
    
    const uint8_t TESTIDLE=0x00;
    const uint8_t TESTOPEN=0x01;
    const uint8_t TESTDOWN=0x02;
    const uint8_t TEST_OFF=0x03;
};

uint8_t SunBlindSimulator::ReadWindPlsNum(void)
{ 
  return(mWindPlsRead);
};

/* --- Simulatore anemometro ---------------------------------------------------
ANALOG0 sul potenziometro RV1 simula la velocit? del vento, letto dal uCsim, lo 
stesso invia impulsi proporzionali su pinWindSwitch_int (D3).

Formule:
  wp=(AN0/51) 
  imp=1000/(wp*5)=(1000*51)/(AN0*5)=200*51/AN0=10200/AN0
  len=imp/5
dove 
  wp=wind pulse, 
  len=lunghezza pulse in unit? di 5mS, 
  imp=periodo impulsi in unit? di 5mS 

- imp imposta il contatore cnt, len la lunghezza impulso
- il contatore conta a ritroso, 
- se cnt<len l'uscita pinWindSwitch_int va a 1 altrimenti 0
--------------------------------------------------------------------------------*/
/* --- SunBlindSimulator WindSpeed----------------------------------------------
Imposta il numero di impulsi del simulatore anemometro in funzione del
valore impostato.
--------------------------------------------------------------------------------*/
void SunBlindSimulator::WindSpeed(uint8_t WindSimValue)
{
  float flWindPlsLen;
  if(WindSimValue==0) mWindPlsLen=0;
  else                
  {
    if(WindSimValue>25) WindSimValue=25;
    flWindPlsLen=0.45+MAX_REED_CYCLE_PER_SECOND/WindSimValue;
    mWindPlsLen=(int)flWindPlsLen;
  }
}

/* --- SunBlindSimulator WindHandler -------------------------------------------
Routine chiamata dall'interrupt
Genera un impulso con duty cicle al 80% di frequenza di frequenza:
              1/0,005 x mWindPlsLen
con 0 =< mWindPulsNum < 40

m/s = impulsi x secondo
1m/s = 1 impulso x secondo  5 pulse 1 on ____________________#####
2m/s = 2 impulsi secondo   10 pulse 2 on _________####________####
5m/s = 5 impulsi secondo   25 pulse 5 on ____#____#____#____#____#

200 impulsi secondo max 
200/5= 40 cicli

Durata singolo impulso:

200 impulsi secondo diviso 5=40 diviso 1 = 40 impulsi max con 1 m/s
200 impulsi secondo diviso 5=40 diviso 2 = 20 impulsi max con 2 m/s
200 impulsi secondo diviso 5=40 diviso 3 = 13 impulsi max con 3 m/s circa
200 impulsi secondo diviso 5=40 diviso 4 = 10 impulsi max con 4 m/s 
200 impulsi secondo diviso 5=40 diviso 5 =  8 impulsi max con 5 m/s 
200 impulsi secondo diviso 5=40 diviso 6 =  6 impulsi max con 6 m/s 

Durata impulsi = 40 / WindSimValue (speed m=s)

200/5=50

windspeed=1000

--------------------------------------------------------------------------------*/
bool SunBlindSimulator::WindHandler(void)
{
  static uint8_t OneSecondCounter=0;
  if(OneSecondCounter>=200) 
  {
    OneSecondCounter=0;
    mWindPlsRead=mWindPlsReadCnt;
    mWindPlsReadCnt=0;
  }
  else 
    OneSecondCounter++;
  
  if(mWindPlsLen==0) 
    digitalWrite(mWindSwitchPin,LOW); // NON VENTO!!!
  else
  {
    if(mWindPlsLenCnt>0) 
    { 
      mWindPlsLenCnt--; 
    }
    else
    {
      mWindPlsLenCnt=mWindPlsLen;

// 0123401234
// _####_#### con PULSE_PER_CYCLE=5

      if(mWindPlsCnt>=(PULSE_PER_REED_CYCLE-1)) // Efficente :-)  
      {
        mWindPlsCnt=0;
        digitalWrite(mWindSwitchPin,LOW);
        mWindPlsReadCnt++;
      }
      else
      {
        mWindPlsCnt++;
        digitalWrite(mWindSwitchPin,HIGH);
      }
    }          
  }
  if(digitalRead(mWindSwitchPin)==HIGH) return(true); else return(false);
}
/* --- SunBlindSimulator ReadReedStatus ------------------------------------------
Leggi posizione ruota del reeed
--------------------------------------------------------------------------------*/
uint16_t SunBlindSimulator::ReadReedStatus(void) { return(mReedPosCnt);}
/* --- SunBlindSimulator ReadPosition ------------------------------------------
Leggi posizione virtuale tenda
--------------------------------------------------------------------------------*/
uint16_t SunBlindSimulator::ReadPosition(void) { return(mPositionCnt);}
/* --- SunBlindSimulator PosReedPinHandler -------------------------------------
Attiva o disattiva il pin del reed in funzione della posizione simulata
--------------------------------------------------------------------------------*/
byte SunBlindSimulator::PosReedPinHandler(int16_t position)
{
//  
//    digitalWrite(mPosReedPin,LOW);
//    pinMode(mPosReedPin,   INPUT);  // 8 Controllo da microcontroller
//    return digitalRead(mPosReedPin);
//    static bool last_pos_reed_pin;  

  if(position > S_REEDPOSDOWN && position < S_REEDPOSOPEN) 
  {
    digitalWrite(mPosReedPin,LOW);
    pinMode(mPosReedPin,  OUTPUT);  // 8 Controllo da microcontroller 
  }
  else                          
  {
    digitalWrite(mPosReedPin,LOW);
    pinMode(mPosReedPin,   INPUT);  // 8 Controllo da microcontroller
  }
  return digitalRead(mPosReedPin);
}
/* --- SunBlindSimulator PositionHandler ----------------------------------------
Ro  utine chiamata dall'interrupt
--------------------------------------------------------------------------------*/
uint16_t SunBlindSimulator::PositionHandler(uint8_t test)
{
  int regPositionCommand=mPositionCommand;
  if((digitalRead(mOpenOutPin)==LOW) && (digitalRead(mDownOutPin)==LOW)) 
  {
    mPositionCommand=S_OFF; // 24
  } else
  if (digitalRead(mOpenOutPin)==LOW)  
  {
    if(regPositionCommand!=S_OFF) mPositionCommand=S_OPEN; // 14
  } else
  if (digitalRead(mDownOutPin)==LOW) 
  {
    if(regPositionCommand!=S_OFF) mPositionCommand=S_DOWN; 
  } 
  else
  {
    if(test==TESTOPEN) mPositionCommand=S_OPEN; else
    if(test==TESTDOWN) mPositionCommand=S_DOWN; else
    if(test==TEST_OFF) mPositionCommand=S_OFF;  
    if(mPositionCommand!=S_OPEN &&
       mPositionCommand!=S_DOWN &&
       mPositionCommand!=S_OFF  ) mPositionCommand=S_IDLE;
  }


  if (mPositionCommand==S_OPEN) 
  {
//  if (digitalRead(2)) digitalWrite(2,LOW); else digitalWrite(2,HIGH);
    if (mPositionCnt==S_CNTMAX) mPositionCommand=S_IDLE; // Posizione massima simu tenda
    else
    {
      mReedPosCntLast=mReedPosCnt;
      mReedPosCnt+=mPosCntInterval;       
    }
    if(mReedPosCnt > S_REEDPOS_MAX) mReedPosCnt=mPosCntInterval; // Fine giro simu reed
    else if((mReedPosCntLast <= S_REEDPOSOPEN) && (mReedPosCnt > S_REEDPOSOPEN)) 
    {
      mPositionCnt++; // Incrementa contatore simu reed
      mReedPosCnt+=mPosCntInterval*2;
    }
  } else       
  if (mPositionCommand==S_DOWN) 
  {
//  if (digitalRead(2)) digitalWrite(2,LOW); else digitalWrite(2,HIGH);
    if (mPositionCnt==S_CNTMIN) mPositionCommand=S_IDLE; // posizione minima simu tenda
    else 
    {
      mReedPosCntLast=mReedPosCnt;
      mReedPosCnt-=mPosCntInterval;
    }
    if(mReedPosCnt < mPosCntInterval) mReedPosCnt=S_REEDPOS_MAX; // Fine giro simu reed
    else if((mReedPosCntLast > S_REEDPOSDOWN) && (mReedPosCnt <= S_REEDPOSDOWN))
    {
      mPositionCnt--; // Decrementa simu reed
      mReedPosCnt-=mPosCntInterval*2;
    }
  } else       
  if (mPositionCommand==S_OFF) 
  {
    mPositionCommand=S_IDLE;
  }
  // Clean position counter da errori
  if(mPositionCnt<0)  mPositionCnt=S_CNTMIN; 
  if(mPositionCnt>16) mPositionCnt=S_CNTMAX;
  PosReedPinHandler(mReedPosCnt);
  
  return(mPositionCnt);
}
