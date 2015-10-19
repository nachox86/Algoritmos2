#include "list_tda.h"
#include "tda_nodo.h"
#include "tda_documentador.h"
#include <string.h>
#include "functions_tools.h"

/*
@funcion showHelp
@descr Muestra la ayuda del programa.
@autor Ignacio
@fecha 03/10/2015
@version "1.0"
@param ninguno
@pre ninguna condici�n necesaria
@pos devuelve en pantalla la ayuda del programa.
*/
void showHelp()
{
    printf("Este programa permite obtener en formato html, la documentaci�n provista de un archivo de texto.\n");
    printf("Uso:\n");
    printf("Documentador.exe -i archivo_a_documentar.c -l archivo_log.txt -o archivo_documentado.html\n");
    printf("Documentador.exe -h\n");
    printf("Documentador.exe --help\n");
    printf("Descripcion de los par�metros:\n");
    printf("-i: indica el nombre del archivo de entrada del que se toma la documentaci�n.\n");
    printf("-l: indica el nombre del archivo de loggeo de eventos.\n");
    printf("-o: indica el nombre del archivo de salida de la documentaci�n en formato html.\n");
    printf("-h o --help: muestra esta ayuda.\n");
}

/*
@funcion checkForKW
@descr Dado un string de caracteres se fija si se encuentra alguna de las palabras claves definidas.
@autor Ignacio
@fecha 01/10/2015
@version "1.0"
@param linea array de caracteres
@pre ninguna condici�n necesaria
@pos Devolver� RES_OK si encontr� alguna palabra clave o RES_ERROR si no.
*/
int checkForKW(char* linea)
{
    if(strstr(linea,KW_TITLE)||
       strstr(linea,KW_SUBTITLE)||
       strstr(linea,KW_FUNCTION)||
       strstr(linea,KW_DESCRIPTION)||
       strstr(linea,KW_AUTHOR)||
       strstr(linea,KW_DATE)||
       strstr(linea,KW_VERSION)||
       strstr(linea,KW_PARAM)||
       strstr(linea,KW_RETURN)||
       strstr(linea,KW_PRE)||
       strstr(linea,KW_POST))
        /*encontr� un parametro*/
    {
        return RES_OK;
    }
    return RES_ERROR;
}

/*
@funcion countFunctions
@descr Cuenta la cantidad de nodos en un lista. El corriente de la lista puede estar en cualquier lado de la lista.
@autor Ignacio
@fecha 01/10/2015
@version "1.0"
@param listed referencia a la lista
@pre la lista debe estar creada
@pos Devolver� la cantidad de nodos que encontr� en la lista.
*/
int countFunctions(T_List* listed)
{
    int i = 0;

    if(EmptyList(listed)!=FALSE)
    {
        MoveC(&listed,M_First);
        do{
            i++;
        }while(MoveC(&listed,M_Next)!=FALSE);
    }
    MoveC(&listed,M_First);
    return i;
}

/*
@funcion getCommentsCount
@descr Esta es una funci�n que devuelve la cantidad de comentarios almacenados en un dato/elemento que se le pasa (se usa para el elemento del nodo).
@autor Ignacio
@fecha 04/10/2015
@version "1.0"
@param Node nodo
@pre Node debe estar creado
@pos Devolver� la cantidad de comentarios almacenados.
*/
int getCommentsCount(char** dato)
{
    int count = 0;

    while(dato[count]!='\0')
        count++;

    return count;
}

int search_site(T_List *lp, const void* data, T_Move *mov)
{
	void *current_data; /*referencia de donde se va a guardar el dato/elemento del corriente de la lista*/
	char **cdata, **rdata;

	if(EmptyList(lp))
		return FALSE;

	GetC(lp,current_data);

	cdata = ((char**)current_data);
	rdata = ((char**)data);
	/*validar que pudo hacer el get porque usa la funci�n de copy*/
    /*data: ser�a un char**, con data[0]="nombre de la funcion", data[1]="nombre del archivo"*/

   	if (strcmp( cdata[0], rdata[0]) > 0)
	{
		MoveC(&lp,M_First);
		GetC(lp,current_data);
	}
    while(strcmp(data,cdata[0])>0 && MoveC(&lp,M_Next))
	{
		GetC(lp,current_data);
	}
	if(strcmp(rdata[0],cdata[0])<0)
	{
		*mov = M_Prev;
	}
	else
		*mov = M_Next;

	return TRUE;
}

/*
@funcion straight_list_order_insert
@descr Esta funci�n realiza la inserci�n ordenada de un dato, en la posici�n correcta en una lista. Realiza la comparaci�n entre nombres de funciones para ordernar.
@autor Ignacio
@fecha 14/10/2015
@version "1.0"
@param lp referencia a la lista
@param data referencia al dato a guardar en la lista
@pre la lista debe estar creada
@pos se guardar� en la posici�n que deba de acuerda al criterio de ordenamiento.
*/
int iListOrderInsert(T_List *lp, void* data)
{
    int insert;
    T_Move mov = straight_list_first;
	search_site(lp,data,&mov);
	insert = InsertE(&lp,mov,data);
	return insert;
}

int slistCopy(void* dst, const void* src)
{
    /*copia data en dst*/
    /*a priori dst va a ser un char***/
    char **buffer, **csrc;

    buffer = malloc((sizeof(char*)*2));
    csrc = ((char**)src);

    if(!csrc)
        return RES_ERROR;

    if(!buffer)
        return FALSE;
    /*
    buffer[0] = malloc((sizeof(char)*strlen(csrc[0]))+1);
    if(!buffer[0])
    {
        free(buffer);
        return FALSE;
    }
    buffer[1] = (char*)malloc((sizeof(char)*strlen(csrc[1]))+1);
    if(!buffer[1])
    {
        free(buffer[0]);
        free(buffer);
        return FALSE;
    }
    */
    strcpy(buffer[0], csrc[0]);
    strcpy(buffer[1], csrc[1]);
    /*
    Original:
    memcpy(dst[0],src[0],strlen(src[0])+1);
    memcpy(dst[1],src[1],strlen(src[1])+1);

    Modified:
    memcpy(&dst, buffer, sizeof(buffer));
    */

    memcpy(&dst, &buffer, (sizeof(char*)*2));
    return RES_OK;
}

void sListDestroy(void* elem)
{
  char **buffer = malloc(sizeof(char*) * 2);
  buffer = ((char**)elem);
  free(buffer[0]);
  free(buffer[1]);
  free(buffer);
  free(elem);
}
