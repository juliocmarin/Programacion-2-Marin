
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

// BUSQUEDA POR INDICE //
int buscarProductoPorId(Tienda* tienda, int id);
int buscarProveedorPorId(Tienda* tienda, int id);
int buscarClientePorId(Tienda* tienda, int id);

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

// BUSQUEDAS POR INDICE //

int buscarProductoPorId(Tienda* tienda, int id){
    for (int i = 0; i < tienda->numProductos; i++)
    {
        if (tienda->productos[i].id == id)
        {
            return i;
        }     
    } 
    return -1;
}

int buscarProveedorPorId(Tienda* tienda, int id){
    for (int i = 0; i < tienda->numProveedores; i++)
    {
        if (tienda->proveedores[i].id == id)
        {
            return i;
        }   
    }
    return-1;
}

int buscarClientePorId(Tienda* tienda, int id){
    for (int i = 0; i < tienda->numClientes; i++)
    {
        if (tienda->clientes[i].id == id)
        {
            return i;
        }
    }
    return -1;   
}

// CRUD DE PRODUCTOS/CALZADO //

void crearProducto(Tienda* tienda){

    cout << "\n=== REGISTRAR NUEVO CALZADO ===\n";
    cout << "(Puede escribir 0 o CANCELAR en cuelquier paso para salir)\n";

    if (tienda->numProductos >= tienda->capacidadProductos){
        redimensionarProductos(tienda);
    }

    Producto* p = &tienda->productos[tienda->numProductos];
    char temp[200];
    int idProv;

    limpiarBuffer();
    cout << "Codigo (EJ. CAL-001): "; cin.getline(temp, 20);

    if(strcmp(temp, "0") == 0 || strcmp(temp, "CANCELAR") == 0)
     { cout << "Operacion cancelada.\n";
         return;
        }

    if(codigoDuplicado(tienda, temp))

    { cout << "ERROR: El codigo '" << temp << "' ya esta registrado.\n";
        return;
    }
    strcpy(p->codigo, temp);

    cout << "Marca ( Nike, Adidas, Puma...): "; cin.getline(p->marca, 50);
    if(strcmp(p->marca, "0") == 0)
    {cout << "Operacion Cancelada.\n";
         return;
    }

    cout << "Modelo: "; cin.getline(p->modelo, 50);
    cout << "Nombre: "; cin.getline(p->nombre, 100);
    if(strcmp(p->nombre, "0") == 0)
    { cout << "Operacion cancelada.\n"; 
        return;
    }

    cout << "Talla: "; cin.getline(p->talla, 10);
    cout << "Color: "; cin.getline(p->color, 30);
    cout << "Genero: "; cin.getline(p->genero, 20);
    cout << "Material: "; cin.getline(p->material, 50);
    cout << "Descripcion: "; cin.getline(p->descripcion, 200);
    
    cout << "ID Proveedor: "; cin >> idProv; limpiarBuffer();
    if(idProv == 0){
        cout << "Operacion cancelada.\n"; 
        return;
    }

    if (!existeProveedor(tienda, idProv))
    {
        cout << "ERROR: El proveedor con ID " << idProv << " no existe. Debe de registrar el proveedor primero.\n";
        return;
    }

    p->idProveedor = idProv;

    cout << "Precio de Compra: "; cin >> p->precioCompra; limpiarBuffer();
    if (p->precioCompra <= 0) 
    {
        cout << "ERROR: el precio ejercido debe ser mayor que 0.\n";
        return;
    }
    
    cout << "Precio de Venta: "; cin >> p->precio; limpiarBuffer();
    if (p->precio <= 0)
    {
        cout << "ERROR: El precio tiene que ser mayor que 0.\n";
        return;
    }
    
    cout << "Stock Inicial: "; cin >> p->stock; limpiarBuffer();
    if (p->stock < 0)
    {
        cout << "ERROR: El stock no puede ser negativo.\n";
        return;
    }
    
    cout << "Stock Minimo (atencion): "; cin >> p->minimoStock; limpiarBuffer();
    if(p->minimoStock < 0) p->minimoStock = 5;

    // confirmacion jeje//

    cout << "\n==== RESUMEN =====\n";
    cout << "Codigo: " << p->codigo << "\n";
    cout << "Marca: " << p->marca << "\n";
    cout << "Nombre: " << p->nombre << "\n";
    cout << "Talla: " << p->talla << "\n";
    cout << "Precio Venta: " << p->precio << "\n";
    cout << "Stock: " << p->stock << "\n";

    char conf;
    cout << "\n¿Guardar calzado? (Si / No): "; cin >> conf; limpiarBuffer();
    if (conf != 'S' && conf != 's')
    {
        cout << "Operacion cancelada.\n";
        return;
    }

   // COLOCAR ID AUTOMATICO // 
  
   p->id = tienda->siguienteIdProducto++;
   obtenerFechaActual(p->fechaRegistro);
   tienda->numProductos++;

   cout << "OK: Calzado registrado de manera exitosa con ID: " << p->id << "\n";


}

void listarProductos(Tienda* tienda){

    cout << "\n === LISTADO DE CALZADOS ===\n";
    cout << left << setw(5) << "ID" << setw(12) << "Codigo" << setw (20) << "Nombre"
         << setw (12) << "Marca" << setw(6) << "Talla" << setw(10) << "Precio" << setw(6) << "Stock" << endl;
    cout << string(75, '-') << endl;


    for (int i = 0; i < tienda->numProductos; i++)
    {
        cout << left << setw(5) << tienda->productos[i].id
                     << setw(12) << tienda->productos[i].codigo
                     << setw(20) << tienda->productos[i].nombre
                     << setw(12) << tienda->productos[i].marca
                     << setw(6) << tienda->productos[i].talla
                     << setw(10) << tienda->productos[i].precio
                     << setw(6) << tienda->productos[i].stock << endl;
            
    }
    
    cout << "Total de calzados: " << tienda->numProductos << endl;

}

void buscarProducto(Tienda* tienda){
    int op;
    cout << "\n===== BUSCAR CALZADO =====\n";
    cout << "1. Por ID\n";
    cout << "2. Por nombre\n";
    cout << "3. Por codigo\n";
    cout << "4. Por marca\n";
    cout << "0. Cancelar\n";
    cout << "Opcion: "; cin >> op; limpiarBuffer();
    if (op == 0)
    {
        return;
    }

    if (op == 1)
    {
        int id; 
        cout << "ID: "; cin >> id; limpiarBuffer();

        for (int i = 0; i < tienda->numProductos; i++)
        {
            if(tienda->productos[i].id == id){
                cout << "Encontrado: " << tienda->productos[i].nombre
                << " (" << tienda->productos[i].marca << " - Talla"
                << tienda->productos[i].talla << ")\n";
                
        return;


            }
        }
        
        cout << "No encontrado.\n";

    } else if (op == 2)
    {
        char busqueda[100]; 
        cout << "Nombre: "; cin.getline(busqueda, 100);
        bool found = false;

        for (int i = 0; i < tienda->numProductos; i++)
        {
            if (contieneSubstring(tienda->productos[i].nombre, busqueda))
            {
                cout << "Encuentra: " << tienda->productos[i].nombre
                    << " (ID: " << tienda->productos[i].id << ")\n";
                found = true;
            }
            
        }

        if(!found) cout << "No hay concidencias.\n";

    } else if (op == 3)
    {
        char busqueda[20];
        cout << "Codigo: "; cin.getline(busqueda, 20);
        bool found = false;

        for (int i = 0; i < tienda->numProductos; i++)
        {
            if (contieneSubstring(tienda->productos[i].codigo, busqueda))
            {
                cout << "Encuentra: " << tienda->productos[i].nombre
                     << "( " << tienda->productos[i].codigo << ")\n";
                found = true;
            }
            
        }

        if(!found) cout << "No hay coincidencias.\n";

    }else if (op == 4)
    {
        char marca[50];
        cout << "Marca: "; cin.getline(marca, 50);
        bool found = false;

        for (int i = 0; i < tienda->numProductos; i++)
        {
            if (contieneSubstring(tienda->productos[i].marca, marca))
            {
                cout << "Encuentra: " << tienda->productos[i].nombre
                     << " - Talla: " << tienda->productos[i].talla
                     << " - Stock: " << tienda->productos[i].stock << ")\n";
                found = true;
            }
        
        }
        if (!found) cout << "No hay hay calzado de esta marca.\n";
    }  
}

void actualizarProducto(Tienda* tienda){
    int id;
    cout << "ID calzado a editar: "; cin >> id; limpiarBuffer();

    int idx = buscarProductoPorId(tienda, id);
   
    if (idx == -1)
    {
        cout << "ERROR: Calzado no existe.\n";
        return;
    }

    Producto* p = &tienda->productos[idx];
    int op;
    char temp[200];
    

    do
    {
        cout << "\n=== EDITAR CALZADO ID: " << p->id << "===\n";
        cout << "1. Marca (" << p->marca << ")\n";
        cout << "2. Modelo (" << p->modelo << ")\n";
        cout << "3. Nombre ("  << p->nombre << ")\n";
        cout << "4. Talla (" << p->talla << ")\n";
        cout << "5. Color (" << p->color << ")\n";
        cout << "6. Precio (" << p->precio << ")\n";
        cout << "7. Stock (" << p->stock << ")\n";
        cout << "8. Guardar cambios\n";
        cout << "0. Cancelar\n";
        cout << "Opcion: "; cin >> op; limpiarBuffer();


        switch (op)
        {

        case 1:
            cout << "Marca: ";
            cin.getline(temp, 50);
            strcpy(p->marca, temp);
            break;
        
        case 2:
             cout << "Modelo: ";
             cin.getline(temp, 50);
             strcpy(p->modelo, temp);
            break;

        case 3:
            cout<< "Nombre: ";
            cin.getline(temp, 100);
            strcpy(p->nombre, temp);
            break;
            
        case 4:
            cout << "Talla: ";
            cin.getline(temp, 10);
            strcpy(p->talla, temp);
            break;

        case 5:
            cout << "Color: ";
            cin.getline(temp, 30);
            strcpy(p->color, temp);
            break;
        
        case 6:
           cout << "Precio: ";
           cin >> p->precio;
           limpiarBuffer();
           break;

        case 7:
            cout << "Stock: ";
            cin >> p->stock;
            limpiarBuffer();
            break;
        
        case 8:
            cout << "cambios guardados correctamente.\n";
            break;
        }    
         
    } while (op != 0 && op != 8);
       
}

void actualizarStockProductos(Tienda* tienda){
    int id, ajuste;
    cout << "ID calzado: "; cin >> id; limpiarBuffer();

    for (int i = 0; i < tienda->numProductos; i++)
    {
        if (tienda->productos[i].id == id)
        {
            cout << "Stock actual: " << tienda->productos[i].stock << "\n";
            cout << "Ajuste (+ para agregar, - para quitar): "; cin >> ajuste; limpiarBuffer();

            int nuevoStock = tienda->productos[i].stock + ajuste;
            if (nuevoStock < 0)
            {
                cout << "ERROR: El stock no puede ser negativo.\n";
                return;
            }

            char conf;
            cout << "Nuevo stock sera de: " << nuevoStock << "¿Desea confirmar? (Si / No): ";
            cin >> conf; limpiarBuffer();

            if (conf == 'S' || conf == 's')
            {
                tienda->productos[i].stock = nuevoStock;
                cout << "Stock actualizado correctamente.\n";

            }else {
                cout << "Operacion cancelada.\n";
            }
            return;            
        }       
    }     
    
    cout << "Calzado no encontrado.\n";

}

void eliminarProducto(Tienda* tienda){
     int id;
     cout << "ID de el calzado a eliminar: "; cin >> id; limpiarBuffer();

     bool tieneTransacciones = false;
     for (int i = 0; i < tienda->numTransacciones; i++)
     {
        if (tienda->transacciones[i].idProducto == id && !tienda->transacciones[i].cancelada)
        {
           tieneTransacciones = true;
           break; 
        }
        
     }

     if (tieneTransacciones)
     {
        cout << "CUIDADO: Este calzado tiene transacciones vinculadas.\n";
     }

     char conf;
     cout << "¿Eliminar calzado? (Si / No): "; cin >> conf; limpiarBuffer();
     if (conf == 'S' || conf == 's')
     {
        cout << "Calzado eliminado correctamente.\n";

     }else{
        cout << "Operacion cancelada.\n";

     }   
     
}

void filtrarPorTalla(Tienda* tienda){
    char talla[10];
    cout << "Talla a buscar: ";
    cin.getline(talla, 10);
    cout << "\nCalzados en talla " << talla << ":\n";

    int encontrados = 0;
    for (int i = 0; i < tienda->numProductos; i++)
    {
        if (strcmp(tienda->productos[i].talla, talla) == 0 && tienda->productos[i].stock > 0)
        {
          cout << "-" << tienda->productos[i].nombre << "(" << tienda->productos[i].marca << ")\n";
          encontrados ++;

        }
        if (encontrados == 0)
        {
            cout << "No hay calzados en esta talla.\n";
        }
        else
        {
            cout << "Total encontrados: " << encontrados << "\n";
        }     
    }   
    
}

void filtradoPorMarca(Tienda* tienda){
    char marca[50];
    cout << "Marca a buscar: ";
    cin.getline(marca, 50);
    cout << "\nCalzados en marca " << marca << ":\n";

    int encontrados = 0;
    for (int i = 0; i < tienda->numProductos; i++)
    {
        if (contieneSubstring(tienda->productos[i].marca, marca))
        {
            cout << "-" << tienda->productos[i].nombre << "- Talla" << tienda->productos[i].talla << "\n";
            encontrados ++;
        }
        if (encontrados == 0)
        {
            cout << "No hay calazado de estar marca.\n";

        }
        else{
            cout << "Total encontrados: " << encontrados << "\n";
        }       
    }
    
}

void alertasStockBajo(Tienda* tienda){
    cout << "\n=== ALERTAS DE STOCK BAJO ===\n";
    int alertas = 0;
    for (int i = 0; i < tienda->numProductos; i++)
    {
        if (tienda->productos[i].stock <= tienda->productos[i].minimoStock)
        {
           cout << "ALERTA: " << tienda->productos[i].nombre
                << "- Stock: " << tienda->productos[i].stock
                << "(Minimo: " << tienda->productos[i].minimoStock << ")\n";
                alertas ++;
        }
        
    }
    if (alertas == 0)
    {
        cout << "Todos los productos tienenn stock suficiente.\n";
    }
    else
    {
       cout << "Total de alerta: " << alertas << "\n";
    }
    
}

 // CRUD PROVEEDORES //

void crearProveedores(Tienda* tienda){
    cout << "\n==== REGISTRAR PROVEEDOR===\n";
    cout << "(Debe escribir 0 o CANCELAR para salir)\n";

    if (tienda->numProveedores >= tienda->capacidadProveedores)
    {
        redimensionarProveedores(tienda);
    }

    Proveedor* p = &tienda->proveedores[tienda->numProveedores];
    char temp[100];


    cout << "Nombre: "; cin.getline(temp, 100);
    if (strcmp(temp, "0") == 0)
    {
        cout << "Operacion cancelada\n";
        return;
    }

    strcpy(p->nombre, temp);
    
    cout << "RIF: "; cin.getline(temp, 20);
    if (strcmp(temp, "0") == 0)
    {
        cout << "Operacion cancelada\n";
        return 0;
    }

    if (rifDuplicado( tienda, temp))
    {
        cout << "ERROR: rif duplicado\n";
        return;
    }
    strcpy(p->email, temp);

    cout << "Direccion: "; cin. getline(p->direccion, 200);

    char conf;
    cout << "\n¿ Guardar proveedor? (Si / No): "; cin >> conf; limpiarBuffer();
    if (conf != 'S' && conf != 's')
    {
        cout << "Operacion cancelada\n";
        return;
    }

    p->id = tienda->siguienteIdProveedor++;
    obtenerFechaActual(p->fechaRegistro);
    tienda->numProveedores++;

    cout << "Ok: Proveedor registrado de manera exitosa con ID: " << p->id << "\n";
   
}

void listarProveedores(Tienda* tienda){
    cout << "\n===== LISTADO DE PROVEEDORES====\n";
    cout << left << setw(5) << "ID" << setw(30) << "Nombre" << setw(20) << "RIF" << setw(15) << "Telefono" << endl;
    cout << string(70, '-') << endl;

    for (int i = 0; i < tienda->numProveedores; i++)
    {
        cout << left << setw(5) << tienda->proveedores[i].id
                     << setw(30) << tienda->numProveedores[i].nombre
                     << setw(20) << tienda->proveedores[i].rif
                     << setw(15) << tienda->proveedores[i].telefono << endl;
            
    }
    
    cout << "Total de proveedores: " << tienda->numProveedores << endl;

}

void buscarProveedor(Tienda* tienda){
    int op;
    cout << "\n=== BUSCAR PROVEEDOR ===\n";
    cout << "1. Por ID\n";
    cout << "2. Por Nombre\n";
    cout << "3. Por rif\n";
    cout << "0. Cancelar\n";
    cout << "Opcion: "; cin >> op, limpiarBuffer();
   
    if (op == 0)
    {
        return;
    }
    
    if (op == 1)
    {
        int id;
        cout << "ID: "; cin >> id; limpiarBuffer();

        for (int i = 0; i < tienda->numProveedores; i++)
        {
            if (tienda->proveedores[i].id == id)
            {
                cout << "Encontrado: " << tienda->proveedores[i].nombre << "\n";
                return;
            }
        }

        cout << "No encontrado.\n";
    
    }else if (op == 2)
    
    {
        char nombre[100];
        cout << "Nombre: "; cin.getline(nombre, 100);
        bool found = false;

        for (int i = 0; i < tienda->numProveedores; i++)
        {
            if (contieneSubstring(tienda->proveedores[i].nombre, nombre))
            {
                cout << "Encuentra: " << tienda->proveedores[i].nombre << "(ID: " << tienda->proveedores[i].id << "\n";
                found = true;
            
            }     
        }
    if(!found) cout << "No hay concidencias.\n";

        
    }else if (op == 3)
    {
        char rif[20];
        cout << "Rif: "; cin.getline(rif, 20);

        for (int i = 0; i < tienda->numProveedores; i++)
        {
            if (strcmp(tienda->proveedores[i].rif, rif) == 0)
            {
                cout << "Encontrado: " << tienda->proveedores[i].nombre << "\n";
                return;
            }             
        }
        cout << "No encontrado\n";
     }
    
}

void actualizarProveedor(Tienda* tienda){
     int id;
     cout << "ID de el proveeedor a editar: "; cin >> id; limpiarBuffer();

     int idx = -1;
     for (int i = 0; i < tienda->numProveedores; i++)
     {
        if (tienda->proveedores[i].id == id)
        {
            idx = i;
            break;
        }
        
     }
     if (idx == -1)
     {
        cout << "ERROR: Proveedor no existe.\n";
        return;
     }

     Proveedor* p = &tienda->proveedores[idx];
     int op;
     char temp[200];    
     
     do
     {
        cout << "\n=== EDITAR PROVEEDOR ID: " << p->id << "===\n";
        cout << "1. Nombre (" << p->nombre << ")\n";
        cout << "2. RIF (" <<p->rif << ")\n";
        cout << "3. Telefono (" << p->telefono << ")\n";
        cout << "4. Email (" << p->email << ")\n";
        cout << "5. Direccion (" << p->direccion << ")\n";
        cout << "6. Guardar cambios\n";
        cout << "0. Cancelar\n";
        cout << "Opcion: "; cin >> op; limpiarBuffer();

    switch (op)
    {
    case 1:
        cout << "Nuevo nombre: ";
        cin.getline(temp, 100);
        strcpy(p->nombre, temp);
        break;
    
    case 2:
        cout << "Nuevo rif: ";
        cin.getline(temp, 20);
        strcpy(p->rif, temp);
        break;

    case 3:
        cout << "Nuevo Telefono: ";
        cin.getline(temp, 20);
        strcpy(p->telefono, temp);
        break;

    case 4:
        cout << "Nuevo Email: ";
        cin.getline(temp, 100);
        strcpy(p->email, temp);
        break;
    
    case 5:
        cout << "Nueva Direccion: ";
        cin.getline(temp, 200);
        strcpy(p->direccion, temp);
        break;

    case 6: 
        cout << "Cambios guardados correctamente.\n";
        break; 
    }

     } while (op != 0 && op != 6);
         
}

void eliminarProveedor(Tienda* tienda){
    int id;
    cout << "ID Proveeedor que desea eliminar: ";
    cin >> id; limpiarBuffer();

    bool tieneProductos = false;
    for (int i = 0; i < tienda->numProductos; i++)
    {
        if (tienda->productos[i].idProveedor == id)
        {
            tieneProductos = true;
        }
        
        if (tieneProductos)
        {
            cout << "CUIDADO: Este proveedor tiene productos vinculados.\n";

        }

    char conf;
    cout << "¿Eliminar Proveedor? (Si / No): ";
    cin >> conf; limpiarBuffer();

         if (conf == 'S' || conf == 's')
         {
            cout << "Proveedor eliminado./n";
         }
         else {
            cout << "Operacion cancelada./n";
         }    
    }
    
}

// CRUD CLIENTES //

void crearClientes(Tienda* tienda){
    cout << "\n=== REGISTRAR CLIENTE ===\n";
    cout << "(Escriba 0 o Cancelar para salir)\n";

    if (tienda->numClientes >= tienda->capacidadClientes)
        {
          redimensionarClientes(tienda);  
        }
    
    Cliente* c = &tienda->clientes[tienda->numClientes];
    char temp[100];

    cout << "Nombre: ";
    cin.getline(c->nombre, 100);
    if (strcmp(c->nombre, "0") == 0)
    {
        cout << "Operacion cancelada.\n";
        return;
    }

    cout << "Cedula: ";
    cin.getline(c->cedula, 20);

    cout << "Telefono: ";
    cin.getline(c->telefono, 20);

    cout << "Email: ";
    cin.getline(c->email, 100);
    if (!validarEmail(temp))
    {
        cout << "ERROR: email invalido.\n";
        strcpy(c->email, temp);     
    }

    cout << "Direccion: "; 
    cin.getline(c->direccion, 200);

    char conf;
    cout << "\n¿Guardar cliente? (Si/No): ";
    cin >> conf; limpiarBuffer();

    if (conf != 'S' || != 's')
    {
        cout << "Operacion cancelada.\n";
        return;
    }

    c->id = tienda->siguienteIdCliente++;
    obtenerFechaActual(c->fechaRegistro);
    tienda->numClientes++;

    cout << "OK: Cliente registrado de manera exitosa con ID: " << c->id << "\n";

}
void listarClientes(Tienda* tienda){
    cout << "\n=== LISTADO DE CLIENTES ===\n";
    cout << left << setw(5) << "ID" << setw(30) << "Nombre" << setw(20) << "Cedula" << endl;
    cout << string(55, '-') << endl;

    for (int i = 0; i < tienda->numClientes; i++)
    {
        cout << left << setw(5) << tienda->clientes[i].id
                     << setw(30) << tienda->clientes[i].nombre
                     << setw(20) << tienda->clientes[i].cedula << endl;
    }
    
    cout << "Total de Clientes: " << tienda->numClientes << endl; 

}

void buscarCliente(Tienda* tienda){
    int op;
    cout << "\n=== BUSCAR CLIENTE ===\n";
    cout << "1. Por ID\n";
    cout << "2. Por Nombre\n";
    cout << "3. Por cedula\n";
    cout << "0. Cancelar\n";
    cout << "Opcion; "; >> op; limpiarBuffer();

    if (op == 0)
    {
        return;
    }

    if (op == 1)
    {
        int id;
        cout << "ID: "; 
        cin >> id; limpiarBuffer();

        for (int i = 0; i < tienda->numClientes; i++)
        {
            if (tienda->clientes[i].id == id)
            {
                cout << "Encontrado: " << tienda->clientes[i].nombre << "\n";
                return;
            }
        }
        cout << "No encontrado.\n";
   
    } else if (op == 2)
    {
        char nombre[100];
        cout << "Nombre: ";
        cin.getline(nombre, 100);
        bool found = false;

        for (int i = 0; i < tienda->numClientes; i++)
        {
            if (contieneSubstring(tienda->clientes[i].nombre, nombre))
            {
                cout << "Encuentra: " << tienda->clientes[i].nombre << "(ID: " << tienda->clientes[i].id << ")\n";
                found = true;
            }     
        }
        if (!found)
        {
            cout << "No hay coincidencias.\n";
        }
        
    }else if (op == 3)
    {
        char cedula{20};
        cout << "Cedula: ";
        cin.getline(cedula, 20);
         
        for (int i = 0; i < tienda->numClientes; i++)
        {
           if (strcmp(tienda->clientes[i].cedula, cedula) == 0)
           {
               cout << "Encontrado: " << tienda->clientes[i].nombre << "\n";
               return;
           }
           
        }
        cout << "No encontrado.\n";
    }

}

void actualizarCliente(Tienda* tienda){
    int id;
    cout << "ID cliente a editar: ";
    cin >> id; limpiarBuffer();

    int idx = -1;
    for (int i = 0; i < tienda->numClientes; i++)
    {
        if (tienda->clientes[i].id == id)
        {
            idx = 1;
            break;
        }
    }
    
    if (idx == -1)
    {
        cout << "ERROR: Cliente no existe.\n";
        return;
    }

    Cliente* c = &tienda->clientes[idx];
    int op;
    char temp[200];
    
    do
    {
        cout << "\n=== EDITAR CLIENTE ===\n";
        cout << "1. Nombre\n";
        cout << "2. Cedula\n";
        cout << "3. Telefono\n";
        cout << "4. Email\n";
        cout << "5. Direccion\n";
        cout << "6. Guardar\n";
        cout << "0. CANCELAR\n";
        cout << "Opcion: "; cin >> op; limpiarBuffer();
    
    switch (op)
    {
    case 1:
        cout << "Nombre: ";
        cin.getline(temp, 100);
        strcpy(c->nombre, temp);
        break;
    
    case 2:
        cout << "Cedula: ";
        cin.getline(temp, 20);
        strcpy(c->cedula, temp);
        break;

    case 3:
        cout << "Telefono: ";
        cin.getline(temp, 20);
        strcpy(c->telefono, temp);
        break;

    case 4:
        cout << "Email: ";
        cin.getline(temp, 100);
        strcpy(c->email, temp);
        break;
    
    case 5:
        cout << "Direccion: ";
        cin.getline(temp, 200);
        strcpy(c->direccion, temp);
        break;
    
    case 6:
        cout << "Cambios guardados exitosamente.\n";
        break;
    }


     } while (op != 0 && op != 6);
    
}

void eliminarCliente(Tienda* tienda){
    int id;
    cout << "ID cliente a eliminar: ";
    cin >> id; limpiarBuffer();

    char conf;
    cout << "¿Eliminar Cliente? (Si / No): ";
    cin >> conf; limpiarBuffer();

    if (conf != 'S' || conf != 's')
    {
        cout << "Cliente eliminado correctamente.\n";

    }else {
        cout << "Operacion cancelada.\n";
    }    
}

// CRUD TRANSACCIONES //

void registrarCompra(Tienda* tienda){
    cout << "\n=== REGISTRAR COMPRA ====\n";

    int idProd, IdProv, cantidad;
    float precio;

    cout << "ID Producto: "; 
    cin >> idProd; limpiarBuffer();

    int idxProd= -1;

    for (int i = 0; i < tienda->numProductos; i++)
    {
        if (tienda->productos[i].id == idProd)
        {
            idxProd = i;
            break;
        }
    }
        if (idxProd == -1)
        {
            cout << "ERROR: Producto no existe.\n";
            return;
        }

        cout << "ID Proveedor: ";
        cin >> IdProv; limpiarBuffer();
        if (!existeProveedor(tienda, IdProv))
        {
            cout << "ERROR: Proveedor no existe.\n";
            return;
        }

        cout << "Cantidad: ";
        cin >> cantidad; limpiarBuffer();
        if (cantidad <= 0)
        {
            cout << "ERROR: El precio tiene ser mayor a 0.\n";
            return;
        }

        cout << "Precio Unitario";
        cin >> precio; limpiarBuffer();
        if (precio <= 0)
        {
            cout << "ERROR: La cantida tiene que ser mayor a 0.\n";
            return;
        }

        float total = cantidad * precio;

        cout << "\n=== RESUMEM ===\n";
        cout << "Producto: " << tienda->productos[idxProd].nombre << "\n";
        cout << "Cantidad: " << cantidad << "\n";
        cout << "Precio unitario: " << precio << "\n";
        cout << "Total: " << total << "\n";

        char conf;
        cout << "¿Desea confirmar la compra? (Si / No): ";
        cin >> conf; limpiarBuffer();

        if (conf != 'S' || conf != 's')
        {
            cout << "Operacion cancelada.\n";
            return;
        }
        
        if (tienda->numTransacciones >= tienda->capacidadTransacciones)
        {
            redimensionarTransacciones(tienda);
        }

        Transaccion* t = &tienda->transacciones[tienda->numTransacciones];
        t->id = tienda->siguienteIdTransaccion++;
        strcpy(t->tipo, "COMPRA");
        t->idProducto = idProd;
        t->idRelacionado = IdProv;
        t->cantidad = cantidad;
        t->precioUnitario = precio;
        t->total = total;
        obtenerFechaActual(t->fecha);
        strcpy(t->descripcion, "Compra a proveedor");
        t->cancelada = false;

        //INCREMENTAR STOCK //

        tienda->productos[idxProd].stock += cantidad;
        tienda->numTransacciones++;

        cout << "OK: Compra Registrada exitosamente. Stock actualizado a:" << tienda->productos[idxProd].stock << "\n";
    
}

void registraVenta(Tienda* tienda){
    cout << "\n=== REGISTRAR VENTA ===\n";

    int idProd, idCli, cantidad;

    cout << "ID Producto: ";
    cin >> idProd; limpiarBuffer();

    int idxProd = -1;

    for (int i = 0; i < tienda->numProductos; i++)
    {
        if (tienda->productos[i].id == idProd)
        {
            idxProd = i;
            break;
        }    
    }

    if (idxProd == -1)
    {
        cout << "ERROR: Producto no existe.\n";
        return;
    }
    
    cout << "Stock disponible: "; tienda->productos[idxProd].stock << "\n";

    cout << "ID Cliente: ";
    cin >> idCli; limpiarBuffer();
    if (!= existeCliente(tienda, idCli))
    {
        cout << "ERROR: Cliente no existe.\n";
        return;
    }

    cout << "Cantidad: ";
    cin >> cantidad; limpiarBuffer();
    if (cantidad <= 0)
    {
        cout << "Error: La cantidad tiene que ser mayor a 0.\n";
        return;
    }

    if (cantidad > tienda->productos[idxProd].stock)
    {
        cout << "ERROR: Stock insuficiente. Quedan disponibles: " << tienda->productos[idxProd].stock << ", Solicitado: " << cantidad << "\n";
        return;
    }

    float precio = tienda->productos[idxProd].precio;
    float total = cantidad * precio;


    cout << "\n=== RESUMEN ===\n";
    cout << "Producto: " << tienda->productos[idxProd].nombre << "\n";
    cout << "Cliente ID: " << idCli << "\n";
    cout << "Cantidad: " << cantidad << "\n";
    cout << "Precio unitario: " << precio << "\n";
    cout << "Total: " << total << "\n";

    char conf;
    cout << "\n¿Desea confirmar la venta? (Si /No): ";
    cin >> conf; limpiarBuffer();

    if (conf != 'S' || conf != 's')
    {
        cout << "Operacion cancelada.\n";
        return;
    }

    if (tienda->numTransacciones >= tienda->capacidadTransacciones)
    {
         redimensionarTransacciones(tienda);
    }

    Transaccion* t = &tienda->transacciones[tienda->numTransacciones];
    t->id = tienda->siguienteIdTransaccion++;
    strcpy(t->tipo, "VENTA");
    t->idProducto = idProd;
    t->idRelacionado = idCli;
    t->cantidad = cantidad;
    t->precioUnitario = precio;
    t->total = total;
    obtenerFechaActual(t->fecha);
    strcpy(t->descripcion, "Venta a cliente:");
    t->cancelada = false;

//DESCREMENTAR STOCK JEJE//

    tienda->productos[idxProd].stock -= cantidad;
    tienda->numTransacciones++;

    cout << "OK: Venta registrada exitosamente. Stock actualizado a:" << tienda->productos[idxProd].stock << "\n";
}

void listarTransacciones(Tienda* tienda){
    cout << "\n=== TRANSACCIONES ===\n";
    cout << left << setw(5) << "ID" << setw(10) << "Tipo" << setw(10) << "Cantidad" << setw(12) << "Total" << setw(12) << "Fecha" << endl;
    cout << string(55, '-') << endl;


    for (int i = 0; i < tienda->numTransacciones; i++)
    {
        if (!tienda->transacciones[i].cancelada)
        {
            cout << left << setw(5) << tienda->transacciones[i].id
                         << setw(10) << tienda->transacciones[i].tipo
                         << setw(10) << tienda->transacciones[i].cantidad
                         << setw(12) << tienda->transacciones[i].total
                         << setw(12) << tienda->transacciones[i].fecha << endl;

        }
        
    }
    cout << "Total de transacciones activas: " << tienda->numTransacciones << endl;

}

void buscarTransacciones(Tienda* tienda){
    int op;
    cout << "\n=== BUSCAR TRANSACCIONES ===\n";
    cout << "1. Por ID de transaccion.\n";
    cout << "2. Por ID de producto.\n";
    cout << "3. Por ID de cliente\n";
    cout << "4. Por ID de proveedor\n";
    cout << "5. Por fecha (YYYY-MM-DD).\n";
    cout << "6. Por tipo (COMPRA/VENTA).\n";
    cout << "0. CANCELAR.\n";
    cout << "Opcion: ";
    cin >> op; limpiarBuffer();

    if (op == 0 )
    {
        return;
    }

    if (op == 1)
    {
        int id;
        cout << "ID Transaccion: ";
        cin >> id; limpiarBuffer();
        bool encontrada = false;
    
    for (int i = 0; i < tienda->numTransacciones; i++)
    {
        if (tienda->transacciones[i].id == id && !tienda->transacciones[i].cancelada)
        {
            cout << "\n=== TRANSACCION ENCONTRADA ===\n";
            cout << "ID: " << tienda->transacciones[i].id << "\n";
            cout << "Tipo: " << tienda->transacciones[i].tipo << "\n";
            cout << "Producto ID: " << tienda->transacciones[i].idProducto << "\n";
            cout << "Relacionado ID: " << tienda->transacciones[i].idRelacionado << "\n";
            cout << "Cantidad: " << tienda->transacciones[i].cantidad << "\n";
            cout << "Total: " << tienda->transacciones[i].total << "\n";
            cout << "Fecha: " << tienda->transacciones[i].fecha << "\n";
            encontrada = true;
            return;
        }
        
    }
    if (!encontrada)
    {
        cout << "Transaccion no encontrada./n";
        return;
    }
    
    } else if (op == 2)
    {
        int idProd;
        cout << "ID Producto: ";
        cin >> idProd; limpiarBuffer();
        bool encontrada = false;

        cout << "\n=== TRANSACCIONES DEL PRODUCTO ID: " << idProd << " ===\n";

    for (int i = 0; i < tienda->numTransacciones; i++)
    {
        if (tienda->transacciones[i].idProducto == idProd && !tienda->transacciones[i].cancelada)
        {
            cout << "Transaccion #" << tienda->transacciones[i].id
                 << " | " << tienda->transacciones[i].tipo
                 << " | " << tienda->transacciones[i].cantidad
                 << " | " << tienda->transacciones[i].total
                 << " | " << tienda->transacciones[i].fecha << "\n";
            encontrada = true;                                             
        }
    }
    
    if (!encontrada)
    {
        cout << "No hay transacciones para este producto.\n";
        return;
    }
    
    
    } else if (op == 3)
    {
       int idCli;
       cout << "ID Cliente: ";
       cin >> idCli; limpiarBuffer();
       bool encontrada = false;

       cout << "\n=== TRANSACCIONES DEL CLIENTE ID: " << idCli << "===\n";

    for ( int i = 0; i < tienda->numTransacciones; i++)
    {
        if (tienda->transacciones[i].idRelacionado == idCli
             && strcmp(tienda->transacciones[i].tipo, "VENTA") == 0
             && !tienda->transacciones[i].cancelada)
        {
            cout << "Transaccion #" << tienda->transacciones[i].id
                 << " | Producto ID: " << tienda->transacciones[i].idProducto
                 << " | Cantidad: " << tienda->transacciones[i].cantidad
                 << " | Total: " << tienda->transacciones[i].total
                 << " | Fecha: " << tienda->transacciones[i].fecha << "\n";
            encontrada = true;
        }
    }
    
    if (!encontrada)
    {
        cout << "No hay transacciones para este cliente.\n";
        return;
    }
    
    
    } else if (op == 4)
    {
        int idProv;
        cout << "ID Proveedor: ";
        cin >> idProv; limpiarBuffer();
        bool encontrada = false;

        cout << "\n=== TRANSACCIONES DEL PROVEEDOR ID: " << idProv << "===\n";
    
    for (int  i = 0; i < tienda->numTransacciones; i++)
    {
        if (tienda->transacciones[i].idRelacionado == idProv
             && strcmp(tienda->transacciones[i].tipo, "COMPRA") == 0
             && !tienda->transacciones[i].cancelada)
        {
            cout << "Transaccion #" << tienda->transacciones[i].id
                 << " | Producto ID: " << tienda->transacciones[i].idProducto
                 << " | Cantidad: " << tienda->transacciones[i].cantidad
                 << " | Total: " << tienda->transacciones[i].total
                 << " | Fecha: " << tienda->transacciones[i]. fecha << "\n";
            encontrada = true;

        }
        
    }
    
    if (!encontrada)
    {
        cout << "No hay transacciones para este proveedor.\n";
        return;
    }
    
    
    } else if (op == 5)
    {
        char fecha[11];
        cout << "Fecha (YYYY-MM-DD): ";
        cin.getline(fecha, 11);
        bool encontrada = false;

        cout << "\n=== TRANSACCIONES DE LA FECHA: " << fecha << " ===\n";
    
    for (int i = 0; i < tienda->numTransacciones; i++)
    {
        if (strcmp(tienda->transacciones[i].fecha, fecha) == 0 && !tienda->transacciones[i].cancelada)
            
        {
            cout << "Transaccion #" << tienda->transacciones[i].id
                << " | " << tienda->transacciones[i].tipo
                << " | Producto ID: " << tienda->transacciones[i].idProducto
                << " | Total: " << tienda->transacciones[i].total << "\n";
            encontrada = true;
        }
    }

    if (!encontrada)
    {
        cout << "No hay transacciones en estas fechas.\n";
        return;
    }
    
    } else if (op == 6)
    {
        char tipo[10];
        cout << "Tipo (COMPRA/VENTA): ";
        cin.getline(tipo, 10);
        bool encontrada = false;
        
        cout << "\n=== TRANSACCIONES DE TIPO ===\n";
    
    for (int i = 0; i < tienda->numTransacciones; i++)
    {
        if (strcmp(tienda->transacciones[i].tipo, tipo) == 0 && !tienda->transacciones[i].cancelada)
        {
            cout << "Transaccion #" << tienda->transacciones[i].id
                 << " | Producto ID: " << tienda->transacciones[i].idProducto
                 << " | Cantidad: " << tienda->transacciones[i].cantidad
                 << " | Total: " << tienda->transacciones[i].total
                 << " | Fecha: " << tienda->transacciones[i].fecha << "\n";

            encontrada = true;
        }
    }

    if (!encontrada)
     {
        cout << "No hay transacciones de este tipo.\n";
        return;
     }
    }
}

void cancelarTransaccion(Tienda* tienda){
    int id;
    cout << "ID Transaccion a anular: ";
    cin >> id; limpiarBuffer();

    for (int i = 0; i < tienda->numTransacciones; i++)
    {
        if (tienda->transacciones[i].id == id && !tienda->transacciones[i].cancelada)
        {
           cout << "\n=== DETALLES ===\n";
           cout << "ID: " << tienda->transacciones[i].id << "\n";
           cout << "Tipo: " << tienda->transacciones[i].tipo << "\n";
           cout << "Cantidad: " << tienda->transacciones[i].cantidad << "\n";
           cout << "Total: " << tienda->transacciones[i].total << "\n";
           
           cout << "\nADVERTENCIA: Esto revertira el stock.\n";

         if (strcmp(tienda->transacciones[i].tipo, "COMPRA") == 0)
           {
             cout << "- SE RESTARAN " << tienda->transacciones[i].cantidad <<  "unidades del stock.\n";
            
            } else{
             
             cout << "- SE SUMARAN " << tienda->transacciones[i].cantidad << "unidades del stock.\n";
           }

           char conf;
           cout << "\n¿Confirmar cancelacion? (Si/No): ";
           cin >> conf; limpiarBuffer();

        if (conf =='S' || conf == 's'){
          
        for (int j = 0; j < tienda->numProductos; j++)
            {
            if (tienda->productos[j].id == tienda->transacciones[i].idProducto)
            {
                
            if (strcmp(tienda->transacciones[i].tipo, "COMPRA") == 0){
              
              tienda->productos[j].stock -= tienda->transacciones[i].cantidad;
               } else {
                    tienda->productos[j].stock += tienda->transacciones[i].cantidad;

               }
                 break;
                     
            }
            
            }

              tienda->transacciones[i].cancelada = true;
              cout << "Transaccion anulada. Stock revertido.\n";

              
           } else {
            
            cout << "Operacion cancelada.\n";

           }
           return;
           
           
           
        }
    }

    cout << "Transaccion no encotrada.\n";
    
}

void reporteGanancias(Tienda* tienda){
    float totalVentas = 0, totalCompras = 0;

    for (int i = 0; i < tienda->numTransacciones; i++)
    {
        if (!tienda->transacciones[i].cancelada)
        {
            if (strcmp(tienda->transacciones[i].tipo, "VENTA") == 0)
            {
                totalVentas += tienda->transacciones[i].total;
            } else {
                totalCompras += tienda->transacciones[i].total;
            }
            
        }
        
    }
    
    cout << "\n=== REPORTE DE GANANIAS ===\n";
    cout << "Total Ventas: " << totalVentas << "\n";
    cout << "Total Compras> " << totalCompras << "\n";
    cout << "Ganancia Obtenida: " << (totalVentas - totalCompras) << "\n";

}

void MenuReportes(Tienda* tienda){
    int op;
    do
    {
        cout << "\n=== REPORTES ===\n";
        cout << "1. Ganancias.\n";
        cout << "2. Stock bajo.\n";
        cout << "3. Volver.\n";
        cout << "\nOpcion: ";
        cin >> op; limpiarBuffer();

    switch (op)
    {
    case 1:
        reporteGanancias(tienda);
        break;
    
    case 2:
        alertasStockBajo(tienda);
        break;
    }
    } while (op != 3);
}

// MENU ESTABLECIDO //

void menuProductos(Tienda* tienda){
    int op;
    do
    {
        cout << "\n=== GESTION DE CALZADOS ===\n";
        cout << "1. Registrar calzado\n";
        cout << "2. Buscar calzado\n";
        cout << "3. actualizar calzado\n";
        cout << "4. Ajustra stock.\n";
        cout << "5. Listar calzado.\n";
        cout << "6. Filtrar por talla.\n";
        cout << "7. Filtra por marca.\n";
        cout << "8. Eliminar calzado.\n";
        cout << "0. CANCELAR.\n";
        cout << "Opcion: ";
        cin >> op; limpiarBuffer();

    switch (op)
    {
    case 1:
        crearProducto(tienda);        
        break;
    
    case 2:
        buscarProducto(tienda);
        break;

    case 3:
        actualizarProducto(tienda);
        break;

    case 4:
        actualizarStockProducto(tienda);
        break;

    case 5:
        listarProductos(tienda);
        break;
    
    case 6:
        filtrarPorTalla(tienda);
        break;
    
    case 7:
        filtrarPorMarca(tienda);
        break;
    
    case 8:
        eliminarProducto(tienda);

    }
    } while (op != 0);
    
}

void menuProveedores(Tienda* tienda){
    int op;
    do
    {
        cout << "\n=== GESTION DE PROVEEDORES ===\n";
        cout << "1. Registrar proveedor\n";
        cout << "2. Buscar proveedor.\n";
        cout << "3. Actualizar Proveedor.\n";
        cout << "4. Listar proveedor.\n";
        cout << "5. Eliminar proveedor.\n";
        cout << "0. Volver\n";
        cout << "Opcion: ";
        cin >> op; limpiarBuffer();

    switch (op)
    {
    case 1:
        crearProveedor(tienda);
        break;
    
    case 2:
        buscarProveedor(tienda);
        break;

    case 3:
        actualizarProveedor(tienda);
        break;

    case 4:
        listarProveedores(tienda);
        break;

    case 5:
        eliminarProveedor(tienda);
        break;
    }

    } while (op != 0);
    
}

void menuClientes(Tienda* tienda){
    int op;
    do
    {
        cout << "\n=== GESTION DE CLIENTES ===\n";
        cout << "1. Registrar Cliente.\n";
        cout << "2. Buscar Cliente.\n";
        cout << "3. Actualizar Cliente.\n";
        cout << "4. Listar Cliente.\n";
        cout << "5. Eliminar Cliente.\n";
        cout << "0. Volver\n";
        cout << "Opcion:";
        cin >> op; limpiarBuffer();

    switch (op)
    {
    case 1:
        crearCliente(tienda);
        break;
    
    case 2:
        buscarCliente(tienda);
        break;

    case 3:
        actualizarCliente(tienda);
        break;

    case 4:
        listarClientes(tienda);
        break;

    case 5:
        eliminarCliente(tienda);
        break;
    }
    } while (op != 0);
    
}

void menuTransacciones(Tienda* tienda){
    int op;
    do
    {
        cout << "\n=== GESTION DE TRANSACCIONES ===\n";
        cout << "1. Registrar Compra.\n";
        cout << "2. Registrar Venta.\n";
        cout << "3. Buscar transaccion.\n";
        cout << "4. Listar transaccion.\n";
        cout << "5. Cancelar transaccion.\n";
        cout << "0. volver\n";
        cout << "Opcion: ";
        cin >> op; limpiarBuffer();

    switch (op)
    {
    case 1:
        registrarCompra(tienda);
        break;
    
    case 2:
        registrarVenta(tienda);
        break;

    case 3:
        buscarTransacciones(tienda);
        break;

    case 4:
        listarTransacciones(tienda);
        break;

    case 5:
        cancelarTransaccion(tienda);
        break;
    }

    } while (op != 0);
    
}

void menuPrincipal(Tienda* tienda){
    int op;
    do
    {
        cout << "\n";
        cout << "========================================\n";
        cout << "   STAR SHOES\n";
        cout << "   Sistema de Gestion de Inventarios\n";
        cout << "========================================\n";
        cout << "   Tienda: " << tienda->nombre << "\n";
        cout << "   Rif: " << tienda->rif << "\n";
        cout << "==========================================\n";
        cout << "1. Gestion de Calzado\n";
        cout << "2. Gestion de Proveedores\n";
        cout << "3. Gestion de Clientes\n";
        cout << "4. Manejo de transacciones\n";
        cout << "5. Reportes\n";
        cout << "0. Salir\n";
        cout << "===========================================\n";
        cout << "Opcion: ";
        cin >> op; limpiarBuffer();

    switch (op)
    {
    case 1:
        menuProductos(tienda);
        break;
    
    case 2:
        menuProveedores(tienda);
        break;

    case 3:
        menuClientes(tienda);
        break;

    case 4:
        menuTransacciones(tienda);
        break;

    case 5:
        menuReportes(tienda);
        break;

    case 0:
        if (tienda->numProductos > 0 || tienda->numTransacciones > 0)
        {
            char conf;
            cout << "\n¿Desea salir? Hay datos registrados que no se han guardado estimado. (Si / No): ";
            cin >> conf; limpiarBuffer();
            if (conf != 'S' && conf != 's')
            {
                break;
            }
            cout << "Vertale Gracias por usar este maravilloso sistema!\n";
             return;
        default: cout << "Opcion invalida.\n";
        }
    }
} while (op != 0);
    
}

// ULTIMA PARTE POR FIN DIOSMIO //
// FUNCIAN PRINCIPAL //

int main() {
    cout << "\n==============================================\n";
    cout << "    UNIVERSIDAD RAFAEL URDANETA\n";
    cout << "    PROGRAMACION 2 - PROYECTO 1\n";
    cout << "    Autores: [JULIO MARIN - HANNA FUENTES]\n";
    cout << "    TRIMESTRE: 2026-A\n";
    cout << "=================================================\n";

    Tienda miTienda;
    inicializarTienda(&miTienda, "STAR SHOES", "J-320700531");

    menuPrincipal(&miTienda);

    liberarTienda(&miTienda);

    return 0;

}
