// --------------------------------------------------------------
// VERSION display: Vx.amggn x=versione a=anno+2019 m=mese(hex, da 1 a C) gg=giorno 
#define SerialPROGMEM_VERSION_ "V1.191103.A"          // Aggiunti messaggi comandi
//efine SerialPROGMEM_VERSION_ "190803.14.0 v1.08030" //inizio giugno 2019
//                        Code1: inizio giugno 2019
// --------------------------------------------------------------

/* =============================================================================
   Funzioni per il controllo della stampa
============================================================================= */
#define VERSION_PrintControl "V1.191103.A" // Sviluppo, aggiunti msg comandi
//
const char SETUPmsg1[]       PROGMEM = {"$ Controllo Tenda "};

const char COMMANDmsg00[] PROGMEM = {"$ Sintax error, valid command are:"};
const char COMMANDmsg01[] PROGMEM = {"$ Command ENMON -------- register monitor enable"};
const char COMMANDmsg02[] PROGMEM = {"$ Command DISMON ------- register monitor disable"};
const char COMMANDmsg03[] PROGMEM = {"$ Command SHOWDATA ----- show database data"};
const char COMMANDmsg04[] PROGMEM = {"$ Command TICKTIME:<t> - graphics display interval in seconds 15s <= t <= 3600s"};

const char VERmsg01[]        PROGMEM = {"$ Version code generated: "};
const char ERRORmsgOperand[] PROGMEM = {"! Operand error"};

const char MONITmsgTemper[] PROGMEM = {" Temper:"};
const char MONITmsgHumidy[] PROGMEM = {" Humidy:"};
const char MONITmsgOpenIn[] PROGMEM = {" OpenIn:"};
const char MONITmsgDownIn[] PROGMEM = {" DownIn:"};
const char MONITmsgOpenOu[] PROGMEM = {" OpenOu:"};
const char MONITmsgDownOu[] PROGMEM = {" DownOu:"};
const char MONITmsgCntOpn[] PROGMEM = {" CntOpn:"};
const char MONITmsgCntDow[] PROGMEM = {" CntDow:"};
const char MONITmsgCntWdt[] PROGMEM = {" CntWdt:"};
const char MONITmsgTenSte[] PROGMEM = {" TenSte:"};
const char MONITmsgTenPos[] PROGMEM = {" TenPos:"};
const char MONITmsgRstReq[] PROGMEM = {" RstReq:"};
const char MONITmsgTenPoH[] PROGMEM = {" TenPoH:"};
const char MONITmsgsmSUNB[] PROGMEM = {" smSUNB:"};
const char MONITmsgfPulse[] PROGMEM = {" fPulse:"};
const char MONITmsgtimROf[] PROGMEM = {" timROf:"};
const char MONITmsgtimROn[] PROGMEM = {" timROn:"};
const char MONITmsgswReed[] PROGMEM = {" swReed:"};
const char MONITmsgtimRof[] PROGMEM = {" timRof:"};
const char MONITmsgtimRon[] PROGMEM = {" timRon:"};
const char MONITmsgtimRee[] PROGMEM = {" timRee:"};
const char MONITmsgWndSpd[] PROGMEM = {" WndSpd:"};
const char MONITmsgsmWind[] PROGMEM = {" smWind:"};
const char MONITmsgtimWnd[] PROGMEM = {" timWnd:"};
const char MONITmsgtmHour[] PROGMEM = {" tmHour:"};
const char MONITmsgtmMinu[] PROGMEM = {" tmMinu:"};
const char MONITmsgtmSeco[] PROGMEM = {" tmSeco:"};
const char MONITmsgstaLED[] PROGMEM = {" staLED:"};
const char MONITmsginChar[] PROGMEM = {" inChar:"};
const char MONITmsgmTempr[] PROGMEM = {" mTempr:"};
const char MONITmsgmHumid[] PROGMEM = {" mHumid:"};
const char MONITmsgmPosit[] PROGMEM = {" mPos:"};
const char MONITmsgmWindy[] PROGMEM = {" mWind:"};

const char LOOPmsgCMD_m[]   PROGMEM = {"+ Monitor Mode "}; 

const char TickInterval[]   PROGMEM = {"+ Tick interval: "}; 

class serialPROGMEM
{
public:
  void BinRJ(int num, int digit, char fillchar, bool LineFeed); // Stampa un numero binario (BinRJ), decimale (DecRJ), Esadecimale (HexRJ)
  void DecRJ(int num, int digit, char fillchar, bool LineFeed); // di lunghezza <digit> con <fillchar> al posto dei numeri mancanti, 
  void HexRJ(int num, int digit, char fillchar, bool LineFeed); // linefeed=true per andare a capo
  void printPROGMEM(const char* strPROGMEM, bool LineFeed);     // Stampa una stringa PROGMEN, linefeed=true per andare a capo
  void printNO(bool LineFeed);                                  // Stampa "N", linefeed=true per andare a capo
  void printSPACE(uint8_t numofspace, bool LineFeed);           // Stampa " " (spazio), linefeed=true per andare a capo
  void printYES(bool LineFeed);                                 // Stampa "Y", linefeed=true per andare a capo
//  void Serial_EEprint(const char* strPROGMEM, bool LineFeed);
};
void serialPROGMEM::printPROGMEM(const char* strPROGMEM, bool LineFeed)
{
  //char strChar;
  // read back a char
  for (uint8_t ptr = 0; ptr < strlen_P(strPROGMEM); ptr++)
  {
    char strChar =  pgm_read_byte_near(strPROGMEM + ptr);
    Serial.print(strChar);
  }
  if (LineFeed) Serial.print("\n");
}
//void serialPROGMEM::Serial_EEprint(const char* strPROGMEM, bool LineFeed)
//{
//  char strChar;
//  // read back a char
//  for (uint8_t ptr=0; ptr < strlen_P(strPROGMEM); ptr++)
//  {
//    strChar =  pgm_read_byte_near(strPROGMEM + ptr);
//    Serial.print(strChar);
//  }
//  if(LineFeed) Serial.print("\n");
//}//-----------------------------------------------------------------------
void serialPROGMEM::printYES(bool LineFeed)
{
  Serial.print (" Y  ");
  if (LineFeed) Serial.print("\n");
}
//-----------------------------------------------------------------------
void serialPROGMEM::printNO(bool LineFeed)
{
  Serial.print (" N  ");
  if (LineFeed) Serial.print("\n");
}
//-----------------------------------------------------------------------
void serialPROGMEM::printSPACE(uint8_t numofspace, bool LineFeed)
{
  for (int cnt = 0; cnt < numofspace; cnt++) Serial.print (" ");
  if (LineFeed)                        Serial.print ("\n");
}
//-----------------------------------------------------------------------
void serialPROGMEM::DecRJ(int num, int digit, char fillchar, bool LineFeed) // --?--
{
  int quo = num;
  int cnt = 0;
  if (num == 0) cnt = 1; else while (quo > 0) {
      cnt++;
      quo /= 10;
    }
  digit -= cnt;

  while (digit-- > 0) Serial.print(fillchar);

  Serial.print(num);

  if (LineFeed) Serial.print("\n");
}
//-----------------------------------------------------------------------
void serialPROGMEM::HexRJ(int num, int digit, char fillchar, bool LineFeed) // --?--
// digit: # of digit,  
// fillchar: char to fill until len=digit 
// if fillchar=x prefix="0x"
// if fillchar=X prefix="0X"
{
  int quo = num;
  int cnt = 0;
  if (num == 0) cnt = 1; else while (quo > 0) {
      cnt++;
      quo /= 16;
    }
  digit -= cnt;

  if     (fillchar == 'x') {
    Serial.print("0x");
    fillchar = '0';
  }
  else if (fillchar == 'X') {
    Serial.print("0X");
    fillchar = '0';
  }

  while (digit-- > 0) Serial.print(fillchar);

  Serial.print(num, HEX);

  if (LineFeed) Serial.print("\n");
}
//-----------------------------------------------------------------------
void serialPROGMEM::BinRJ(int num, int digit, char fillchar, bool LineFeed) // --?--
{
  int quo = num;
  int cnt = 0;
  if (num == 0) cnt = 1; else while (quo > 0) {
      cnt++;
      quo /= 2;
    }
  digit -= cnt;

  while (digit-- > 0) Serial.print(fillchar);

  Serial.print(num, BIN);

  if (LineFeed) Serial.print("\n");
}
