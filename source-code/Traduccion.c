#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Traduccion.h"

void leeArchivoImg (reg vec[], reg vec_registros[], short int memoria[],  char *fuenteASM, char *imagenIMG, regeti vegeti[]) // Genera un String de cada una de las lineas del .img.
{
    FILE *arch;
    char linea[100];
    int actual=16;
    int cont=0;


    printf("\n");

    arch=fopen(fuenteASM,"rt");

    if (arch!=NULL)
    {
        fscanf(arch, "%[^\n] \n", linea);

        analiza(linea, vec, vec_registros,memoria,&actual,&cont, vegeti);// Analiza el primer renglón del archivo imagen

        printf("\n");

        while(!feof(arch))
        {
           fscanf(arch, "%[^\n] \n", linea);
           analiza(linea, vec, vec_registros,memoria,&actual,&cont, vegeti);
           printf("\n");
        }
    }
    memoria[0x0002]=cont*3+16;
    guardaArchivoImagen(memoria,imagenIMG);
    fclose(arch);


}

void genera_tabla(reg vec[], int n) // Genera MOV cod , ADN cod,..., ETC.
{
    FILE *arch;
    int i;

    arch=fopen("tablafunciones.txt", "rt");

    if(arch!=NULL)
        for(i=0; i<n; i++)
           fscanf(arch,"%s %x", vec[i].instruccion,&(vec[i].cod));

    fclose(arch);
}


void genera_tablaRegistros(reg vec_reg[], int n) // Genera AX cod,BX cod,..., ETC.
{
    FILE *arch;
    int i;

    arch=fopen("tablaregistros.txt", "rt");
      if(arch!=NULL)
        for(i=0; i<n; i++)
           fscanf(arch,"%s %x", vec_reg[i].instruccion,  &(vec_reg[i].cod));


    fclose(arch);
}
void analiza(char linea[], reg vec[], reg vec_registros[],short int memoria[], int *actual, int *cont, regeti vegeti[]) // Separa por palabra y analiza errores (excluye comas).
{
    int i=0,k=0;
    char aux_palabra[100];
    instruccionbase inst;
    int longetiqueta=0;
    char etiqueta[10]= "\0";
    int valor;


    inst.codop[0] = '\0';             // Inicializamos el registro instruccion
    inst.op1[0] = '\0';
    inst.op2[0] = '\0';

    while (linea[i] != '\0' && linea[i]!='*')
    {
        while( linea[i] == ' ' || linea[i]== ',' || linea[i]==9) // Obvea los espacios.
            i++;
        copiaPalabra(linea, aux_palabra, &i); // Cuando encuentra un caracter copia la palabra en la funcion.
        longetiqueta = strlen(aux_palabra);

        if( aux_palabra[longetiqueta-1] == ':')
        {
            etiqueta[0]= ' ';
            strcpy(&(etiqueta[1]), aux_palabra);

        }
        else
         if( aux_palabra[0]== 92)
        {
            while((linea[i]>='A'&& linea[i]<='Z') || linea[i]=='/' || linea[i]==' ') // Obvea los espacios.
                  i++;
            valor = atoi(&linea[i+1]);
            while(linea[i]!='\0')
                i++;
            memoria[0x0006]= valor;
            memoria[0x0004]= 4016 - valor;
            printf("\t\t\t  %s",linea);
        }
        else
        {
            if(k==0)
            {
               strcpy(inst.codop,aux_palabra); // si k==0 va el CODOP
               strupr(inst.codop);
            }
            else
                if(k==1)
                  strcpy(inst.op1,aux_palabra); // si k==1 va el OP1
                else
                    if(k==2)
                       strcpy(inst.op2,aux_palabra); // si k==2 va el OP2.
            k++;
        }
    }



    if(inst.codop[0]!='\0'&& aux_palabra[0]!= 92)
            validaCodop(inst, vec, vec_registros,memoria,actual,cont, etiqueta, vegeti);// Ya obtuve las 3 partes de la instrucción y empiezo a validar y traducir,

    if (linea[i]=='*') // Copia palabra salta si viene un comentario y este se copia en la sig función.
    {
        copiaComentario(linea, aux_palabra, &i);
        printf(" %s",aux_palabra);
    }


}

void copiaPalabra(char linea[], char aux_palabra[],int *i) // Devuelve la palabra obtenida sin comas ni comentarios.
{
    int  j;

    j=0;

    if(linea[*i]=='[')
        while (linea[*i-1] != ']')
        {
                aux_palabra[j]= linea[*i];
                (*i)++;
                j++;

        }
    else
    if(linea[*i]==39) // Si es comilla simple '
        for(j=0; j<3; j++)
        {
                aux_palabra[j]= linea[*i];
                (*i)++;
        }
    else
        while( linea[*i] != ' '  && linea[*i]!= '\0' && linea[*i]!= ',' && linea[*i]!='*' && linea[*i]!=9)//copia
        {
                aux_palabra[j]= linea[*i];
                (*i)++;
                j++;

        }

aux_palabra[j]='\0';


}

void copiaComentario(char linea[], char aux_palabra[], int *i)
{
    int j=0;
        while (linea[*i] != '\0')
        {
         aux_palabra[j]= linea[*i];
               (* i)++;
                j++;
        }
        aux_palabra[j]='\0';
}

void validaCodop(instruccionbase inst,reg vec[], reg vec_registros[],short int memoria[], int *actual,int *cont, char etiqueta[], regeti vegeti[]) // Verifica si es erróneo el nemónico.
{
    int primero = 0;
    int ultimo = (32) - 1;
    int medio = (primero + ultimo)/2;
    short int flag=0;
    short int inst_traducida[3];

    while(primero <= ultimo && flag!=1)
    {
        if (strcmp(vec[medio].instruccion, inst.codop)==-1)
            primero = medio + 1;
        else
            if(strcmp(vec[medio].instruccion, inst.codop)==1)
               ultimo = medio - 1;
            else
                flag=1;
        medio = (primero + ultimo)/2;
    }

    if(flag)
       traducir(inst,vec[medio].cod,inst_traducida,vec_registros, vegeti);

    guardaMemoria(inst_traducida,flag,memoria,actual,cont,inst, etiqueta);

}

short int buscaReg(reg vec_registros[], char op[])  // Busca el registro.
{
    int primero = 0;
    int ultimo = (16) - 1;
    int medio = (primero + ultimo)/2;
    int flag=0,aux;
    strupr(op);

    while(primero <= ultimo && flag!=1 )
    {
        if (strcmp(vec_registros[medio].instruccion, op)==-1)
            primero = medio + 1;
        else
            if(strcmp(vec_registros[medio].instruccion, op)==1)
               ultimo = medio - 1;
            else
               flag=1;

        medio = (primero + ultimo)/2;
    }
    if(flag)
       aux = vec_registros[medio].cod;
    else
        aux = -1;

    return aux;
}

short int analizaCorchetesMemoria(char op[]) // Devuelve posición de memoria.
{

    return (short int ) atoi(&op[1]);
}

int validaVInmediato (char op[])
{
    short int respuesta;

    switch(op[0])
    {
        case '%': respuesta = (int)strtol(&op[1], NULL, 16);
        break;
        case '#': respuesta = (int)strtol(&op[1], NULL, 10);
        break;
        case '@': respuesta = (int)strtol(&op[1], NULL, 8);
        break;
        case '-': // valor negativo.
        case '0'...'9': respuesta = (int)strtol(op, NULL, 10);
        break;
        default:
            respuesta=(int)op[1];
        break;

    }
    return respuesta;
}


void guardaMemoria(short int inst_traducida[],short int flag,short int memoria[], int *actual, int *cont, instruccionbase inst, char etiqueta[])
{

   if(!(strcmp(inst.op1,"EQU")==0))
   {
       if(flag)
        {
            memoria[*actual]=inst_traducida[0];
            *actual+=1;
            memoria[*actual]=inst_traducida[1];
            *actual+=1;
            memoria[*actual]=inst_traducida[2];
            *actual+=1;
        }
        else
        {
            memoria[*actual]=0;
            *actual+=1;
            memoria[*actual]=0;
            *actual+=1;
            memoria[*actual]=0;
            *actual+=1;

            memoria[0x0008]=-1;

        }
   }


   if(strcmp(inst.op1,"EQU")==0)
   {
       printf("\t\t\t   %s %s %s %s",etiqueta, inst.codop,inst.op1,inst.op2);
   }
   else
   {
       int aux=*actual-3;
       printf("[%4.4x] %4.4x %4.4x %4.4x",  aux-16, (unsigned short int)memoria[aux], (unsigned short int) memoria[aux+1], (unsigned short int) memoria[aux+2]);
       if(strcmp(inst.codop, "STOP")==0 || strcmp(inst.codop,"POP")==0 || strcmp(inst.codop,"PUSH")==0 || strcmp(inst.codop,"CALL")==0 || strcmp(inst.codop,"RET")==0  || strcmp(inst.codop,"JMP")==0 || strcmp(inst.codop,"SYS")==0 || strcmp(inst.codop,"NOT")==0  || strcmp(inst.codop,"JE")==0  || strcmp(inst.codop,"JZ")==0  || strcmp(inst.codop,"JNZ")==0  || strcmp(inst.codop,"JP")==0)
            printf(" %4d %s %s %s %s",*cont+1, etiqueta, inst.codop,inst.op1,inst.op2);
       else
            printf(" %4d %s %s %s, %s",*cont+1,etiqueta, inst.codop,inst.op1,inst.op2);
        if(flag==0 )
            printf(" \nError en la instruccion");
       (*cont)++;
   }
}

void traducir (instruccionbase inst,short int cod, short int inst_traducida[], reg vec_registros[], regeti vegeti[])
{
    int longitud=0;
    cod = cod << 8;
    inst_traducida[0]= cod;
    char auxupr[40];
    if(inst.op1[0]=='\0')
    {
        inst_traducida[1]=0;
        inst_traducida[2]=0;
    }
    else
        {
            strcpy(auxupr,inst.op1);
            strupr(auxupr);
            if(verifica_etiqueta(auxupr, vegeti) == 0)  // SI ENCUENTRA DEVUELVE 0 ERROR DE JUAN JE
            {
                inst_traducida[1] = busca_etiqueta(auxupr, vegeti);
            }
            else
            {
                    switch (inst.op1[0])
                   {

                        case '[':       if(!((inst.op1[1]>='a' && inst.op1[1]<='z') || (inst.op1[1]>='A' && inst.op1[1]<='Z')))
                                        {
                                            inst_traducida[1] = analizaCorchetesMemoria(inst.op1);
                                            inst_traducida[0]+=8 ; // le sumo 1000 en binario
                                        }
                                        else
                                        {
                                            if(inst.op1[3]=='+' || inst.op1[3]=='-' || inst.op1[3]==']')
                                            {
                                                inst_traducida[1] = analizaIndirecto(inst.op1, vec_registros, vegeti);
                                                inst_traducida[0]+= 12;
                                            }
                                            else
                                            {
                                                longitud= strlen(inst.op1);
                                                inst.op1[longitud-1] = '\0';
                                                if(verifica_etiqueta(&(inst.op1[1]), vegeti)==0)
                                                {
                                                    inst_traducida[1]= busca_etiqueta(&(inst.op1[1]), vegeti);
                                                    inst_traducida[0]+=8;
                                                }
                                                else
                                                {
                                                    printf("Error, etiqueta no encontrada");
                                                   // memoria[0x0008]=-1;
                                                }
                                            }

                                        }
                                        break;
                        case 'a'...'f':
                        case 's':
                        case 'S':
                        case 'A'...'F': inst_traducida[1]= buscaReg(vec_registros, inst.op1);
                                        inst_traducida[0]+=4;
                                        break;
                        default:        inst_traducida[1]=validaVInmediato (inst.op1);
                                        break;

                   }


            }

            if (inst.op2[0]=='\0')
            {
                inst_traducida[2]=0;
            }
            else
            {
                 if(verifica_etiqueta(inst.op2, vegeti)==0)
                    inst_traducida[2] = busca_etiqueta(inst.op2, vegeti);

                else
                {
                    switch (inst.op2[0])
                    {
                        case '[':   if(!((inst.op2[1]>='a' && inst.op2[1]<='z') || (inst.op2[1]>='A' && inst.op2[1]<='Z')))
                                    {
                                        inst_traducida[2] = analizaCorchetesMemoria(inst.op2);
                                        inst_traducida[0]+=2 ; // le sumo 1000 en binario
                                    }
                                    else
                                    {
                                        if(inst.op2[3]=='+' || inst.op2[3]=='-' || inst.op2[3]==']')
                                            {
                                                inst_traducida[2] = analizaIndirecto(inst.op2, vec_registros, vegeti);
                                                inst_traducida[0]+= 3;
                                            }
                                            else
                                            {
                                                longitud= strlen(inst.op2);
                                                inst.op2[longitud-1] = '\0';
                                                if(verifica_etiqueta(&(inst.op2[1]), vegeti)==0)
                                                {
                                                    inst_traducida[2]= busca_etiqueta(&(inst.op2[1]), vegeti);
                                                    inst_traducida[0]+=2;
                                                }
                                                else
                                                {
                                                    printf("Error, etiqueta no encontrada");
                                                   // memoria[0x0008]=-1;
                                                }
                                            }

                                    }
                                    break;
                                        break;
                        case 'a'...'f':
                        case 's':
                        case 'S':
                        case 'A'...'F': inst_traducida[2]= buscaReg(vec_registros, inst.op2);
                                        inst_traducida[0]+=1;
                                        break;
                        default:        inst_traducida[2]=validaVInmediato (inst.op2);
                                        break;
                    }
                }
            }


        }
}

short int analizaIndirecto(char op[], reg vec_registros[], regeti vegeti[])
{
    char auxReg[2];
    char auxOffset[6];
    short int offset=0;
    short int valorReg;
    short int longitud = 0;
    short int respuesta= 0;
    int i=0,j;

    auxReg[0]= op[1];   // copiamos registro;
    auxReg[1] = op[2];
    auxReg[2] = '\0';

    valorReg= buscaReg(vec_registros, auxReg);

    if(valorReg==-1 ) //Es una etiqueta
    {
        longitud= strlen(op);
        op[longitud-1] = '\0';
        if(verifica_etiqueta(&(op[1]), vegeti)==0)
            respuesta = busca_etiqueta(&(op[1]), vegeti);
    }
    else
    {

        i=3; j=0;
        while( op[i] != ']')
        {
            auxOffset[j]= op[i];
            i++; j++;
        }
        auxOffset[j] = '\0';

        if(verifica_etiqueta(&(auxOffset[1]), vegeti)==0)// encontre el offset como etiqueta
        {
            offset= (short int) busca_etiqueta(&(auxOffset[1]), vegeti);
            if(auxOffset[0] == '-')
                offset*= -1;
        }
        else
            offset = (short int)atoi(auxOffset);

        offset = offset << 4;

        respuesta= (offset | valorReg);

    }

    return respuesta;

}

void guardaArchivoImagen(short int memoria[], char *imagenIMG)
{
    FILE *arch;
    arch=fopen(imagenIMG,"wb");
    fwrite(memoria, sizeof(short int), 4016, arch);
    fclose(arch);

}

void genera_tablaEtiquetas(regeti vec_eti[]  ,int n , char *fuenteASM,reg vec[], reg vec_registros[], short int memoria[], int *flag_etiqueta){

    FILE *arch;
    char linea[100];
    int actual=16;
    int cont=1;
    int j=0;

    arch=fopen(fuenteASM,"rt");

    if (arch!=NULL)
    {
        fscanf(arch, "%[^\n] \n", linea);
        analiza_etiquieta(linea, vec, vec_registros,memoria,&actual,&cont,vec_eti, flag_etiqueta,&j);


        while(!feof(arch) && *flag_etiqueta)
        {
           fscanf(arch, "%[^\n] \n", linea);
           analiza_etiquieta(linea, vec, vec_registros,memoria,&actual,&cont,vec_eti, flag_etiqueta,&j);
        }
    }
    fclose(arch);

}

void analiza_etiquieta(char linea[], reg vec[], reg vec_registros[],short int memoria[], int *actual, int *cont, regeti vec_eti[], int *flag_etiqueta, int *j)
{
    int i=0,k=0;
    char aux_palabra[100];
    instruccionbase inst;
    int longetiqueta=0;


    inst.codop[0] = '\0';             // Inicializamos el registro instruccion
    inst.op1[0] = '\0';
    inst.op2[0] = '\0';


    while (linea[i] != '\0' && linea[i]!='*')
    {
        while( linea[i] == ' ' || linea[i]== ',' || linea[i]==9) // Obvea los espacios.
            i++;
        copiaPalabra(linea, aux_palabra, &i); // Cuando encuentra un caracter copia la palabra en la funcion.
        longetiqueta = strlen(aux_palabra);

        if( aux_palabra[0]==92)
            *(cont) -=1;

        else

        if( aux_palabra[longetiqueta-1] == ':')
        {
            strupr(aux_palabra);
            aux_palabra[longetiqueta-1] = '\0';
            if(verifica_etiqueta(aux_palabra, vec_eti))
            {


                strcpy(vec_eti[*j].instruccion,aux_palabra);
                vec_eti[*j].cod=*cont;
                (*j)+=1;
            }
            else
            {
                printf("Error, %s es etiqueta duplicada\n", aux_palabra);
                (*flag_etiqueta) = 0 ;   // para que no ejecute
            }
        }

        else
        {
            if(k==0)
            {
               strcpy(inst.codop,aux_palabra); // si k==0 va el CODOP
               strupr(inst.codop);
               (*cont)+=1;
            }
            else
                if(k==1)
                  strcpy(inst.op1,aux_palabra);
                else
                    if(k==2)
                    {

                       strcpy(inst.op2,aux_palabra);// si k==2 va el OP2.

                       if(strcmp(inst.op1, "EQU")==0)
                       {

                            (*cont)-=1;
                            strupr(inst.codop);
                            if(verifica_etiqueta(inst.codop, vec_eti))
                            {

                                strcpy(vec_eti[*j].instruccion, inst.codop);
                                vec_eti[*j].cod= validaVInmediato(inst.op2);
                                (*j)+=1;
                            }
                            else
                            {
                                printf("Error, etiqueta de constante duplicada\n");
                                (*flag_etiqueta) = 0 ;   // para que no ejecute
                            }
                       }
                    }
            k++;

        }
    }

    if (linea[i]=='*') // Copia palabra salta si viene un comentario y este se copia en la sig función.
    {
        copiaComentario(linea, aux_palabra, &i);
    }


}



int verifica_etiqueta(char aux_palabra[],regeti vec_eti[] )
{
    int i=0, aux=1;

    while( aux ==1 && i< 50)
    {
        if(strcmp(aux_palabra, vec_eti[i].instruccion)==0)
             aux=0;
         i++;
    }

    return aux;
}

int busca_etiqueta(char aux_palabra[],regeti vec_eti[] )
{
    int i=0, aux=1;

    while( aux ==1 && i< 50)
    {
        if(strcmp(aux_palabra, vec_eti[i].instruccion)==0)
             aux=0;
         i++;
    }

    return vec_eti[i-1].cod;
}

void inicia_vec_eti(regeti veceti[])
{
    int i;

    for(i=0; i< 50; i++)
    {
        veceti[i].cod=0;
        strcpy(veceti[i].instruccion, "\0");
    }
}
