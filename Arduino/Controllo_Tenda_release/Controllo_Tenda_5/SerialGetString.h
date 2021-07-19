/*C++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
  Class:    CSerialGetString

  Summary:  Compone una stringa prelevando un carattere dalla porta seriale ad 
            ogni chiamata. Un '\n' pone fine alla stringa.
			La lunghezza massima della sringa è 127 caratteri.

  Methods:  GetString
              Ad ogni chiamata compone la stringa
            CSerialGetString
              Costruttore: imposta la lunghezza della stringa
            ~CMyClass
              Destructor: Non necessario
----------------------------------------------------------------------------C*/

// VERSION display: Vx.amggn x=versione a=anno+2019 m=mese(hex, da 1 a C) gg=giorno 
#define SerialGetString_VERSION_ "190803.14.0 v1.08030" //inizio giugno 2019

class CSerialGetString
{
	private:
//	uint8_t mPointer;
		uint8_t mStringSize;
    char    mLastChar;
    bool    en_toupper;
	    char  mGetChar(void);
	public: 
		int8_t  GetString(char*);
    char    GetLastChar(void);
            CSerialGetString(void); // Costruttore
            CSerialGetString(uint8_t); // Costruttore
            CSerialGetString(uint8_t,bool); // Costruttore
};
CSerialGetString::CSerialGetString(uint8_t size)
{
  if(size >= 127) mStringSize=127;
  else            mStringSize=size;
  en_toupper=true;
}
CSerialGetString::CSerialGetString(void)
{
  mStringSize=80;
  en_toupper=true;
}
CSerialGetString::CSerialGetString(uint8_t size, bool toupper_set)
{
  if(size >= 127) mStringSize=127;
  else            mStringSize=size;
  en_toupper=toupper_set;  
}
char CSerialGetString::mGetChar(void) // --§--
{
	char c = 0;
	if (Serial.available() > 0)
	{
    if(en_toupper) c = toupper((char)Serial.read());
    else           c = (char)Serial.read();
	}
	return (c);
}
int8_t CSerialGetString::GetString(char inString[])
{
	mLastChar = mGetChar();
  
	uint8_t pointer=strlen(inString);

	if (mLastChar > 0)
	{
		if (pointer >= (mStringSize-1)) return(-1);

    if (mLastChar == '\n')          return(pointer);

		inString[pointer++] = mLastChar;
		inString[pointer] = '\0';
	}
	return(0);
}

char CSerialGetString::GetLastChar(void) {return (mLastChar);}
