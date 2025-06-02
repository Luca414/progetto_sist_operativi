# progetto_sist_operativi

Sistema di allocazione dinamica della memoria personalizzato: progettato per simulare il funzionamento di un allocatore reale all'interno di un contesto controllato. Il sistema combina due strategie di gestione:

Buddy Allocator: utilizzato per allocazioni di piccole dimensioni (inferiori a 1024 byte). Questa tecnica suddivide la memoria in blocchi di dimensioni crescenti in potenze di due, consentendo un'efficiente gestione dello spazio e riducendo la frammentazione.

MMAP: utilizzato per allocazioni più grandi (uguali o superiori a 1024 byte), demandando la gestione al sistema operativo tramite mapping diretto in memoria virtuale.

L’interfaccia pubblica fornita (malloc_red() e free_red()) replica il comportamento delle classiche malloc() e free(), ma sfrutta internamente l’allocatore sviluppato. È stato inoltre implementato un sistema di bitmap per tracciare lo stato dei blocchi di memoria nella struttura buddy.

Il progetto include anche un file di test dettagliato, che esegue varie tipologie di scenari: allocazioni, deallocazioni, doppie free, scrittura su memoria allocata, con output esplicativi e stampa della bitmap per verificarne la coerenza.
