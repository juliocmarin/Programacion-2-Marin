#include "operacionesClientes.hpp"
#include "Cliente.hpp"
#include "../Persistencias/GestorArchivos.hpp"
#include "../Persistencias/Constantes.hpp"
#include "../Utilidades/Validaciones.hpp"
#include "../Utilidades/Formatos.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <ctime>
using namespace std;

void registrarCliente() {
    Formatos::titulo("REGISTRAR CLIENTE");
    cout << "(Escriba 0 o CANCELAR para salir)\n\n";

    Cliente c;
    char tmp[200];

    cout << "Nombre: "; cin.getline(tmp, 100);
    if (Validaciones::esCancelar(tmp)) { cout << "Operacion cancelada.\n"; return; }
    c.setNombre(tmp);

    do {
        cout << "Cedula: "; cin.getline(tmp, 20);
        if (Validaciones::esCancelar(tmp)) { cout << "Operacion cancelada.\n"; return; }
        if (Validaciones::cedulaDuplicada(tmp)) { cout << "ERROR: cedula ya registrada.\n"; return; }
        if (!Validaciones::esNumero(tmp)) cout << "ERROR: ingrese solo numeros.\n";
    } while (!Validaciones::esNumero(tmp));
    c.setCedula(tmp);

    do {
        cout << "Telefono: "; cin.getline(tmp, 20);
        if (Validaciones::esCancelar(tmp)) { cout << "Operacion cancelada.\n"; return; }
        if (!Validaciones::esNumero(tmp)) cout << "ERROR: ingrese solo numeros.\n";
    } while (!Validaciones::esNumero(tmp));
    c.setTelefono(tmp);

    cout << "Email: "; cin.getline(tmp, 100);
    if (Validaciones::esCancelar(tmp)) { cout << "Operacion cancelada.\n"; return; }
    if (!Validaciones::validarEmail(tmp)) { cout << "ERROR: email invalido.\n"; return; }
    c.setEmail(tmp);

    cout << "Direccion: "; cin.getline(tmp, 200);
    if (Validaciones::esCancelar(tmp)) { cout << "Operacion cancelada.\n"; return; }
    c.setDireccion(tmp);

    char conf;
    cout << "\nGuardar cliente? (S/N): "; cin >> conf; Formatos::limpiarBuffer();
    if (conf != 'S' && conf != 's') { cout << "Operacion cancelada.\n"; return; }

    ArchivoHeader h = GestorArchivos::leerHeader(ARCH_CLIENTES);
    c.setId(h.proximoId);
    c.setEliminado(false);
    c.setFechaCreacion(time(0));
    c.setFechaModif(time(0));

    GestorArchivos::guardarRegistro<Cliente>(ARCH_CLIENTES, c);
    cout << "\nCliente registrado. ID: " << c.getId() << "\n";
}

void listarClientes() {
    Formatos::titulo("LISTADO DE CLIENTES");
    ArchivoHeader h = GestorArchivos::leerHeader(ARCH_CLIENTES);
    if (h.registrosActivos == 0) { cout << "No hay clientes.\n"; return; }

    cout << left << setw(5) << "ID" << setw(25) << "Nombre"
         << setw(15) << "Cedula" << setw(15) << "Telefono"
         << setw(10) << "Compras" << setw(15) << "T.Gastado" << "\n";
    Formatos::linea(85);

    ifstream f(ARCH_CLIENTES, ios::binary);
    if (!f.is_open()) { cerr << "Error al abrir clientes.bin\n"; return; }
    f.seekg(sizeof(ArchivoHeader), ios::beg);
    Cliente c;
    while (f.read(reinterpret_cast<char*>(&c), sizeof(Cliente))) {
        if (c.isEliminado()) continue;
        c.mostrarInformacionBasica();
    }
    f.close();
    Formatos::linea(85);
    cout << "Total: " << h.registrosActivos << " clientes.\n";
}

void buscarCliente() {
    Formatos::titulo("BUSCAR CLIENTE");
    cout << "1. Por ID\n2. Por nombre\n3. Por cedula\n0. Cancelar\n";
    cout << "Opcion: ";

    char tmpOp[10]; cin.getline(tmpOp, 10);
    if (Validaciones::esCancelar(tmpOp)) return;
    if (!Validaciones::esNumero(tmpOp)) { cout << "ERROR: ingrese un numero.\n"; return; }
    int op = atoi(tmpOp); if (op == 0) return;

    char fb[40], bus[100];

    if (op == 1) {
        cout << "ID: "; cin.getline(bus, 10);
        if (!Validaciones::esNumero(bus)) { cout << "ERROR: ID debe ser numero.\n"; return; }
        int idx = GestorArchivos::buscarIndicePorId<Cliente>(ARCH_CLIENTES, atoi(bus));
        if (idx == -1) { cout << "No encontrado.\n"; return; }
        Cliente c;
        GestorArchivos::leerRegistroPorIndice<Cliente>(ARCH_CLIENTES, idx, c);
        c.mostrarInformacionCompleta();
        cout << "  Desde    : " << Formatos::formatearFecha(c.getFechaCreacion(), fb) << "\n";
        return;
    }
    if      (op == 2) { cout << "Nombre: "; cin.getline(bus, 100); }
    else if (op == 3) { cout << "Cedula: "; cin.getline(bus, 20);  }
    else { cout << "Opcion invalida.\n"; return; }

    bool enc = false;
    ifstream f(ARCH_CLIENTES, ios::binary);
    if (!f.is_open()) { cerr << "Error al abrir clientes.bin\n"; return; }
    f.seekg(sizeof(ArchivoHeader), ios::beg);
    Cliente c;
    while (f.read(reinterpret_cast<char*>(&c), sizeof(Cliente))) {
        if (c.isEliminado()) continue;
        bool ok = (op == 2) ? Formatos::contieneStr(c.getNombre(), bus)
                            : Formatos::contieneStr(c.getCedula(), bus);
        if (ok) {
            cout << "  [" << c.getId() << "] " << c.getNombre()
                 << " CI:" << c.getCedula() << " Tel:" << c.getTelefono() << "\n";
            enc = true;
        }
    }
    f.close();
    if (!enc) cout << "Sin resultados.\n";
}

void actualizarCliente() {
    Formatos::titulo("ACTUALIZAR CLIENTE");
    char tmpId[10];
    cout << "ID (0 para cancelar): "; cin.getline(tmpId, 10);
    if (Validaciones::esCancelar(tmpId)) { cout << "Operacion cancelada.\n"; return; }
    if (!Validaciones::esNumero(tmpId))  { cout << "ERROR: ingrese un numero.\n"; return; }

    int idx = GestorArchivos::buscarIndicePorId<Cliente>(ARCH_CLIENTES, atoi(tmpId));
    if (idx == -1) { cout << "No encontrado.\n"; return; }

    Cliente c;
    GestorArchivos::leerRegistroPorIndice<Cliente>(ARCH_CLIENTES, idx, c);
    char tmp[200]; int op;

    do {
        cout << "\n1. Nombre   (" << c.getNombre()   << ")\n";
        cout << "2. Telefono (" << c.getTelefono() << ")\n";
        cout << "3. Email    (" << c.getEmail()    << ")\n";
        cout << "4. Direccion\n0. Guardar y salir\n";
        cout << "Opcion: "; cin >> op; Formatos::limpiarBuffer();
        switch(op) {
        case 1: cout << "Nombre: ";   cin.getline(tmp,100); if(!Validaciones::esCancelar(tmp)) c.setNombre(tmp);   else cout<<"Campo no modificado.\n"; break;
        case 2:
            do {
                cout << "Telefono: "; cin.getline(tmp, 20);
                if (Validaciones::esCancelar(tmp)) { cout << "Campo no modificado.\n"; break; }
                if (!Validaciones::esNumero(tmp)) cout << "ERROR: solo numeros.\n";
            } while (!Validaciones::esNumero(tmp) && !Validaciones::esCancelar(tmp));
            if (!Validaciones::esCancelar(tmp)) c.setTelefono(tmp);
            break;
        case 3:
            cout << "Email: "; cin.getline(tmp, 100);
            if (!Validaciones::esCancelar(tmp)) {
                if (Validaciones::validarEmail(tmp)) c.setEmail(tmp);
                else cout << "Email invalido.\n";
            } else cout << "Campo no modificado.\n";
            break;
        case 4: cout << "Direccion: "; cin.getline(tmp,200); if(!Validaciones::esCancelar(tmp)) c.setDireccion(tmp); else cout<<"Campo no modificado.\n"; break;
        }
    } while (op != 0);

    c.setFechaModif(time(0));
    GestorArchivos::actualizarRegistro<Cliente>(ARCH_CLIENTES, idx, c);
    cout << "\nCliente actualizado.\n";
}

void eliminarCliente() {
    Formatos::titulo("ELIMINAR CLIENTE");
    char tmpId[10];
    cout << "ID (0 para cancelar): "; cin.getline(tmpId, 10);
    if (Validaciones::esCancelar(tmpId)) { cout << "Operacion cancelada.\n"; return; }
    if (!Validaciones::esNumero(tmpId))  { cout << "ERROR: ingrese un numero.\n"; return; }

    int idx = GestorArchivos::buscarIndicePorId<Cliente>(ARCH_CLIENTES, atoi(tmpId));
    if (idx == -1) { cout << "No encontrado.\n"; return; }

    Cliente c;
    GestorArchivos::leerRegistroPorIndice<Cliente>(ARCH_CLIENTES, idx, c);

    char conf;
    cout << "Eliminar '" << c.getNombre() << "'? (S/N): "; cin >> conf; Formatos::limpiarBuffer();
    if (conf != 'S' && conf != 's') { cout << "Operacion cancelada.\n"; return; }

    c.setEliminado(true);
    c.setFechaModif(time(0));
    GestorArchivos::actualizarRegistro<Cliente>(ARCH_CLIENTES, idx, c);

    ArchivoHeader h = GestorArchivos::leerHeader(ARCH_CLIENTES);
    h.registrosActivos--;
    GestorArchivos::actualizarHeader(ARCH_CLIENTES, h);
    cout << "\nCliente eliminado.\n";
}