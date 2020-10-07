#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "Ejecucion.h"
#include "Traduccion.h"

//Grupo 5


int main(int argc ,char *argv[])
{
    short int memoria[4016]={0};
    int n=32;
    reg vec[32], vec_registros[16];
    regeti veceti[50];
    char *fuenteASM;
    char *imagenIMG;
    char *archivoDAT;
    char *archivoTXT;
    char *auxpal;
    char * formatoLectura ;
    formatoLectura=(char*)malloc(sizeof(char)*20);
    formatoLectura="-a";
    char extension[10];
    int flag_etiqueta=1;
    int i=1,j=0,k=0;
    //Setea el valor de la pila.
    memoria[0x0006]=200;
    memoria[0x0004]= 4016 - memoria[0x0006];
    memoria[0x0008]=16;
    inicia_vec_eti(veceti);

    genera_tabla(vec,n);                      // Genera MOV cod , ADN cod,..., ETC.
    genera_tablaRegistros(vec_registros,15); // Genera AX cod,BX cod,..., ETC.

    argc=5;
    argv[5]= "-a";
    argv[1]= "i7.img";
    argv[2]= "t2 - copia.asm";
    argv[3]= "input.dat";
    argv[4]= "salida.txt";

    extension[0]='\0';
    for (i=1; i<argc; i++)
    {
         j=0;
         k=0;
         auxpal=argv[i];

        while(auxpal[j]!='.' && auxpal[j]!='\0')
            j++;

        while(auxpal[j]!='\0')
        {
            j++;
            extension[k]=auxpal[j];
            k++;
        }

        if(strcmp(extension,"txt")==0)
            archivoTXT=argv[i];
        else
           if(strcmp(extension,"dat")==0)
               archivoDAT=argv[i];
            else
                if(strcmp(extension,"asm")==0)
                    fuenteASM=argv[i];
                else
                    if(strcmp(extension,"img")==0)
                        imagenIMG=argv[i];
                    else
                        formatoLectura = argv[i];
    extension[0]='\0';
    }

    if(fuenteASM==NULL){
        printf("Ingrese el nombre del asm para compilar: \n");
        fuenteASM=(char*)malloc(sizeof(char)*20);
        gets(fuenteASM);
        printf("----%s-------",fuenteASM);
    }


    if( strcmp(formatoLectura, "-T")==0 || strcmp(formatoLectura, "-t")==0 )
    {
        genera_tablaEtiquetas(veceti,50,fuenteASM,vec, vec_registros,memoria, &flag_etiqueta);
        leeArchivoImg(vec, vec_registros,memoria, fuenteASM, imagenIMG, veceti);   // Lee linea a linea el archivo imagen para traducir.
    }
    else
        if(strcmp(formatoLectura, "-X")==0 || strcmp(formatoLectura, "-x")==0)
            ejecucion(memoria, imagenIMG, archivoDAT, archivoTXT);
        else
            {
               genera_tablaEtiquetas(veceti,50,fuenteASM,vec, vec_registros,memoria, &flag_etiqueta);
               if(flag_etiqueta)
                {
                    leeArchivoImg(vec, vec_registros,memoria, fuenteASM, imagenIMG, veceti);
                    ejecucion(memoria,imagenIMG,archivoDAT, archivoTXT);
                }
            }

return 0;
}
