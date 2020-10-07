#include <stdio.h>
#include "Ejecucion.h"
#include <conio.h>
#include <stdlib.h>

FILE *archEntrada;

void ejecucion(short int memoria[], char *imagenIMG, char *archivoDAT, char *archivoTXT)

{
    int cod=0, op1=0, op2=0, ip=0, codH=0;
    FILE *arch;
    void (*f[ 144])( short int , short int, short int, short int[], char *, char *, char *) = { 0,MOV, ADD, SUB,MUL, DIV,MOD,0,0,0,0,0,0,0,0,0,0,0,0,CMP,0,0,0,SWAP,0,RNDM,0,0,0,0,0,0,0,JMP,JE,JG,JL,JZ,JP,JN,JNZ,0,0,0,0,0,0,0,0,AND,OR,NOT,XOR,0,0,LSHIFT,RSHIFT,0,0,0,0,0,0,0,0,PUSH,POP,CALL,RET,0,0,0,0,0,0,0,0,0,0,0,SLEN,SMOV,0,SCMP,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,SYS,0,0,0,0,0,0,0,0,0,0,0,0,0, STOP};
    arch= fopen(imagenIMG, "rb");
    archEntrada= fopen(archivoDAT, "rt");

    if(arch!=NULL){
        fread(memoria,sizeof(short int),4016,arch);
        fclose(arch);
    }

        while ( 0 <= memoria[0x0008] && memoria[0x0008] < memoria[0x0002])    //  ( 0 <= ip && ip < ds)
        {
            ip= memoria[0x0008];    // obtengo siguiente instruccion

            cod= memoria[ip ];         //obtengo operandos
            op1= memoria[ip +1];
            op2= memoria[ip+2];

            codH = (cod >> 8 ) & 255;   // cod >> 8 && 00FF  Interpreto instruccion

            (*f[ codH ])( cod, op1, op2, memoria, imagenIMG, archivoDAT, archivoTXT ); // ejecutar instruccion

            memoria[0x0008]+=3;

        }
        fclose(archEntrada);
}

void MOV( short int codop , short int op1, short int op2, short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT )
{
    short int aux_op2= codop;
    aux_op2= aux_op2 & 0x0003;
    short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);
    short int direccion2 = calculaDireccionamiento(codop,op2,2,memoria);

    if(aux_op2 == 0)
        memoria[direccion1] = op2;
    else
        memoria[direccion1] = memoria[direccion2];
}

void ADD( short int codop , short int op1, short int op2 , short int memoria[], char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
    short int aux_op2= codop;
    aux_op2= aux_op2 & 0x0003;
    short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);
    short int direccion2 = calculaDireccionamiento(codop,op2,2,memoria);

    if(aux_op2 == 0)
        memoria[direccion1] += op2;
    else
        memoria[direccion1] += memoria[direccion2];

    if (memoria[direccion1] == 0)        //seteo del CC
        memoria[0x0009] = 1;
    else
        if(memoria[direccion1]<0)
            memoria[0x0009] = 0x8000;
            else
                memoria[0x0009] =0;
}

void SUB( short int codop , short int op1, short int op2 , short int memoria[], char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
    short int aux_op2= codop;
    aux_op2= aux_op2 & 0x0003;
    short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);
    short int direccion2 = calculaDireccionamiento(codop,op2,2,memoria);

    if(aux_op2 == 0)
        memoria[direccion1] -= op2;
    else
        memoria[direccion1] -= memoria[direccion2];

    if (memoria[direccion1] == 0)
        memoria[0x0009] = 1;
    else
        if(memoria[direccion1]<0)
            memoria[0x0009] = 0x8000;
            else
                memoria[0x0009] =0;

}

void MUL( short int codop , short int op1, short int op2 , short int memoria[],char *imagenIM, char *archivoDAT, char *archivoTXT)
{
    short int aux_op2= codop;
    aux_op2= aux_op2 & 0x0003;
    short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);
    short int direccion2 = calculaDireccionamiento(codop,op2,2,memoria);

    if(aux_op2 == 0)
        memoria[direccion1] *= op2;
    else
        memoria[direccion1] *= memoria[direccion2];

    if (memoria[direccion1] == 0)
        memoria[0x0009] = 1;
    else
        if(memoria[direccion1]<0)
            memoria[0x0009] = 0x8000;
            else
                memoria[0x0009] =0;
}

void DIV( short int codop , short int op1, short int op2, short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
    short int aux_op2= codop;
    aux_op2= aux_op2 & 0x0003;
    short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);
    short int direccion2 = calculaDireccionamiento(codop,op2,2,memoria);

    if(aux_op2 == 0)
        memoria[direccion1] /= op2;
    else
        memoria[direccion1] /= memoria[direccion2];

    if (memoria[direccion1] == 0)
        memoria[0x0009] = 1;
    else
        if(memoria[direccion1]<0)
            memoria[0x0009] = 0x8000;
            else
                memoria[0x0009] =0;
}

void MOD( short int codop , short int op1, short int op2, short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
    short int aux_op2= codop;
    aux_op2= aux_op2 & 0x0003;
    short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);
    short int direccion2 = calculaDireccionamiento(codop,op2,2,memoria);

    if(aux_op2 == 0)
        memoria[direccion1] %= op2;
    else
        memoria[direccion1] %= memoria[direccion2];

    if (memoria[direccion1] == 0)
        memoria[0x0009] = 1;
    else
        if(memoria[direccion1]<0)
            memoria[0x0009] = 0x8000;
            else
                memoria[0x0009] =0;
}

void CMP( short int codop , short int op1, short int op2, short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
        short int aux_op1 = codop, aux_op2= codop, aux;
        aux_op1= (aux_op1 >> 2) & 0x0003;
        aux_op2= aux_op2 & 0x0003;
        short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);
        short int direccion2 = calculaDireccionamiento(codop,op2,2,memoria);

        if(aux_op1 == 0)
        {
            if(aux_op2 == 0)
                aux = op1-op2;
            else
                aux = op1 - memoria[direccion2];
        }
        else
            if(aux_op2 == 0)
                aux = memoria[direccion1]-op2;
            else
                aux = memoria[direccion1]- memoria[direccion2];

        if (aux == 0)
            memoria[0x0009] = 1;
        else
            if(aux<0)
                memoria[0x0009] = 0x8000;
        else
            memoria[0x0009] =0;

}

void SWAP( short int codop , short int op1, short int op2, short int memoria[],char *imagenIMG , char *archivoDAT, char *archivoTXT)
{
     short int aux;
     short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);
     short int direccion2 = calculaDireccionamiento(codop,op2,2,memoria);;

      aux = memoria[direccion1];
      memoria[direccion1]= memoria[direccion2];
      memoria[direccion2]= aux;

}

void RNDM( short int codop , short int op1, short int op2, short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT )
{
    short int aux_op2= codop;
    aux_op2= aux_op2 & 0x0003;
    short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);
    short int direccion2 = calculaDireccionamiento(codop,op2,2,memoria);

    if(aux_op2 == 0)
    {
        memoria[direccion1] = rand() % (op2+1);
    }
    else
        memoria[direccion1] = rand() % (memoria[direccion2]+1);


}

void JMP( short int codop , short int op1, short int op2 , short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
    memoria[0x0008] = op1*3+ 10; // Porque después de ejecutar el salto le suma 3 a la sig instrucción.
}

void JE( short int codop , short int op1, short int op2 ,  short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
    short int aux_op1 = codop, aux_op2= codop;
    short int AX;
    aux_op1= (aux_op1 >> 2) & 0x0003;
    aux_op2= aux_op2 & 0x0003;
    short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);
    short int direccion2 = calculaDireccionamiento(codop,op2,2,memoria);

    AX=memoria[0x000a];

    if(aux_op1 == 0)
    {
        if(op1==AX)
        {
            if(aux_op2 == 0)
               memoria[0x0008]= op2*3+10;
            else
               memoria[0x0008]= memoria[direccion2]*3+10;
        }
    }
    else
        if(memoria[direccion1]==AX)
       {
           if(aux_op2 == 0)
               memoria[0x0008]= op2*3+10;
            else
               memoria[0x0008]= memoria[direccion2]*3+10;
      }

}

void JG( short int codop , short int op1, short int op2 , short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
    short int aux_op1 = codop, aux_op2= codop;
    short int AX;
    aux_op1= (aux_op1 >> 2) & 0x0003;
    aux_op2= aux_op2 & 0x0003;
    short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);
    short int direccion2 = calculaDireccionamiento(codop,op2,2,memoria);

    AX=memoria[0x000a];

    if(aux_op1 == 0)
    {
        if(op1>AX)
        {
            if(aux_op2 == 0)
               memoria[0x0008]= op2*3+10;
            else
               memoria[0x0008]= memoria[direccion2]*3+10;
        }
    }
    else
        if(memoria[direccion1]>AX)
       {
           if(aux_op2 == 0)
               memoria[0x0008]= op2*3+10;
            else
               memoria[0x0008]= memoria[direccion2]*3+10;
      }

}

void JL( short int codop , short int op1, short int op2 , short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
    short int aux_op1 = codop, aux_op2= codop;
    short int AX;
    aux_op1= (aux_op1 >> 2) & 0x0003;
    aux_op2= aux_op2 & 0x0003;
    short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);
    short int direccion2 = calculaDireccionamiento(codop,op2,2,memoria);

    AX=memoria[0x000a];

    if(aux_op1 == 0)
    {
        if(op1<AX)
        {
            if(aux_op2 == 0)
               memoria[0x0008]= op2*3+10;
            else
               memoria[0x0008]= memoria[direccion2]*3+10;
        }
    }
    else
        if(memoria[direccion1]<AX)
       {
           if(aux_op2 == 0)
               memoria[0x0008]= op2*3+10;
            else
               memoria[0x0008]= memoria[direccion2]*3+10;
      }

}

void JZ( short int codop , short int op1, short int op2 , short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
    short int aux_op1 = codop,aux;
    aux_op1= (aux_op1 >> 2) & 0x0003;
    aux= memoria[0x0009]&0x0001; //Utiliza el cc.
    short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);

    if(aux==1)
    {
        if(aux_op1==0)
        {
            memoria[0x0008]= op1*3+10;
        }
        else
            memoria[0x0008] = memoria[direccion1]*3+10;
    }
}

void JP( short int codop , short int op1, short int op2 , short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
     short int aux_op1 = codop,aux;
     aux_op1= (aux_op1 >> 2) & 0x0003;
     aux=memoria[0x0009]&0x8000; // Me quedo con el bit más significativo (bit de signo)
     short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);

    if(aux==0) // si el bit de signo está en 0
    {
        if(aux_op1==0)
        {
            memoria[0x0008]= op1*3+10;
        }
        else
            memoria[0x0008] = memoria[direccion1]*3+10;
    }
}

void JN( short int codop , short int op1, short int op2, short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
     short int aux_op1 = codop,aux;
     aux_op1= (aux_op1 >> 2) & 0x0003;
     aux=memoria[0x0009]& 0x8000;
     short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);

    if(aux!=0)
    {
        if(aux_op1==0)
        {
            memoria[0x0008]= op1*3+10;
        }
        else
            memoria[0x0008] = memoria[direccion1]*3+10;
    }
}

void JNZ( short int codop , short int op1, short int op2, short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
     short int aux_op1 = codop,aux;;
     aux_op1= (aux_op1 >> 2) & 0x0003;
     aux= memoria[0x0009]&0x0001;
     short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);

    if(aux==0)
    {
        if(aux_op1==0)
        {
            memoria[0x0008]= op1*3+10;
        }
        else
            memoria[0x0008] = memoria[direccion1]*3+10;
    }
}

void AND( short int codop , short int op1, short int op2 , short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
 short int aux_op2= codop;
    aux_op2= aux_op2 & 0x0003;
    short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);
    short int direccion2 = calculaDireccionamiento(codop,op2,2,memoria);

    if(aux_op2 == 0)
        memoria[direccion1] &= op2;
    else
        memoria[direccion1] &= memoria[direccion2];

    if (memoria[direccion1] == 0)
        memoria[0x0009] = 1;
    else
        if(memoria[direccion1]<0)
            memoria[0x0009] = 0x8000;
            else
                memoria[0x0009] =0;
}

void OR( short int codop , short int op1, short int op2 , short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
    short int aux_op2= codop;
    aux_op2= aux_op2 & 0x0003;
    short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);
    short int direccion2 = calculaDireccionamiento(codop,op2,2,memoria);

    if(aux_op2 == 0)
        memoria[direccion1] |= op2;
    else
        memoria[direccion1] |= memoria[direccion2];

    if (memoria[direccion1] == 0)
        memoria[0x0009] = 1;
    else
        if(memoria[direccion1]<0)
            memoria[0x0009] = 0x8000;
            else
                memoria[0x0009] =0;
}

void NOT( short int codop , short int op1, short int op2, short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
    short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);

    memoria[direccion1] = ~(memoria[direccion1]);

    if (memoria[direccion1] == 0)
        memoria[0x0009] = 1;
    else
        if(memoria[direccion1]<0)
            memoria[0x0009] = 0x8000;
            else
                memoria[0x0009] =0;
}

void XOR( short int codop , short int op1, short int op2, short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT )
{
    short int aux_op2= codop;
    aux_op2= aux_op2 & 0x0003;
    short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);
    short int direccion2 = calculaDireccionamiento(codop,op2,2,memoria);

    if(aux_op2 == 0)
        memoria[direccion1] ^= op2;
    else
        memoria[direccion1] ^= memoria[direccion2];

    if (memoria[direccion1] == 0)
        memoria[0x0009] = 1;
    else
        if(memoria[direccion1]<0)
            memoria[0x0009] = 0x8000;
            else
                memoria[0x0009] =0;
}

void LSHIFT( short int codop , short int op1, short int op2, short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT )
{
    short int aux_op2= codop;
    aux_op2= aux_op2 & 0x0003;
    short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);
    short int direccion2 = calculaDireccionamiento(codop,op2,2,memoria);

    if(aux_op2 == 0)
        memoria[direccion1] = memoria[direccion1] << op2;
    else
        memoria[direccion1] = memoria[direccion1] << memoria[direccion2];

}

void RSHIFT( short int codop , short int op1, short int op2, short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
    short int aux_op2= codop;
    aux_op2= aux_op2 & 0x0003;
    short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);
    short int direccion2 = calculaDireccionamiento(codop,op2,2,memoria);

    if(aux_op2 == 0)
        memoria[direccion1] = memoria[direccion1] >> op2;
    else
        memoria[direccion1] = memoria[direccion1] >> memoria[direccion2];
}

void SYS( short int codop , short int op1, short int op2 , short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
                 switch(op1)
                 {
                    case 1:
                    READ(memoria, archivoDAT);
                    break;
                    case 2:
                    WRITE(memoria, imagenIMG, archivoTXT);
                    break;
                    case 3:
                    DUMP(memoria, imagenIMG, archivoTXT);
                    break;

                 }
}

void STOP( short int codop , short int op1, short int op2, short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
    fclose(archEntrada);
    getch();
    exit (-1);
}

void READ (short int memoria[],  char *archivoDAT)
{
 short int aux, aux3;
 short int DX;
 short int CX;
 short int k=0, i=0,j=0;
 short int ds=memoria[0x0002];
 aux=memoria[0x000a]&0x00f8;
 aux3=aux & 0x00af;
 DX=memoria[0x000d];

 if( DX > memoria[0x0004])
    ds=0;

 CX=memoria[0x000c];
 char c, pal[100] ;
 char aux2[20];
 short int parteAlta=0;
 short int lectura=0;

 if(!(aux & 0x0040))   //LEE POR TECLADO
 {
         if(CX == -1)
         {
             if(aux&0x0020)
                printf("[%04d]:",DX);

             if(!(aux&0x0008))
                gets(pal);
             else
             {
                pal[i]=getch();
                while(pal[i] != 13)
                {
                     i++;
                     pal[i]=getch();
                }
                pal[i]='\0';
             }

             i=0;
             while(pal[i] != '\0')
             {
                memoria[DX+ds+i]= pal[i];
                i++;
             }
             memoria[DX+ds+i]=0;
         }
         else
         {
             switch (aux3)
             {
             case 0:
                    scanf("%s",pal);
                    if(!(aux&0x0010))
                    {
                        for(i=0; i<(CX-1); i++ )
                        {
                            memoria[DX+ds+i]= pal[i];
                            memoria[DX+ds+i]&= 0x00ff; // limpio la parte baja
                        }
                        memoria[DX+ds+i]=0;
                    }
                     else
                     {
                            CX*=2;
                            j=0;
                            for(i=0; i<(CX-1); i++ )
                            {
                                lectura= pal[i];

                                if(i%2==0)
                                {
                                  parteAlta = (lectura << 8) & 0XFF00;
                                }
                                else
                                {
                                   memoria[DX+ds+j] = parteAlta | lectura;
                                   j++;
                                }
                            }
                     }

                    break;
             case 8:
                    if(!(aux & 0x0010))
                    {
                        for(i=0; i<CX; i++)
                        {
                            memoria[DX+ds+i]= getch();
                            memoria[DX+ds+i]&= 0x00ff; // limpio la parte baja
                        }
                         memoria[DX+ds+i]=00;
                    }
                    else
                     {
                        j=0;
                        CX*=2;
                        for(i=0; i<CX; i++)
                            lectura= getch();

                         if(i%2==0)
                            parteAlta = (lectura << 8) & 0XFF00;
                         else
                            {
                               memoria[DX+ds+j] = parteAlta | lectura;
                               j++;
                            }

                    }
                    break;

             case 32:
                    scanf("%s",pal);
                    printf("[%04d]",DX);
                    if(!(aux&0x0010))
                    {
                        for(i=0; i<(CX-1); i++ )
                        {
                            memoria[DX+ds+i]= pal[i];
                            memoria[DX+ds+i]&= 0x00ff; // limpio la parte baja
                        }
                        memoria[DX+ds+i]=0;
                    }
                     else
                     {
                            CX*=2;
                            j=0;
                            for(i=0; i<(CX-1); i++ )
                            {
                                lectura= pal[i];

                                if(i%2==0)
                                {
                                  parteAlta = (lectura << 8) & 0XFF00;
                                }
                                else
                                {
                                   memoria[DX+ds+j] = parteAlta | lectura;
                                   j++;
                                }
                            }
                     }
                    break;

             case 40:   printf("[%04d]",DX);
                        if(!(aux & 0x0010))
                        {
                            for(i=0; i<CX; i++)
                            {
                                memoria[DX+ds+i]= getch();
                                memoria[DX+ds+i]&= 0x00ff; // limpio la parte baja
                            }
                             memoria[DX+ds+i]=00;
                        }
                        else
                         {
                            j=0;
                            CX*=2;
                            for(i=0; i<CX; i++)
                                lectura= getch();

                             if(i%2==0)
                                parteAlta = (lectura << 8) & 0XFF00;
                             else
                                {
                                   memoria[DX+ds+j] = parteAlta | lectura;
                                   j++;
                                }

                        }
                    break;

             case 128:  if(!(aux & 0x0010))
                        {
                            for(i=0; i<(CX); i++ )
                            {
                                    scanf("%hd", &memoria[DX+ds+i]);

                            }
                            memoria[DX+ds+i]=00;
                        }
                        else
                        {
                            CX*=2;
                            j=0;
                            for(i=0; i<(CX); i++ )
                            {
                                scanf("%hd", &lectura);
                                if(i%2==0)
                                    parteAlta = (lectura << 8) & 0XFF00;
                                else
                                {
                                    memoria[DX+ds+j] = (parteAlta | lectura);
                                    j++;
                                }
                            }
                        }
                        break;

            case 136:
                      if(!(aux & 0x0010))
                      {

                          for(i=0; i<CX; i++)
                          {
                                c= getch();
                                while ( c!= 13)
                                {
                                   aux2[k]= c;
                                   k++;
                                   c= getch();
                                }
                                 aux2[k]='\0';
                                 k=0;
                                 memoria[DX+ds+i] = atoi(aux2);

                           }
                      }
                       else
                      {
                          j=0;
                          CX*=2;
                          for(i=0; i<CX; i++)
                          {
                                c= getch();
                                while ( c!= 13)
                                {
                                   aux2[k]= c;
                                   k++;
                                   c= getch();
                                }
                                 aux2[k]='\0';
                                 k=0;
                                lectura = atoi(aux2);
                            if(i%2==0)
                               parteAlta = (lectura << 8) & 0XFF00;
                            else
                            {
                               memoria[DX+ds+j] = parteAlta | lectura;
                               j++;
                            }

                        }
                      }
                      break;
             case 160:
                       if(aux & 0x0010)
                        CX*=2;
                       j=0;
                       for(i=0; i<CX; i++)
                      {
                         printf("[%04d]:",DX+i);
                         scanf("%hd",&memoria[DX+ds+i]);

                            if(i%2==0)
                            {
                              parteAlta = (memoria[DX+ds+i] << 8) & 0XFF00;
                            }
                            else
                            {
                               memoria[DX+ds+j] = parteAlta | memoria[DX+ds+i];
                               j++;
                            }


                      }
                      break;

             case 168:
                       if(!(aux & 0x0010))
                      {

                          for(i=0; i<CX; i++)
                          {
                                printf("[%04d]:",DX+i);
                                c= getch();
                                while ( c!= 13)
                                {
                                   aux2[k]= c;
                                   k++;
                                   c= getch();
                                }
                                 aux2[k]='\0';
                                 k=0;
                                 memoria[DX+ds+i] = atoi(aux2);
                           }
                      }
                       else
                      {
                          j=0;
                          CX*=2;
                          for(i=0; i<CX; i++)
                          {
                                printf("[%04d]:",DX+j);
                                c= getch();
                                while ( c!= 13)
                                {
                                   aux2[k]= c;
                                   k++;
                                   c= getch();
                                }
                                 aux2[k]='\0';
                                 k=0;
                                lectura = atoi(aux2);
                            if(i%2==0)
                               parteAlta = (lectura << 8) & 0XFF00;
                            else
                            {
                               memoria[DX+ds+j] = parteAlta | lectura;
                               j++;

                            }

                        }
                      }
                      break;

            }
      }
 }
 else  //LEE POR ARCHIVODAT
 {
     if(archEntrada!= NULL)
     {
         if(CX == -1)
         {
             fscanf(archEntrada,"%[^\n] \n",pal);
             while(pal[i] != 0)
             {
                memoria[DX+ds+i]= pal[i];
                i++;
             }
             memoria[DX+ds+i]=0;
             fscanf(archEntrada, "\n");
         }
         else
         {
            switch (aux3)
            {
                 case 0:
                 case 8:
                 case 32:
                 case 40:
                           if(!(aux & 0x0010))
                            {

                                for(i=0; i<(CX); i++ )
                                {
                                    fscanf(archEntrada, "%c", &memoria[DX+ds+i]);
                                    memoria[DX+ds+i]&= 0x00ff; // limpio la parte baja
                                }
                                 memoria[DX+ds+i]=00;
                            }
                            else
                                {
                                    CX*=2;
                                    j=0;
                                     for(i=0; i<(CX); i++ )
                                    {
                                        fscanf(archEntrada, "%c", &lectura);
                                        if(i%2==0)
                                            parteAlta = (lectura << 8) & 0XFF00;
                                        else
                                        {
                                            memoria[DX+ds+j] = (parteAlta | lectura);
                                            j++;
                                        }
                                    }
                                }

                          break;

              default:  if(!(aux & 0x0010))
                        {
                            for(i=0; i<(CX); i++ )
                            {
                                    fscanf(archEntrada, "%hd", &memoria[DX+ds+i]);
                            }
                            memoria[DX+ds+i]=00;
                        }
                        else
                        {
                            CX*=2;
                            j=0;
                            for(i=0; i<(CX); i++ )
                            {
                                fscanf(archEntrada, "%hd", &lectura);
                                if(i%2==0)
                                    parteAlta = (lectura << 8) & 0XFF00;
                                else
                                {
                                    memoria[DX+ds+j] = (parteAlta | lectura);
                                    printf("%c%c",memoria[DX+ds+j]>>8,memoria[DX+ds+j]);
                                    j++;
                                }
                            }
                        }

            }
                fscanf(archEntrada,"\n");
        }

    }
    else
        printf("Archivo dat inexistente\n");
}

}

void WRITE(short int memoria[], char *imagenIMG,  char *archivoTXT)
{
 short int aux;
 short int DX;
 short int CX;
 short int i;
 short int ds=memoria[0x0002];
 short int auxC;

 aux=memoria[0x000a];
 if(memoria[0x0006] == memoria[0x000d])
    DX= memoria[0x000d] + memoria[0x0004];
 else
    DX=memoria[0x000d];

 if( DX > memoria[0x0004])
    ds=0;
 CX=memoria[0x000c];

 if(!(aux & 0x0040))      //WRITE por pantalla
 {
     if(CX == -1)
     {
        if(aux & 0x0020)
            printf("[%04d]:",DX);
        i=0;
        while(memoria[DX+ds+i] != '\0')
        {
          if(aux & 0x0004)
            printf("%c %04x",'%',memoria[DX+ds+i]);
          if(aux & 0x0001)
            printf("%d", memoria[DX+ds+i]);
          if(aux & 0x0008)
          {
             auxC= (memoria[DX + ds + i] & 0x00ff );
             if(((auxC >= 0) && (auxC <= 31)) ) //|| (auxC == 0) || (auxC==13))
                printf(".");
             else
                printf("%c", auxC);
          }
          if(aux & 0x0002)
            printf("%c",memoria[DX+ds+i]);

          i++;
        }
     }
     else
     {
      for(i=0; i< CX ; i++)
      {

        if(aux & 0x20)
            printf("[%04d]:", DX +i);
        if(aux & 0x10)
        {
            auxC= (memoria[DX + ds + i] & 0xff00 ) >> 8;
            if(((auxC >= 0) && (auxC <= 31)))// || (auxC == 0) || (auxC==13))
                printf(".");
            else
                printf("%c", auxC);
        }
        if (aux & 0x0008)
        {
            auxC= (memoria[DX + ds + i] & 0x00ff );
            if(((auxC >= 0) && (auxC <= 31))) // || (auxC == 0) || (auxC==13))
                printf(". ");
            else
                printf("%c", auxC);
        }
        if (aux & 0x0004)
            printf("%c%4.4x ",'%',(unsigned short int)memoria[DX + ds + i]);

        if (aux & 0x0002)
           printf("%c",memoria[DX + ds + i]);
        if (aux & 0x0001)
            printf("%c%d ",'#',memoria[DX + ds + i]);
        if( aux & 0x0080)
            printf("\n");
       }
       if (CX == 0 && (aux & 0x0080))
                printf("\n");
     }
 }
 else
 {
      FILE *arch;
      arch=fopen(archivoTXT, "at");

      if(arch != NULL )
      {
         if(CX == -1)
           {
                if(aux & 0x0020)
                   fprintf(arch,"[%04d]:",DX);
                i=0;
                while(memoria[DX+ds+i] != '\0')
                {
                  if(aux & 0x0004)
                    fprintf(arch,"%c%04x",'%',memoria[DX+ds+i]);
                  if(aux & 0x0001)
                    fprintf(arch,"%d", memoria[DX+ds+i]);
                  if(aux & 0x0008)
                  {
                     auxC= (memoria[DX + ds + i] & 0x00ff );
                     if(((auxC >= 0) && (auxC <= 31))) // || (auxC == 0) || (auxC==13))
                        fprintf(arch,".");
                     else
                        fprintf(arch,"%c", auxC);
                  }
                  if(aux & 0x0002)
                    fprintf(arch,"%c",memoria[DX+ds+i]);

                  i++;
                }
           }
         else
        {
          for(i=0; i< CX ; i++)
          {

            if(aux & 0x0020)
                fprintf(arch,"[%04d]:", DX +i);
            if(aux & 0x10)
            {
                auxC= (memoria[DX + ds + i] & 0xff00 ) >> 8;
                if(((auxC >= 0) && (auxC <= 31)) ) //|| (auxC == 0) || (auxC==13))
                    fprintf(arch,".");
                else
                    fprintf(arch,"%c", auxC);
            }
            if (aux & 0x0008)
            {
                auxC= (memoria[DX + ds + i] & 0x00ff );
                if(((auxC >= 0) && (auxC <= 31))) // || (auxC == 0) || (auxC==13))
                    fprintf(arch,". ");
                else
                    fprintf(arch,"%c", auxC);
            }
            if (aux & 0x0004)
                fprintf(arch,"%c%4.4x ",'%',(unsigned short int)memoria[DX + ds + i]);

            if (aux & 0x0002)
               fprintf(arch,"%c",memoria[DX + ds + i]);
            if (aux & 0x0001)
                fprintf(arch,"%c%d ",'#',memoria[DX + ds + i]);
            if( aux & 0x0080)
                fprintf(arch,"\n");
           }
         if (CX == 0 && (aux & 0x0080))
             fprintf(arch,"\n");
        }
      }

      else
        printf("Archivo.txt inexistente\n");
     fclose(arch);
 }

}

void DUMP(short int memoria[], char *imagenIMG, char *archivoTXT)
{
 short int i;
 short int aux, auxC;
 FILE *arch;
 arch = fopen(archivoTXT , "wt");
 aux=memoria[0x000a];
 memoria[0x0002]-=16;
short int DX;
short int CX;
short int ds=memoria[0x0002];

if(aux & 0x0040)
{
  if(arch!=NULL)
    {

         for(i=0; i< 4016 ; i++)
         {
            if(aux & 0x20)
            {
                if( i < 16)
                    if( i <10 )
                       fprintf(arch,"\n[%x]: ", i);
                    else
                        fprintf(arch,"\n[%xx]: ", i);
                else
                    fprintf(arch,"\n[%04d]: ", i -16);
            }

            if(aux & 0x10)
            {
                auxC= (memoria[i] & 0xff00 ) >> 8;
                if(((auxC >= 0) && (auxC <= 31))) // || (auxC == 0) || (auxC==13))
                    fprintf(arch,"\n.");
                else
                    fprintf(arch,"\n%c", auxC);
            }
            if (aux & 0x0008)
            {
                auxC= (memoria[i] & 0x00ff );
                if(((auxC >= 0) && (auxC <= 31))) // || (auxC == 0) || (auxC==13))
                    fprintf(arch,"\n.");
                else
                    fprintf(arch,"\n%c", auxC);
            }
            if (aux & 0x0004)
                fprintf(arch,"\n%4.4x", (unsigned short int) memoria[i]);
            if (aux & 0x0002)
               fprintf(arch,"\n%c", memoria[i]);
            if (aux & 0x0001)
                fprintf(arch,"\n%d", memoria[i]);
            if( aux & 0x0080)
                fprintf(arch,"\n");
           }

 }
 else
        printf("archivo.txt inexistente\n");
}
     else
        WRITE(memoria,imagenIMG,archivoTXT);


}

void PUSH( short int codop , short int op1, short int op2, short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
    short int aux_op1 = codop;
    aux_op1= (aux_op1 >> 2) & 0x0003;
    short int ss = memoria[0x0004], sp= memoria[0x0006];
    short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);

    if( ss < ss+sp)
    {
        sp= memoria[0x0006] -=1 ; //decrementamos SP
        if(aux_op1 == 0)
            memoria[sp + ss] = op1;
        else
            memoria[sp+ ss] = memoria[direccion1];
    }
    else
    {
        printf("Stack Overflow\n");
        STOP(codop, op1,op2,memoria,imagenIMG, archivoDAT, archivoTXT);
    }
}

void POP( short int codop , short int op1, short int op2, short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
    short int ss = memoria[0x0004], sp= memoria[0x0006];
    short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);

    if( ! (ss+ sp == 4016))   // si la pila no esta vacia
    {
       memoria[direccion1] = memoria[sp+ ss];
       memoria[0x0006] +=1 ; //incrementamos SP
    }
    else
    {
        printf("Error, pila vacía\n");
        STOP(codop, op1,op2,memoria,imagenIMG, archivoDAT, archivoTXT);
    }
}

void CALL( short int codop , short int op1, short int op2, short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
    PUSH(0, memoria[0x0008], op2, memoria, imagenIMG, archivoDAT, archivoTXT);
    JMP(0, op1,op2,memoria, imagenIMG, archivoDAT, archivoTXT);
}

void RET( short int codop , short int op1, short int op2, short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
    POP(4,8,op2,memoria,imagenIMG,archivoDAT,archivoTXT);
}

void SLEN( short int codop , short int op1, short int op2, short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
    short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);
    short int direccion2 = calculaDireccionamiento(codop,op2,2,memoria);
    short int  cant=0;

    while(memoria[direccion2] != 0)
    {
        cant++;
        direccion2++;
    }
    memoria[direccion1] = cant;
}

void SMOV( short int codop , short int op1, short int op2, short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
    short int  i;
    short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);
    short int direccion2 = calculaDireccionamiento(codop,op2,2,memoria);

    i=0;
    while( memoria[direccion2] !=0)
    {
        memoria[direccion1 + i] = memoria[direccion2];
        i++;
        direccion2 ++;
    }
    memoria[direccion1 + i] = 0;
}

void SCMP( short int codop , short int op1, short int op2, short int memoria[],char *imagenIMG, char *archivoDAT, char *archivoTXT)
{
    char cad1[100], cad2[100];
    int i=0,comp=0;
    short int direccion1 = calculaDireccionamiento(codop,op1,1,memoria);
    short int direccion2 = calculaDireccionamiento(codop,op2,2,memoria);

    while(memoria[direccion1]!= 0)
    {
        cad1[i] = memoria[direccion1];
        direccion1++; i++;
    }
    cad1[i] = '\0';

    i=0;
    while(memoria[direccion2]!= 0)
    {
        cad2[i] = memoria[direccion2];
        direccion2++; i++;
    }
    cad2[i] = '\0';
    comp = strcmp(cad1, cad2);

    if (comp == 0)
        memoria[0x0009] = 1;
    else
        if(comp<0)
            memoria[0x0009] = 0x8000;
            else
                memoria[0x0009] =0;
}

short int calculaDireccionamiento(short int codop, short int op,int tipo_op, short int memoria[])
{
    short int aux_op, ds, ss,resultado, suma=0;
    int registroOp  ,offsetOp;
    aux_op = codop;
    ds = memoria[0x0002];
    ss = memoria[0x0004];

    if(tipo_op==1){
           if( (((aux_op >> 2) & 0x0003)==3)){
                registroOp = op & 0x000f;
                offsetOp = op >> 4;
            }
    }
    else{
        if( ((aux_op & 0x0003)==3)){
            registroOp = op & 0x000f;
            offsetOp = op >> 4;
            }
    }

    if(registroOp == 0x0007 || registroOp == 0x0006)
        suma = ss;
    else
        suma = ds;


    if(tipo_op==1)
        aux_op= (aux_op >> 2) & 0x0003;
    else
        aux_op= aux_op & 0x0003;

    switch(aux_op)
    {
      case 1 : resultado = op ;
                break;

      case 2 :
               resultado = op + suma;
               break;

      case 3 : resultado = memoria[registroOp]+ offsetOp + suma;
               break;
    }
    return resultado;
}
