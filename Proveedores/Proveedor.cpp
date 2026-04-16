#include "Proveedor.hpp"
#include <cstring>
#include <iostream>
#include <iomanip>
using namespace std;

// =====================================================================
// CONSTRUCTORES
// =====================================================================

Proveedor::Proveedor() {
    id             = 0;
    cantProductos  = 0;
    cantTrans      = 0;
    totalComprado  = 0.0f;
    eliminado      = false;
    fechaCreacion  = 0;
    fechaModif     = 0;
    memset(nombre,      0, sizeof(nombre));
    memset(rif,         0, sizeof(rif));
    memset(telefono,    0, sizeof(telefono));
    memset(email,       0, sizeof(email));
    memset(direccion,   0, sizeof(direccion));
    memset(productosIds,0, sizeof(productosIds));
    memset(transIDs,    0, sizeof(transIDs));
}

Proveedor::Proveedor(const char* nom, const char* r, const char* tel, const char* mail)
    : Proveedor() {
    setNombre(nom);
    setRif(r);
    setTelefono(tel);
    setEmail(mail);
    fechaCreacion = time(0);
    fechaModif    = time(0);
}

Proveedor::Proveedor(const Proveedor& otro) {
    id            = otro.id;
    cantProductos = otro.cantProductos;
    cantTrans     = otro.cantTrans;
    totalComprado = otro.totalComprado;
    eliminado     = otro.eliminado;
    fechaCreacion = otro.fechaCreacion;
    fechaModif    = otro.fechaModif;
    strncpy(nombre,    otro.nombre,    99);  nombre[99]    = '\0';
    strncpy(rif,       otro.rif,       19);  rif[19]       = '\0';
    strncpy(telefono,  otro.telefono,  19);  telefono[19]  = '\0';
    strncpy(email,     otro.email,     99);  email[99]     = '\0';
    strncpy(direccion, otro.direccion, 199); direccion[199]= '\0';
    for (int i = 0; i < MAX_PRODS; i++) productosIds[i] = otro.productosIds[i];
    for (int i = 0; i < MAX_HIST;  i++) transIDs[i]     = otro.transIDs[i];
}

Proveedor::~Proveedor() {}

// =====================================================================
// GETTERS
// =====================================================================

int         Proveedor::getId()            const { return id; }
const char* Proveedor::getNombre()        const { return nombre; }
const char* Proveedor::getRif()           const { return rif; }
const char* Proveedor::getTelefono()      const { return telefono; }
const char* Proveedor::getEmail()         const { return email; }
const char* Proveedor::getDireccion()     const { return direccion; }
int         Proveedor::getCantProductos() const { return cantProductos; }
int         Proveedor::getCantTrans()     const { return cantTrans; }
float       Proveedor::getTotalComprado() const { return totalComprado; }
bool        Proveedor::isEliminado()      const { return eliminado; }
time_t      Proveedor::getFechaCreacion() const { return fechaCreacion; }
time_t      Proveedor::getFechaModif()    const { return fechaModif; }
const int*  Proveedor::getProductosIds()  const { return productosIds; }
const int*  Proveedor::getTransIDs()      const { return transIDs; }

// =====================================================================
// SETTERS
// =====================================================================

void Proveedor::setId(int i)                  { id = i; }
void Proveedor::setEliminado(bool e)          { eliminado = e; }
void Proveedor::setFechaCreacion(time_t f)    { fechaCreacion = f; }
void Proveedor::setFechaModif(time_t f)       { fechaModif = f; }

void Proveedor::setNombre(const char* n) {
    if (!n) return;
    strncpy(nombre, n, 99); nombre[99] = '\0';
}

void Proveedor::setRif(const char* r) {
    if (!r) return;
    strncpy(rif, r, 19); rif[19] = '\0';
}

void Proveedor::setTelefono(const char* t) {
    if (!t) return;
    strncpy(telefono, t, 19); telefono[19] = '\0';
}

bool Proveedor::setEmail(const char* mail) {
    if (!mail) return false;
    // validacion basica: debe tener @ y punto despues del @
    bool arroba = false;
    bool punto  = false;
    int  posArr = -1;
    for (int i = 0; mail[i]; i++) {
        if (mail[i] == '@') { arroba = true; posArr = i; }
        if (arroba && mail[i] == '.' && i > posArr) punto = true;
    }
    if (!arroba || !punto) return false;
    strncpy(email, mail, 99); email[99] = '\0';
    return true;
}

void Proveedor::setDireccion(const char* dir) {
    if (!dir) return;
    strncpy(direccion, dir, 199); direccion[199] = '\0';
}

// =====================================================================
// METODOS DE GESTION DE RELACIONES
// =====================================================================

bool Proveedor::agregarProductoID(int idProducto) {
    if (cantProductos >= MAX_PRODS) return false;
    productosIds[cantProductos++] = idProducto;
    fechaModif = time(0);
    return true;
}

bool Proveedor::agregarTransaccionID(int idTransaccion) {
    if (cantTrans >= MAX_HIST) return false;
    transIDs[cantTrans++] = idTransaccion;
    fechaModif = time(0);
    return true;
}

void Proveedor::sumarTotalComprado(float monto) {
    if (monto > 0) totalComprado += monto;
}

// =====================================================================
// METODOS DE PRESENTACION
// =====================================================================

void Proveedor::mostrarInformacionBasica() const {
    cout << left
         << setw(5)  << id
         << setw(25) << nombre
         << setw(15) << rif
         << setw(15) << telefono
         << setw(8)  << cantProductos
         << "\n";
}

void Proveedor::mostrarInformacionCompleta() const {
    cout << "\n  ID          : " << id            << "\n";
    cout << "  Nombre      : " << nombre          << "\n";
    cout << "  RIF         : " << rif             << "\n";
    cout << "  Telefono    : " << telefono        << "\n";
    cout << "  Email       : " << email           << "\n";
    cout << "  Direccion   : " << direccion       << "\n";
    cout << "  Productos   : " << cantProductos   << "\n";
    cout << "  T.Comprado  : $ " << fixed << setprecision(2) << totalComprado << "\n";
}

// =====================================================================
// METODO ESTATICO
// =====================================================================

int Proveedor::obtenerTamano() {
    return sizeof(Proveedor);
}