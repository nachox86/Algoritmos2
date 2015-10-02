#include "list_tda.h"
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>


void CreateList (T_List *Lp, int Se){
    Lp->C = NULL;
    Lp->First = NULL;
    Lp->SizeE = Se;
    }


int EmptyList (T_List L){
    return (L.First == NULL);
    }


void ClearList (T_List *Lp){
    T_Node *Np, *Next;
    for (Np=Lp->First; Np; Np=Next){
        Next=Np->Next;
        free(Np->elem);
        free(Np)
        }
    Lp->First=Lp->C=NULL
    }

int InsertE (T_List *Lp, T_Move M, void* E){
    T_Node *Np;
    Np=(T_Node*)malloc(sizeof(T_Node));
    if (!Np)
        return FALSE;
    if ((Lp->First == NULL)||(M == M_First)||((M == M_Prev)&(Lp->First == Lp->C))){
        Np->Next=Lp->First;
        Lp->First=Lp->C=Np;
    }
    else{
        if (M==M_Next){
            Np->Next=Lp->C->Next;
            Lp->C->Next=Np;
        }
        else /*M==M_Prev*/ {
            T_Node *Auxp;
            Auxp=Lp->First;
            while (Auxp->Next != Lp->C)
                Auxp=Auxp->Next;
            Auxp->Next=Np;
            Np->Next=Lp->C;
        }
    }
    Np->elem=malloc(Lp->SizeE);
    if (!Np->elem){
        free(Np);
        return FALSE;
    }
    memcpy(Np->elem,E,Lp->SizeE);
    Lp->C=Np;
    return TRUE;
}


void GetC(T_List L, void* E){
    memcpy(E,L.C->elem,L.SizeE)
}


void ChangeC (T_List *Lp, void* E){
    memcpy(Lp->C->elem, E, Lp->SizeE);
}


int MoveC (T_List *Lp, T_Move M){
    switch(M){
        case M_First: Lp->C=Lp->First;break;
        case M_Next: if(Lp->C->Next==NULL)
                        return FALSE;
                     else
                        Lp->C=Lp->C->Next;
                     break;
        case M_Ant: return FALSE
    }
    return TRUE
}


void DeleteC (T_List *Lp){
    T_Node *Np;
    Np=Lp->C;
    if (Lp->C == Lp->First){
        Lp->First=Lp->C->Next;
        Lp->C=Lp->First;
    }
    else{
        T_Nodo *Auxp;
        Auxp=Lp->First;
        while(Auxp->Next != Lp->C)
            Auxp=Auxp->Next;
        Auxp->Next=Lp->C->Next;
        if (Auxp->Next)
            Lp->C=Auxp->Next;
        else
            Lp->C=Auxp;
    }
    free(Np->elem);
    free(Np);
}
