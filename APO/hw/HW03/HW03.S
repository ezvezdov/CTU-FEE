
.text
.globl start
.set noat
.set noreorder
.ent start
start:
      // s0 - bazova adresa zacatku vstupniho pole
      // s1 - pocet prvku vstupniho pole
      // s2 - bazova adresa zacatku vystupniho pole
      // s3 - velikost okna

      lui s0, pole
      addi s0, s0, pole
      //la   s0, pole     // Nastaveni adresy pocatku vstupniho pole do registru s0
      addi s1, zero, 16   // Nastaveni poctu prvku vstupniho pole do s1
      slli  s1, s1, 2    // Vypocteni velikosti vstupniho pole (v Bajtech)
  
      add  s2, s0, s1   // Nastaveni adresy pocatku vystupniho za konec vstupniho pole
      addi s3, zero, 3    // Nastaveni poctu prvku okna
      slli  s3, s3, 2    // Vypocteni velikosti vystupniho okna
      addi t1, zero, 0     // i = 0;
      sub  s1, s1, s3    // Nastaveni horni meze vnejsiho cyklu na velikost...
      addi s1, s1, 0x4   // ...vystupniho pole
L1:
      addi t2, zero, 0     // j = 0; - offset v okne (plati pro vnitrni smycku)
      addi t4, zero, 0     // t4 = 0;  - nulovani souctu v okne
L2:
      add  s4, s0, t2    // s4 - adresa aktualniho prkvu v okne
      lw   t0, 0(s4)     // t0 <- Mem[s4]
      add  t4, t4, t0    // Pripocetni dalsiho prvku
      addi t2, t2, 0x4   // j += 4; - posun na dalsi prvek v okne
      slt  t3, t2, s3    // t3 = (t2 < s3) ? 1 : 0;  
      bne  t3, zero, L2    // Test na ukonceni cyklu
  
      srli  t4, t4, 2     //
      sw   t4, 0(s2)     // Ulozeni vysledku do vystupniho pole: Mem[s2] <- t4
      addi s2, s2, 0x4   // Posun na dalsi prvek ve vystupnim poli
      addi s0, s0, 0x4   // Posun okna ve vstupnim poli
  
      addi t1, t1, 0x4   // i += 4; - inkrementace pocitadla
      slt  t3, t1, s1    // t3 = (t1 < s1) ? 1 : 0;
      bne  t3, zero, L1
  
      nop
inf_loop:
      beq zero,zero, inf_loop
.end start


.globl    pole
.data
pole:
.word    3, 7, 12, 13, 8, 20, 8, 9, 7, 10, 15, 11, 6, 8, 11, 6, 15, 14, 13