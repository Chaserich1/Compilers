READ x1
LOAD 1
STORE V0
LOAD x1
SUB V0
BRNEG L0
LOAD 10
STORE V1
LOAD x1
SUB V1
BRZPOS L1
LOAD x1
STORE V2
WRITE V2
L1: NOOP
L0: NOOP
STOP 
x1 0
V0 0
V1 0
V2 0