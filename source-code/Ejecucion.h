#include <string.h>


void ejecucion(short int memoria[], char *imagenIMG, char *archivoDAT, char *archivoTXT);
void MOV( short int codop , short int op1, short int op2 , short int memoria[], char *imagenIMG, char *archivoDAT, char *archivoTXT);
void ADD( short int codop , short int op1, short int op2 , short int memoria[], char *imagenIMG, char *archivoDAT, char *archivoTXT);
void SUB( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void MUL( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void DIV( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void MOD( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void SUB( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void CMP( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void SWAP( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void RNDM( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void JMP( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void JE( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void JG( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void JL( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void JZ( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void JP( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void JN( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void JNZ( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void AND( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void OR( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void NOT( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void XOR( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void LSHIFT( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void RSHIFT( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void SYS( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void STOP( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void PUSH( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void POP( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void RET( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void CALL( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);

void SMOV( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void SLEN( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);
void SCMP( short int codop , short int op1, short int op2 , short int memoria[],  char *imagenIMG, char *archivoDAT, char *archivoTXT);

void READ (short int memoria[], char *archivoDAT);
void WRITE(short int memoria[], char *imagenIMG, char *archivoTXT);
void DUMP(short int memoria[], char *imagenIMG, char *archivoTXT);



short int calculaDireccionamiento(short int codop, short int op,int tipo_op, short int memoria[]);
