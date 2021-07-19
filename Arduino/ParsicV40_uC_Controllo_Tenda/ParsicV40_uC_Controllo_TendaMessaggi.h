//----------------------------------------------------------------------------------------------------------------------
// *** ParsivV40_ContrlloserialPROGMEMParsicControlloTenda.h ***
//
/*+===================================================================
File: serialPROGMEMmessages.h
Summary: Contenitore di messaggi per ControlloTenda
Classes: nessuna
Functions: nessuna
Origin: Richiama serialProgMem
##
Copyright none
===================================================================+*/
#define VERSION_ParsicV40_uC_Controllo_TendaMessaggi "20190813 v1.00.19081316a"
//                        Improved of 3.00.00
//                        Code3: Prima versione
//                        Fix00: Prima vesione
//...............................................................

#include "SerialPROGMEM.h"
const char VERSION_ParsicV40_uC_Controllo_TendaMessaggi_msg[]     
                         PROGMEM = {VERSION_ParsicV40_uC_Controllo_TendaMessaggi};

const char STARTmsg01a[] PROGMEM = {"$ Controllo Tenda, emulatore "};
const char CWINDmsg01a[] PROGMEM = {"$ Wind speed modificata e corretta a "};
const char CWINDmsg02a[] PROGMEM = {"$ Wind speed: "};

const char ERPARmsg01a[] PROGMEM = {"! Numero di parametri errato"};
const char ERSTXmsg01a[] PROGMEM = {"! Errore di sintassi. Comandi disponibili:"};

const char SBSTOmsg01a[] PROGMEM = {"$ Test apertura tenda"};
const char SBSTDmsg01a[] PROGMEM = {"$ Test chiusura tenda"}; 
const char SBSTXmsg01a[] PROGMEM = {"$ Test stop movimento tenda"};  

const char MSGTMmsg01a[] PROGMEM = {"$ Aggionamento messaggi modificato: "};

const char HELPmsg01[]   PROGMEM = {"$ LABEL    : visualizza le label dei registri"};
const char HELPmsg02[]   PROGMEM = {"$ STAT     : visualizza lo stato dei registri"};
const char HELPmsg03[]   PROGMEM = {"$ TURN+    : simula l'apertura della tenda"};
const char HELPmsg04[]   PROGMEM = {"$ TURN-    : simula la chiusura della tenda"};
const char HELPmsg05[]   PROGMEM = {"$ TURNX    : ferma il movimento della tenda simulato"};
const char HELPmsg06[]   PROGMEM = {"$ TMSG:nn  : varia il tempo dei messaggi, nn in mS, default 1000"};
const char HELPmsg07[]   PROGMEM = {"$ WIND:nn  : varia la velocità del vento simulata in metri al secondo, default 3"};
