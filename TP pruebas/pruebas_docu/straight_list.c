#include "straight_list.h"

int straight_list_create(straight_list_t* lp, size_t size, straight_list_copy_t copy, straight_list_destroy_t destroy)
{
	lp->current = NULL;
	lp->first = NULL;
	lp->size = size;
	lp->copy = copy;
	lp->destroy = destroy;
	return RES_OK;
}

void straight_list_destroy(straight_list_t *lp)
{
	straight_list_clear(lp);
}

void straight_list_clear(straight_list_t *lp)
{
	straight_list_node_t *current, *next;
	for(current = lp->first; current; current = next)
	{
		next = current->next;
		lp->destroy(current->data);
		free(current->data);
		free(current);
	}
	lp->first=lp->current=NULL;
}

int straight_list_is_empty(const straight_list_t *lp)
{
	return (lp->first == NULL);
}

void straight_list_get(const straight_list_t *lp, void *data)
{
	lp->copy(data, lp->current->data);
}

int straight_list_move(straight_list_t *lp, straight_list_movement_t m)
{
	switch (m)
	{
		case straight_list_first:
			lp->current=lp->first;
		break;
		case straight_list_next:
			if (lp->current->next==NULL)
				return FALSE;
			else
				lp->current=lp->current->next;
		break;
		case straight_list_previous:return FALSE;
	}
	return TRUE;
}

void straight_list_delete(straight_list_t *lp)
{
	straight_list_node_t *current = lp->current;
	if (lp->current==lp->first)
	{
		lp->first = lp->current->next;
		lp->current = lp->first;
	}
	else
	{
		if(lp->current->next){
			/* En este caso en que el corriente no es el ultimo, puedo evitarme
			 * recorrer toda la lista buscando el anterior */
			current=lp->current->next;
			memcpy(lp->current->data, current->data, lp->size);
			lp->current->next = current->next;
		}else {
			straight_list_node_t *aux = lp->first;
			while (aux->next != lp->current) {
				aux = aux->next;
			}
			aux->next=lp->current->next;
			lp->current = aux; /*Si es el ultimo queda en el Anterior al borrado */
		}
	}

	lp->destroy(current->data);
	free(current->data);
	free(current);
}

int straight_list_insert(straight_list_t *lp, straight_list_movement_t m, const void* data)
{
	straight_list_node_t *new_node = (straight_list_node_t*) malloc(sizeof(straight_list_node_t));
	if (!new_node) {
		return FALSE; /* No hay memoria disponible */
	}
	new_node->data = malloc (lp->size);
	if(!new_node->data)
	{
		free(new_node);
		return FALSE;
	}

	lp->copy(new_node->data, data);
	if ((lp->first == NULL) || (m==straight_list_first) || ((m==straight_list_previous) && (lp->first==lp->current)))
	{
		/*Si esta vacia o hay que insertar en el Primero o
		hay que insertar en el Anterior y el actual es el Primero */
		new_node->next = lp->first;
		lp->first = lp->current = new_node;
	}
	else
	{
		/* Siempre inserto como siguiente, el nodo nuevo, porque es mÃ¡s fÃ¡cil */
		new_node->next = lp->current->next;
		lp->current->next = new_node;
		if (m == straight_list_previous)
		{
			/* Pero cuando el movimiento es Anterior, entonces swapeo los elementos */
			void* tmp = new_node->data;
			new_node->data = lp->current->data;
			lp->current->data = tmp;
		}
	}
	lp->current=new_node;
	return TRUE;
}
