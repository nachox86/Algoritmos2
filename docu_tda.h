#ifndef DOCU_TDA_H_INCLUDED
#define DOCU_TDA_H_INCLUDED

#define RES_OK      0
#define RES_ERROR   1

typedef struct {
    /* TODO: Define TDA */
} TDA_Doc;

/*

    PRE:    Documentador no creado
    POST:   Devuelve RES_OK si pudo crear el documentador, y lo registra en el log en modo INFO.
            Devuelve RES_ERROR en otro caso, o la constante que se defina pertinentemente y lo registra en el log en modo ERROR.
    PARAMS: docu: Documentador a usar.
            log:  Logger a usar.
*/

int createDoc(TDA_Doc *docu, Logger log);


/*
    PRE:    Documentador creado e inicializado.
    POST:   Devuelve RES_OK si pudo extraer la información a documentar del archivo_entrada, escribiéndola en el archivo_salida y lo registra en el log.
            Devuelve RES_ERROR en otro caso, o la constante que se defina pertinentemente y lo registra en el log en modo ERROR.
    PARAMS: docu:       Documentador a usar.
            inputFile:  Archivo del cual extraer la documentacion.
            outputFile: Archivo al cual guardar la documentacion en el formato preestablecido.
*/

int extractDocumentation(TDA_Doc *docu, char *inputFile, char *outputFile);

/*
    PRE:    Documentador creado e inicializado.
    POST:   El archivo de salida “arch_indice” contiene el índice construido en formato html.
            Devuelve RES_OK en caso de que todo haya funcionado correctamente.
            Decuelve RES_ERROR en otro caso.
    PARAMS: docu:       Documentador a usar.
            indexFile:  Archivo de salida para el inidice generado.
*/

int createIndex(TDA_Doc *docu, char *indexFile);

/*
    PRE:    Documentador creado en inicializado.
    POST:   Devuelve RES_ERROR en otro caso, o la constante que se defina pertinentemente y lo registra en el log en modo ERROR.
            En ambos casos registra la actividad en el log según corresponda
    PARAMS: docu: Documentador a usar.
*/

int destroyDoc(TDA_Doc *docu);


#endif // DOCU_TDA_H_INCLUDED
