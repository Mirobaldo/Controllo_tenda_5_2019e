// PIC12F675 Configuration Bit Settings
//
// 20161030_1044: inserito parametro fine posit (= 20)
// 20161105_1111: modificati pin uC e inserito handler three state  
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

#define GPIO1_EMU    GPIO1 // Out  pin7
#define GPIO5_SP2    GPIO5 // Out  pin6
#define GPIO4_WDrst  GPIO4 // Out  pin5
#define GPIO0_SP1    GPIO0 // In   pin4
#define GPIO3_pinDw  GPIO3 // In   pin3
#define GPIO2_pinUp  GPIO2 // In   pin2

#define TRIS1_EMU     TRISIO1 // Out
#define TRIS5_TestI   TRISIO5 // Out
#define TRIS4_WDrst   TRISIO4 // Out
#define TRIS0_WDinp   TRISIO0 // In 
#define TRIS3_pinDw   TRISIO3 // In 
#define TRIS2_pinUp   TRISIO2 // In

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

#define CNTWDrst    200
#define CNTWD       1000

#define END_POSIT   20

#define CMCON_DISABLE 0x07

#define ANSEL_DISABLE 0x00

typedef unsigned char byte;

typedef union
{
    struct 
    {
        unsigned stSwitch :2; // Switch status 
        unsigned stRun    :2; // Run status
//§     unsigned flgReed  :1; // Emulated Reed Status 
        unsigned cntUp    :2; // Filtro up
        unsigned cntDown  :2; // Filtro up
        unsigned cntReed  :8; // Tempi del reed
        unsigned flgPos :1;
        unsigned Posit  :8;
    };
} EMUREEDbits_t;

// Proto
void  SwitchHandler   (void);
void  ReedEMUHandler  (void);
void  WatchDogHandler (void);
byte  GPIOwrite       (byte Pin, byte Status);
byte  GPIOread        (byte Pin);
void pinWDrst_handler (int);
void pinEQU_handler   (int);


// Variables
EMUREEDbits_t EMU;

unsigned int cntWDog;

byte GPIOmirror;

byte lastGPIO0_WDinp;

void interrupt myIsr(void)
{
// Only process timer-triggered interrupts
    if(INTCONbits.TMR0IE && INTCONbits.TMR0IF) 
    {
        GPIO5_TestI = PIN_NOTACT;
        
        INTCONbits.TMR0IF = 0b0; // clear this interrupt condition
        TMR0 = 156;              // Divider to 10ms (1MHz/64/156=100Hz)
// -----------------------------------------------------------------------------
        if(GPIO4_WDrst == PIN_NOTACT) 
        {
            SwitchHandler();
// -----------------------------------------------------------------------------
            ReedEMUHandler();
        }
// -----------------------------------------------------------------------------
        WatchDogHandler();
// -----------------------------------------------------------------------------
        GPIO5_TestI = PIN_ACTIVE;
    }
}

int main(void)
{
    EMU.cntDown  = CNTSW_SET;
    EMU.cntUp    = CNTSW_SET;
    EMU.stSwitch = SW_IDLE;
    EMU.stRun    = RUN_STOP;
    EMU.cntReed  = CNTREED_INIT;
    EMU.flgPos   = 0;  
    EMU.Posit    = 3;

    
    CMCON = CMCON_DISABLE;
    
    ANSEL = ANSEL_DISABLE;
    
// Prescaler settings
    OPTION_REGbits.PSA  = 0b0;    // Prescaler is assigned to the Timer0 module
    OPTION_REGbits.PS   = 0b110;  // Prescaler Rate Select bits 1/128 128uS w 1uS CLKOUT
    OPTION_REGbits.T0CS = 0b0;    // Internal instruction cycle clock (CLKOUT)
//
    
    GPIO = 0xFF;
    
    TRIS1_EMU   = SET_OUTPUT;
    TRIS5_TestI = SET_OUTPUT;
    TRIS4_WDrst = SET_OUTPUT;
    TRIS0_WDinp = SET_INPUT ;
    TRIS3_pinDw = SET_INPUT ;
    TRIS2_pinUp = SET_INPUT ;

    GPIO1_EMU   = PIN_NOTACT;
    GPIO5_TestI = PIN_NOTACT;
    GPIO4_WDrst = PIN_ACTIVE;
    
    GPIOmirror = GPIO;
    
    cntWDog = CNTWD;
 
// Timer settings   
    TMR0 = 255;              // Divider to 32ms (1MHz/128/255=31Hz)
// Internal clock, no prescaler
    INTCONbits.TMR0IE = 0b1; // Enable interrupts for timer 0
    INTCONbits.GIE    = 0b1; // Enable interrupts
    while(1) 
    {
        if(EMU.cntReed > 0 && EMU.cntReed < CNTREED_ACT) GPIO1_EMU = PIN_ACTIVE;
        else                                             GPIO1_EMU = PIN_NOTACT;
    }
}

void  WatchDogHandler (void)
{
    if(GPIO4_WDrst == PIN_NOTACT)
    {
        if(GPIO0_WDinp != lastGPIO0_WDinp) 
        {
            cntWDog = CNTWD;
        }
        else
        {
            if(cntWDog-- == 0) 
            {
                GPIO4_WDrst = PIN_ACTIVE;
                cntWDog = CNTWDrst;
            }
        }
        lastGPIO0_WDinp = GPIO0_WDinp;
    }
    else // 
    {
        if(cntWDog-- == 0)
        {
            GPIO4_WDrst = PIN_NOTACT;
            cntWDog = CNTWD;
        }
    }
    return;
}
void SwitchHandler(void)
{
    if (GPIO2_pinUp == PIN_ACTIVE && EMU.stSwitch != SW_STOP)
    {
        switch (EMU.cntUp)
        {
        case CNTSW_SET: EMU.cntUp = CNTSW_ONE; break;
        case CNTSW_ONE: EMU.cntUp = CNTSW_END; break;
        case CNTSW_END: if (EMU.stSwitch == SW_DW)      
                            EMU.stSwitch = SW_STOP;       
                        else                              
                            EMU.stSwitch = SW_UP;       
                        EMU.cntUp = CNTSW_IDL; break;     
        case CNTSW_IDL: break; 
        };
    }
    if (GPIO3_pinDw == PIN_ACTIVE && EMU.stSwitch != SW_STOP)
    {
        switch (EMU.cntDown)                                
        {                                                 
        case CNTSW_SET: EMU.cntDown = CNTSW_ONE; break;     
        case CNTSW_ONE: EMU.cntDown = CNTSW_END; break;     
        case CNTSW_END: if (EMU.stSwitch == SW_UP)      
                            EMU.stSwitch = SW_STOP;       
                        else                              
                            EMU.stSwitch = SW_DW;       
                        EMU.cntDown = CNTSW_IDL; break;     
        case CNTSW_IDL: break;                            
        };                                                    
    }
    if (GPIO3_pinDw != PIN_ACTIVE && GPIO2_pinUp != PIN_ACTIVE)
    {
        EMU.cntUp   = CNTSW_SET;
        EMU.cntDown = CNTSW_SET;
        EMU.stSwitch = SW_IDLE;
    } 

    return;
}
// -----------------------------------------------------------------------------
// ReedEMUHandler
// -----------------------------------------------------------------------------
void ReedEMUHandler(void)
{
    if(EMU.stRun == RUN_STOP) 
    {
        if (EMU.stSwitch == SW_DW) 
            EMU.stRun = RUN_DW;
        if (EMU.stSwitch == SW_UP)   
            EMU.stRun = RUN_UP;
//§?    EMU.stSwitch = SW_IDLE;
    }
    else if(EMU.stRun == RUN_UP && EMU.stSwitch == SW_DW)
    {
            EMU.stRun = RUN_DW;
    }
    else if(EMU.stRun == RUN_DW && EMU.stSwitch == SW_UP)
    {
            EMU.stRun = RUN_UP;
    }
    else 
    {
        if (EMU.stSwitch == SW_STOP) 
        {
            EMU.stRun = RUN_STOP;
//§?        EMU.stSwitch = SW_IDLE;
        }
        else 
        {
            if (EMU.stRun == RUN_UP)   
            {
                if(EMU.cntReed == CNTREED_INIT)
                {   
                    if(EMU.Posit < END_POSIT) 
                        EMU.Posit++; 
                }
                if(EMU.Posit < END_POSIT) 
                    EMU.cntReed++; 
            }
            else
            if (EMU.stRun == RUN_DW) 
            {
                if(EMU.cntReed == CNTREED_INIT)
                {   
                    if(EMU.Posit > 0)
                        EMU.Posit--; 
                }
                if(EMU.Posit > 0) 
                    EMU.cntReed--; 
            }
            if (EMU.cntReed == 0xFF)        
                EMU.cntReed = CNTREED_END-1; 
            if (EMU.cntReed == CNTREED_END) 
                EMU.cntReed = 0x00; 
        }
    }
}

void pinWDrst_handler (int Status)
{
    if (Status) {TRIS4_WDrst = SET_INPUT;  GPIO4_WDrst = PIN_NOTACT;}
    else        {GPIO4_WDrst = PIN_ACTIVE; TRIS4_WDrst = SET_OUTPUT;}
}

void pinEQU_handler (int Status)
{
    if (Status) {TRIS1_EMU = SET_INPUT;  GPIO1_EMU = PIN_NOTACT;}
    else        {GPIO1_EMU = PIN_ACTIVE; TRIS1_EMU = SET_OUTPUT;}
}
