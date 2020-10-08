# Virtual Machine - Assembly translator (Tanembaum Architecture)

This was a project for my University's Architecture of Computers subject developed in 2018 by Lucia G., Ezequiel W and me. 
It's a 16-bit virtual machine (replicating the Tanembaum's famous architecture. 
The system can :

* read a program written in assembly (.asm files)
* translate it to machine code (for Tanembaum virtual architecture)
* simulate the execution.


## Architecture description:

This virtual machine has the following components:

* RAM Memory of 2000 cells of 2 bytes each
* 6 general use registers: AX, BX, CX, DX, EX, FX
* 1 Program Counter register : IP
* 1 Condition register: CC
* 1 Data Segment register: DS
* 1 Stack Segment register: SS (3999 - StackSIZE + 1)
* 1 Stack Pointer register : SP
* 1 Base Pointer register: BP

## Assembly instructions supported:

* MOV
* PUSH, POP (for the stack)
* CALL
* RET
* ADD, SUB, MUL, DIV
* MOD
* CMP
* SMOV, SCMP, SLEN (for strings comparisons)
* AND, OR, XOR, NOT
* LSHITF, RSHIFT
* SWAP
* RNDM
* JMP
* JE
* JG
* JL
* JZ
* JP 
* JN
* JNZ
* SYS
* STOP

It also supports custom labes with the EQU instruction, for example
```
CUSTOM_LABEL EQU 0032
   ...
CUSTOM_LABEL: ADD[1000], 1
```
For comment lines, use *

## Usage

 ```
virtual-machine-architecture.exe [-op] [code.asm] [machine.img] [inputs.dat] [output.txt]
```
where
 ```
 - [op] is the operation ( <<T>> translate, <<X>> execute and <<A>> translate and execute )
 - [code.asm] is the source code of your assembly program
 - [machine.img] is the binary file generated post translation
 - [inputs.dat] is a read only file to load a custom dump to the memory 
 - [output.txt] is the output file for memory dump
```

## Program Example
Write your code in a text file with the .asm extension like this:
 ```assembly
*	void inorden(Arbol *A){
*  	if(*A != null){
*		inorden(A->izq);
*		printf("%d\n, A->dato);
*        	inorden(A->der);
*	}
 
    IZQ EQU 1
    DER EQU 2

    PUSH BX
    CALL INORDEN
    ADD SP,1
    STOP


INORDEN: PUSH BP
         MOV BP, SP
         PUSH FX
      
         MOV FX, [BP+2]
         CMP FX, -1
         JZ FIN2
      
         PUSH [FX+IZQ]
         CALL INORDEN
         ADD SP, 1
 
         MOV AX, %A5
         MOV CX, 1
         MOV DX, FX
         SYS 2

         
         PUSH [FX+DER]
         CALL INORDEN
         ADD SP, 1

FIN2:    POP FX
         MOV SP, BP
         POP BP
         RET
```
