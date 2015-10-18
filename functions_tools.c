#include "list_tda.h"
#include "tda_nodo.h"
#include "tda_documentador.h"
#include <string.h>
#include "functions_tools.h"
#include "straight_list.h"

/*
@funcion showHelp
@descr Muestra la ayuda del programa.
@autor Ignacio
@fecha 03/10/2015
@version "1.0"
@param ninguno
@pre ninguna condición necesaria
@pos devuelve en pantalla la ayuda del programa.
*/
void showHelp()
{
    printf("Este programa permite obtener en formato html, la documentación provista de un archivo de texto.\n");
    printf("Uso:\n");
    printf("Documentador.exe -i archivo_a_documentar.c -l archivo_log.txt -o archivo_documentado.html\n");
    printf("Documentador.exe -h\n");
    printf("Documentador.exe --help\n");
    printf("Descripcion de los parámetros:\n");
    printf("-i: indica el nombre del archivo de entrada del que se toma la documentación.\n");
    printf("-l: indica el nombre del archivo de loggeo de eventos.\n");
    printf("-o: indica el nombre del archivo de salida de la documentación en formato html.\n");
    printf("-h o --help: muestra esta ayuda.\n");
}

/*
@funcion checkForKW
@descr Dado un string de caracteres se fija si se encuentra alguna de las palabras claves definidas.
@autor Ignacio
@fecha 01/10/2015
@version "1.0"
@param linea array de caracteres
@pre ninguna condición necesaria
@pos Devolverá RES_OK si encontró alguna palabra clave o RES_ERROR si no.
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
        /*encontró un parametro*/
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
@pos Devolverá la cantidad de nodos que encontró en la lista.
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
@descr Esta es una función que devuelve la cantidad de comentarios almacenados en un dato/elemento que se le pasa (se usa para el elemento del nodo).
@autor Ignacio
@fecha 04/10/2015
@version "1.0"
@param Node nodo
@pre Node debe estar creado
@pos Devolverá la cantidad de comentarios almacenados.
*/
int getCommentsCount(char** dato)
{
    int count = 0;

    while(dato[count]!='\0')
        count++;

    return count;
}

int search_site(straight_list_t *lp, const void* data, straight_list_movement_t* mov)
{
	void *current_data; /*referencia de donde se va a guardar el dato/elemento del corriente de la lista*/
	char **cdata, **rdata;

	if(straight_list_is_empty(lp))
		return FALSE;

	straight_list_get(lp,current_data);

	cdata = ((char**)current_data);
	rdata = ((char**)data);
	/*validar que pudo hacer el get porque usa la función de copy*/
    /*data: sería un char**, con data[0]="nombre de la funcion", data[1]="nombre del archivo"*/

   	if (strcmp( cdata[0], rdata[0]) > 0)
	{
		straight_list_move(lp,straight_list_first);
		straight_list_get(lp,current_data);
	}
    while(strcmp(data,cdata[0])>0 && straight_list_move(lp,straight_list_next))
	{
		straight_list_get(lp,current_data);
	}
	if(strcmp(rdata[0],cdata[0])<0)
	{
		*mov = straight_list_previous;
	}
	else
		*mov = straight_list_next;

	return TRUE;
}

/*
@funcion straight_list_order_insert
@descr Esta función realiza la inserción ordenada de un dato, en la posición correcta en una lista. Realiza la comparación entre nombres de funciones para ordernar.
@autor Ignacio
@fecha 14/10/2015
@version "1.0"
@param lp referencia a la lista
@param data referencia al dato a guardar en la lista
@pre la lista debe estar creada
@pos se guardará en la posición que deba de acuerda al criterio de ordenamiento.
*/
int straight_list_order_insert(straight_list_t *lp,const void* data)
{
    int insert;
    straight_list_movement_t mov = straight_list_first;
	search_site(lp,data,&mov);
	insert = straight_list_insert(lp,mov,data);
	return insert;
}

int slistCopy(void* dst, const void* src)
{
    /*copia data en dst*/
    /*a priori dst va a ser un char***/
    char **buffer, **csrc;

    buffer = (char**)malloc(sizeof(char*)*2);
    csrc = ((char**)src);

    if(!buffer)
        return FALSE;
    buffer[0] = (char*)malloc((sizeof(char)*strlen(csrc[0]))+1);
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
    /*
    Original:
    memcpy(dst[0],src[0],strlen(src[0])+1);
    memcpy(dst[1],src[1],strlen(src[1])+1);

    Modified:
    memcpy(&dst, buffer, sizeof(buffer));
    */
    dst = &buffer; /* if this sigfaults, use the modified definition above */
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
