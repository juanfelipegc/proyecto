#define _CRT_SECURE_NO_DEPRECATE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



// NOMBRES Y CODIGOS DE LOS INTEGRANTES
//Gregorio Ospina 201631760
//Sergio Naranjo 201631670
//Juan Felipe Garcia 201531352



//-- Constantes
#define TAMANIO_MAX 10000    //Tamanio maximo del archivo
#define NUMERO_CODIGOS 26      //Maximo de valores distintos en el archivo

//-- Definicion de la estructura para el archivo de entrada y el comprimido
typedef struct archivo
{
	int tamanio;
	unsigned char *informacion;
} Archivo;

//-- Arreglos que contendran el codigo de compresion
// arreglo que contiene el codigo. Cada byte contiene un codigo de compresion

unsigned char codigoCompresion[NUMERO_CODIGOS];
// arreglo con la longitud. Cada byte tiene la longitud en bits del codigo de compresion correspondiente
// en el arreglo de arriba
int longitudCodigo[NUMERO_CODIGOS];



//-- Prototipos de las funciones
int readFile(Archivo * archivo, char *);
int readFileCode(unsigned char[], char *);
void writeFile(int, Archivo * archivoCodificado, char *);
int codificar(Archivo * archivo, Archivo * archivoCodificado);
void agregarAlArreglo(unsigned char[], unsigned char, int, int, int);
void uploadCode(unsigned char[], char *);
char printCodigo(char letra);
char getCodigo(char letra);
int getLong(char letra);


//-- Funciones

// Esta funcion se encarga de abrir un archivo y leerlo en el vector datos.
// Retorna el n�mero de bytes leidos.
// No hay que completar nada en esta funcion.
// NO MODIFICAR
int readFile(Archivo * archivo, char * nombreArchivo)
{
	FILE *file;
	int n;
	int x;

	if (!(file = fopen(nombreArchivo, "rb"))) {
		printf("No se puede abrir el archivo: %s\n", nombreArchivo);
		exit(EXIT_FAILURE);
	}
	fseek(file, 0, SEEK_END);
	n = ftell(file);
	fseek(file, 0, SEEK_SET);

	archivo->tamanio = n;
	archivo->informacion = (unsigned char *)calloc(n, sizeof(unsigned char));

	for (x = 0; x < n; x++) {
		fread(&archivo->informacion[x], sizeof(unsigned char), 1, file);
	}

	fclose(file);

	return n;
}

// Esta funcion se encarga de abrir un archivo de codigo
// No hay que completar nada en esta funcion.
// NO MODIFICAR
int readFileCode(unsigned char datos[], char *nombreArchivo)
{
	FILE *file;
	int n;

	if (!(file = fopen(nombreArchivo, "rb"))) {
		printf("No se puede abrir el archivo: %s\n", nombreArchivo);
		exit(EXIT_FAILURE);
	}
	fseek(file, 0, SEEK_END);
	n = ftell(file);
	fseek(file, 0, SEEK_SET);
	if (n > TAMANIO_MAX) {
		printf("Archivo demasiado grande: %s\n", nombreArchivo);
		exit(EXIT_FAILURE);
	}

	fread(datos, 1, n, file);

	fclose(file);

	return n;
}

// Esta funcion se encarga de abrir el archivo que contiene el codigo de compresion.
// Hace uso de la funcion readFile
// Guarda el codigo de compresion en el arreglo llamado codigo
// No hay que completar nada en esta funcion.
// NO MODIFICAR
void uploadCode(unsigned char codigo[], char *nombreArchivoCodigo)
{
	unsigned char archivoCodigo[TAMANIO_MAX];
	//en la primera mitad guardamos el codigo de compresion, en la segunda mitad, en cada byte,
	//guardamos la lognitud en bits de cada traduccion

	int tamanioCodigoTotal = readFileCode(archivoCodigo, nombreArchivoCodigo); // esto deberia ser 52
	int tamanioCodigo = tamanioCodigoTotal / 2; // deberia ser 26
	int i;

	printf("El tamanio del codigo es %d \n", tamanioCodigoTotal);

	//guardamos el codigo y su longitud en los arreglos de arriba
	for (i = 0; i < tamanioCodigo; i++) {
		codigo[i] = archivoCodigo[i];
	}

	for (i = tamanioCodigo; i < tamanioCodigoTotal; i++) {
		longitudCodigo[i - tamanioCodigo] = (int)archivoCodigo[i];
	}
}

/*
// Esta funcion se encarga de escribir un archivo a partir del vector datos.
// El numero de bytes que se deben escribir viene en el parametro n.
// No hay que completar nada en esta funcion.
// NO MODIFICAR
void writeFile(int n, Archivo * archivoCodificado, char *nombreArchivo)
{
FILE *file;
char datos[] = "";
char filter = (char) 127;
int i;

strcpy(datos, archivoCodificado->informacion);
for( i = 0; i < n; i++){
datos[i] = datos[i] & filter;
}

if (!(file = fopen(nombreArchivo, "wb"))) {
printf("No se puede abrir el archivo: %s\n", nombreArchivo);
exit(EXIT_FAILURE);
}

fwrite(datos, 1, n, file);

fclose(file);
}
*/

// Esta funcion se encarga de escribir un archivo a partir del vector datos.

// El numero de bytes que se deben escribir viene en el parametro n.

// No hay que completar nada en esta funcion.

void writeFile(int n, Archivo * archivoCodificado, char *nombreArchivo)

{

	FILE *file;

	if (!(file = fopen(nombreArchivo, "wb")))

	{

		printf("No se puede abrir el archivo: %s\n", nombreArchivo);

		exit(EXIT_FAILURE);

	}

	fwrite((archivoCodificado->informacion), 1, n, file);
	fclose(file);

}

// Esta funcion se encarga de codificar cada uno de los valores que se encuentran en
// la estructura llamada archivo y asignarlos a la estructura llamada archivocodificado.
// DESARROLLAR ESTA FUNCION EN SU TOTALIDAD.
int codificar(Archivo * archivo, Archivo * archivoCodificado)
{
	// Representa la posicion del bit donde se debe poner el sguiente codigo.
	int posicion = 0;
	// Cuantos codigos se han procesado.
	int cantidadDeCodigos = 0; 


	while (cantidadDeCodigos < archivo->tamanio) {
		char agregar = getCodigo(*archivo->informacion); //el char con el codigo respectivo a la letra actual
		int longitud = getLong(*archivo->informacion); // la longitud del cdigo respectivo a la letra actual.

		//llama al metodo encargado de agregar la infomracion.
		agregarAlArreglo(archivoCodificado->informacion, agregar, longitud, posicion, archivoCodificado->tamanio + posicion);
								
		//Se incrementa la posicion de busqueda
		archivo->informacion++;
		cantidadDeCodigos++;
		posicion = posicion + longitud;

	}

	// retorna la posicion a codificar.
	return posicion ;
}


//Encuentra el codigo de la letra respectiva usando el sistema ASCII donde 'A' es 0 y 'Z' es 90
char getCodigo(char letra) {
	int respu = (int)letra - 65;
	return codigoCompresion[respu];
}

//Encuentra y retorna la longitud del codigo de la letra asociada
int getLong(char letra) {
	int respu = (int)letra - 65;
	return longitudCodigo[respu];
}
// Esta funcion recibe como parametros el vector de datos codificados,
// el codigo que se debe insertar, la longitud de este ultimo y la posicion donde
// debe insertar el codigo en el vector codificado (posicionBit).
// Se encarga de convertir los datos de codigo a bits e insertarlos en la posicion
// que les corresponde en datosCodificados. Longitud es la longitud de cada código, posicionBit es la posición del bit en el que se va
//a insertar, y nuevoTamanio es el tamanio del archivo comprimido.
// DESARROLLAR ESTA FUNCION EN SU TOTALIDAD.
void agregarAlArreglo(unsigned char datosCodificados[], unsigned char codigo, int longitud, int posicionBit, int nuevoTamanio)
{
	//Se calcula cuantos bytes ha codificado el programa. la funcion de floor() encuentra el enero mas grande que ssea igual o mas pequenho que x (redondea hacia abajo a x).
	int byteActual = (int)floor(posicionBit / 8); 
	//El % 8 lo que hace es dar el bit en el que se esta codificando actualmente
	int bitActual = (posicionBit % 8); 
	// Caso donde si quepa en el byte actual
	if (bitActual + longitud <= 8) {
		//Crea el char Shifted, el cual contiene la informacion del codigo a agregar
		int shiftq = longitud + bitActual;
		unsigned char codigoTemporal = codigo << shiftq;
		printf("%c",codigoTemporal);
		// La opreacion OR lo que hace es coger el dato codificado y el char shifted y le agrega a datosCodificados la secuencia binaria de codigoTemporal.
		datosCodificados[byteActual] = datosCodificados[byteActual] | codigoTemporal;

	}

	// Esto es para el caso donde el codigo enviado por parametro no cabe dentro de el byte actual.
	else {


		// Es necesario dividir el codigo de entrada en dos, esto se hace al correr a la derecha de un codigo temporal solo
		//la cantidad de bits que le falta al byte y usar la funcion OR con datos Codificados.
		int shift = 8 - bitActual;
		int stemp = shift - longitud;
		unsigned char codigoTemporal = codigo >> stemp;
		datosCodificados[byteActual] = datosCodificados[byteActual] | codigoTemporal;


		//Se busca cuanto se debe shift el codigo para alocar los bits que faltan en datos codificados y se aplica el mismo proceso anterior
		int shift2 = 8 - shift;
		unsigned char codigoTemporal2 = codigo << shift2;
		printf("%c",codigoTemporal2);
		datosCodificados[byteActual + 1] = datosCodificados[byteActual + 1] | codigoTemporal2;
	}


}

//-- Funcion main de la aplicacion
// No hay que completar nada en esta funcion.
// NO MODIFICAR

int main()
{
	int tamanio;
	int tamanioCodificado;
	unsigned char nombreArchivo[] = "";
	unsigned char nombreCodigo[] = "";
	unsigned char nombreCodificado[] = "";
	Archivo * archivo = (Archivo *)malloc(sizeof(Archivo));
	Archivo * archivoCodificado = (Archivo *)malloc(sizeof(Archivo));

	printf("Ingrese el nombre del archivo a comprimir (incluya el .txt): \n");
	scanf("%s", &nombreArchivo);
	tamanio = readFile(archivo, nombreArchivo);
	printf("El tamanio del archivo es %d \n", tamanio);
	printf("Ingrese el nombre del archivo que contiene el codigo (sin la extension de archivo): \n");
	scanf("%s", &nombreCodigo);
	uploadCode(codigoCompresion, nombreCodigo);
	archivoCodificado->informacion = (unsigned char *)calloc(tamanio, sizeof(unsigned char));
	printf("Ingrese el nombre del archivo para guardar el archivo codificado (incluya el .txt): \n");
	scanf("%s", &nombreCodificado);
	tamanioCodificado = codificar(archivo, archivoCodificado);
	writeFile((tamanioCodificado + 7) / 8, archivoCodificado, nombreCodificado);

	system("PAUSE");
}

