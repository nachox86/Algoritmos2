#include "tda_documentador.h"
#include "logger.h"
#include "list_tda.h"
#include "tda_nodo.h"
#include "htmlParser.h"
#include "functions_tools.c"
#define MAX_LINE 300

#define MAX_KW 11
#define KW_INIT        "/*"
#define KW_END         "*/"
#define KW_TITLE       "@titulo"
#define KW_SUBTITLE    "@subtitulo"
#define KW_FUNCTION    "@funcion"
#define KW_DESCRIPTION "@descr"
#define KW_AUTHOR      "@autor"
#define KW_DATE        "@fecha"
#define KW_VERSION     "@version"
#define KW_PARAM       "@param"
#define KW_RETURN      "@return"
#define KW_PRE         "@pre"
#define KW_POST        "@pos"

int createDoc(TDA_Doc *docu, Logger* log)
{
    docu->inputFile = NULL;
    docu->outputFile = NULL;
    docu->logFile = log;
    docu->listado = NULL;
    return RES_OK;
}

int extractDocumentation(TDA_Doc *docu, char *iFile, char *oFile)
{
    FILE* inputFile;
    int commentsInit = 0;
    int commentsEnd = 0;
    int commentsFound = 0; /* 0 significa que no encontró, 1 que si*/
    int count = 0;
    int j;
    int lenElem = 0;
    char **comms;
    htmlFile** outPut;


    inputFile = fopen(iFile,"r");
    if(inputFile)
    {
        loge(docu->logFile, "Ocurrió un error al abrir el archivo de entrada de datos.");
        return RES_ERROR;
    }

    /* asigno recursos para la variable auxiliar */
    comms = (char **) malloc(sizeof(char*)*MAX_KW);

    /*El archivo se abrió correctamente por lo que ahora paso a recopilar la información*/
    /*Recorro línea por línea*/

    while(!feof(inputFile))
    {
        char linea[MAX_LINE];
        char *token;
        char *token2;

        if(fgets(linea,MAX_LINE,inputFile)!=NULL)
        {
            token = strstr(linea,KW_INIT);
            token2 = strstr(linea,KW_END);
            if(token && !token2)
            {
                /*Encontramos el inicio de los comentarios de la función*/
                commentsInit = 1;
            }
            else if(!token && token2)
            {
                commentsEnd = 1;
                if(commentsInit == 1)
                    commentsFound = 1;
            }
                /* else es un comentario trivial */

            if(commentsFound==1)
            /* a esta altura ya se cerraron los comentarios para una función y los guardé en comms */
            {
                if(count==0)
                {
                    free(comms);
                }
                else
                {
                    /*inserto los comentarios en la lista*/
                    if(EmptyList(*(docu->listado)))
                    {
                        /* está vacía la lista, entonces debe ser el primero*/
                        InsertE(docu->listado,M_First,comms);
                    }
                    else
                    {
                        InsertE(docu->listado,M_Next,comms);
                    }

                    /* reinicializo las variabls auxiliares locales */
                    /* libero los recursos de comms */
                    for(j=count;j>=0;--j)
                    {
                        free(comms[j]);
                    }
                    free(comms);
                }
                commentsInit = 0;
                commentsEnd = 0;
                commentsFound = 0;
                count = 0;
                /* asigno recursos para la variable auxiliar */
                comms = (char **) malloc(sizeof(char*)*MAX_KW);
            }
            else if(commentsInit==1 && commentsEnd==0 && checkForKW(linea)==0)
            {
                /*encontró un KW*/
                /*lo tengo que agregar a comms*/

                comms[count] = (char *) malloc(size_t(char)*strlen(linea)+1);
                if(!comms[count])
                {
                    loge(docu->logFile,"Error al pedir recursos para la matriz de comentarios.");
                    /*tengo que liberar los recursos*/
                    if(count==0)
                    {
                        free(comms);
                    }
                    else
                    {
                        for(j=count;j>=0;--j)
                        {
                            free(comms[j]);
                        }
                        free(comms);
                    }
                    return RES_MEM_ERROR;
                }
                comms[count] = linea;
                count++;
            }
        }

    }

    free(comms);
    fclose(inputFile);

    /*Ahora lo pasamos al archivo en formato HTML*/
    /*vamos a recorrer la lista, nodo por nodo, tomando cada uno de los comentarios e insertándolos en el archivo de salida*/

    /*creo la lista de nodos; los nodos son los comentarios de cada función*/
    CreateList(docu.listado,sizeof(char**));

    /*Muevo al primero el corriente*/
    MoveC(docu.listado,M_First);

    /*Creo el archivo de salida*/
    createHtmlFile(outPut,oFile);

    do
    {
        ChangeC(docu.listado,comms);
        lenElem = strlen(comms);
        for(i=0;i<lenElem;i++)
        {
            if(parseStringToHtml(outPut,comms[i])==RES_ERROR)
            {
                loge(docu->logFile,"Error al guardar en el archivo de salida.");
                return RES_ERROR;
            }
        }
    }while(MoveC(docu.listado,M_Next)!=FALSE)
        closeHtmlFile(outPut);

    ClearList(docu->listado);

    return RES_OK;
}


int createIndex(TDA_Doc *docu, char *indexFile)
{

}

int destroyDoc(TDA_Doc *docu)
{
  docu->inputFile = NULL;
  docu->outputFile = NULL;
  docu->logFile = NULL;
  if(ClearList(listado)!=RES_OK)
  {
      return RES_ERROR;
  }
  return RES_OK;
}
