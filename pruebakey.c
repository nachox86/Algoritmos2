int set_keyword(t_keyword* kw,char* data)
{
    /* tengo que parsear */
    int ldata = strlen(data);
    int ltag;
    int lname;
    int lvalue;
    int i;
    char* token;
    char* token2;
    char* token3;

    /*auxiliar*/
    token = (char*)malloc(ldata*sizeof(char)+1);
    if(!token)
        return RES_MEM_ERROR;

    strcpy(token,data); /*tengo el tag*/
    lvalue = strlen(strstr(token," ")); /*tamaño de lo que está después del tag*/
    ltag = ldata - lvalue; /* tamaño del tag */
    kw->tag = (char*)malloc(sizeof(char)*ltag+1); /*pido memoria para el tag*/
    if(!kw->tag)
    {
        free(token);
        return RES_MEM_ERROR;
    }
    memcpy(kw->tag,token,ltag); /*copio al struct*/
    kw->tag[ltag] = '\0'; /*agrego el fin de linea*/

    /*copia a token2 lo que hay después del tag*/
    token2 = (char*)malloc(sizeof(char)*(lvalue-1)+1);
    if(!token2)
    {
        free(kw->tag);
        free(token);
        return RES_MEM_ERROR;
    }
    for(i=0;i<lvalue-1;i++)
        token2[i] = token[ltag+i+1];
    token2[lvalue-1] = '\0'; /*aca tengo lo que está después del tag sin el espacio de adelante*/

    /*ahora veo si es el tag de alguno de los que tienen un tercer parametro asociado : nombre del tag, nombre funcion/parametro , cadena */
    if(strcmp(kw->tag,KW_FUNCTION)==0||strcmp(kw->tag,KW_PARAM)==0||strcmp(kw->tag,KW_RETURN)==0)
    {
        /* si el tag es alguno de los que tiene un tercer parametro para mostrar */
        /* tengo que recuperar el parametro que va despues del tag y lo guardo en kw->name */
        ldata = strlen(token2);
        lvalue = strlen(strstr(token2," ")); /*tamaño de lo que está después del tag*/
        lname = ldata - lvalue; /* tamaño del tag */
        kw->name = (char*)malloc(sizeof(char)*lname+1); /*pido memoria para el tag*/
        if(!kw->name)
        {
            free(kw->tag);
            free(token2);
            free(token);
            return RES_MEM_ERROR;
        }
        memcpy(kw->name,token2,lname); /*copio al struct*/
        kw->name[lname] = '\0'; /*agrego el fin de linea*/
    }
    else
    {
        kw->name = (char*)malloc(sizeof(char)); /*pido memoria para el tag*/
        if(!kw->name)
        {
            free(kw->tag);
            free(token2);
            free(token);
            return RES_MEM_ERROR;
        }
        kw->name='\0';
    }

    /*ahora le toca al resto que va a ir a value*/

    free(token);
    free(switched2);
    free(switched);
}
