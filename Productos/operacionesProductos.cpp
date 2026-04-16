#include "operacionesProductos.hpp"
#include "Producto.hpp"
#include "../Persistencias/GestorArchivos.hpp"
#include "../Persistencias/Constantes.hpp"
#include "../Utilidades/Validaciones.hpp"
#include "../Utilidades/Formatos.hpp"
#include "../Proveedores/Proveedor.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>
#include <ctime>
using namespace std;

// =====================================================================
// REGISTRAR PRODUCTO
// =====================================================================

void registrarProducto() {
    Formatos::titulo("REGISTRAR NUEVO CALZADO");
    cout << "(Escriba 0 o CANCELAR en cualquier campo para salir)\n\n";

    Producto p;
    char tmp[200];

    // CODIGO //
    cout << "Codigo (ej. CAL-001): ";
    cin.getline(tmp, 20);
    if (Validaciones::esCancelar(tmp)) { cout << "Operacion cancelada.\n"; return; }
    if (Validaciones::codigoDuplicado(tmp)) { cout << "ERROR: el codigo ya existe.\n"; return; }
    p.setCodigo(tmp);

    // MARCA - do-while hasta que sea valida //
    do {
        cout << "Marca (Nike, Adidas o Puma): ";
        cin.getline(tmp, 50);
        if (Validaciones::esCancelar(tmp)) { cout << "Operacion cancelada.\n"; return; }
        if (!Validaciones::validarMarca(tmp)) cout << "ERROR: solo Nike, Adidas o Puma.\n";
    } while (!Validaciones::validarMarca(tmp));
    p.setMarca(tmp);

    cout << "Modelo: ";
    cin.getline(tmp, 50);
    if (Validaciones::esCancelar(tmp)) { cout << "Operacion cancelada.\n"; return; }
    p.setModelo(tmp);

    cout << "Nombre: ";
    cin.getline(tmp, 100);
    if (Validaciones::esCancelar(tmp)) { cout << "Operacion cancelada.\n"; return; }
    p.setNombre(tmp);

    // TALLA - do-while hasta que sea numero //
    do {
        cout << "Talla: ";
        cin.getline(tmp, 10);
        if (Validaciones::esCancelar(tmp)) { cout << "Operacion cancelada.\n"; return; }
        if (!Validaciones::esNumero(tmp)) cout << "ERROR: la talla debe ser un numero.\n";
    } while (!Validaciones::esNumero(tmp));
    p.setTalla(tmp);

    cout << "Color: ";
    cin.getline(tmp, 30);
    if (Validaciones::esCancelar(tmp)) { cout << "Operacion cancelada.\n"; return; }
    p.setColor(tmp);

    cout << "Genero (Hombre/Mujer/Unisex): ";
    cin.getline(tmp, 20);
    if (Validaciones::esCancelar(tmp)) { cout << "Operacion cancelada.\n"; return; }
    p.setGenero(tmp);

    cout << "Material: ";
    cin.getline(tmp, 50);
    if (Validaciones::esCancelar(tmp)) { cout << "Operacion cancelada.\n"; return; }
    p.setMaterial(tmp);

    cout << "Descripcion: ";
    cin.getline(tmp, 200);
    if (Validaciones::esCancelar(tmp)) { cout << "Operacion cancelada.\n"; return; }
    p.setDescripcion(tmp);

    // MOSTRAR PROVEEDORES DISPONIBLES //
    cout << "\n--- Proveedores disponibles ---\n";
    ArchivoHeader hpv = GestorArchivos::leerHeader(ARCH_PROVEEDORES);
    if (hpv.registrosActivos == 0) { cout << "ERROR: no hay proveedores, registre uno primero.\n"; return; }
    {
        ifstream fp(ARCH_PROVEEDORES, ios::binary);
        fp.seekg(sizeof(ArchivoHeader), ios::beg);
        Proveedor pvTmp;
        while (fp.read(reinterpret_cast<char*>(&pvTmp), sizeof(Proveedor)))
            if (!pvTmp.isEliminado())
                cout << "  [" << pvTmp.getId() << "] " << pvTmp.getNombre() << "\n";
        fp.close();
    }

    // ID PROVEEDOR - do-while hasta que sea numero valido //
    int idPv = 0;
    do {
        cout << "ID Proveedor: ";
        cin.getline(tmp, 10);
        if (Validaciones::esCancelar(tmp)) { cout << "Operacion cancelada.\n"; return; }
        if (!Validaciones::esNumero(tmp)) { cout << "ERROR: el ID debe ser un numero.\n"; continue; }
        idPv = atoi(tmp);
        if (!Validaciones::existeProveedor(idPv)) { cout << "ERROR: proveedor no existe.\n"; idPv = 0; }
    } while (idPv == 0);
    p.setIdProveedor(idPv);

    // PRECIO COMPRA - do-while //
    do {
        cout << "Precio Compra: ";
        cin.getline(tmp, 20);
        if (Validaciones::esCancelar(tmp)) { cout << "Operacion cancelada.\n"; return; }
        if (!Validaciones::esNumero(tmp) || atof(tmp) <= 0) cout << "ERROR: precio invalido.\n";
    } while (!Validaciones::esNumero(tmp) || atof(tmp) <= 0);
    p.setPrecioCompra(atof(tmp));

    // PRECIO VENTA - do-while //
    do {
        cout << "Precio Venta: ";
        cin.getline(tmp, 20);
        if (Validaciones::esCancelar(tmp)) { cout << "Operacion cancelada.\n"; return; }
        if (!Validaciones::esNumero(tmp) || atof(tmp) <= 0) cout << "ERROR: precio invalido.\n";
    } while (!Validaciones::esNumero(tmp) || atof(tmp) <= 0);
    p.setPrecio(atof(tmp));

    // STOCK - do-while //
    do {
        cout << "Stock inicial: ";
        cin.getline(tmp, 10);
        if (Validaciones::esCancelar(tmp)) { cout << "Operacion cancelada.\n"; return; }
        if (!Validaciones::esNumero(tmp) || atoi(tmp) < 0) cout << "ERROR: stock invalido.\n";
    } while (!Validaciones::esNumero(tmp) || atoi(tmp) < 0);
    p.setStock(atoi(tmp));

    // STOCK MINIMO - si falla usa 5 por defecto //
    cout << "Stock minimo (Enter para usar 5): ";
    cin.getline(tmp, 10);
    if (Validaciones::esNumero(tmp) && atoi(tmp) >= 0) p.setStockMinimo(atoi(tmp));
    else { p.setStockMinimo(5); cout << "Valor invalido, se uso 5.\n"; }

    // RESUMEN //
    cout << "\n";
    Formatos::linea(40);
    cout << "  RESUMEN\n";
    Formatos::linea(40);
    cout << "  Codigo : " << p.getCodigo() << "\n";
    cout << "  Marca  : " << p.getMarca()  << " - " << p.getModelo() << "\n";
    cout << "  Nombre : " << p.getNombre() << "\n";
    cout << "  Talla  : " << p.getTalla()  << "\n";
    cout << "  Precio : $ " << fixed << setprecision(2) << p.getPrecio() << "\n";
    cout << "  Stock  : " << p.getStock() << "\n";

    char conf;
    cout << "\nGuardar calzado? (S/N): ";
    cin >> conf; Formatos::limpiarBuffer();
    if (conf != 'S' && conf != 's') { cout << "Operacion cancelada.\n"; return; }

    // asignar ID y fechas antes de guardar //
    ArchivoHeader h = GestorArchivos::leerHeader(ARCH_PRODUCTOS);
    p.setId(h.proximoId);
    p.setEliminado(false);
    p.setFechaCreacion(time(0));
    p.setFechaModif(time(0));

    GestorArchivos::guardarRegistro<Producto>(ARCH_PRODUCTOS, p);

    // vincular producto al proveedor //
    int idxPv = GestorArchivos::buscarIndicePorId<Proveedor>(ARCH_PROVEEDORES, idPv);
    if (idxPv != -1) {
        Proveedor pv;
        GestorArchivos::leerRegistroPorIndice<Proveedor>(ARCH_PROVEEDORES, idxPv, pv);
        pv.agregarProductoID(p.getId());
        GestorArchivos::actualizarRegistro<Proveedor>(ARCH_PROVEEDORES, idxPv, pv);
    }

    cout << "\nCalzado registrado. ID asignado: " << p.getId() << "\n";
}

// =====================================================================
// LISTAR PRODUCTOS
// =====================================================================

void listarProductos() {
    Formatos::titulo("LISTADO DE CALZADOS");
    ArchivoHeader h = GestorArchivos::leerHeader(ARCH_PRODUCTOS);
    if (h.registrosActivos == 0) { cout << "No hay calzados registrados.\n"; return; }

    cout << left
         << setw(5)  << "ID"
         << setw(12) << "Codigo"
         << setw(20) << "Nombre"
         << setw(10) << "Marca"
         << setw(7)  << "Talla"
         << setw(12) << "P.Venta"
         << setw(7)  << "Stock"
         << "\n";
    Formatos::linea(73);

    ifstream f(ARCH_PRODUCTOS, ios::binary);
    if (!f.is_open()) { cerr << "Error al abrir productos.bin\n"; return; }
    f.seekg(sizeof(ArchivoHeader), ios::beg);
    Producto p;
    while (f.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.isEliminado()) continue;
        if (p.stockCritico()) cout << "* "; // marca stock critico
        else cout << "  ";
        p.mostrarInformacionBasica();
    }
    f.close();
    Formatos::linea(73);
    cout << "Activos: " << h.registrosActivos << "  (* = stock critico)\n";
}

// =====================================================================
// BUSCAR PRODUCTO
// =====================================================================

void buscarProducto() {
    Formatos::titulo("BUSCAR CALZADO");
    cout << "1. Por ID\n2. Por nombre\n3. Por codigo\n4. Por marca\n5. Por talla\n0. Cancelar\n";
    cout << "Opcion: ";

    char tmpOp[10]; cin.getline(tmpOp, 10);
    if (Validaciones::esCancelar(tmpOp)) return;
    if (!Validaciones::esNumero(tmpOp)) { cout << "ERROR: ingrese un numero.\n"; return; }
    int op = atoi(tmpOp);
    if (op == 0) return;

    char fb[40], bus[100];

    if (op == 1) {
        cout << "ID: "; cin.getline(bus, 10);
        if (!Validaciones::esNumero(bus)) { cout << "ERROR: ID debe ser numero.\n"; return; }
        int idx = GestorArchivos::buscarIndicePorId<Producto>(ARCH_PRODUCTOS, atoi(bus));
        if (idx == -1) { cout << "No encontrado.\n"; return; }
        Producto p;
        GestorArchivos::leerRegistroPorIndice<Producto>(ARCH_PRODUCTOS, idx, p);

        // buscar nombre del proveedor
        char nomPv[100] = "Desconocido";
        int idxPv = GestorArchivos::buscarIndicePorId<Proveedor>(ARCH_PROVEEDORES, p.getIdProveedor());
        if (idxPv != -1) {
            Proveedor pv;
            GestorArchivos::leerRegistroPorIndice<Proveedor>(ARCH_PROVEEDORES, idxPv, pv);
            strncpy(nomPv, pv.getNombre(), 99);
        }
        p.mostrarInformacionCompleta();
        cout << "  Proveedor   : [" << p.getIdProveedor() << "] " << nomPv << "\n";
        cout << "  Registrado  : " << Formatos::formatearFecha(p.getFechaCreacion(), fb) << "\n";
        return;
    }

    if      (op == 2) { cout << "Nombre: "; cin.getline(bus, 100); }
    else if (op == 3) { cout << "Codigo: "; cin.getline(bus, 20); }
    else if (op == 4) {
        do {
            cout << "Marca (Nike/Adidas/Puma): "; cin.getline(bus, 50);
            if (!Validaciones::validarMarca(bus)) cout << "ERROR: solo Nike, Adidas o Puma.\n";
        } while (!Validaciones::validarMarca(bus));
    }
    else if (op == 5) {
        cout << "Talla: "; cin.getline(bus, 10);
        if (!Validaciones::esNumero(bus)) { cout << "ERROR: talla debe ser numero.\n"; return; }
    }
    else { cout << "Opcion invalida.\n"; return; }

    bool enc = false;
    ifstream f(ARCH_PRODUCTOS, ios::binary);
    if (!f.is_open()) { cerr << "Error al abrir productos.bin\n"; return; }
    f.seekg(sizeof(ArchivoHeader), ios::beg);
    Producto p;
    while (f.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (p.isEliminado()) continue;
        bool ok = false;
        if      (op == 2) ok = Formatos::contieneStr(p.getNombre(), bus);
        else if (op == 3) ok = Formatos::contieneStr(p.getCodigo(), bus);
        else if (op == 4) ok = Formatos::contieneStr(p.getMarca(),  bus);
        else if (op == 5) ok = (strcmp(p.getTalla(), bus) == 0);
        if (ok) {
            cout << "  [" << p.getId() << "] " << p.getNombre()
                 << " (" << p.getMarca() << ") Talla " << p.getTalla()
                 << " - Stock: " << p.getStock()
                 << " - $" << fixed << setprecision(2) << p.getPrecio() << "\n";
            enc = true;
        }
    }
    f.close();
    if (!enc) cout << "Sin resultados.\n";
}

// =====================================================================
// ACTUALIZAR PRODUCTO
// =====================================================================

void actualizarProducto() {
    Formatos::titulo("EDITAR CALZADO");
    char tmpId[10];
    cout << "ID (0 para cancelar): "; cin.getline(tmpId, 10);
    if (Validaciones::esCancelar(tmpId)) { cout << "Operacion cancelada.\n"; return; }
    if (!Validaciones::esNumero(tmpId))  { cout << "ERROR: ingrese un numero.\n"; return; }

    int idx = GestorArchivos::buscarIndicePorId<Producto>(ARCH_PRODUCTOS, atoi(tmpId));
    if (idx == -1) { cout << "No encontrado.\n"; return; }

    Producto p;
    GestorArchivos::leerRegistroPorIndice<Producto>(ARCH_PRODUCTOS, idx, p);
    char tmp[200];
    int op;

    do {
        cout << "\n[" << p.getNombre() << "]\n";
        cout << "1. Marca      (" << p.getMarca()       << ")\n";
        cout << "2. Modelo     (" << p.getModelo()      << ")\n";
        cout << "3. Nombre     (" << p.getNombre()      << ")\n";
        cout << "4. Talla      (" << p.getTalla()       << ")\n";
        cout << "5. Color      (" << p.getColor()       << ")\n";
        cout << "6. Genero     (" << p.getGenero()      << ")\n";
        cout << "7. Material   (" << p.getMaterial()    << ")\n";
        cout << "8. Descripcion\n";
        cout << "9. Precio venta    ($" << fixed << setprecision(2) << p.getPrecio()       << ")\n";
        cout << "10. Precio compra  ($" << fixed << setprecision(2) << p.getPrecioCompra() << ")\n";
        cout << "11. Stock minimo   (" << p.getStockMinimo() << ")\n";
        cout << "0. Guardar y salir\n";
        cout << "Opcion: "; cin >> op; Formatos::limpiarBuffer();

        switch (op) {
        case 1:
            do {
                cout << "Marca (Nike/Adidas/Puma): "; cin.getline(tmp, 50);
                if (Validaciones::esCancelar(tmp)) { cout << "Campo no modificado.\n"; break; }
                if (!Validaciones::validarMarca(tmp)) cout << "ERROR: solo Nike, Adidas o Puma.\n";
            } while (!Validaciones::validarMarca(tmp));
            if (!Validaciones::esCancelar(tmp)) p.setMarca(tmp);
            break;
        case 2:  cout << "Modelo: ";   cin.getline(tmp,50);  if(!Validaciones::esCancelar(tmp)) p.setModelo(tmp);  else cout<<"Campo no modificado.\n"; break;
        case 3:  cout << "Nombre: ";   cin.getline(tmp,100); if(!Validaciones::esCancelar(tmp)) p.setNombre(tmp);  else cout<<"Campo no modificado.\n"; break;
        case 4:
            do {
                cout << "Talla: "; cin.getline(tmp, 10);
                if (Validaciones::esCancelar(tmp)) { cout << "Campo no modificado.\n"; break; }
                if (!Validaciones::esNumero(tmp)) cout << "ERROR: debe ser numero.\n";
            } while (!Validaciones::esNumero(tmp) && !Validaciones::esCancelar(tmp));
            if (!Validaciones::esCancelar(tmp)) p.setTalla(tmp);
            break;
        case 5:  cout << "Color: ";    cin.getline(tmp,30);  if(!Validaciones::esCancelar(tmp)) p.setColor(tmp);    else cout<<"Campo no modificado.\n"; break;
        case 6:  cout << "Genero: ";   cin.getline(tmp,20);  if(!Validaciones::esCancelar(tmp)) p.setGenero(tmp);   else cout<<"Campo no modificado.\n"; break;
        case 7:  cout << "Material: "; cin.getline(tmp,50);  if(!Validaciones::esCancelar(tmp)) p.setMaterial(tmp); else cout<<"Campo no modificado.\n"; break;
        case 8:  cout << "Descripcion: "; cin.getline(tmp,200); if(!Validaciones::esCancelar(tmp)) p.setDescripcion(tmp); else cout<<"Campo no modificado.\n"; break;
        case 9:
            do {
                cout << "Precio venta: "; cin.getline(tmp, 20);
                if (Validaciones::esCancelar(tmp)) { cout << "Campo no modificado.\n"; break; }
                if (!Validaciones::esNumero(tmp) || atof(tmp) <= 0) cout << "ERROR: precio invalido.\n";
            } while (!Validaciones::esNumero(tmp) && !Validaciones::esCancelar(tmp));
            if (!Validaciones::esCancelar(tmp)) p.setPrecio(atof(tmp));
            break;
        case 10:
            do {
                cout << "Precio compra: "; cin.getline(tmp, 20);
                if (Validaciones::esCancelar(tmp)) { cout << "Campo no modificado.\n"; break; }
                if (!Validaciones::esNumero(tmp) || atof(tmp) <= 0) cout << "ERROR: precio invalido.\n";
            } while (!Validaciones::esNumero(tmp) && !Validaciones::esCancelar(tmp));
            if (!Validaciones::esCancelar(tmp)) p.setPrecioCompra(atof(tmp));
            break;
        case 11:
            do {
                cout << "Stock minimo: "; cin.getline(tmp, 10);
                if (Validaciones::esCancelar(tmp)) { cout << "Campo no modificado.\n"; break; }
                if (!Validaciones::esNumero(tmp)) cout << "ERROR: debe ser numero.\n";
            } while (!Validaciones::esNumero(tmp) && !Validaciones::esCancelar(tmp));
            if (!Validaciones::esCancelar(tmp)) p.setStockMinimo(atoi(tmp));
            break;
        }
    } while (op != 0);

    p.setFechaModif(time(0));
    GestorArchivos::actualizarRegistro<Producto>(ARCH_PRODUCTOS, idx, p);
    cout << "\nCalzado actualizado.\n";
}

// =====================================================================
// AJUSTAR STOCK
// =====================================================================

void ajustarStock() {
    Formatos::titulo("AJUSTAR STOCK");
    char tmpId[10];
    cout << "ID (0 para cancelar): "; cin.getline(tmpId, 10);
    if (Validaciones::esCancelar(tmpId)) { cout << "Operacion cancelada.\n"; return; }
    if (!Validaciones::esNumero(tmpId))  { cout << "ERROR: ingrese un numero.\n"; return; }

    int idx = GestorArchivos::buscarIndicePorId<Producto>(ARCH_PRODUCTOS, atoi(tmpId));
    if (idx == -1) { cout << "No encontrado.\n"; return; }

    Producto p;
    GestorArchivos::leerRegistroPorIndice<Producto>(ARCH_PRODUCTOS, idx, p);
    cout << "  Producto    : " << p.getNombre() << "\n";
    cout << "  Stock actual: " << p.getStock()  << "\n";
    cout << "  Ajuste (+5 sumar, -3 restar): ";

    char tmpAj[10]; cin.getline(tmpAj, 10);
    if (Validaciones::esCancelar(tmpAj)) { cout << "Operacion cancelada.\n"; return; }
    if (!Validaciones::esNumero(tmpAj))  { cout << "ERROR: debe ser un numero.\n"; return; }

    int aj    = atoi(tmpAj);
    int nuevo = p.getStock() + aj;
    if (nuevo < 0) { cout << "ERROR: resultaria en stock negativo.\n"; return; }

    char conf;
    cout << "Nuevo stock: " << nuevo << "  Confirmar? (S/N): ";
    cin >> conf; Formatos::limpiarBuffer();
    if (conf != 'S' && conf != 's') { cout << "Operacion cancelada.\n"; return; }

    p.setStock(nuevo);
    p.setFechaModif(time(0));
    GestorArchivos::actualizarRegistro<Producto>(ARCH_PRODUCTOS, idx, p);
    cout << "\nStock actualizado a " << nuevo << " unidades.\n";
}

// =====================================================================
// ELIMINAR PRODUCTO
// =====================================================================

void eliminarProducto() {
    Formatos::titulo("ELIMINAR CALZADO");
    char tmpId[10];
    cout << "ID (0 para cancelar): "; cin.getline(tmpId, 10);
    if (Validaciones::esCancelar(tmpId)) { cout << "Operacion cancelada.\n"; return; }
    if (!Validaciones::esNumero(tmpId))  { cout << "ERROR: ingrese un numero.\n"; return; }

    int idx = GestorArchivos::buscarIndicePorId<Producto>(ARCH_PRODUCTOS, atoi(tmpId));
    if (idx == -1) { cout << "No encontrado.\n"; return; }

    Producto p;
    GestorArchivos::leerRegistroPorIndice<Producto>(ARCH_PRODUCTOS, idx, p);
    cout << "  Calzado: " << p.getNombre() << " (" << p.getMarca() << " - Talla " << p.getTalla() << ")\n";

    char conf;
    cout << "Eliminar? (S/N): "; cin >> conf; Formatos::limpiarBuffer();
    if (conf != 'S' && conf != 's') { cout << "Operacion cancelada.\n"; return; }

    // borrado logico - solo cambia el flag, el registro sigue en disco
    p.setEliminado(true);
    p.setFechaModif(time(0));
    GestorArchivos::actualizarRegistro<Producto>(ARCH_PRODUCTOS, idx, p);

    ArchivoHeader h = GestorArchivos::leerHeader(ARCH_PRODUCTOS);
    h.registrosActivos--;
    GestorArchivos::actualizarHeader(ARCH_PRODUCTOS, h);
    cout << "\nCalzado eliminado.\n";
}

// =====================================================================
// FILTROS
// =====================================================================

void filtrarPorTalla() {
    Formatos::titulo("FILTRAR POR TALLA");
    char t[10];
    cout << "Talla: "; cin.getline(t, 10);
    if (!Validaciones::esNumero(t)) { cout << "ERROR: talla debe ser numero.\n"; return; }

    int enc = 0;
    ifstream f(ARCH_PRODUCTOS, ios::binary);
    if (!f.is_open()) { cerr << "Error al abrir productos.bin\n"; return; }
    f.seekg(sizeof(ArchivoHeader), ios::beg);
    Producto p;
    while (f.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (!p.isEliminado() && strcmp(p.getTalla(), t) == 0 && p.getStock() > 0) {
            p.mostrarInformacionBasica(); enc++;
        }
    }
    f.close();
    if (enc == 0) cout << "No hay calzados en talla " << t << ".\n";
    else cout << "Total: " << enc << " calzados.\n";
}

void filtrarPorMarca() {
    Formatos::titulo("FILTRAR POR MARCA");
    char m[50];
    do {
        cout << "Marca (Nike/Adidas/Puma): "; cin.getline(m, 50);
        if (Validaciones::esCancelar(m)) { cout << "Operacion cancelada.\n"; return; }
        if (!Validaciones::validarMarca(m)) cout << "ERROR: solo Nike, Adidas o Puma.\n";
    } while (!Validaciones::validarMarca(m));

    int enc = 0;
    ifstream f(ARCH_PRODUCTOS, ios::binary);
    if (!f.is_open()) { cerr << "Error al abrir productos.bin\n"; return; }
    f.seekg(sizeof(ArchivoHeader), ios::beg);
    Producto p;
    while (f.read(reinterpret_cast<char*>(&p), sizeof(Producto))) {
        if (!p.isEliminado() && Formatos::contieneStr(p.getMarca(), m)) {
            p.mostrarInformacionBasica(); enc++;
        }
    }
    f.close();
    if (enc == 0) cout << "Sin resultados.\n";
    else cout << "Total: " << enc << " calzados.\n";
}