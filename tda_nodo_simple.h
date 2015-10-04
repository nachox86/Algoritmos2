#ifndef tda_nodo_simple
    #define tda_nodo_simple

    #if !defined(NULL)
    #define NULL 0
    #endif

    typedef struct TDA_Nodo_Simple
    {
        char* tname;
        char* outputFile;
        struct TDA_Nodo_Simple* Siguiente;
    } TDA_Nodo_Simple;

    /*
    @funcion createSimpleNode
    @descr Esta es una primitiva del TDA_Nodo_Simple que crea un nuevo nodo y pide los recursos necesarios para ello.
    @autor Ignacio
    @fecha algundia
    @version "1.0"
    @param *Node nodo a ser creado
    @pre ninguna condici�n necesaria
    @pos devolver� el c�digo de �xito si pudo crear y solicitar los recursos necesarios para el nodo, en caso contrario devolver� el c�digo de error.
    */
    int createSimpleNode(TDA_Nodo_Simple* Node);

    /*
    @funcion setNextSNode
    @descr Esta es una primitiva del TDA_Nodo_Simple que inicializa la referencia al siguiente nodo.
    @autor Ignacio
    @fecha algundia
    @version "1.0"
    @param *Node nodo a ser inicializado
    @param nextNode referencia al siguiente nodo
    @pre Node debe estar creado
    @pos Inicializar� Node con la referencia del siguiente nodo.
    */
    int setNextSNode(TDA_Nodo_Simple* Node,TDA_Nodo_Simple* nextNode);

    /*
    @funcion setNameSNode
    @descr Esta es una primitiva del TDA_Nodo_Simple que inicializa el nombre del nodo.
    @autor Ignacio
    @fecha algundia
    @version "1.0"
    @param *Node nodo a ser inicializado
    @param nextNode referencia al siguiente nodo
    @pre Node debe estar creado
    @pos Inicializar� Node con el nombre de la funci�n.
    */
    int setNameSNode(TDA_Nodo_Simple* Node,char* namet);

    /*
    @funcion setOutputFileSNode
    @descr Esta es una primitiva del TDA_Nodo_Simple que inicializa el campo de archivo de salida.
    @autor Ignacio
    @fecha algundia
    @version "1.0"
    @param *Node nodo a ser inicializado
    @param Comments matriz con los comentarios de las palabras claves
    @pre *Node debe estar creado
    @pos Inicializar� Node con los comentarios de la funci�n correspondiente al nodo.
    */
    int setOutputFileSNode(TDA_Nodo_Simple* Node,char* output);

    /*
    @funcion getNameSNode
    @descr Esta es una primitiva del TDA_Nodo_Simple que devuelve el nombre del nodo.
    @autor Ignacio
    @fecha algundia
    @version "1.0"
    @param Node nodo
    @pre Node debe estar creado
    @pos Devuelve la referencia a la matriz donde est�n los comentarios.
    */
    char* getNameSNode(TDA_Nodo_Simple* Node);

    /*
    @funcion getOutputFileSNode
    @descr Esta es una primitiva del TDA_Nodo_Simple que devuelve el nombre del archivo de salida.
    @autor Ignacio
    @fecha algundia
    @version "1.0"
    @param Node nodo
    @pre Node debe estar creado
    @pos Devuelve la referencia a la matriz donde est�n los comentarios.
    */
    char* getOutputFileSNode(TDA_Nodo_Simple* Node);

    /*
    @funcion getNextNode
    @descr Esta es una primitiva del TDA_Nodo_Simple que devuelve la referencia al siguiente nodo.
    @autor Ignacio
    @fecha algundia
    @version "1.0"
    @param Node nodo
    @pre Node debe estar creado
    @pos Devolver� la referencia al siguiente nodo.
    */
    TDA_Nodo_Simple* getNextSNode(TDA_Nodo_Simple* Node);

    /*
    @funcion destroyNode
    @descr Esta es una primitiva del TDA_Nodo_Simple que destruye el nodo devolviendo al sistema los recursos utilizados.
    @autor Ignacio
    @fecha algundia
    @version "1.0"
    @param *Node nodo a ser destruido
    @pre *Node debe estar creado
    @pos Devolver� el c�digo de �xito si pudo devolver los recursos al sistema sin problemas o devolver� el c�digo de error en otro caso.
    */
    int destroySNode(TDA_Nodo_Simple* Node);
#endif

