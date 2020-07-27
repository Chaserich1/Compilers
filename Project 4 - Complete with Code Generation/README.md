Storage=Global
Working TestFiles: S1, S2, P1, P2, P3, P4, P5, P6, P7, P9, P10
Not Working TestFiles:
   P8: Prints 2 instead of 1
   P11: Error "BRs must be followed by defined labels". On line 21 of P11.asm, need to change
        L1: NOOP to L0: NOOP so that L0 is defined and it works.
   P12: When run on the virtMach it just outputs the users input. On line 22 of P12.asm, need to
        change BR L1 to BR L0 and it works.
   P13: Error "BRs must be followed by defined labels". On line 24 of P13.asm, need to change 
        BR L3 to BR L2. On line 31, need to change BR L3 to BR L0. On line 32, need to change
        L3: NOOP to L1: NOOP so that L1 is defined and it will work.
I compiled the four not working testfiles and made the changes in the testfiles/codeGen directory. There
seems to be an issue with my if and loop nonterminals labels that I have not been able to figure out.
