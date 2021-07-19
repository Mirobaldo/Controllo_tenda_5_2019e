#include "Arduino.h"
#include "DispChar.h"

// ## Funzioni display ----------------------------------------------------------------------------

void MatrixWriteBar(int8_t data[], int pos, char barset)
{
  float last=(float)data[0];
  float mini=last;
  float maxi=last;
  float ref, ndiv, actual;
  // --- CERCA il massimo e il minimo nell'array dati
  for(uint8_t p=1; p<arraySize; p++)
  {
      actual=(float)data[p];
      if (actual<mini) mini=actual;
      if (actual>maxi) maxi=actual;
//DBGif (barset==BARTEMP) {Serial.print(" data:");Serial.print(actual);} // ----------------- DEBUG
  }
//DBGif (barset==BARTEMP) Serial.println(" <<<<");// ----------------- DEBUG
  // --- Selezione BAR
  switch (barset)
  {
    case BARTEMP:
    case BARHUMID:
      if ((maxi-mini)>7)
      {
        ndiv = (maxi-mini)/8;
        ref  = mini/ndiv;
      }
      else
      {
        ndiv = 1;
        ref  = mini-3;
      }
      break;
    case BARPOSIT:
    case BARWIND:
      if (maxi>8) ndiv = maxi/8.0; else ndiv=1.0;
      ref  = -1.0;
      break;
    default:
      break;
  }

    for(uint8_t p=0;p<arraySize;p++) //------------------------ Display matrix pointer loop
    {
      byte val;
      val=(byte)(((float)data[p]/ndiv)-ref); //---------------- Val difference
      byte bar=0; //------------------------------------------- Bar filling
      if      (val<=0) {bar=0x01;} //-------------------------- All off bar
      else if (val> 7) {bar=0xFF;} //-------------------------- All on bar
      else //-------------------------------------------------- Bar filling and Matrix update
      {
        do { bar=(bar<<1)+1; val-=1;} while (val>0);
      }
      // LSB bar ON
      bar=bar|1;
      // ON OFF bit
      for(uint8_t row=0; row<=7; row++)
      {
        uint32_t MatrixAreaMask=1L<<(p);
        byte     barMask=1<<row;
        if ((bar & barMask)==0)   MatrixArea[7-row]&=~MatrixAreaMask; // OFF
        else                      MatrixArea[7-row]|= MatrixAreaMask; // ON
      }
    }
    // Barra verticale
    for(uint8_t row=0; row<=7; row++)
    {
      uint32_t MatrixAreaMask=1L<<arraySize;
      MatrixArea[7-row]|= MatrixAreaMask;
    }

};

int MatrixWriteChar(char c, int posChar, int rowChar)
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
      if ((row<8)) // row=8-1; // Safe pointer
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

int MatrixWriteString(char *s,int posString, bool CLEAR)
{
  int ptr, row;
  int len=strlen(s);

  if (CLEAR)
  {
    for(row=0; row<8; row++) MatrixArea[row]=0x0L;
  }

  for(ptr=0;ptr<len;ptr++)
  {
    posString=MatrixWriteChar(s[ptr], posString, 0);
    if(posString>=31) break;
  }

  CleanBarDisplay();

  return ptr;
}

void CleanBarDisplay(void)                  // Clean display
{                                        //
  byte     data_byte, addr;              // *
  uint32_t data_row;                     // *
                                         // *
  for(uint8_t row=0; row<8; row++)       // *
  {                                      // *
                                         // *
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

void RotateCH(int codeASCII, byte *RotCH) // Debug completato, OK
{
  uint8_t codeChar=codeASCII-0x20;
  uint8_t rowCHdata;
  uint8_t ptrCH, ptrRotCH; //, ptrRotCHbit;

  for(ptrRotCH=1; ptrRotCH<9; ptrRotCH++)  RotCH[ptrRotCH]=0;
  RotCH[0]=CH[codeChar][0];

  for(ptrCH=1; ptrCH<(CH[codeChar][0]+1); ptrCH++)
  {
    rowCHdata = CH[codeChar][ptrCH];
    for(ptrRotCH=1; ptrRotCH<9; ptrRotCH++)
    {
      RotCH[ptrRotCH]=(RotCH[ptrRotCH]<<1)+(rowCHdata%2);
      rowCHdata/=2;
    }
  }
  return;
}

/*
void PrintByte(byte number)
{
  int bn;
  byte mask, bitExtract;

  for(bn=7;bn>0;bn--)
  {
    mask=1<<bn;
    bitExtract=number&mask;
    if(!bitExtract) Serial.print("0"); else break; //
  }
  Serial.println(number,BIN);
  return;
}  
*/

/*
void PrintLword(uint32_t number)
{
  int bn;
  uint32_t mask;
  uint32_t bitExtract;

  for(bn=31;bn>=0;bn--)
  {
    mask=1<<bn;
    bitExtract=number&mask;
    if(!bitExtract) Serial.print("0"); else break; //
  }
  for(;bn>=0;bn--)
  {
    mask=1<<bn;
    bitExtract=number&mask;
    if(bitExtract) Serial.print("1"); else Serial.print("0");  //
  }
  return;
}
*/
/*
int FloatDivision(int dividend, int divisor)
{
  float fdividend, fdivisor, fquotient;
  fdividend=(float)dividend;
  fdivisor= (float)divisor;

  fquotient=fdividend/fdivisor;

  if (fquotient >= 0)
    return((int)(fquotient + 0.5));
  else
    return((int)(fquotient - 0.5));
}
*/

#endif