#include "Cliente.hpp"
#include <cstring>
#include <iostream>
#include <iomanip>
using namespace std;

Cliente::Cliente() {
    id           = 0;
    cantCompras  = 0;
    totalGastado = 0.0f;
    eliminado    = false;
    fechaCreacion= 0;
    fechaModif   = 0;
    memset(nombre,     0, sizeof(nombre));
    memset(cedula,     0, sizeof(cedula));
    memset(telefono,   0, sizeof(telefono));
    memset(email,      0, sizeof(email));
    memset(direccion,  0, sizeof(direccion));
    memset(comprasIDs, 0, sizeof(comprasIDs));
}

Cliente::Cliente(const char* nom, const char* ced, const char* tel, const char* mail)
    : Cliente() {
    setNombre(nom);
    setCedula(ced);
    setTelefono(tel);
    setEmail(mail);
    fechaCreacion = time(0);
    fechaModif    = time(0);
}

Cliente::Cliente(const Cliente& otro) {
    id           = otro.id;
    cantCompras  = otro.cantCompras;
    totalGastado = otro.totalGastado;
    eliminado    = otro.eliminado;
    fechaCreacion= otro.fechaCreacion;
    fechaModif   = otro.fechaModif;
    strncpy(nombre,    otro.nombre,    99);  nombre[99]    = '\0';
    strncpy(cedula,    otro.cedula,    19);  cedula[19]    = '\0';
    strncpy(telefono,  otro.telefono,  19);  telefono[19]  = '\0';
    strncpy(email,     otro.email,     99);  email[99]     = '\0';
    strncpy(direccion, otro.direccion, 199); direccion[199]= '\0';
    for (int i = 0; i < MAX_HIST; i++) comprasIDs[i] = otro.comprasIDs[i];
}

Cliente::~Cliente() {}

// GETTERS
int         Cliente::getId()           const { return id; }
const char* Cliente::getNombre()       const { return nombre; }
const char* Cliente::getCedula()       const { return cedula; }
const char* Cliente::getTelefono()     const { return telefono; }
const char* Cliente::getEmail()        const { return email; }
const char* Cliente::getDireccion()    const { return direccion; }
int         Cliente::getCantCompras()  const { return cantCompras; }
float       Cliente::getTotalGastado() const { return totalGastado; }
bool        Cliente::isEliminado()     const { return eliminado; }
time_t      Cliente::getFechaCreacion()const { return fechaCreacion; }
time_t      Cliente::getFechaModif()   const { return fechaModif; }
const int*  Cliente::getComprasIDs()   const { return comprasIDs; }

// SETTERS
void Cliente::setId(int i)                { id = i; }
void Cliente::setEliminado(bool e)        { eliminado = e; }
void Cliente::setFechaCreacion(time_t f)  { fechaCreacion = f; }
void Cliente::setFechaModif(time_t f)     { fechaModif = f; }

void Cliente::setNombre(const char* n) {
    if (!n) return;
    strncpy(nombre, n, 99); nombre[99] = '\0';
}

void Cliente::setCedula(const char* c) {
    if (!c) return;
    strncpy(cedula, c, 19); cedula[19] = '\0';
}

void Cliente::setTelefono(const char* t) {
    if (!t) return;
    strncpy(telefono, t, 19); telefono[19] = '\0';
}

bool Cliente::setEmail(const char* mail) {
    if (!mail) return false;
    bool arroba = false, punto = false;
    int posArr = -1;
    for (int i = 0; mail[i]; i++) {
        if (mail[i] == '@') { arroba = true; posArr = i; }
        if (arroba && mail[i] == '.' && i > posArr) punto = true;
    }
    if (!arroba || !punto) return false;
    strncpy(email, mail, 99); email[99] = '\0';
    return true;
}

void Cliente::setDireccion(const char* dir) {
    if (!dir) return;
    strncpy(direccion, dir, 199); direccion[199] = '\0';
}

// METODOS DE GESTION
bool Cliente::agregarCompraID(int idTransaccion) {
    if (cantCompras >= MAX_HIST) return false;
    comprasIDs[cantCompras++] = idTransaccion;
    fechaModif = time(0);
    return true;
}

void Cliente::sumarTotalGastado(float monto) {
    if (monto > 0) totalGastado += monto;
}

// METODOS DE PRESENTACION
void Cliente::mostrarInformacionBasica() const {
    cout << left
         << setw(5)  << id
         << setw(25) << nombre
         << setw(15) << cedula
         << setw(15) << telefono
         << setw(10) << cantCompras
         << "$ " << fixed << setprecision(2) << totalGastado
         << "\n";
}

void Cliente::mostrarInformacionCompleta() const {
    cout << "\n  ID       : " << id           << "\n";
    cout << "  Nombre   : " << nombre         << "\n";
    cout << "  Cedula   : " << cedula         << "\n";
    cout << "  Telefono : " << telefono       << "\n";
    cout << "  Email    : " << email          << "\n";
    cout << "  Direccion: " << direccion      << "\n";
    cout << "  Compras  : " << cantCompras    << "\n";
    cout << "  Gastado  : $ " << fixed << setprecision(2) << totalGastado << "\n";
}

int Cliente::obtenerTamano() {
    return sizeof(Cliente);
}