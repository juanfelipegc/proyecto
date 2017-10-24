#define _CRT_SECURE_NO_DEPRECATE 
#include <stdlib.h>
#include <stdio.h>
#include <string.h>



// NOMBRES Y CODIGOS DE LOS INTEGRANTES
Gregorio Ospina codigo
Sergio Naranjo 201631670
Juan Felipe Garcia 201531352



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


//-- Funciones

// Esta funcion se encarga de abrir un archivo y leerlo en el vector datos.
// Retorna el n�mero de bytes leidos.
// No hay que completar nada en esta funcion.
// NO MODIFICAR
int readFile( Archivo * archivo, char * nombreArchivo )
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
	
	archivo -> tamanio = n;	
	archivo -> informacion = (unsigned char *) calloc (n, sizeof (unsigned char));

	for ( x = 0; x < n; x++) {
			fread (&archivo -> informacion [x], sizeof(unsigned char ), 1, file);
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
	for ( i = 0; i < tamanioCodigo; i++) {
		codigo[i]   = archivoCodigo[i];
	}

	for ( i = tamanioCodigo; i < tamanioCodigoTotal; i++) {
		longitudCodigo[i - tamanioCodigo] = (int)archivoCodigo[i];
	}
}


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


// Esta funcion se encarga de codificar cada uno de los valores que se encuentran en
// la estructura llamada archivo y asignarlos a la estructura llamada archivocodificado.
// DESARROLLAR ESTA FUNCION EN SU TOTALIDAD.
int codificar(Archivo * archivo, Archivo * archivocodificado)
{
	
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
	Archivo * archivo = (Archivo *) malloc (sizeof (Archivo));
	Archivo * archivoCodificado = (Archivo *) malloc (sizeof (Archivo));

	printf("Ingrese el nombre del archivo a comprimir (incluya el .txt): \n");
	scanf("%s", &nombreArchivo);
	tamanio = readFile(archivo, nombreArchivo);
	printf("El tamanio del archivo es %d \n", tamanio);
	printf("Ingrese el nombre del archivo que contiene el codigo (sin la extension de archivo): \n");
	scanf("%s", &nombreCodigo);
	uploadCode(codigoCompresion, nombreCodigo);
	archivoCodificado -> informacion = (unsigned char *) calloc (tamanio, sizeof(unsigned char));
	printf("Ingrese el nombre del archivo para guardar el archivo codificado (incluya el .txt): \n");
	scanf("%s", &nombreCodificado);
	tamanioCodificado = codificar(archivo, archivoCodificado);
	writeFile((tamanioCodificado + 7) / 8, archivoCodificado, nombreCodificado);

	system("PAUSE");
}
