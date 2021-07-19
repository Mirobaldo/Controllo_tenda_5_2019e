/* =============================================================================
   Funzioni per il controllo della stampa
============================================================================= */

class serialPROGMEM
{
public:
	void BinRJ(int num, int digit, char fillchar, bool LineFeed);
	void DecRJ(int num, int digit, char fillchar, bool LineFeed);
	void HexRJ(int num, int digit, char fillchar, bool LineFeed);
	void printPROGMEM(const char* strPROGMEM, bool LineFeed);
	void printNO(bool LineFeed);
	void printSPACE(uint8_t numofspace, bool LineFeed);
	void printYES(bool LineFeed);
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
//-----------------------------------------------------------------------
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
void serialPROGMEM::DecRJ(int num, int digit, char fillchar, bool LineFeed) // --�--
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
void serialPROGMEM::HexRJ(int num, int digit, char fillchar, bool LineFeed) // --�--
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
void serialPROGMEM::BinRJ(int num, int digit, char fillchar, bool LineFeed) // --�--
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
