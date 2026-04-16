#include "GestorArchivos.hpp"
#include "../Persistencias/Constantes.hpp"
using namespace std;

// =====================================================================
// INICIALIZACION
// =====================================================================

// crea el archivo con un header vacio si no existe
// si ya existe no lo toca para no perder los datos
bool GestorArchivos::inicializarArchivo(const char* nombre) {
    ifstream check(nombre, ios::binary);
    if (check.good()) { check.close(); return true; }
    check.close();

    ofstream f(nombre, ios::binary);
    if (!f.is_open()) {
        cerr << "Error al crear el archivo: " << nombre << endl;
        return false;
    }
    ArchivoHeader h;
    h.cantidadRegistros = 0;
    h.proximoId         = 1;
    h.registrosActivos  = 0;
    h.version           = 1;
    f.write(reinterpret_cast<const char*>(&h), sizeof(ArchivoHeader));
    f.close();
    return true;
}

// inicializa todos los archivos del sistema
bool GestorArchivos::inicializarSistemaArchivos() {
    bool ok = true;
    ok &= inicializarArchivo(ARCH_PRODUCTOS);
    ok &= inicializarArchivo(ARCH_PROVEEDORES);
    ok &= inicializarArchivo(ARCH_CLIENTES);
    ok &= inicializarArchivo(ARCH_TRANSACC);
    return ok;
}

// =====================================================================
// HEADER
// =====================================================================

ArchivoHeader GestorArchivos::leerHeader(const char* nombre) {
    ArchivoHeader h;
    h.cantidadRegistros = 0;
    h.proximoId         = 1;
    h.registrosActivos  = 0;
    h.version           = 1;

    ifstream f(nombre, ios::binary);
    if (!f.is_open()) return h;
    f.read(reinterpret_cast<char*>(&h), sizeof(ArchivoHeader));
    f.close();
    return h;
}

// sobreescribe el header en el byte 0 sin tocar los registros
bool GestorArchivos::actualizarHeader(const char* nombre, ArchivoHeader h) {
    fstream f(nombre, ios::binary | ios::in | ios::out);
    if (!f.is_open()) return false;
    f.seekp(0, ios::beg);
    f.write(reinterpret_cast<const char*>(&h), sizeof(ArchivoHeader));
    f.close();
    return true;
}