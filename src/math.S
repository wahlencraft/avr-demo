.global mul16
; R25:R24 = R25:R24 * R23:R22
#define Z1 R25
#define Z0 R24
#define X1 R19
#define X0 R18
#define Y1 R23
#define Y0 R22
mul16:
    PUSH R0;
    MOVW R18, R24;
    ; Now calculate R25:R24 = R19:R18 * R23:R22
    ; Let R25:R24 = Y, R19:R18 = X, R23:R22 = Y
    ; We have Z = X*Y or Z2*H^2 + Z1*H + Z0 = (X1*H + X0)(Y1*H + Y0)
    ; were H = 2^9
    ;
    ; Z0 = Lo(X0*Y0)
    ; Z1 = Hi(X0*Y0) + Lo(X1*Y0) + Lo(X0*Y1)
    ; Z2 = X1*Y1 (Overflow, don't even calculate)

    ; Z1:Z0 = X0*Y0
    MUL X0, Y0; R1:R0 = X0*Y0
    MOVW Z0, R0; Z1:Z0 = R1:R0

    ; Z1 = Z1 + X1*Y0
    MUL X1, Y0; R1:R0 = X1*Y0
    ADD Z1, R0; Z1 += R0

    ; Z1 = Z1 + X0*X1
    MUL X0, Y1; R1:R0 = X0*Y1
    ADD Z1, R0; Z1 += R0

    CLR R1;
    POP R0;

    RET

.global div16
; Divide two 16 bit numbers.
; R25:R24 = R25:R24 / R23:R22
#define counter R1
#define numerH R17
#define numerL R16
#define denomH R23
#define denomL R22
#define storageL R18
#define resultH R25
#define resultL R24
div16:
    PUSH R16;
    PUSH R17;
    MOVW numerL, R24;

    CLR resultL;
    CLR resultH;
div16_shift_denom:
    ; Shift denominator left until overflow in high byte
    INC counter;
    LSL denomL;
    ROL denomH;
    BRCC div16_shift_denom;
div16_divide:
    ROR denomH;
    ROR denomL;
    MOVW storageL, numerL; Make safe copy of numerator
    SUB numerL, denomL; numerator = numerator - denomenator
    SBC numerH, denomH;
    BRCC div16_res1; If carry not set, result bit is 1
    ; Carry set, result bit is 0
    MOVW numerL, storageL; Undo subtraction
    CLC; Clear carry (we will use it in the result)
    JMP div16_result;
div16_res1:
    SEC; Set carry (we will use it in the result)
div16_result:
    ; Shift result left, add result bit
    ROL resultL;
    ROL resultH;
    ; Decrement counter, restart if not 0
    DEC counter;
    BRNE div16_divide;
    ; Return
    POP R17;
    POP R16;
    CLR R1;
    RET;
