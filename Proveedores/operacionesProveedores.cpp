#include "operacionesProveedores.hpp"
#include "Proveedor.hpp"
#include "../Persistencias/GestorArchivos.hpp"
#include "../Persistencias/Constantes.hpp"
#include "../Utilidades/Validaciones.hpp"
#include "../Utilidades/Formatos.hpp"
#include "../Productos/Producto.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <ctime>
using namespace std;

void registrarProveedor() {
    Formatos::titulo("REGISTRAR PROVEEDOR");
    cout << "(Escriba 0 o CANCELAR para salir)\n\n";

    Proveedor pv;
    char tmp[200];

    cout << "Nombre: "; cin.getline(tmp, 100);
    if (Validaciones::esCancelar(tmp)) { cout << "Operacion cancelada.\n"; return; }
    pv.setNombre(tmp);

    cout << "RIF: "; cin.getline(tmp, 20);
    if (Validaciones::esCancelar(tmp)) { cout << "Operacion cancelada.\n"; return; }
    if (Validaciones::rifDuplicado(tmp)) { cout << "ERROR: RIF ya registrado.\n"; return; }
    pv.setRif(tmp);

    do {
        cout << "Telefono: "; cin.getline(tmp, 20);
        if (Validaciones::esCancelar(tmp)) { cout << "Operacion cancelada.\n"; return; }
        if (!Validaciones::esNumero(tmp)) cout << "ERROR: ingrese solo numeros.\n";
    } while (!Validaciones::esNumero(tmp));
    pv.setTelefono(tmp);

    cout << "Email: "; cin.getline(tmp, 100);
    if (Validaciones::esCancelar(tmp)) { cout << "Operacion cancelada.\n"; return; }
    if (!Validaciones::validarEmail(tmp)) { cout << "ERROR: email invalido.\n"; return; }
    pv.setEmail(tmp);

    cout << "Direccion: "; cin.getline(tmp, 200);
    if (Validaciones::esCancelar(tmp)) { cout << "Operacion cancelada.\n"; return; }
    pv.setDireccion(tmp);

    char conf;
    cout << "\nGuardar proveedor? (S/N): "; cin >> conf; Formatos::limpiarBuffer();
    if (conf != 'S' && conf != 's') { cout << "Operacion cancelada.\n"; return; }

    ArchivoHeader h = GestorArchivos::leerHeader(ARCH_PROVEEDORES);
    pv.setId(h.proximoId);
    pv.setEliminado(false);
    pv.setFechaCreacion(time(0));
    pv.setFechaModif(time(0));

    GestorArchivos::guardarRegistro<Proveedor>(ARCH_PROVEEDORES, pv);
    cout << "\nProveedor registrado. ID: " << pv.getId() << "\n";
}

void listarProveedores() {
    Formatos::titulo("LISTADO DE PROVEEDORES");
    ArchivoHeader h = GestorArchivos::leerHeader(ARCH_PROVEEDORES);
    if (h.registrosActivos == 0) { cout << "No hay proveedores.\n"; return; }

    cout << left << setw(5) << "ID" << setw(25) << "Nombre"
         << setw(15) << "RIF" << setw(15) << "Telefono" << setw(8) << "Prods" << "\n";
    Formatos::linea(68);

    ifstream f(ARCH_PROVEEDORES, ios::binary);
    if (!f.is_open()) { cerr << "Error al abrir proveedores.bin\n"; return; }
    f.seekg(sizeof(ArchivoHeader), ios::beg);
    Proveedor pv;
    while (f.read(reinterpret_cast<char*>(&pv), sizeof(Proveedor))) {
        if (pv.isEliminado()) continue;
        pv.mostrarInformacionBasica();
    }
    f.close();
    Formatos::linea(68);
    cout << "Total: " << h.registrosActivos << " proveedores.\n";
}

void buscarProveedor() {
    Formatos::titulo("BUSCAR PROVEEDOR");
    cout << "1. Por ID\n2. Por nombre\n3. Por RIF\n0. Cancelar\n";
    cout << "Opcion: ";

    char tmpOp[10]; cin.getline(tmpOp, 10);
    if (Validaciones::esCancelar(tmpOp)) return;
    if (!Validaciones::esNumero(tmpOp)) { cout << "ERROR: ingrese un numero.\n"; return; }
    int op = atoi(tmpOp); if (op == 0) return;

    char fb[40], bus[100];

    if (op == 1) {
        cout << "ID: "; cin.getline(bus, 10);
        if (!Validaciones::esNumero(bus)) { cout << "ERROR: ID debe ser numero.\n"; return; }
        int idx = GestorArchivos::buscarIndicePorId<Proveedor>(ARCH_PROVEEDORES, atoi(bus));
        if (idx == -1) { cout << "No encontrado.\n"; return; }
        Proveedor pv;
        GestorArchivos::leerRegistroPorIndice<Proveedor>(ARCH_PROVEEDORES, idx, pv);
        pv.mostrarInformacionCompleta();
        cout << "  Registro  : " << Formatos::formatearFecha(pv.getFechaCreacion(), fb) << "\n";
        if (pv.getCantProductos() > 0) {
            cout << "\n  Productos de este proveedor:\n";
            for (int j = 0; j < pv.getCantProductos(); j++) {
                int idxP = GestorArchivos::buscarIndicePorId<Producto>(ARCH_PRODUCTOS, pv.getProductosIds()[j]);
                if (idxP != -1) {
                    Producto p;
                    GestorArchivos::leerRegistroPorIndice<Producto>(ARCH_PRODUCTOS, idxP, p);
                    if (!p.isEliminado())
                        cout << "    [" << p.getId() << "] " << p.getNombre()
                             << " Talla " << p.getTalla() << " Stock:" << p.getStock() << "\n";
                }
            }
        }
        return;
    }
    if      (op == 2) { cout << "Nombre: "; cin.getline(bus, 100); }
    else if (op == 3) { cout << "RIF: ";    cin.getline(bus, 20);  }
    else { cout << "Opcion invalida.\n"; return; }

    bool enc = false;
    ifstream f(ARCH_PROVEEDORES, ios::binary);
    if (!f.is_open()) { cerr << "Error al abrir proveedores.bin\n"; return; }
    f.seekg(sizeof(ArchivoHeader), ios::beg);
    Proveedor pv;
    while (f.read(reinterpret_cast<char*>(&pv), sizeof(Proveedor))) {
        if (pv.isEliminado()) continue;
        bool ok = (op == 2) ? Formatos::contieneStr(pv.getNombre(), bus)
                            : Formatos::contieneStr(pv.getRif(),    bus);
        if (ok) { cout << "  [" << pv.getId() << "] " << pv.getNombre() << " - " << pv.getRif() << "\n"; enc = true; }
    }
    f.close();
    if (!enc) cout << "Sin resultados.\n";
}

void actualizarProveedor() {
    Formatos::titulo("EDITAR PROVEEDOR");
    char tmpId[10];
    cout << "ID (0 para cancelar): "; cin.getline(tmpId, 10);
    if (Validaciones::esCancelar(tmpId)) { cout << "Operacion cancelada.\n"; return; }
    if (!Validaciones::esNumero(tmpId))  { cout << "ERROR: ingrese un numero.\n"; return; }

    int idx = GestorArchivos::buscarIndicePorId<Proveedor>(ARCH_PROVEEDORES, atoi(tmpId));
    if (idx == -1) { cout << "No encontrado.\n"; return; }

    Proveedor pv;
    GestorArchivos::leerRegistroPorIndice<Proveedor>(ARCH_PROVEEDORES, idx, pv);
    char tmp[200]; int op;

    do {
        cout << "\n1. Nombre   (" << pv.getNombre()   << ")\n";
        cout << "2. Telefono (" << pv.getTelefono() << ")\n";
        cout << "3. Email    (" << pv.getEmail()    << ")\n";
        cout << "4. Direccion\n0. Guardar y salir\n";
        cout << "Opcion: "; cin >> op; Formatos::limpiarBuffer();
        switch(op) {
        case 1: cout << "Nombre: ";   cin.getline(tmp,100); if(!Validaciones::esCancelar(tmp)) pv.setNombre(tmp);   else cout<<"Campo no modificado.\n"; break;
        case 2:
            do {
                cout << "Telefono: "; cin.getline(tmp, 20);
                if (Validaciones::esCancelar(tmp)) { cout << "Campo no modificado.\n"; break; }
                if (!Validaciones::esNumero(tmp)) cout << "ERROR: solo numeros.\n";
            } while (!Validaciones::esNumero(tmp) && !Validaciones::esCancelar(tmp));
            if (!Validaciones::esCancelar(tmp)) pv.setTelefono(tmp);
            break;
        case 3:
            cout << "Email: "; cin.getline(tmp, 100);
            if (!Validaciones::esCancelar(tmp)) {
                if (Validaciones::validarEmail(tmp)) pv.setEmail(tmp);
                else cout << "Email invalido.\n";
            } else cout << "Campo no modificado.\n";
            break;
        case 4: cout << "Direccion: "; cin.getline(tmp,200); if(!Validaciones::esCancelar(tmp)) pv.setDireccion(tmp); else cout<<"Campo no modificado.\n"; break;
        }
    } while (op != 0);

    pv.setFechaModif(time(0));
    GestorArchivos::actualizarRegistro<Proveedor>(ARCH_PROVEEDORES, idx, pv);
    cout << "\nProveedor actualizado.\n";
}

void eliminarProveedor() {
    Formatos::titulo("ELIMINAR PROVEEDOR");
    char tmpId[10];
    cout << "ID (0 para cancelar): "; cin.getline(tmpId, 10);
    if (Validaciones::esCancelar(tmpId)) { cout << "Operacion cancelada.\n"; return; }
    if (!Validaciones::esNumero(tmpId))  { cout << "ERROR: ingrese un numero.\n"; return; }

    int idx = GestorArchivos::buscarIndicePorId<Proveedor>(ARCH_PROVEEDORES, atoi(tmpId));
    if (idx == -1) { cout << "No encontrado.\n"; return; }

    Proveedor pv;
    GestorArchivos::leerRegistroPorIndice<Proveedor>(ARCH_PROVEEDORES, idx, pv);
    if (pv.getCantProductos() > 0)
        cout << "ATENCION: tiene " << pv.getCantProductos() << " productos vinculados.\n";

    char conf;
    cout << "Eliminar '" << pv.getNombre() << "'? (S/N): "; cin >> conf; Formatos::limpiarBuffer();
    if (conf != 'S' && conf != 's') { cout << "Operacion cancelada.\n"; return; }

    pv.setEliminado(true);
    pv.setFechaModif(time(0));
    GestorArchivos::actualizarRegistro<Proveedor>(ARCH_PROVEEDORES, idx, pv);

    ArchivoHeader h = GestorArchivos::leerHeader(ARCH_PROVEEDORES);
    h.registrosActivos--;
    GestorArchivos::actualizarHeader(ARCH_PROVEEDORES, h);
    cout << "\nProveedor eliminado.\n";
}