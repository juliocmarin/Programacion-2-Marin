
#include <iostream>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <cctype>
#include <limits>

using namespace std;

//ESTRUCTURA DE DATOS DE STAR SHOES//

struct Producto {
    int id;
    char codigo[20];
    char nombre[100];
    char descripcion[200];
    int idProveedor;
    float precio;
    int stock;
    char fechaRegistro[11];

    //zapateria//

    char marca[50];
    char modelo[50];
    char talla[10];
    char color[30];
    char genero[20];
    char material[50];
    float precioCompra;
    int minimoStock;
};

struct Proveedor {
    int id;
    char nombre[100];
    char rif[20];
    char telefono[20];
    char email[100];
    char direccion[200];
    char fechaRegistro[11];
};

struct Cliente {
    int id;
    char nombre[100];
    char cedula[20];
    char telefono[20];
    char email[100];
    char direccion[200];
    char fechaRegistro[11];
};

struct Transaccion {
    int id;
    char tipo[10];
    int idProducto;
    int idRelacionado;
    int cantidad;
    float precioUnitario;
    float total;
    char fecha[11];
    char descripcion[200];
    bool cancelada;
};

struct Tienda {
    char nombre[100];
    char rif[20];
    
    Producto* productos;
    int numProductos;
    int capacidadProductos;
    
    Proveedor* proveedores;
    int numProveedores;
    int capacidadProveedores;
    
    Cliente* clientes;
    int numClientes;
    int capacidadClientes;
    
    Transaccion* transacciones;
    int numTransacciones;
    int capacidadTransacciones;
    
    int siguienteIdProducto;
    int siguienteIdProveedor;
    int siguienteIdCliente;
    int siguienteIdTransaccion;
};

//  FUNCIONES NECESARIAS //

void inicializarTienda(Tienda* tienda, const char* nombre, const char* rif);
void liberarTienda(Tienda* tienda);
void redimensionarProductos(Tienda* tienda);
void redimensionarProveedores(Tienda* tienda);
void redimensionarClientes(Tienda* tienda);
void redimensionarTransacciones(Tienda* tienda);

// Utilidades //

void limpiarBuffer();
void obtenerFechaActual(char* buffer);
void convertirAMinusculas(char* cadena);
bool contieneSubstring(const char* texto, const char* busqueda);
bool validarEmail(const char* email);
bool validarFecha(const char* fecha);

// Validaciones //

bool existeProducto(Tienda* tienda, int id);
bool existeProveedor(Tienda* tienda, int id);
bool existeCliente(Tienda* tienda, int id);
bool codigoDuplicado(Tienda* tienda, const char* codigo);
bool rifDuplicado(Tienda* tienda, const char* rif);

// CRUD Productos //

void crearProducto(Tienda* tienda);
void buscarProducto(Tienda* tienda);
void actualizarProducto(Tienda* tienda);
void actualizarStockProducto(Tienda* tienda);
void listarProductos(Tienda* tienda);
void eliminarProducto(Tienda* tienda);

// Funciones extras para STAR SHOES //

void filtrarPorTalla(Tienda* tienda);
void filtrarPorMarca(Tienda* tienda);
void alertasStockBajo(Tienda* tienda);

// CRUD Proveedores //
void crearProveedor(Tienda* tienda);
void buscarProveedor(Tienda* tienda);
void actualizarProveedor(Tienda* tienda);
void listarProveedores(Tienda* tienda);
void eliminarProveedor(Tienda* tienda);

// CRUD Clientes //

void crearCliente(Tienda* tienda);
void buscarCliente(Tienda* tienda);
void actualizarCliente(Tienda* tienda);
void listarClientes(Tienda* tienda);
void eliminarCliente(Tienda* tienda);

// Transacciones //

void registrarCompra(Tienda* tienda);
void registrarVenta(Tienda* tienda);
void buscarTransacciones(Tienda* tienda);
void listarTransacciones(Tienda* tienda);
void cancelarTransaccion(Tienda* tienda);

// Reportes extra //

void reporteGanancias(Tienda* tienda);
void menuReportes(Tienda* tienda);

// Menús //

void menuPrincipal(Tienda* tienda);
void menuProductos(Tienda* tienda);
void menuProveedores(Tienda* tienda);
void menuClientes(Tienda* tienda);
void menuTransacciones(Tienda* tienda);


//  FUNCIONES AUXILIARES //

void limpiarBuffer() {
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void obtenerFechaActual(char* buffer) {
    time_t now = time(0);
    tm* ltm = localtime(&now);
    sprintf(buffer, "%04d-%02d-%02d", 1900 + ltm->tm_year, 1 + ltm->tm_mon, ltm->tm_mday);
}

void convertirAMinusculas(char* cadena) {
    for (int i = 0; cadena[i]; i++) {
        cadena[i] = tolower(cadena[i]);
    }
}

bool contieneSubstring(const char* texto, const char* busqueda) {
    char t[200], b[200];
    strcpy(t, texto);
    strcpy(b, busqueda);
    convertirAMinusculas(t);
    convertirAMinusculas(b);
    return (strstr(t, b) != NULL);
}

bool validarEmail(const char* email) {
    bool tieneArroba = false;
    bool tienePuntoDespues = false;
    int posArroba = -1;
    for (int i = 0; email[i] != '\0'; i++) {
        if (email[i] == '@') {
            tieneArroba = true;
            posArroba = i;
        }
        if (tieneArroba && email[i] == '.' && i > posArroba) {
            tienePuntoDespues = true;
        }
    }
    return tieneArroba && tienePuntoDespues;
}

bool validarFecha(const char* fecha) {
    if (strlen(fecha) != 10) return false;
    if (fecha[4] != '-' || fecha[7] != '-') return false;
    return true;
}


// MANEJO DE MEMORIA //

void inicializarTienda(Tienda* tienda, const char* nombre, const char* rif) {
    strcpy(tienda->nombre, nombre);
    strcpy(tienda->rif, rif);
    
    // Capacidad inicial de 5 elementos jeje//

    tienda->capacidadProductos = 5;
    tienda->capacidadProveedores = 5;
    tienda->capacidadClientes = 5;
    tienda->capacidadTransacciones = 5;
    
    // Designacion dinamica con new //

    tienda->productos = new Producto[tienda->capacidadProductos];
    tienda->proveedores = new Proveedor[tienda->capacidadProveedores];
    tienda->clientes = new Cliente[tienda->capacidadClientes];
    tienda->transacciones = new Transaccion[tienda->capacidadTransacciones];
    
    // Contadores en 0 //

    tienda->numProductos = 0;
    tienda->numProveedores = 0;
    tienda->numClientes = 0;
    tienda->numTransacciones = 0;
    
    // ID comienzan en 1 //

    tienda->siguienteIdProducto = 1;
    tienda->siguienteIdProveedor = 1;
    tienda->siguienteIdCliente = 1;
    tienda->siguienteIdTransaccion = 1;
    
    cout << "\n[OK] Tienda '" << nombre << "' inicializada correctamente.\n";
}

void liberarTienda(Tienda* tienda) {

    if (tienda->productos != nullptr){
        delete[] tienda->productos;
        tienda->productos = nullptr;
    }

    if (tienda->proveedores != nullptr){
        delete[] tienda->proveedores;
        tienda->proveedores = nullptr;

    }

    if (tienda->clientes != nullptr)
    {
        delete[] tienda->clientes;
        tienda->clientes = nullptr;

    }
    
    if (tienda->transacciones != nullptr)
    {
        delete[] tienda->transacciones;
        tienda->transacciones = nullptr;

    }
    
    tienda->numProductos = 0;
    tienda->numProveedores = 0;
    tienda->numClientes = 0;
    tienda->numTransacciones = 0;

    tienda->capacidadProductos = 0;
    tienda->capacidadProveedores = 0;
    tienda->capacidadClientes = 0;
    tienda->capacidadTransacciones = 0;

cout << "\n[OK] Memoria liberada correctamente (no hay punteros colgantes, que bendicion).\n";


}

void redimensionarProductos(Tienda* tienda) {

    int nuevaCapacidad = tienda->capacidadProductos * 2;
    Producto* nuevoArray = new Producto[nuevaCapacidad];
    
for (int i = 0; i < tienda->numProductos; i++)
 {
        nuevoArray[i] = tienda->productos[i];
    }
    
    delete[] tienda->productos;
    tienda->productos = nuevoArray;
    tienda->capacidadProductos = nuevaCapacidad;
    cout << "[OK] Array redimensionado a " << nuevaCapacidad << " elementos.\n";
}

void redimensionarProveedores(Tienda* tienda){

    int nuevaCapacidad = tienda->capacidadProveedores * 2;
    Proveedor* nuevoArray = new Proveedor[nuevaCapacidad];

for (int i = 0; i < tienda->numProveedores; i++)

    {
        nuevoArray[i] = tienda->proveedores[i];

    }

    delete[] tienda->proveedores;
    tienda->proveedores = nuevoArray;
    tienda->capacidadProveedores = nuevaCapacidad;

}

void redimensionarClientes(Tienda* tienda){

    int nuevaCapacidad = tienda->capacidadClientes * 2;
    Cliente* nuevoArray = new Cliente[nuevaCapacidad];
    
for (int i = 0; i < tienda->numClientes; i++)

    {
        nuevoArray[i] = tienda->clientes[i];

    }

    delete[] tienda->clientes;
    tienda->clientes = nuevoArray;
    tienda->capacidadClientes = nuevaCapacidad;

}   

void redimensionarTransacciones(Tienda* tienda){

    int nuevaCapacidad = tienda->capacidadTransacciones * 2;
    Transaccion* nuevoArray = new Transaccion[nuevaCapacidad];

for (int i = 0; i < tienda->numTransacciones; i++)
{
    nuevoArray[i] = tienda->transacciones[i];

}

    delete[] tienda->transacciones;
    tienda->transacciones = nuevoArray;
    tienda->capacidadTransacciones = nuevaCapacidad;

}

//VALIDACIONES//


bool existeProducto(Tienda* tienda, int id){
    for (int i = 0; i < tienda->numProductos; i++)
    {
        if(tienda->productos[i].id == id) return true;

    }
    return false;
}

bool existeProveedor(Tienda* tienda, int id){
    for (int i = 0; i < tienda->numProveedores; i++)
    {
        if(tienda->proveedores[i].id == id) return true;

    }
    return false;

}

bool existeCliente(Tienda* tienda, int id){
    for (int i = 0; i < tienda->numClientes; i++)
    {
        if(tienda->clientes[i].id == id) return true;

    }
    
    return false;
}

bool codigoDuplicado(Tienda* tienda, const char* codigo){
    for (int i = 0; i < tienda->numProductos; i++)
    {
        if(strcmp(tienda->productos[i].codigo, codigo) == 0) return true;

    }
    
    return false;

}

bool rifDuplicado(Tienda* tienda, const char* rif){
     
    for (int i = 0; i < tienda->numProveedores; i++)
    {
        if(strcmp(tienda->proveedores[i]. rif, rif) == 0) return true;

    }

    return false;
    
}