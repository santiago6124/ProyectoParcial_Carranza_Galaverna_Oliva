#ifndef U05_ARBOL_ARBOL_ARBOLBINARIOAVL_H_
#define U05_ARBOL_ARBOL_ARBOLBINARIOAVL_H_

#include "NodoArbolAVL.h"

template <class T>
class ArbolBinarioAVL
{
protected:
  NodoArbolAVL<T> *root;

public:
  ArbolBinarioAVL();

  void put(T data);

  T search(T data);

  void remove(T data);

  void preorder();

  void inorder();

  void postorder();

  ~ArbolBinarioAVL();

  bool esVacio();

  void print();

  int getBalance()
  {
    return getBalance(root);
  }

private:
  T search(T data, NodoArbolAVL<T> *r);
  void preorder(NodoArbolAVL<T> *r);
  void inorder(NodoArbolAVL<T> *r);
  void postorder(NodoArbolAVL<T> *r);

  int max(int a, int b);
  int calculateHeight(NodoArbolAVL<T> *r);
  int getBalance(NodoArbolAVL<T> *r);
  NodoArbolAVL<T> *rotacionDerecha(NodoArbolAVL<T> *y);
  NodoArbolAVL<T> *rotacionIzquierda(NodoArbolAVL<T> *x);
  NodoArbolAVL<T> *put(T data, NodoArbolAVL<T> *r);
  NodoArbolAVL<T> *remove(T data, NodoArbolAVL<T> *r);

  NodoArbolAVL<T> *findMin(NodoArbolAVL<T> *r);
};

/**
 * Constructor del Arbol
 * @tparam K Clave por la cual va a ordenar el árbol
 * @tparam T Valor guardado por el árbol
 */
template <class T>
ArbolBinarioAVL<T>::ArbolBinarioAVL() { root = nullptr; }

/**
 * Destructor del Arbol
 */
template <class T>
ArbolBinarioAVL<T>::~ArbolBinarioAVL() {}

/**
 * Busca un dato en el árbol. Si no esta el dato en el árbol
 * tira una excepción
 * @param clave Valor a buscar
 * @return el valor buscado
 */
template <class T>
T ArbolBinarioAVL<T>::search(T data)
{
  return search(data, root);
}

template <class T>
T ArbolBinarioAVL<T>::search(T data, NodoArbolAVL<T> *r)
{
  if (r == nullptr)
  {
    throw 404;
  }

  if (r->getData() == data)
  {
    return r->getData();
  }

  if (r->getData() > data)
  {
    return search(data, r->getLeft());
  }
  else
  {
    return search(data, r->getRight());
  }
}

/**
 * Agrega un dato al árbol
 * @param clave Clave para agregar el dato
 * @param dato Dato a agregar
 */
template <class T>
void ArbolBinarioAVL<T>::put(T data) { root = put(data, root); }

template <class T>
NodoArbolAVL<T> *ArbolBinarioAVL<T>::put(T data, NodoArbolAVL<T> *r)
{
  //1. Realizar Insercion normal
  if (r == nullptr)
  {
    return new NodoArbolAVL<T>(data);
  }

  if (r->getData() == data)
  {
    throw 200;
  }

  if (r->getData() > data)
  {
    r->setLeft(put(data, r->getLeft()));
  }
  else
  {
    r->setRight(put(data, r->getRight()));
  }

  //2. Actualizar altura de este nodo padre
  r->setHeight(max(calculateHeight(r->getLeft()), calculateHeight(r->getRight())) + 1);

  //3. Obtener el factor de balance de este nodo padre
  // y chequear si este nodo se desbalanceo
  int balance = getBalance(r);

  //Quedo desbalanceado II: corresponde una rot Der
  if (balance > 1 && data < r->getLeft()->getData())
  {
    return rotacionDerecha(r);
  }

  //Quedo desbalanceado ID: corresponde una rot Izq Der
  if (balance > 1 && data > r->getLeft()->getData())
  {
    r->setLeft(rotacionIzquierda(r->getLeft()));
    return rotacionDerecha(r);
  }

  //Quedo desbalanceado DD: corresponde una rot Izq
  if (balance < -1 && data > r->getRight()->getData())
  {
    return rotacionIzquierda(r);
  }

  //Quedo desbalanceado DI: corresponde una rot Der Izq
  if (balance < -1 && data < r->getRight()->getData())
  {
    r->setRight(rotacionDerecha(r->getRight()));
    return rotacionIzquierda(r);
  }

  return r;
}

/**
 * Elimina un dato del árbol
 * @param clave Clave para identificar el nodo a borrar
 */
template <class T>
void ArbolBinarioAVL<T>::remove(T data)
{
  root = remove(data, root);
}

template <class T>
NodoArbolAVL<T> *ArbolBinarioAVL<T>::remove(T data, NodoArbolAVL<T> *r)
{
  if (r == nullptr)
    return r;

  // Si el valor data es menor al valor almacenado
  // en r, entonces debe estar en el subarbol
  // izquierdo
  if (data < r->getData())
    r->setLeft(remove(data, r->getLeft()));

  // Si el valor data es mayor al valor almacenado
  // en r, entonces debe estar en el subarbol
  // derecho
  else if (data > r->getData())
    r->setRight(remove(data, r->getRight()));

  // Si data es el mismo valor que esta almacenado
  // en r, entonces es el nodo que hay que eliminar
  else
  {
    // Si r no tiene sub nodos, o si tiene uno solo
    if ((r->getLeft() == nullptr) || (r->getRight() == nullptr))
    {
      NodoArbolAVL<T> *temp = r->getLeft() ? r->getLeft() : r->getRight();

      // En caso de que no tenga
      if (temp == nullptr)
      {
        temp = r;
        r = nullptr;
      }
      else          // Caso con un solo sub nodo
        *r = *temp; // Se copian los valores del
                    // sub nodo no vacio
      delete temp;
    }
    else
    {

      // Nodo r con dos sub nodos: Se debe obtener el
      // sucesor inorder (el mas chico en el subarbol derecho)
      NodoArbolAVL<T> *temp = findMin(r->getRight());

      // Copiar la data del sucesor inorder en r
      r->setData(temp->getData());

      // Eliminar el sucesor inorder
      r->setRight(remove(temp->getData(), r->getRight()));
    }
  }

  // Si el arbol tuviera un solo nodo, return
  if (r == nullptr)
    return r;

  // 2. Actualizar la altura del nodo r
  r->setHeight(1 + max(calculateHeight(r->getLeft()),
                       calculateHeight(r->getRight())));

  // 3. Obtener el factor de balance del nodo r
  // para determinar si r se desbalanceo o no
  // luego de la eliminacion
  int balance = getBalance(r);

  //Quedo desbalanceado II: corresponde una rot Der
  if (balance > 1 && getBalance(r->getLeft()) >= 0)
    return rotacionDerecha(r);

  //Quedo desbalanceado ID: corresponde una rot Der Izq
  if (balance > 1 && getBalance(r->getLeft()) < 0)
  {
    r->setLeft(rotacionIzquierda(r->getLeft()));
    return rotacionDerecha(r);
  }

  //Quedo desbalanceado DD: corresponde una rot Izq
  if (balance < -1 && getBalance(r->getRight()) <= 0)
    return rotacionIzquierda(r);

  //Quedo desbalanceado DI: corresponde una rot Izq Der
  if (balance < -1 && getBalance(r->getRight()) > 0)
  {
    r->setRight(rotacionDerecha(r->getRight()));
    return rotacionIzquierda(r);
  }

  return r;
}

template <class T>
NodoArbolAVL<T> *ArbolBinarioAVL<T>::findMin(NodoArbolAVL<T> *r)
{
  {
    NodoArbolAVL<T> *ret = r;
    while (ret->getLeft() != nullptr)
      ret = ret->getLeft();

    return ret;
  }
}

/**
 * Informa si un árbol esta vacío
 * @return
 */
template <class T>
bool ArbolBinarioAVL<T>::esVacio() { return root == nullptr; }

/**
 * Recorre un árbol en preorden
 */
template <class T>
void ArbolBinarioAVL<T>::preorder()
{
  preorder(root);
  std::cout << std::endl;
}

template <class T>
void ArbolBinarioAVL<T>::preorder(NodoArbolAVL<T> *r)
{
  if (r == nullptr)
  {
    return;
  }

  std::cout << r->getData() << " ";
  preorder(r->getLeft());
  preorder(r->getRight());
}

/**
 * Recorre un árbol en orden
 */
template <class T>
void ArbolBinarioAVL<T>::inorder()
{
  inorder(root);
  std::cout << std::endl;
}

template <class T>
void ArbolBinarioAVL<T>::inorder(NodoArbolAVL<T> *r)
{
  if (r == nullptr)
  {
    return;
  }

  inorder(r->getLeft());
  std::cout << r->getData() << " ";
  inorder(r->getRight());
}

/**
 * Recorre un árbol en postorden
 */
template <class T>
void ArbolBinarioAVL<T>::postorder()
{
  postorder(root);
  std::cout << std::endl;
}

template <class T>
void ArbolBinarioAVL<T>::postorder(NodoArbolAVL<T> *r)
{
  if (r == nullptr)
  {
    return;
  }

  postorder(r->getLeft());
  postorder(r->getRight());
  std::cout << r->getData() << " ";
}

/**
 * Muestra un árbol por consola
 */
template <class T>
void ArbolBinarioAVL<T>::print()
{
  if (root != NULL)
    root->print(false, "");
}

template <class T>
int ArbolBinarioAVL<T>::max(int a, int b)
{
  return (a > b) ? a : b;
}

template <class T>
int ArbolBinarioAVL<T>::calculateHeight(NodoArbolAVL<T> *r)
{
  if (r == nullptr)
  {
    return 0;
  }
  else
    return r->getHeight();
}
template <class T>
int ArbolBinarioAVL<T>::getBalance(NodoArbolAVL<T> *r)
{
  if (r == nullptr)
  {
    return 0;
  }
  return calculateHeight(r->getLeft()) - calculateHeight(r->getRight());
}
template <class T>
NodoArbolAVL<T> *ArbolBinarioAVL<T>::rotacionDerecha(NodoArbolAVL<T> *y)
{
  NodoArbolAVL<T> *x = y->getLeft();
  NodoArbolAVL<T> *T2 = x->getRight();

  //Rotacion
  x->setRight(y);
  y->setLeft(T2);

  //Actualizar alturas
  y->setHeight(max(calculateHeight(y->getLeft()), calculateHeight(y->getRight())) + 1);
  x->setHeight(max(calculateHeight(x->getLeft()), calculateHeight(x->getRight())) + 1);

  //Devolver la nueva raiz;
  return x;
}

template <class T>
NodoArbolAVL<T> *ArbolBinarioAVL<T>::rotacionIzquierda(NodoArbolAVL<T> *x)
{
  NodoArbolAVL<T> *y = x->getRight();
  NodoArbolAVL<T> *T2 = y->getLeft();

  //Rotacion
  y->setLeft(x);
  x->setRight(T2);

  //Atualizar alturas
  x->setHeight(max(calculateHeight(x->getLeft()), calculateHeight(x->getRight())) + 1);
  y->setHeight(max(calculateHeight(y->getLeft()), calculateHeight(y->getRight())) + 1);

  //Devolver la nueva raiz
  return y;
}

#endif // U05_ARBOL_ARBOL_ARBOLBINARIOAVL_H_
#ifndef NODO_H
#define NODO_H

template <class T>
class Nodo
{
private:
    T dato;
    Nodo<T> *siguiente;

public:
    T getDato()
    {
        return dato;
    }

    void setDato(T d)
    {
        dato = d;
    }

    Nodo<T> *getSiguiente()
    {
        return siguiente;
    }

    void setSiguiente(Nodo<T> *s)
    {
        this->siguiente = s;
    }
};

#endif //NODO_H
#ifndef U03_PILAS_PILA_PILA_H_
#define U03_PILAS_PILA_PILA_H_

#include "nodo.h"

/**
 * Clase que implementa una Pila generica, ya que puede
 * almacenar cualquier tipo de dato T
 * @tparam T cualquier tipo de dato
 */
template <class T>
class Pila
{
private:
  Nodo<T> *tope;

public:
  Pila();

  ~Pila();

  void push(T dato);

  T pop();

  bool esVacia();
};

/**
 * Constructor de la clase Pila
 * @tparam T
 */
template <class T>
Pila<T>::Pila()
{
  tope = nullptr;
}

/**
 * Destructor de la clase Lista, se encarga de liberar la memoria de todos los
 * nodos utilizados en la lista
 * @tparam T
 */
template <class T>
Pila<T>::~Pila() {
  while(!esVacia()){
    pop();
  }

  delete tope;
}

/**
 * Inserta un dato en la pila
 * @tparam T
 * @param dato  dato a insertar
 */
template <class T>
void Pila<T>::push(T dato)
{

  Nodo<T> *nuevo;
  nuevo = new Nodo<T>;
  nuevo->setDato(dato);
  nuevo->setSiguiente(tope);
  tope = nuevo;
}

/**
 * Obtener el dato de la pila
 * @tparam T
 * @return dato almacenado en el nodo
 */
template <class T>
T Pila<T>::pop()
{
  if(esVacia()){
    throw 400;
  }

  T dato = tope->getDato();
  Nodo<T> *aBorrar = tope;
  tope = tope->getSiguiente();

  delete aBorrar;
  return dato;
}

/**
 * Responde si la pila se encuentra Vacía
 * @tparam T
 * @return
 */
template <class T>
bool Pila<T>::esVacia()
{
  return tope == nullptr;
}

#endif // U03_PILAS_PILA_PILA_H_
#ifndef LISTA_DOBLE_H
#define LISTA_DOBLE_H

#include <iostream>

template <class T>
class NodoDoble {
private:
    T dato;
    NodoDoble<T>* siguiente;
    NodoDoble<T>* anterior;

public:
    NodoDoble(T d) : dato(d), siguiente(nullptr), anterior(nullptr) {}

    T getDato() const { return dato; }
    void setDato(T d) { dato = d; }

    NodoDoble<T>* getSiguiente() const { return siguiente; }
    void setSiguiente(NodoDoble<T>* sig) { siguiente = sig; }

    NodoDoble<T>* getAnterior() const { return anterior; }
    void setAnterior(NodoDoble<T>* ant) { anterior = ant; }
};

template <class T>
class ListaDoble {
private:
    NodoDoble<T>* inicio;
    NodoDoble<T>* fin;

public:
    ListaDoble();
    ~ListaDoble();

    bool esVacia() const;
    int getTamanio() const;
    void insertarPrimero(T dato);
    void insertarUltimo(T dato);
    void remover(int pos);
    T getDato(int pos) const;
    void reemplazar(int pos, T dato);
    void imprimir() const;
    void vaciar();
};

template <class T>
ListaDoble<T>::ListaDoble() : inicio(nullptr), fin(nullptr) {}

template <class T>
ListaDoble<T>::~ListaDoble() {
    vaciar();
}

template <class T>
bool ListaDoble<T>::esVacia() const {
    return inicio == nullptr;
}

template <class T>
int ListaDoble<T>::getTamanio() const {
    NodoDoble<T>* aux = inicio;
    int size = 0;

    while (aux != nullptr) {
        aux = aux->getSiguiente();
        size++;
    }

    return size;
}

template <class T>
void ListaDoble<T>::insertarPrimero(T dato) {
    NodoDoble<T>* nuevo = new NodoDoble<T>(dato);

    if (esVacia()) {
        inicio = fin = nuevo;
    } else {
        nuevo->setSiguiente(inicio);
        inicio->setAnterior(nuevo);
        inicio = nuevo;
    }
}

template <class T>
void ListaDoble<T>::insertarUltimo(T dato) {
    NodoDoble<T>* nuevo = new NodoDoble<T>(dato);

    if (esVacia()) {
        inicio = fin = nuevo;
    } else {
        fin->setSiguiente(nuevo);
        nuevo->setAnterior(fin);
        fin = nuevo;
    }
}

template <class T>
void ListaDoble<T>::remover(int pos) {
    if (esVacia() || pos < 0 || pos >= getTamanio()) {
        throw std::out_of_range("Posición inválida");
    }

    NodoDoble<T>* aux = inicio;
    int posActual = 0;

    while (aux != nullptr && posActual < pos) {
        aux = aux->getSiguiente();
        posActual++;
    }

    if (aux == inicio) {
        inicio = inicio->getSiguiente();
        if (inicio != nullptr) {
            inicio->setAnterior(nullptr);
        }
    } else {
        aux->getAnterior()->setSiguiente(aux->getSiguiente());
        if (aux->getSiguiente() != nullptr) {
            aux->getSiguiente()->setAnterior(aux->getAnterior());
        }
    }

    if (aux == fin) {
        fin = fin->getAnterior();
        if (fin != nullptr) {
            fin->setSiguiente(nullptr);
        }
    }

    delete aux;
}

template <class T>
T ListaDoble<T>::getDato(int pos) const {
    if (pos < 0 || pos >= getTamanio()) {
        throw std::out_of_range("Posición inválida");
    }

    NodoDoble<T>* aux = inicio;
    int posActual = 0;

    while (aux != nullptr && posActual < pos) {
        aux = aux->getSiguiente();
        posActual++;
    }

    return aux->getDato();
}

template <class T>
void ListaDoble<T>::reemplazar(int pos, T dato) {
    if (pos < 0 || pos >= getTamanio()) {
        throw std::out_of_range("Posición inválida");
    }

    NodoDoble<T>* aux = inicio;
    int posActual = 0;

    while (aux != nullptr && posActual < pos) {
        aux = aux->getSiguiente();
        posActual++;
    }

    aux->setDato(dato);
}

template <class T>
void ListaDoble<T>::imprimir() const {
    NodoDoble<T>* aux = inicio;

    while (aux != nullptr) {
        std::cout << aux->getDato() << " <-> ";
        aux = aux->getSiguiente();
    }
    std::cout << "NULL" << std::endl;
}

template <class T>
void ListaDoble<T>::vaciar() {
    NodoDoble<T>* aux = inicio;

    while (aux != nullptr) {
        NodoDoble<T>* aBorrar = aux;
        aux = aux->getSiguiente();
        delete aBorrar;
    }

    inicio = fin = nullptr;
}

#endif // LISTA_DOBLE_H
#ifndef U02_LISTAS_LISTA_LISTA_H_
#define U02_LISTAS_LISTA_LISTA_H_
#include "nodo.h"

/**
 * Clase que implementa una Lista Enlasada generica, ya que puede
 * almacenar cualquier tipo de dato T
 * @tparam T cualquier tipo de dato
 */
template <class T> class Lista {
private:
  Nodo<T> *inicio;

public:
  Lista();

  Lista(const Lista<T> &li);

  ~Lista();

  bool esVacia();

  int getTamanio();

  void insertar(int pos, T dato);

  void insertarPrimero(T dato);

  void insertarUltimo(T dato);

  void remover(int pos);

  T getDato(int pos);

  void reemplazar(int pos, T dato);

  void vaciar();

  void print();
};

/**
 * Constructor de la clase Lista
 * @tparam T
 */
template <class T> Lista<T>::Lista() { inicio = nullptr; }

/**
 * Constructor por copia de la clase Lista
 * @tparam T
 * @param li
 */
template <class T> Lista<T>::Lista(const Lista<T> &li) { inicio = li.inicio; }

/**
 * Destructor de la clase Lista, se encarga de liberar la memoria de todos los
 * nodos utilizados en la lista
 * @tparam T
 */
template <class T> Lista<T>::~Lista() { vaciar(); }

/**
 * Metodo para saber si la lista esta vacia
 * @tparam T
 * @return true si la lista esta vacia, sino false
 */
template <class T> bool Lista<T>::esVacia() { return inicio == nullptr; }

/**
 * Metodo para obtener la cantidad de nodos de la lista
 * @tparam T
 * @return la cantidad de nodos de la lista
 */
template <class T> int Lista<T>::getTamanio() {
  Nodo<T> *aux = inicio;
  int size = 0;

  while (aux != nullptr) {
    aux = aux->getSiguiente();
    size++;
  }

  return size;
}

/**
 * Inserta un nodo con el dato en la posicion pos
 * @tparam T
 * @param pos lugar donde será insertado el dato
 * @param dato  dato a insertar
 */
template <class T> void Lista<T>::insertar(int pos, T dato) {
  int posActual = 0;
  Nodo<T> *aux = inicio, *nuevo;
  nuevo = new Nodo<T>;
  nuevo->setDato(dato);

  if (pos == 0) {
    nuevo->setSiguiente(inicio);
    inicio = nuevo;
    return;
  }

  while (aux != nullptr && posActual < pos - 1) {
    aux = aux->getSiguiente();
    posActual++;
  }

  if (aux == nullptr) {
    throw 404;
  }

  nuevo->setSiguiente(aux->getSiguiente());
  aux->setSiguiente(nuevo);
}

/**
 * Inserta un nodo con el dato en la primera posicion
 * @tparam T
 * @param dato dato a insertar
 */
template <class T> void Lista<T>::insertarPrimero(T dato) { insertar(0, dato); }

/**
 * Inserta un nodo con el dato en la ultima posicion
 * @tparam T
 * @param dato dato a insertar
 */
template <class T> void Lista<T>::insertarUltimo(T dato) {
  Nodo<T> *aux = inicio, *nuevo;
  nuevo = new Nodo<T>;
  nuevo->setDato(dato);

  if (aux == nullptr) {
    nuevo->setSiguiente(inicio);
    inicio = nuevo;
    return;
  }

  while (aux->getSiguiente() != nullptr) {
    aux = aux->getSiguiente();
  }

  nuevo->setSiguiente(aux->getSiguiente());
  aux->setSiguiente(nuevo);
}

/**
 * Elimina el nodo en la posicion 'pos' de la lista enlasada, reenlazando los nodos
 * adecuadamente.
 * @tparam T
 * @param pos posicion del nodo a eliminar
 */
template <class T> void Lista<T>::remover(int pos) {
  Nodo<T> *aux = inicio, *aBorrar;
  int posActual = 0;

  if (pos == 0) {
    inicio = inicio->getSiguiente();
    delete aux;
    return;
  }

  while (aux != nullptr && posActual < pos - 1) {
    aux = aux->getSiguiente();
    posActual++;
  }

  if (aux == nullptr) {
    throw 404;
  }

  aBorrar = aux->getSiguiente();
  aux->setSiguiente(aBorrar->getSiguiente());

  delete aBorrar;
}

/**
 * Obtener el dato del nodo en la posicion pos
 * @tparam T
 * @param pos posicion del dato
 * @return dato almacenado en el nodo
 */
template <class T> T Lista<T>::getDato(int pos) {
  Nodo<T> *aux = inicio;
  int posActual = 0;

  while (aux != nullptr && posActual < pos) {
    aux = aux->getSiguiente();
    posActual++;
  }

  if (aux == nullptr) {
    throw 404;
  }

  return aux->getDato();
}

/**
 * Reemplaza el dato almacenado en un nodo por este otro
 * @tparam T
 * @param pos posicion donde se desea reemplazar
 * @param dato nuevo dato a almacenar
 */
template <class T> void Lista<T>::reemplazar(int pos, T dato) {
  Nodo<T> *aux = inicio;
  int posActual = 0;

  while (aux != nullptr && posActual < pos) {
    aux = aux->getSiguiente();
    posActual++;
  }

  if (aux == nullptr) {
    throw 404;
  }

  aux->setDato( dato );
}

/**
 * Función que vacia la lista enlazada
 * @tparam T
 */
template <class T> void Lista<T>::vaciar() {
	Nodo<T> *aux = inicio, *aBorrar;

	while( aux != nullptr ){
		aBorrar = aux;
		aux = aux->getSiguiente();
		delete aBorrar;
	}

	inicio = nullptr;

}

template<class T>
void Lista<T>::print() {
    Nodo<T> *aux = inicio;

    while (aux != nullptr) {
        std::cout << aux->getDato() << "->";
        aux = aux->getSiguiente();
    }
    std::cout << "NULL" << std::endl;
}

#endif // U02_LISTAS_LISTA_LISTA_H_
#ifndef U02_LISTAS_LISTA_CIRCLIST_H_
#define U02_LISTAS_LISTA_CIRCLIST_H_
#include "nodo.h"

template <class T>
class CircList
{
private:
    Nodo<T> *inicio;

public:
    CircList();

    CircList(const CircList<T> &li);

    ~CircList();

    bool esVacia();

    int getTamanio();

    void insertar(int pos, T dato);

    void insertarPrimero(T dato);

    void insertarUltimo(T dato);

    T getDato(int pos);

    void eliminarPorValor(const T& valor);

    void remover(int pos);

    void imprimir();
};

/**
 * Constructor de la clase CircList
 * @tparam T
 */
template <class T>
CircList<T>::CircList() { inicio = nullptr; }

/**
 * Constructor por copia de la clase CircList
 * @tparam T
 * @param li
 */
template <class T>
CircList<T>::CircList(const CircList<T> &li) { inicio = li.inicio; }

/**
 * Destructor de la clase CircList, se encarga de liberar la memoria de todos los
 * nodos utilizados en la Circlist
 * @tparam T
 */
template <class T>
CircList<T>::~CircList() {}

template <class T>
bool CircList<T>::esVacia()
{
    return inicio == nullptr;
}

template <class T>
int CircList<T>::getTamanio()
{
    Nodo<T> *aux = inicio;
    int tam = 0;

    if (esVacia())
    {
        return tam;
    }

    while (aux->getSiguiente() != inicio)
    {
        aux = aux->getSiguiente();
        tam++;
    }

    tam++;
    return tam;
}
template <class T>
void CircList<T>::insertar(int pos, T dato)
{
    if (pos < 0) {
        throw std::out_of_range("Posición negativa no válida");
    }

    if (pos != 0 && esVacia()) {
        throw 400;  // No se puede insertar en una posición diferente a 0 si la lista está vacía
    }

    Nodo<T> *nuevo = new Nodo<T>();
    nuevo->setDato(dato);

    if (pos == 0) {
        if (esVacia()) {
            nuevo->setSiguiente(nuevo);
            inicio = nuevo;
        } else {
            Nodo<T> *aux = inicio;
            while (aux->getSiguiente() != inicio) {
                aux = aux->getSiguiente();
            }
            nuevo->setSiguiente(inicio);
            aux->setSiguiente(nuevo);
            inicio = nuevo;  // El nuevo nodo se convierte en el inicio
        }
        return;
    }

    Nodo<T> *aux = inicio;
    int posActual = 0;

    while (aux->getSiguiente() != inicio && posActual < pos - 1) {
        aux = aux->getSiguiente();
        posActual++;
    }

    if (posActual < pos - 1) {
        throw std::out_of_range("Posición fuera del rango de la lista");
    }

    nuevo->setSiguiente(aux->getSiguiente());
    aux->setSiguiente(nuevo);
}

template <class T>
void CircList<T>::insertarPrimero(T dato)
{
    insertar(0, dato);
}
template <class T>
void CircList<T>::insertarUltimo(T dato)
{
    Nodo<T> *nuevo = new Nodo<T>;
    nuevo->setDato(dato);

    if (esVacia())
    {
        nuevo->setSiguiente(nuevo);
        inicio = nuevo;
        return;
    }

    Nodo<T> *aux = inicio;
    while (aux->getSiguiente() != inicio)
    {
        aux = aux->getSiguiente();
    }

    nuevo->setSiguiente(inicio); // Simplificación del código
    aux->setSiguiente(nuevo);
}

template <class T>
T CircList<T>::getDato(int pos) {
    if (pos < 0 || esVacia()) {
        throw 404;  // Posición inválida
    }

    Nodo<T> *aux = inicio;
    int posActual = 0;

    do {
        if (posActual == pos) {
            return aux->getDato();
        }
        aux = aux->getSiguiente();
        posActual++;
    } while (aux != inicio);

    throw 404;  // Posición fuera del rango de la lista
}


template <class T>
void CircList<T>::imprimir()
{
    if (esVacia()) {
        std::cout << "La lista está vacía." << std::endl;
        return;
    }

    Nodo<T> *aux = inicio;

    do {
        std::cout << aux->getDato() << "->";
        aux = aux->getSiguiente();
    } while (aux != inicio);

    std::cout << "..." << std::endl;
}
template <class T>
void CircList<T>::eliminarPorValor(const T& valor) {
    if (esVacia()) {
        return;
    }

    Nodo<T> *actual = inicio;
    Nodo<T> *previo = nullptr;
    Nodo<T> *ultimo = inicio;

    // Encontrar el último nodo
    while (ultimo->getSiguiente() != inicio) {
        ultimo = ultimo->getSiguiente();
    }

    do {
        if (actual->getDato() == valor) {
            if (actual == inicio) {
                // Caso especial cuando se elimina el primer nodo
                if (inicio == inicio->getSiguiente()) { // Un solo nodo en la lista
                    delete inicio;
                    inicio = nullptr;
                    return;
                } else { // Más de un nodo
                    ultimo->setSiguiente(inicio->getSiguiente());
                    Nodo<T>* temp = inicio;
                    inicio = inicio->getSiguiente();
                    delete temp;
                    actual = inicio; // Continuar con el siguiente nodo
                }
            } else {
                // Nodo en cualquier otra posición
                previo->setSiguiente(actual->getSiguiente());
                delete actual;
                return;
            }
        } else {
            previo = actual;
            actual = actual->getSiguiente();
        }
    } while (actual != inicio);
}


/**
 * Elimina el nodo en la posicion 'pos' de la lista enlasada, reenlazando los nodos
 * adecuadamente.
 * @tparam T
 * @param pos posicion del nodo a eliminar
 */
template <class T> void CircList<T>::remover(int pos) {
    if (inicio == nullptr) {  // Verificar si la lista está vacía
        throw 404;
    }

    Nodo<T> *aux = inicio, *aBorrar;
    int posActual = 0;

    if (pos == 0) {
        // Si hay solo un nodo en la lista
        if (inicio->getSiguiente() == inicio) {
            delete inicio;
            inicio = nullptr;
            return;
        }

        // Buscar el último nodo para actualizar su puntero
        while (aux->getSiguiente() != inicio) {
            aux = aux->getSiguiente();
        }

        aux->setSiguiente(inicio->getSiguiente());  // Conectar el último nodo al nuevo inicio
        aBorrar = inicio;
        inicio = inicio->getSiguiente();  // Actualizar el inicio al siguiente nodo
        delete aBorrar;
        return;
    }

    // Búsqueda del nodo anterior al que se desea eliminar
    while (aux->getSiguiente() != inicio && posActual < pos - 1) {
        aux = aux->getSiguiente();
        posActual++;
    }

    if (posActual < pos - 1 || aux->getSiguiente() == inicio) {  // Si la posición no es válida
        throw 404;
    }

    aBorrar = aux->getSiguiente();
    aux->setSiguiente(aBorrar->getSiguiente());

    delete aBorrar;
}



#endif // U02_LISTAS_LISTA_CIRCLIST_H_
