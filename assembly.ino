# include "fir1.h"
# include "input1.h"
# include "input2.h"
signed int output[125];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);

  asm volatile(
 
    "ldi r25,26 \n\t"
    "push r0 \n\t"
    "push r1 \n\t" 
    "ldi r16,26 \n\t"   //index r16 starts from 26
   "for_loop1: \n\t"
   "clr r9 \n\t"
"clr r10 \n\t"
"clr r23 \n\t"
"clr r24 \n\t"
   "cpi r16,125 \n\t"
    "brsh endif1 \n\t"//branch to endif1 if r16 >= 125
  "ldi r17,0 \n\t"    // index r17 for fir 1
    "for_loop2: \n\t" 
    "cpi r17,27 \n\t"
    "brsh endif2 \n\t" //branch to endif2 if r17 >=25
  "ldi r28,lo8(fir1) \n\t"
    "ldi r29,hi8(fir1) \n\t"
    "add r28,r17 \n\t"
    "adc r29,r21 \n\t"
    "add r28,r17 \n\t" 
    "adc r29,r21 \n\t"
    "ld r20,y+ \n\t"  // r20 and r22 stores 16 bits coefficient fir1
  "ld r22,y \n\t"
    ////////////////////////////
  "ldi r30,lo8(input2) \n\t"
    "ldi r31,hi8(input2) \n\t"
    "sub r16,r17 \n\t"
    "add r30,r16 \n\t"
    "adc r31,r21 \n\t"
    "add r30,r16 \n\t"
    "adc r31,r21 \n\t"
    "ld r18,z+ \n\t" //load low byte into r18
  "ld r19,z \n\t"  //load high byte into r19
  "add r16,r17 \n\t"

  //..............multiply.....................................

  "clr r2 \n\t"
    "muls r22, r19 \n\t"
    " mov r15,r1 \n\t"
    "mov r14,r0 \n\t"
    "mul r20, r18; \n\t" 
    "mov r13,r1 \n\t"
    "mov r12,r0 \n\t"
    "mulsu r22, r18 \n\t" 
    "sbc r15, r2 \n\t"
    "add r13, r0 \n\t"
    "adc r14, r1 \n\t"
    "adc r15, r2 \n\t"
    "mulsu r19, r20 \n\t" 
    "sbc r15, r2 \n\t"
    "add r13, r0 \n\t"                   
    "adc r14, r1 \n\t"         //*r15:r14r13:r13:r12  32bits mul value in these 4 registers.
  "adc r15, r2 \n\t"

  //  /////// sum them up
  "add r24,r12 \n\t"          //so r24,r23,r9,r10 stores  yi
  "adc r23,r13 \n\t"
    "adc r9,r14 \n\t"
    "adc r10 ,r15 \n\t"
    
  "inc r17 \n\t"  // increase inner loop index

    "jmp for_loop2 \n\t"

    ::: 
  "r24","r23","r18","r19","r20","r22","r9","r10","r12","r13","r14","r15","r16", "r17","r2","r26","r27","r28","r29","r30", "r31"

    );

  asm volatile(
  "endif2:\n\t" 
    
"clr r21 \n\t"
    "ldi r26, lo8(output) \n\t"
    "ldi r27, hi8(output) \n\t"
    
    "add r26,r25 \n\t"  
    "adc r27,r21 \n\t"
    "add r26,r25 \n\t"
    "adc r27,r21 \n\t"
    //   "lsl r23 \n\t"
  //   "adc r9,r21 \n\t"
  //    "lsl r9 \n\t"
  //  "lsl r10 \n\t"
  //   "adc r10,r21 \n\t"
  "rol r23 \n\t"
    "rol r9 \n\t"
    "rol r10 \n\t"
    "st x+, r9 \n\t"               
    "st x, r10 \n\t"
     "inc r16 \n\t"
"inc r25 \n\t"

    "jmp for_loop1"
  );
    
 
    asm volatile(
    "endif1: \n\t"
    "pop r1 \n\t"
    "pop r0 \n\t"
      );
      // printRegs(); 
   for(int i = 0; i<125; i++){
     Serial.print("input");
     Serial.print(i);
     Serial.print(": ");
     Serial.print(input1[i]);
     Serial.print(",");
     Serial.print("output1");
      Serial.print(i);
     Serial.print(": ");
  Serial.println(output[i]);
  }    

}

void loop() {

}




