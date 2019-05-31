#include <stdio.h>
#include <stdlib.h>

typedef struct avlnode
{
    int clave;
    int balance; /* Factor de balance -1,0,1 */
    struct avlnode *izquierdo, *derecho;
} nodo, *pnodo;

# define max(A,B) ((A)>(B)?(A):(B)) /* Definimos la funcion mayor o menor*/
# define min(A,B) ((A)>(B)?(B):(A))

int flag;
//flag = 1 indica que debe seguir el ascenso rebalanceando.
int key;
int alto_avl = 0;

/* Rotaci�n Izquierda (A) *
*  A               B
* / \             / \
* a  B ==>       A   c
*   / \         / \ / \
*   b  c       a  b d  e
      / \
     d   e
* S�lo cambian los factores de balance de los nodos A y B
* Los factores de balance de los sub-�rboles no cambian. */



static pnodo rotacionIzquierda(pnodo nodo)
{
    pnodo temp;
    int x,y;
    temp = nodo;
    nodo = nodo->derecho;
    temp->derecho = nodo->izquierdo;
    nodo->izquierdo = temp;
    //Recalcula factores de balance de los dos nodos
    x = temp->balance; // oldbal(A)
    y = nodo->balance; // oldbalance(B)
    temp->balance = x-1-max(y, 0); // nA
    nodo->balance = min(x-2+min(y, 0), y-1); // nB
    return nodo;
}

static pnodo rotacionDerecha(pnodo nodo)
{
    pnodo temp = nodo;
    int x,y;
    nodo = nodo->izquierdo;
    temp->izquierdo = nodo->derecho;
    nodo->derecho = temp;
    x = temp->balance; // oldbal(A)
    y = nodo->balance;    // oldbal(B)
    temp->balance = x+1-min(y, 0);  // nA
    nodo->balance = max(x+2+max(y, 0), y+1); //nB
    return nodo;

}

static void Error(int tipo)
{
    if (tipo) printf("\nError en insercion\n");
    else printf("\nError en descarte\n");
}

int Altura(void) //te da la altura actual del arboi
{
    return alto_avl;
}

pnodo CreaNodo(int key)//crea un nodo sin hijos
{
    pnodo nodo;
    nodo = (pnodo) malloc(sizeof(nodo));
    nodo->clave=key;
    nodo->izquierdo=NULL;
    nodo->derecho=NULL;
    return nodo;
}

pnodo insertR(pnodo nodo)
{
    if (nodo == NULL)  /* Lleg� a un punto de inserci�n */
    {
        nodo = CreaNodo(key);
        nodo->balance = 0; /* Los dos hijos son nulos */
        flag = 1; /* Marca necesidad de revisar balances */
        return nodo; /* retorna puntero al insertado */
    }
    else if (nodo->clave < key)
    {
        //desciende por la derecha
        nodo->derecho = insertR(nodo->derecho);
        //se pasa por la siguiente l�nea en la revisi�n ascendente
        nodo->balance += flag; /* Incrementa factor de balance cuando sumo uno a la derecha sumo 1 al balance*/
    }
    else if (nodo->clave > key)
    {
        //desciende por la izquierda
        nodo->izquierdo = insertR(nodo->izquierdo);
        //se corrige en el ascenso
        nodo->balance -= flag; /* Decrementa balance a la izquierda resto 1 si es 0 el total esta balanceado*/
    }
    else   /* (t->k == key) Ya estaba en el �rbol */
    {
        Error(1);
        flag = 0;
    }

    if (flag == 0) /* No hay que rebalancear. Sigue el ascenso */
        return nodo;

    /*El c�digo a continuaci�n es el costo adicional para mantener propiedad AVL */
    /* Mantiene �rbol balanceado avl. S�lo una o dos rotaciones por inserci�n */
    if (nodo->balance < -1)
    {
        /* Qued� desbalanceado por la izquierda. Espejos Casos c y d.*/
        if (nodo->izquierdo->balance > 0)
            /* Si hijo izquierdo est� cargado a la derecha */
            nodo->izquierdo = rotacionIzquierda(nodo->izquierdo);
        nodo = rotacionDerecha(nodo);
        flag = 0; /* El sub�rbol no aumenta su altura */
    }
    else if (nodo->balance > 1)
    {
        /* Si qued� desbalanceado por la derecha: Casos c y d.*/
        if (nodo->derecho->balance < 0)
            /* Si hijo derecho est� cargado a la izquierda Caso d.*/
            nodo->derecho = rotacionDerecha(nodo->derecho);
        nodo = rotacionIzquierda(nodo); /* Caso c.*/
        flag = 0; /* El sub�rbol no aumenta su altura */
    }
    else if (nodo->balance == 0)/* La inserci�n lo balanceo */
        flag = 0; /* El sub�rbol no aumenta su altura. Caso a*/
    else /* Qued� desbalanceado con -1 � +1 Caso b */
        flag = 1; /* Propaga ascendentemente la necesidad de rebalancear */
    return nodo;
}

/* Mantiene variable global con el alto del �rbol. */

pnodo InsertarAVL(int clave, pnodo nodo)
{
    key = clave; //pasa argumento a global.
    nodo = insertR(nodo);
    if (flag == 1)
        alto_avl++;
    //si la propagaci�n llega hasta la ra�z, aumenta la altura.
    return nodo;
}

pnodo deleteR( pnodo nodo)
{
    pnodo p;
    if (nodo == NULL)  /* No encontr� nodo a descartar */
    {
        Error(0);
        flag = 0;
    }
    else if (nodo->clave < key)
    {
        //Comienza el descenso por la derecha
        nodo->derecho = deleteR(nodo->derecho);
        //aqu� se llega en el retorno ascendente.
        nodo->balance -= flag; /* Se descart� por la derecha. Disminuye factor */
        //Se retorna despu�s de la revisi�n de los factores
    }
    else if (nodo->clave > key)
    {
        //Desciende por la izquierda
        nodo->izquierdo = deleteR(nodo->izquierdo);
        //o se llega por esta v�a si se descart� por la izquierda.
        nodo->balance += flag; /* se descart� por la izq. Aumenta factor de balance */
    }
    else   /* (t->clave == key) */
    {
        /* Encontr� el nodo a descartar */
        if (nodo->izquierdo == NULL)   /*Si hay hijo derecho debe ser hoja, por ser AVL */
        {
            p = nodo;
            nodo = nodo->derecho;
            free(p);
            flag = 1; /* Debe seguir revisando factores de balance */
            return nodo; /* ascendentemente */
        }
        else if (nodo->derecho == NULL)   /*Si hay hijo izquierdo debe ser hoja */
        {
            p = nodo;
            nodo = nodo->izquierdo;
            free(p);
            flag = 1; /* Asciende revisando factores de balance */
            return nodo; /* Corrigiendo */
        }
        else   /* Tiene dos hijos */
        {
            if (nodo->balance<0)
            {
                /* Si cargado a la izquierda, elimina mayor descendiente hijo izq */
                p = nodo->izquierdo;
                while (p->derecho != NULL) p = p->derecho;
                nodo->clave = p->clave;
                key = p->clave; //busca hoja a eliminar
                nodo->izquierdo = deleteR(nodo->izquierdo);
                nodo->balance += flag; /* incrementa factor de balance */
            }
            else   /* Si cargado a la derecha, elimina menor descendiente hijo der */
            {
                p = nodo->derecho;
                while (p->izquierdo != NULL)
                    p = p->izquierdo;
                nodo->clave = p->clave;
                key = p->clave;
                nodo->derecho = deleteR(nodo->derecho);
                nodo->balance -= flag; /* decrementa balance */
            }
        }
    }

    /* Mantiene �rbol balanceado avl. S�lo una o dos rotaciones por descarte */
    if (flag == 0 ) /* No hay que rebalancear. Sigue el ascenso, sin rebalancear */
        return nodo;

    /* Hay que revisar factores de balance en el ascenso*/
    if (nodo->balance < -1)  /* Si qued� desbalanceado por la izquierda y dej� de ser AVL */
    {
        if (nodo->izquierdo->balance > 0)  /*espejos casos c, d y e */
        {
            /* Si el hijo izquierdo est� cargado a la derecha */
            nodo->izquierdo = rotacionIzquierda(nodo->izquierdo);
            flag = 1; /*Continuar revisando factores */
        }
        else if (nodo->izquierdo->balance == 0)
            flag = 0; /*No debe seguir el rebalance */
        else
            flag = 1;/* Debe seguir revisando factores de balance */
        nodo = rotacionDerecha(nodo);
    }
    else if (nodo->balance > 1)  /* Si qued� desbalaceado por la derecha */
    {
        if (nodo->derecho->balance < 0)
        {
            /* Si hijo derecho est� cargado a la izquierda */
            nodo->derecho = rotacionDerecha(nodo->derecho);
            flag = 1; //debe seguir revisando. Caso d.
        }
        else if (nodo->derecho->balance == 0)
            flag = 0; /* No debe seguir el rebalance. Caso c. */
        else //positivo
            flag = 1;/* Debe seguir revisando factores de balance. Caso e. */
        nodo = rotacionIzquierda(nodo);
    }
    else if (nodo->balance == 0) /* Si estaba en +1 � -1 y queda en cero */
        flag = 1; /* Debe seguir corrigiendo. Caso b.*/
    else /* Si estaba en cero y queda en -1 � +1 */
        flag = 0; /* No debe seguir rebalanceando. Caso a.*/
    return nodo;
}
pnodo DescartarAVL(int clave, pnodo t)
{
    key = clave;
    t = deleteR(t);
    if (flag == 1) alto_avl--;
    return t;
}

pnodo busqueda(pnodo nodo)
{
    if (nodo==NULL || nodo->clave==key)
        return nodo;
    if(nodo->clave < key)
        return busqueda(nodo->derecho);
    return busqueda(nodo->izquierdo);
}
pnodo  BuscarAVL(int clave, pnodo nodo)
{
    key= clave;
    pnodo resultado=busqueda(nodo);
    if(resultado==NULL)
        printf("\nNo encontrado\n");
    else
        printf("\nEncontrado\n");
    return nodo;
}

void salir(){
    printf("Programa finalizado");
}



void menu(){

    int opcion;
    pnodo t=NULL;
    int valor;


    do{
        printf("*****Menu*****\n");

        printf("Elige una opcion\n");

        printf("1. Cargar valor \n");
        printf("2. Listar in order\n");
        printf("3. Listar pos order \n");
        printf("4. Listar Pre order \n");
        printf("5. Borrar Nodo \n");
        printf("6. Buscar Nodo \n");
        printf("7. Salir \n");

        printf("SELECCIONA LA OPCION \n");
        scanf("%d", &opcion);

        switch(opcion)
        {

            case 1:
                printf("ingresar un valor \n");
                scanf("%d", &valor);
                break;

            case 2:
                break;

            case 3:
                break;

            case 4:
                break;

            case 5:
                break;

            case 6:
                break;

            case 7:
                salir();
                break;


        }

    }while (opcion != 7);


}


int main() {
    menu();
    return 0;

}



