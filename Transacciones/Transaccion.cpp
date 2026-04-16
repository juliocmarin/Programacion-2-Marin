#include "Transaccion.hpp"
#include <cstring>
#include <iostream>
#include <iomanip>
using namespace std;

Transaccion::Transaccion() {
    id            = 0;
    idRelacionado = 0;
    cantItems     = 0;
    total         = 0.0f;
    cancelada     = false;
    fechaCreacion = 0;
    fechaModif    = 0;
    memset(tipo,        0, sizeof(tipo));
    memset(descripcion, 0, sizeof(descripcion));
    memset(items,       0, sizeof(items));
}

Transaccion::Transaccion(const char* tip, int idRel) : Transaccion() {
    setTipo(tip);
    setIdRelacionado(idRel);
    fechaCreacion = time(0);
    fechaModif    = time(0);
}

Transaccion::Transaccion(const Transaccion& otro) {
    id            = otro.id;
    idRelacionado = otro.idRelacionado;
    cantItems     = otro.cantItems;
    total         = otro.total;
    cancelada     = otro.cancelada;
    fechaCreacion = otro.fechaCreacion;
    fechaModif    = otro.fechaModif;
    strncpy(tipo,        otro.tipo,        9);   tipo[9]         = '\0';
    strncpy(descripcion, otro.descripcion, 199); descripcion[199]= '\0';
    for (int i = 0; i < MAX_ITEMS; i++) items[i] = otro.items[i];
}

Transaccion::~Transaccion() {}

// GETTERS
int                    Transaccion::getId()            const { return id; }
const char*            Transaccion::getTipo()          const { return tipo; }
int                    Transaccion::getIdRelacionado()  const { return idRelacionado; }
int                    Transaccion::getCantItems()     const { return cantItems; }
float                  Transaccion::getTotal()         const { return total; }
const char*            Transaccion::getDescripcion()   const { return descripcion; }
bool                   Transaccion::isCancelada()      const { return cancelada; }
time_t                 Transaccion::getFechaCreacion() const { return fechaCreacion; }
time_t                 Transaccion::getFechaModif()    const { return fechaModif; }
const ItemTransaccion* Transaccion::getItems()         const { return items; }

// SETTERS
void Transaccion::setId(int i)                { id = i; }
void Transaccion::setIdRelacionado(int i)     { idRelacionado = i; }
void Transaccion::setCancelada(bool c)        { cancelada = c; }
void Transaccion::setFechaCreacion(time_t f)  { fechaCreacion = f; }
void Transaccion::setFechaModif(time_t f)     { fechaModif = f; }

void Transaccion::setTipo(const char* t) {
    if (!t) return;
    strncpy(tipo, t, 9); tipo[9] = '\0';
}

void Transaccion::setDescripcion(const char* d) {
    if (!d) return;
    strncpy(descripcion, d, 199); descripcion[199] = '\0';
}

// METODOS DE GESTION
bool Transaccion::agregarItem(int idProd, int cant, float precioUnit) {
    if (cantItems >= MAX_ITEMS) return false;
    if (cant <= 0 || precioUnit <= 0) return false;
    items[cantItems].idProducto     = idProd;
    items[cantItems].cantidad       = cant;
    items[cantItems].precioUnitario = precioUnit;
    items[cantItems].subTotal       = precioUnit * cant;
    total += items[cantItems].subTotal;
    cantItems++;
    return true;
}

bool Transaccion::esVenta()  const { return strcmp(tipo, "VENTA")  == 0; }
bool Transaccion::esCompra() const { return strcmp(tipo, "COMPRA") == 0; }

// METODOS DE PRESENTACION
void Transaccion::mostrarInformacionBasica() const {
    char fb[40] = "N/A";
    if (fechaCreacion != 0) {
        tm* lt = localtime(&fechaCreacion);
        sprintf(fb, "%04d-%02d-%02d", 1900+lt->tm_year, 1+lt->tm_mon, lt->tm_mday);
    }
    cout << left
         << setw(5)  << id
         << setw(9)  << tipo
         << setw(8)  << cantItems
         << "$ " << setw(10) << fixed << setprecision(2) << total
         << setw(12) << fb
         << setw(10) << idRelacionado
         << "\n";
}

void Transaccion::mostrarInformacionCompleta() const {
    cout << "\n  ID    : " << id        << "\n";
    cout << "  Tipo  : " << tipo        << "\n";
    cout << "  Items : " << cantItems   << "\n";
    for (int k = 0; k < cantItems; k++) {
        cout << "    [" << k+1 << "] Prod#" << items[k].idProducto
             << " x" << items[k].cantidad
             << " @ $" << fixed << setprecision(2) << items[k].precioUnitario
             << " = $" << items[k].subTotal << "\n";
    }
    cout << "  TOTAL : $ " << fixed << setprecision(2) << total << "\n";
    cout << "  Estado: " << (cancelada ? "CANCELADA" : "ACTIVA") << "\n";
}

int Transaccion::obtenerTamano() {
    return sizeof(Transaccion);
}