#include "Producto.hpp"
#include <cstring>
#include <iostream>
#include <iomanip>
using namespace std;

// =====================================================================
// CONSTRUCTORES
// =====================================================================

// constructor por defecto - inicializa todo en cero/vacio
Producto::Producto() {
    id           = 0;
    idProveedor  = 0;
    precio       = 0.0f;
    precioCompra = 0.0f;
    stock        = 0;
    stockMinimo  = 5;
    totalVendido = 0;
    cantTrans    = 0;
    eliminado    = false;
    fechaCreacion= 0;
    fechaModif   = 0;

    // limpiar todos los char[] con memset para no dejar basura en el .bin
    memset(codigo,      0, sizeof(codigo));
    memset(nombre,      0, sizeof(nombre));
    memset(descripcion, 0, sizeof(descripcion));
    memset(fechaRegistro,0,sizeof(fechaRegistro));
    memset(marca,       0, sizeof(marca));
    memset(modelo,      0, sizeof(modelo));
    memset(talla,       0, sizeof(talla));
    memset(color,       0, sizeof(color));
    memset(genero,      0, sizeof(genero));
    memset(material,    0, sizeof(material));
    memset(transIDs,    0, sizeof(transIDs));
}

// constructor con parametros principales
Producto::Producto(const char* cod, const char* nom, const char* mar,
                   float prec, int stk, int idProv) : Producto() {
    // llama al constructor por defecto primero para limpiar todo
    setCodigo(cod);
    setNombre(nom);
    setMarca(mar);
    setPrecio(prec);
    setStock(stk);
    setIdProveedor(idProv);
    fechaCreacion = time(0);
    fechaModif    = time(0);
}

// constructor de copia
Producto::Producto(const Producto& otro) {
    id           = otro.id;
    idProveedor  = otro.idProveedor;
    precio       = otro.precio;
    precioCompra = otro.precioCompra;
    stock        = otro.stock;
    stockMinimo  = otro.stockMinimo;
    totalVendido = otro.totalVendido;
    cantTrans    = otro.cantTrans;
    eliminado    = otro.eliminado;
    fechaCreacion= otro.fechaCreacion;
    fechaModif   = otro.fechaModif;

    strncpy(codigo,       otro.codigo,       19);  codigo[19]       = '\0';
    strncpy(nombre,       otro.nombre,       99);  nombre[99]       = '\0';
    strncpy(descripcion,  otro.descripcion,  199); descripcion[199] = '\0';
    strncpy(fechaRegistro,otro.fechaRegistro,10);  fechaRegistro[10]= '\0';
    strncpy(marca,        otro.marca,        49);  marca[49]        = '\0';
    strncpy(modelo,       otro.modelo,       49);  modelo[49]       = '\0';
    strncpy(talla,        otro.talla,        9);   talla[9]         = '\0';
    strncpy(color,        otro.color,        29);  color[29]        = '\0';
    strncpy(genero,       otro.genero,       19);  genero[19]       = '\0';
    strncpy(material,     otro.material,     49);  material[49]     = '\0';

    for (int i = 0; i < MAX_HIST; i++) transIDs[i] = otro.transIDs[i];
}

// destructor - no hay memoria dinamica pero el profe lo pide
Producto::~Producto() {}


// =====================================================================
// GETTERS
// =====================================================================

int         Producto::getId()           const { return id; }
const char* Producto::getCodigo()       const { return codigo; }
const char* Producto::getNombre()       const { return nombre; }
const char* Producto::getDescripcion()  const { return descripcion; }
int         Producto::getIdProveedor()  const { return idProveedor; }
float       Producto::getPrecio()       const { return precio; }
int         Producto::getStock()        const { return stock; }
const char* Producto::getMarca()        const { return marca; }
const char* Producto::getModelo()       const { return modelo; }
const char* Producto::getTalla()        const { return talla; }
const char* Producto::getColor()        const { return color; }
const char* Producto::getGenero()       const { return genero; }
const char* Producto::getMaterial()     const { return material; }
float       Producto::getPrecioCompra() const { return precioCompra; }
int         Producto::getStockMinimo()  const { return stockMinimo; }
int         Producto::getTotalVendido() const { return totalVendido; }
int         Producto::getCantTrans()    const { return cantTrans; }
bool        Producto::isEliminado()     const { return eliminado; }
time_t      Producto::getFechaCreacion()const { return fechaCreacion; }
time_t      Producto::getFechaModif()   const { return fechaModif; }
const int*  Producto::getTransIDs()     const { return transIDs; }


// =====================================================================
// SETTERS - con validacion
// =====================================================================

void Producto::setId(int i)                    { id = i; }
void Producto::setIdProveedor(int idProv)      { idProveedor = idProv; }
void Producto::setEliminado(bool e)            { eliminado = e; }
void Producto::setFechaCreacion(time_t f)      { fechaCreacion = f; }
void Producto::setFechaModif(time_t f)         { fechaModif = f; }

void Producto::setCodigo(const char* cod) {
    if (!cod) return;
    strncpy(codigo, cod, 19); codigo[19] = '\0';
}

void Producto::setNombre(const char* nom) {
    if (!nom) return;
    strncpy(nombre, nom, 99); nombre[99] = '\0';
}

void Producto::setDescripcion(const char* desc) {
    if (!desc) return;
    strncpy(descripcion, desc, 199); descripcion[199] = '\0';
}

void Producto::setMarca(const char* mar) {
    if (!mar) return;
    strncpy(marca, mar, 49); marca[49] = '\0';
}

void Producto::setModelo(const char* mod) {
    if (!mod) return;
    strncpy(modelo, mod, 49); modelo[49] = '\0';
}

void Producto::setTalla(const char* tal) {
    if (!tal) return;
    strncpy(talla, tal, 9); talla[9] = '\0';
}

void Producto::setColor(const char* col) {
    if (!col) return;
    strncpy(color, col, 29); color[29] = '\0';
}

void Producto::setGenero(const char* gen) {
    if (!gen) return;
    strncpy(genero, gen, 19); genero[19] = '\0';
}

void Producto::setMaterial(const char* mat) {
    if (!mat) return;
    strncpy(material, mat, 49); material[49] = '\0';
}

// setters con validacion - retornan false si el valor es invalido
bool Producto::setPrecio(float p) {
    if (p <= 0) return false; // precio no puede ser 0 ni negativo
    precio = p;
    return true;
}

bool Producto::setPrecioCompra(float p) {
    if (p <= 0) return false;
    precioCompra = p;
    return true;
}

bool Producto::setStock(int s) {
    if (s < 0) return false; // stock no puede ser negativo
    stock = s;
    return true;
}

bool Producto::setStockMinimo(int s) {
    if (s < 0) return false;
    stockMinimo = s;
    return true;
}


// =====================================================================
// METODOS DE GESTION DE RELACIONES
// =====================================================================

// agrega el ID de una transaccion al historial del producto
bool Producto::agregarTransaccionID(int idTransaccion) {
    if (cantTrans >= MAX_HIST) return false; // arreglo lleno
    transIDs[cantTrans++] = idTransaccion;
    fechaModif = time(0);
    return true;
}

// suma al contador de unidades vendidas
void Producto::incrementarVendido(int cantidad) {
    if (cantidad > 0) totalVendido += cantidad;
}

// verifica si hay suficiente stock para una cantidad pedida
bool Producto::stockSuficiente(int cantidad) const {
    return stock >= cantidad && cantidad > 0;
}

// retorna true si el stock llego al minimo o lo supero
bool Producto::stockCritico() const {
    return stock <= stockMinimo;
}


// =====================================================================
// METODOS DE PRESENTACION
// =====================================================================

void Producto::mostrarInformacionBasica() const {
    cout << left
         << setw(5)  << id
         << setw(12) << codigo
         << setw(20) << nombre
         << setw(10) << marca
         << setw(7)  << talla
         << "$ " << setw(9) << fixed << setprecision(2) << precio
         << setw(7)  << stock
         << "\n";
}

void Producto::mostrarInformacionCompleta() const {
    cout << "\n  ID          : " << id          << "\n";
    cout << "  Codigo      : " << codigo       << "\n";
    cout << "  Marca       : " << marca        << "\n";
    cout << "  Modelo      : " << modelo       << "\n";
    cout << "  Nombre      : " << nombre       << "\n";
    cout << "  Talla       : " << talla        << "\n";
    cout << "  Color       : " << color        << "\n";
    cout << "  Genero      : " << genero       << "\n";
    cout << "  Material    : " << material     << "\n";
    cout << "  P. Compra   : $ " << fixed << setprecision(2) << precioCompra << "\n";
    cout << "  P. Venta    : $ " << fixed << setprecision(2) << precio       << "\n";
    cout << "  Stock       : " << stock        << " uds\n";
    cout << "  Stock Min.  : " << stockMinimo  << "\n";
    cout << "  Vendidos    : " << totalVendido << " en total\n";
    if (stockCritico()) cout << "  [ALERTA] Stock bajo!\n";
}


// =====================================================================
// METODO ESTATICO
// =====================================================================

// el profe lo pide para poder calcular offsets en GestorArchivos
// offset = sizeof(ArchivoHeader) + (indice * Producto::obtenerTamano())
int Producto::obtenerTamano() {
    return sizeof(Producto);
}