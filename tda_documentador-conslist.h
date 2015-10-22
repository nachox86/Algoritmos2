#ifndef DOCU_TDA_H_INCLUDED
#define DOCU_TDA_H_INCLUDED

#include "logger.h"
#include "list_tda.h"
#include "straight_list.h"

#define RES_OK      0
#define RES_ERROR   1

typedef struct {
    char *inputFile;
    char *outputFile;
    Logger *logFile;
    straight_list_t* listado;
    straight_list_t* indice;
} TDA_Doc;

    /*
    @funcion createDoc
    @descr Esta es una primitiva del TDA_Doc que crea el documentador.
    @autor Ignacio
    @fecha 02/10/2015
    @version "1.0"
    @param *docu documentador a ser creado
    @param Log objeto que permite el logueo
    @pre ninguna condición necesaria
    @pos devolverá el código de éxito si pudo crear y solicitar los recursos necesarios para el documentador, en caso contrario devolverá el código de error.
    */
    int createDoc(TDA_Doc **docu, Logger *log);

    /*
    @funcion extractDocumentation
    @descr Esta es una primitiva del TDA_Doc que toma los comentarios del archivo de texto de entrada y los vuelca en formato html en el archivo de salida.
    @autor Ignacio
    @fecha 02/10/2015
    @version "1.0"
    @param *docu es el documentador.
    @param *inputFile es el archivo de entrada del que se toman los comentarios.
    @param *outputFile es el archivo de salida donde estará toda la información en formato html.
    @pre el documentador debe estar creado e inicializado
    @pos Devuelve RES_OK si pudo extraer la información a documentar del archivo_entrada, escribiéndola en el archivo_salida y lo registra en el log.
         Devuelve RES_ERROR en otro caso, o la constante que se defina pertinentemente y lo registra en el log en modo ERROR.
    */
    int extractDocumentation(TDA_Doc *docu, char *inputFile, char *outputFile);

    /*
    @funcion createIndex
    @descr Esta es una primitiva del TDA_Doc que toma la información guardada en la estructura del documentador, y vuelca en una archivo en formato html un índice de las funciones existentes del que fue el archivo de entrada.
    @autor Ignacio
    @fecha 02/10/2015
    @version "1.0"
    @param *docu es el documentador.
    @param *indexFile es el archivo de índices.
    @pre Documentador creado e inicializado.
    @pos El archivo de salida “arch_indice” contiene el índice construido en formato html.
         Devuelve RES_OK en caso de que todo haya funcionado correctamente.
         Decuelve RES_ERROR en otro caso.
    */
    int createIndex(TDA_Doc *docu, char *indexFile);

    /*
    @funcion destroyDoc
    @descr Esta es una primitiva del TDA_Doc que destruye el documentador credo anteriormente, devolviendo los recursos utilizados al sistema.
    @autor Ignacio
    @fecha 02/10/2015
    @version "1.0"
    @param *docu es el documentador.
    @pre Documentador creado e inicializado.
    @pos Devuelve RES_ERROR en otro caso, o la constante que se defina pertinentemente y lo registra en el log en modo ERROR.
         En ambos casos registra la actividad en el log según corresponda
    */
    int destroyDoc(TDA_Doc **docu);
#endif
