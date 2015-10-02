#ifndef __tda_lista_h__
    #define __tda_lista_h__

    #if !defined(FALSE)
        #define FALSE 0
    #endif

    #if !defined(TRUE)
        #define TRUE 1
    #endif

    typedef struct tnode {
        void* elem;
        struct tnode *Next;
        } T_Node;

    typedef struct {
        T_Node *First, *C;
        int SizeE;
        } T_List;

    typedef enum {
        M_First;
        M_Next;
        M_Prev;
        } T_Move;



    /*
        @funcion CreateList
        @descr Primitiva del TDA_LISTA que crea una nueva lista vacia
        @autor Joaquin
        @fecha 02/10/2015
        @version 1.0
        @param *Lp puntero a la lista a crear
        @param SizeE tamanio del elemento q manejaremos
        @pre L no fue creada
        @post L creada y vacia
    */
    void CreateList (T_List *Lp, int SizeE);


    /*
        @funcion EmptyList
        @descr primitiva del TDA_LISTA que devuelve si una lista esta vacia o tiene elementos
        @autor Joaquin
        @fecha 02/10/2015
        @version 1.0
        @param L lista que queremos saber si esta vacia
        @pre L creada
        @post devuelve FALSE si L esta vacia, devuelve TRUE si L tiene elementos
    */
    int EmptyList (T_List L);


    /*
        @funcion ClearList
        @descr Primitiva del TDA_LISTA que borra todos los elementos de la lista dejandola vacia
        @autor Joaquin
        @fecha 02/10/2015
        @version 1.0
        @param *Lp puntero a la lista a vaciar
        @pre L esta creada
        @post L vacia
    */
    void ClearList (T_List *Lp);


    /*
        @funcion InsertE
        @descr Primitiva del TDA_LISTA que inserta un elemento en la lista en la primer posicion de la lista,a continuacion o previo al corriente
        @autor Joaquin
        @fecha 02/10/2015
        @version 1.0
        @param *Lp Lista en la cual insertar el elemento
        @param M indica donde insertar el elemento, al principio, despues del corriente o antes del mismo
        @param E puntero al elemento a insertar
        @pre L creada
        @post Se agrega E, pasa a ser el corriente y se devuelve TRUE
              Si M=M_First, se inserta primero en la lista
              Si M=M_Next, se inserta despues del corriente
              Si M=M_Prev, se inserta antes del corriente
              Si no se pudo insertar, se devuele FALSE
    */
    int InsertE (T_List *Lp, T_Move M, void* E);


    /*
        @funcion GetC
        @descr Primitiva del TDA_LISTA que obtiene el contenido del corriente de una lista
        @autor Joaquin
        @fecha 02/10/2015
        @version 1.0
        @param L lista de la cual queremos obtener el corriente
        @param E puntero al elemento donde se guardara el contenido del corriente
        @pre L creada y con al menos un elemento
        @post Se devuelve el corriente en C
    */
    void GetC (T_List L, void* E);


    /*
        @funcion ChangeC
        @descr Primitiva del TDA_LISTA que modifica el elemento del corriente de una lista
        @autor Joaquin
        @fecha 02/10/2015
        @version 1.0
        @param *Lp puntero a la lista a la que se le modificara el elemento del corriente
        @param E puntero al elemento con el cual se modificara el corriente de la lista
        @pre creada y no vacia
        @post E pasa a ser el elemento del corriente
    */
    void ChangeC (T_List *Lp, void* E);


    /*
        @funcion MoveC
        @descr Primitiva del TDA_LISTA que mueve el corriente a la siguiente o primer posicion
        @autor Joaquin
        @fecha 02/10/2015
        @version 1.0
        @param *Lp puntero a la lista a la que se le cambiara la posicion del corriente
        @param M movimiento a efectuar, mover al siguiente o a la primer posicion
        @pre L creada y no vacia
        @post Si M=M_First, el corriente es el primero y devuelvo TRUE.
              Si M=M_Next, y hay elemento despes del corriente, este pasa a ser el corriente y se devuelve TRUE, si no se devuelve FALSE.
              Si M=M_Prev, se devuelve FALSE.
              Si L esta vacia, devuelvo FALSE.
    */
    int MoveC (T_List *Lp, T_Move M);


    /*
        @funcion DeleteC
        @descr Primitiva del TDA_LISTA que borra el corriente de una lista
        @autor Joaquin
        @fecha 02/10/2015
        @version 1.0
        @param *Lp puntero a la lista a la que se le borrara el corriente
        @pre L creada y no vacia
        @post Se elimino C y el corriente pasa a ser al siguiente a menos q este sea el ultimo, en este caso pasa a ser el anterior
    */
    void DeleteC (T_List *Lp);




#endif