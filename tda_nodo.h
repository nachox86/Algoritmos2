#ifndef tda_nodo
    #define tda_nodo

    #if !defined(NULL)
    #define NULL 0
    #endif

    typedef struct TDA_Nodo {
        char** comments;
        struct TDA_Nodo* Siguiente;
    } TDA_Nodo;

    /*
    @funcion createNode
    @descr Esta es una primitiva del TDA_Nodo que crea un nuevo nodo y pide los recursos necesarios para ello.
    @autor Ignacio
    @fecha algundia
    @version "1.0"
    @param *Node nodo a ser creado
    @param commentCount cantidad de palabras claves a ingresar al nodo
    @pre ninguna condición necesaria
    @pos devolverá el código de éxito si pudo crear y solicitar los recursos necesarios para el nodo, en caso contrario devolverá el código de error.
    */
    int createNode(TDA_Nodo* Node,int commentCount);

    /*
    @funcion setNextNode
    @descr Esta es una primitiva del TDA_Nodo que inicializa la referencia al siguiente nodo.
    @autor Ignacio
    @fecha algundia
    @version "1.0"
    @param *Node nodo a ser inicializado
    @param nextNode referencia al siguiente nodo
    @pre Node debe estar creado
    @pos Inicializará Node con la referencia del siguiente nodo.
    */
    int setNextNode(TDA_Nodo* Node,TDA_Nodo* nextNode);

    /*
    @funcion setCommentsNode
    @descr Esta es una primitiva del TDA_Nodo que inicializa dándole un valor a los elementos de la matriz del TDA.
    @autor Ignacio
    @fecha algundia
    @version "1.0"
    @param *Node nodo a ser inicializado
    @param Comments matriz con los comentarios de las palabras claves
    @pre *Node debe estar creado
    @pos Inicializará Node con los comentarios de la función correspondiente al nodo.
    */
    int setCommentsNode(TDA_Nodo* Node,char** Comments);

    /*
    @funcion getCommentsNode
    @descr Esta es una primitiva del TDA_Nodo que devuelve una referencia a los comentarios del nodo.
    @autor Ignacio
    @fecha algundia
    @version "1.0"
    @param Node nodo
    @pre Node debe estar creado
    @pos Devuelve la referencia a la matriz donde están los comentarios.
    */
    char** getCommentsNode(TDA_Nodo Node);

    /*
    @funcion getNextNode
    @descr Esta es una primitiva del TDA_Nodo que devuelve la referencia al siguiente nodo.
    @autor Ignacio
    @fecha algundia
    @version "1.0"
    @param Node nodo
    @pre Node debe estar creado
    @pos Devolverá la referencia al siguiente nodo.
    */
    TDA_Nodo* getNextNode(TDA_Nodo Node);

    /*
    @funcion destroyNode
    @descr Esta es una primitiva del TDA_Nodo que destruye el nodo devolviendo al sistema los recursos utilizados.
    @autor Ignacio
    @fecha algundia
    @version "1.0"
    @param *Node nodo a ser destruido
    @pre *Node debe estar creado
    @pos Devolverá el código de éxito si pudo devolver los recursos al sistema sin problemas o devolverá el código de error en otro caso.
    */
    int destroyNode(TDA_Nodo* Node);
#endif
