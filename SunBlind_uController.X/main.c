// PIC12F675 Configuration Bit Settings
//
// CONFIG
#pragma config FOSC = INTRCIO   // Oscillator Selection bits (INTOSC oscillator: I/O function on GP4/OSC2/CLKOUT pin, I/O function on GP5/OSC1/CLKIN)
#pragma config WDTE = OFF       // Watchdog Timer Enable bit (WDT disabled)
#pragma config PWRTE = ON       // Power-Up Timer Enable bit (PWRT enabled)
#pragma config MCLRE = OFF      // GP3/MCLR pin function select (GP3/MCLR pin function is digital I/O, MCLR internally tied to VDD)
#pragma config BOREN = OFF      // Brown-out Detect Enable bit (BOD disabled)
#pragma config CP = OFF         // Code Protection bit (Program Memory code protection is disabled)
#pragma config CPD = OFF        // Data Code Protection bit (Data memory code protection is disabled)

// #pragma config statements should precede project file includes.
// Use project enums instead of #define for ON and OFF.

#include <xc.h>
// your configuration bit settings go here
// configuration code (indicated earlier) omitted for brevity

#define GPIO0_SP1    GPIObits.GP0 // In   pin7
#define GPIO1_nRST   GPIObits.GP1 // Out  pin6
#define GPIO2_psbDW  GPIObits.GP2 // In   pin5
#define GPIO3_psbUP  GPIObits.GP3 // In   pin4
#define GPIO4_WDrst  GPIObits.GP4 // In   pin3
#define GPIO5_TEST    GPIObits.GP5 // Out   pin2

#define TRIS0_SP1    TRISIO0 // In 
#define TRIS1_nRST   TRISIO1 // Out
#define TRIS3_psbDW  TRISIO2 // In 
#define TRIS2_psbUP  TRISIO3 // In
#define TRIS4_WDrst  TRISIO4 // In
#define TRIS5_TEST    TRISIO5 // Out

#define SW_IDLE 3
#define SW_UP   2
#define SW_DW   1
#define SW_STOP 0

#define CNTSW_IDL 0
#define CNTSW_SET 3
#define CNTSW_ONE 2
#define CNTSW_END 1

#define RUN_UP    2
#define RUN_DW    1
#define RUN_STOP  0

#define CNTREED_ACT  50
#define CNTREED_END  250
#define CNTREED_INIT 150

#define PIN_ACTIVE  0b0
#define PIN_NOTACT  0b1

#define SET_OUTPUT  0b0
#define SET_INPUT   0b1

#define CNTRSTval    10*100 // Secondi*100
#define CNTWDval     60*100  // Secondi*100
#define CNTRSTPLSval  1*100  // Secondi*100

#define END_POSIT   20

#define CMCON_DISABLE 0x07

#define ANSEL_DISABLE 0x00

#define ISR_PERIOD_mS 10

typedef unsigned char byte;

// Proto
void  SwitchHandler   (void);
void  ReedEMUHandler  (void);
void  WatchDogHandler (void);

void  SETpinRST       (int);

byte SetTMR0(unsigned int);

// Variables
unsigned int cntRST;
unsigned int cntWD;
unsigned int cntRSTpls; 

byte lastGPIO4mirror;

byte TMR0_value;

void interrupt myIsr(void)
{
// Only process timer-triggered interrupts
    GPIO5_TEST=PIN_ACTIVE;
    if(INTCONbits.TMR0IE && INTCONbits.TMR0IF) 
    {
        INTCONbits.TMR0IF = 0b0; // clear this interrupt condition
        TMR0 = TMR0_value;       // Divider to 10ms (1MHz/64/156=100Hz)
// -----------------------------------------------------------------------------
		if(cntRSTpls>0)
        {
            cntRST = CNTRSTval;
            cntWD  = CNTWDval;
            cntRSTpls--;
        }
        else
        {
            if((GPIO3_psbUP==PIN_NOTACT) || (GPIO2_psbDW==PIN_NOTACT)) 
               cntRST=CNTRSTval;
            else
               if (cntRST>0) cntRST--;
                        
            if(GPIO4_WDrst!=lastGPIO4mirror) 
               cntWD = CNTWDval;
            else
               if(cntWD>0) cntWD--;
            
            lastGPIO4mirror=GPIO4_WDrst;

            if((cntWD==0) || (cntRST==0)) 
            {
                SETpinRST(PIN_ACTIVE);
                cntWD = CNTWDval;
                cntRSTpls = CNTRSTPLSval;
            }
            else                      
                SETpinRST(PIN_NOTACT);
        }
    }
    GPIO5_TEST=PIN_NOTACT;

}

void main(void)
{

    CMCON = CMCON_DISABLE;
    
    ANSEL = ANSEL_DISABLE;
    
// Prescaler setting è nella routine SetTMR0
    
    GPIO = 0xFF;
    
	TRIS0_SP1    = SET_INPUT ; 
	TRIS1_nRST   = SET_OUTPUT; 
	TRIS2_psbUP  = SET_INPUT ; 
	TRIS3_psbDW  = SET_INPUT ;
	TRIS4_WDrst  = SET_INPUT ;
	TRIS5_TEST    = SET_OUTPUT ;

    SETpinRST(PIN_NOTACT);
    
    lastGPIO4mirror=GPIO4_WDrst;
    
    cntWD     = CNTWDval;
    cntRST    = CNTRSTval;
    cntRSTpls = 0;
 
// Timer settings   
    TMR0_value=(byte)SetTMR0(ISR_PERIOD_mS);     // 100mS, 10Hz
// Internal clock, no prescaler
    INTCONbits.TMR0IE = 0b1; // Enable interrupts for timer 0
    INTCONbits.GIE    = 0b1; // Enable interrupts

    while(1);
    
}

void SETpinRST(int Status)
{
    if (Status) {TRIS1_nRST = SET_INPUT;  GPIO1_nRST = PIN_NOTACT;}
    else        {GPIO1_nRST = PIN_ACTIVE; TRIS1_nRST = SET_OUTPUT;}
//    if (Status) {TRIS1_nRST = SET_OUTPUT;  GPIO1_nRST = PIN_NOTACT;}
//    else        {GPIO1_nRST = PIN_ACTIVE; TRIS1_nRST = SET_OUTPUT;}
}

byte SetTMR0(unsigned int value_ms)
{
    unsigned int ps_value;
    unsigned int ps_cnt;
    unsigned long TMR0value;
    const unsigned long clk_freq=1000000;
    
    for(ps_cnt=0;ps_cnt<=8;ps_cnt++)
    {
        ps_value=(unsigned int)0b10<<ps_cnt;
        TMR0value=clk_freq*value_ms;
        TMR0value/=ps_value;
        TMR0value/=1000;
        if(TMR0value<256) break;
    }
    if(ps_cnt==8) return(0); 
    else 
    {
// Prescaler settings
        OPTION_REGbits.PSA  = 0b0;    // Prescaler is assigned to the Timer0 module
        OPTION_REGbits.PS   = ps_cnt; // Prescaler Rate Select bits 1/128 128uS w 1uS CLKOUT
        OPTION_REGbits.T0CS = 0b0;    // Internal instruction cycle clock (CLKOUT)
// Timer settings
        TMR0value=0x100-TMR0value;
        TMR0=(byte)TMR0value;
        return(TMR0value);
    }
}