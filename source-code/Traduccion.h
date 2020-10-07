typedef struct reg
{
    char instruccion[7];
    short int cod;
} reg;

typedef struct regeti
{
    char instruccion[10];
    short int cod ;
} regeti;

typedef struct instruccionbase
{
    char codop[20];
    char op1[20];
    char op2[20];
}instruccionbase;

void leeArchivoImg(reg vec[], reg vec_registros[], short int memoria[], char *fuenteASM,char *imagenIMG, regeti vegeti[]);

void copiaPalabra(char linea[], char aux_palabra[],int *i);
void genera_tabla(reg vec[], int n);
void genera_tablaRegistros(reg vec[], int n);
void genera_tablaEtiquetas(regeti vec_eti[]  ,int n,char *fuenteASM,reg vec[], reg vec_registros[], short int memoria[],  int *flag_etiqueta);
void copiaComentario(char linea[], char aux_palabra[], int *i);

void analiza_etiquieta(char linea[], reg vec[], reg vec_registros[],short int memoria[], int *actual, int *cont,regeti vec_eti[],  int *flag_etiqueta, int *j);
int verifica_etiqueta(char aux_palabra[],regeti vec_eti[] );
void analiza(char linea[], reg vec[], reg vec_registros[],short int memoria[], int *actual,int *cont, regeti vegeti[]);
void validaCodop(instruccionbase inst,reg vec[], reg vec_registros[],short int memoria[], int *actual,int *cont, char etiqueta[],regeti vegeti[]);
int busca_etiqueta(char aux_palabra[],regeti vec_eti[] );
short int buscaReg(reg vec_registros[], char op[]);
short int analizaCorchetesMemoria(char op[]);
int validaVInmediato (char op[]);
void guardaMemoria(short int inst_traducida[],short int flag,short int memoria[], int *actual,int *cont,instruccionbase inst, char etiqueta[]);
void traducir (instruccionbase inst,short int cod, short int inst_traducido[], reg vec_registros[], regeti vegeti[]);
void guardaArchivoImagen(short int memoria[],char *imagenIMG);
short int analizaIndirecto(char op[],reg vec_registros[], regeti vegeti[]);
void inicia_vec_eti(regeti veceti[]);
