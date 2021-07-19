// --------------------------------------------------------------
// VERSION display: Vx.amggn x=versione a=anno+2019 m=mese(hex, da 1 a C) gg=giorno 
#define LedMatrix_VERSION_ "V4.191103.1" // Correzione bug p<=[hdim] 
//efine LedMatrix_VERSION_ "V4.191025.1" // Nuovo algoritmo grafico 
//                        Code1: inizio giugno 2019
//...............................................................
//                        Sub00: inizio
//...............................................................
#include "Arduino.h"

#include "LedControl.h"
LedControl lc=LedControl(pinLCData,pinLClock,pinLCLoad,4);

#include "ROMchar8x5.h"

extern serialPROGMEM SPM; // === Serial messages from PROGMEM

// --- DEBUG lines ----------------------------------------------
#ifndef STOP_PROG 
  #define STOP_PROG 1 
#endif
// while(STOP_PROG); // ---- STOP PROGRAMMA!!! ----
// Serial.print("§ MatrixWriteChar s[ptr]: ");      Serial.print(s[ptr]);    Serial.println(" ");
// -------------------------------------------------------------------------------------------------

// -------------------------------------------------------------------------------------------------
// ## Funzioni display -----------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
// -------------------------------------------------------------------------------------------------
//const char BARTEMP_ch  ='T';
//const char BARHUMID_ch ='H';
//const char BARPOSIT_ch ='P';
//const char BARWIND_ch  ='W';

const int8_t BAR_WINDOW  = 0;   // La base è calcolata in funzione dell'ultimo dato e del valore minimo del database
                                // Il valore massimo è calcolato in funzione dell'ultimo dato e del valore massimo del database
const int8_t BAR_FULLHUM = 91; // La base è il valore 0 del database
                                // Il valore massimo è il valore massimo del sensore umidità (100)
const int8_t BAR_FULLPOS = 16;  // La base è il valore 0 del database
                                // Il valore massimo è il valore massimo della posizione tenda (16)
const int8_t BAR_FULLWND = 28;  // La base è il valore 0 del database
                                // Il valore massimo è il valore 24(m/s) della velocità del vento

const int arrayMaxDim=32;

struct SunBlindData_t // Database dei dati storici del sistema
{
  int8_t  data[arrayMaxDim];
  uint8_t datasize;
  uint8_t datatype;
};

struct DatabaseBar_t // Struttura di trasferimento dati alla classe della matrice di led
{
  SunBlindData_t *data;
  uint8_t         hdim;
  uint8_t         vdim;
  uint8_t         hpos;
};

//extern const uint8_t TYPE_TEMP;
//extern const uint8_t TYPE_HUMI;
//extern const uint8_t TYPE_POSI;
//extern const uint8_t TYPE_WIND;

class MatrixDisplay
{
  private:
    void RotateCH         (int, byte*);
    uint32_t MatrixArea[8];
    
    int arraySize;
    int arrayDim;

  public:
            MatrixDisplay();
    int     MatrixWriteChar   (char,  int, int);
    int     MatrixWriteString (char*, int, bool);
    void    RefreshBarDisplay  (void);
    uint8_t MatrixWriteArrayGraphicBar(DatabaseBar_t *, uint8_t);
//  void    MatrixWriteBar(uint8_t[], uint8_t, uint8_t);
    void    MatrixWriteBar(uint8_t[], uint8_t);
};
//--------------------------------------------------------------------------------------------------

MatrixDisplay::MatrixDisplay()
{
  arraySize=arrayMaxDim;
// lc_ptr=lc;
  //Serial.println("> shutdown");
  lc.shutdown(0,false);
  lc.shutdown(1,false);
  lc.shutdown(2,false);
  lc.shutdown(3,false);
//Serial.println("> bright");
  /* Set the brightness to a medium values */
  lc.setIntensity(0,5);
  lc.setIntensity(1,5);
  lc.setIntensity(2,5);
  lc.setIntensity(3,5);
//Serial.println("> clear");
  /* and clear the display */
  lc.clearDisplay(0);
  lc.clearDisplay(1);
  lc.clearDisplay(2);
  lc.clearDisplay(3);
};

//--------------------------------------------------------------------------------------------------
int MatrixDisplay::MatrixWriteChar(char c, int posChar, int rowChar)
{
  byte      RotCH[9];
  uint32_t  maskMatrix;
  byte      maskChar;
  int       pos;
  int       row, col;

  RotateCH(c,RotCH);

  if (rowChar<0) rowChar=0; // Safe pointer
  if (rowChar>8) rowChar=8; // Safe pointer

  pos=31-posChar; // '31-' inserisce i caratteri a destra

  for(col=RotCH[0]-1;col>=0;col--)
  {
    maskMatrix=1L<<pos;
    maskChar  =1<<col;
    for(row=rowChar;row<(rowChar+8);row++)
    {
      if ((row<8)) // row=8-1; // Safe datasize
      {
        if ((RotCH[row+1]&maskChar)) MatrixArea[row] |=  maskMatrix;
        else                         MatrixArea[row] &= ~maskMatrix;
      }
      else 
      {
        break;
      }
    }
    if(pos>0) pos--; else break;
  }
  if (isAlphaNumeric(c) && pos>0)
  {
    maskMatrix=1L<<pos;
    for(row=rowChar;row<(rowChar+8);row++)
    {
      if ((row<8)) // row=8-1; // Safe pointer
      {
      if ((row+1)>8) row=8; // Safe pointer
      MatrixArea[row] &= ~maskMatrix;
      }
      else break;
    }
    if(pos>0) pos--;
  }
  
  return (31-pos);
}
//--------------------------------------------------------------------------------------------------
int MatrixDisplay::MatrixWriteString(char *s,int posString, bool CLEAR)
{
  int ptr, row;
  int len=strlen(s);
  if (CLEAR)
  {
    for(row=0; row<8; row++) 
    {
      MatrixArea[row]=0x0L;
    }
//  DispString[0]='\0';
  }

  for(ptr=0;ptr<len;ptr++)
  {
    posString=MatrixWriteChar(s[ptr], posString, 0);
    if(posString>=31) break;
  }
  RefreshBarDisplay();

  return ptr;
}
//--------------------------------------------------------------------------------------------------
void MatrixDisplay::RefreshBarDisplay(void)                  // Clean display
{                                        //
  byte     data_byte, addr;              // *
  uint32_t data_row;                     // *
                                         // *
  for(uint8_t row=0; row<8; row++)       // *
  {                                      // *
    for(addr=4; addr>0; addr--)          // *
    {                                    // * * * * * * * * * * * * * * * *
      data_row=MatrixArea[row];          //
      for(uint8_t x=0; x<addr; x++)          // Clean the bar display
      {                                  //
          data_byte=data_row % 0x100L;   //
          data_row/=0x100L;              //
      }                                  //
      lc.setRow (addr-1,row,data_byte);  //
    }
  }
}
//--------------------------------------------------------------------------------------------------
void MatrixDisplay::RotateCH(int codeASCII, byte *RotCH) // Debug completato, OK
{
  uint16_t codePTR=(codeASCII-0x20)*6; // il carattere ' ' <spazio> è 0x00
  uint8_t rowCHdata;
  uint8_t ptrCH, ptrRotCH; // ptrRotCHbit;

  for(ptrRotCH=1; ptrRotCH<9; ptrRotCH++) RotCH[ptrRotCH]=0; // 8 bit, carattere ruotato
  
  RotCH[0]=pgm_read_byte_near(CH+codePTR); // Salva dimensioni carattere
  
  for(ptrCH=1; ptrCH<=RotCH[0]; ptrCH++)
  {
    rowCHdata = pgm_read_byte_near(CH+codePTR+ptrCH);
//    SPM.BinRJ(rowCHdata, 8, '0', false);

    for(ptrRotCH=1; ptrRotCH<9; ptrRotCH++)
    {
      RotCH[ptrRotCH]=(RotCH[ptrRotCH]<<1)+(rowCHdata%2);
      rowCHdata/=2;
    }
//    SPM.BinRJ(RotCH[ptrRotCH], 8, '0', true);
  }
  for(ptrRotCH=1; ptrRotCH<9; ptrRotCH++)
  {
//    SPM.BinRJ(RotCH[ptrRotCH], 8, '0', true);
  }
//  OK FINO QUI ---------------------------
  return;
}
//--------------------------------------------------------------------------------------------------
//------------------ bardata array e variabili -----------------------------------------------------
//--------------------------------------------------------------------------------------------------
// ... 1 1 1 1 1 1 2 2 2 2 2 2 2 2 2 2 3 3 --> arraySize                         
// ... 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1                                       
// ... o o o o o o o o o o o o o o o o o o bit#7  o  o  o     * ; val>=7 ; row#7 
// ... o o o o o o o o o o o o o o o o o o bit#6  o  o  o     * ; val==6 ; row#6 
// ... o o o o o o o o o o o o o o o o o o bit#5  o  o  o     * ; val==5 ; row#5 
// ... o o o o o o o o o o o o o o o o o o bit#4  o  o  o     * ; val==4 ; row#4 
// ... o o o o o o o o o o o o o o o o o o bit#3  o  o  o ... * ; val==3 ; row#3 
// ... o o o o o o o o o o o o o o o o o o bit#2  o  o  *     * ; val==2 ; row#2 
// ... o o o o o o o o o o o o o o o o o o bit#1  o  *  *     * ; val==1 ; row#1 
// ... o o o o o o o o o o o o o o o o o o bit#0  *  *  *     * ; val==0 ; row#0 
//                     ^                    ^                  ;   ^             
//                    LED                  bar  =0 =1 =3    =FF;  val            
//--------------------------------------------------------------------------------------------------
uint8_t MatrixDisplay::MatrixWriteArrayGraphicBar(DatabaseBar_t *database, uint8_t index)
{
  uint8_t bardata[database->hdim];
  uint8_t min_actual, max_actual, offset, actual, diff, diff_max, diff_min, p;

//DBGSerial.print("$ Input data: ");  
//  for(uint8_t p=0; p<database->hdim; p++)
//  {
//    Serial.print(database->data[index].data[p]); Serial.print(" ");           
//  }
//DBGSerial.println(" <=");
    
//DBGSerial.print(F("$ Index ")); Serial.println(index);     
//DBGSerial.print(F("$ Type  ")); Serial.println(database->data[index].datatype);     
//DBGSerial.print(F("$ Size  ")); Serial.println(database->data[index].datasize);     

  if(database->data[index].datatype==BAR_WINDOW)
  {
    max_actual=database->data[index].data[0]; 
    min_actual=database->data[index].data[0];
    
    for(uint8_t p=0; p<database->data[index].datasize; p++)
    {
      uint8_t actual = database->data[index].data[p];
      if(actual >= max_actual) max_actual=actual; else 
      if(actual <= min_actual) min_actual=actual;
    }
    
    actual   = database->data[index].data[0];
    diff     = max_actual-min_actual;
    diff_min = actual-min_actual;
    diff_max = max_actual-actual;
    
//DBGSerial.print("$ Max ");       Serial.print(max_actual);     
//DBGSerial.print(", Min ");       Serial.print(min_actual);     
//DBGSerial.print(", Diff ");      Serial.print(diff);     
//DBGSerial.print(", Diff_min ");  Serial.print(diff_min);     
//DBGSerial.print(", Diff_max ");  Serial.println(diff_max);     

    if(diff<=7)
    {
      if((diff_max<4)&&(diff_min<4))  offset = 4; else
      if (diff_max>diff_min)          offset = 7-diff_max; 
      else                            offset = diff_min;
    } else
    if((diff_min>7)&&(diff_max>7))    offset = 4; else
    if (diff_min<=7)                  offset = diff_min; else
    if (diff_max<=7)                  offset = 7-diff_max;
    else                              offset = 4; 
   
//DBGSerial.print("$ Bar data (w):"); 
    for(uint8_t p=0; p<=database->hdim; p++)
    {     
      if(p<database->data[index].datasize)
      {
        bardata[p]=database->data[index].data[p]-actual+offset;
        if(bardata[p]<0)  bardata[p] = 0; else
        if(bardata[p]>7)  bardata[p] = 7;
//      bardata[p]+=1;       
      }
      else 
        bardata[p]=0;
//DBGSerial.print(bardata[p]); Serial.print(" ");    
    }
//DBGSerial.println(" <=");
  } 
  else
  {
  for(uint8_t p=0; p<=database->hdim; p++)
    {     
      if(p<database->data[index].datasize-1)
      {
        unsigned int mult = database->data[index].datatype*18; // 18 è circa 128/7
        unsigned int dat  = database->data[index].data[p]*128; bardata[p] = dat/mult;
        if(bardata[p]<0)  bardata[p] = 0; else
        if(bardata[p]>7)  bardata[p] = 7;
        bardata[p]++; // Incremento di uno 
//DBGSerial.print("$ BarData(f) - Mult:"); Serial.print(mult);     
//DBGSerial.print(", Dat:");               Serial.print(dat);     
//DBGSerial.print(", Bardata[");           Serial.print(p);     
//DBGSerial.print("]: ");                  Serial.println(bardata[p]);     
      }
      else 
        bardata[p]=1;
    }   

  }

Serial.print(F("$ Index ")); Serial.println(index);     
  
//MatrixWriteBar(bardata, database->hpos, database->hdim);
  MatrixWriteBar(bardata, database->hdim); // DEBUG!!!

  return(database->data[index].datasize);
}

//id MatrixDisplay::MatrixWriteBar(uint8_t data[], uint8_t pos, uint8_t hdim)
// Prepara la matrice di punti del grafico, il display avviene da un'altra parte
void MatrixDisplay::MatrixWriteBar(uint8_t data[], uint8_t hdim)
{
  // HERE!
  for(uint8_t p=0;p<=hdim;p++) // ------------------------ Display matrix pointer loop
  {
    byte val;
    val=(byte)data[p]; // ------------------------------------ Val difference
    byte bar=0; // ------------------------------------------- Bar filling
    if      (val<=0) {bar=0x01;} // -------------------------- All off bar
    else if (val>=8) {bar=0xFF;} // -------------------------- All on bar
    else // -------------------------------------------------- Bar filling and Matrix update
    {
      switch(val)
      {
        case 1: bar=0x01; break;
        case 2: bar=0x03; break;
        case 3: bar=0x07; break;
        case 4: bar=0x0F; break;
        case 5: bar=0x1F; break;
        case 6: bar=0x3F; break;
        case 7: bar=0x7F; break;
      }
    }
//DBGSerial.print("$ Bar led: "); 
//DBGSerial.print(bar,BIN); Serial.println(" "); 
    // ON OFF bit
    for(uint8_t row=0; row<=7; row++)
    {
      uint32_t MatrixAreaMask=1L<<(p);
      byte     barMask=1<<row;
      if ((bar & barMask)==0)   MatrixArea[7-row]&=~MatrixAreaMask; // OFF
      else                      MatrixArea[7-row]|= MatrixAreaMask; // ON
    }
  }
//DBGSerial.println(" <=");

  // Barra verticale

  for(int8_t row=7; row>=0; row--)
  {
    uint32_t MatrixAreaMask=1L<<hdim;
    MatrixArea[7-row]|= MatrixAreaMask;
Serial.print(F("$ matrix data: ")); SPM.BinRJ(MatrixArea[7-row], 32, '0', true);
  }
Serial.println(F(">"));
  return;

};
