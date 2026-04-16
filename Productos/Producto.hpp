#ifndef PRODUCTO_HPP
#define PRODUCTO_HPP

#include <ctime>
#include "../Persistencias/Constantes.hpp"



class Producto {
private:
    // ATRIBUTOS PRIVADOS - mismos campos del struct del Proyecto 2 //
    int    id;
    char   codigo[20];
    char   nombre[100];
    char   descripcion[200];
    int    idProveedor;
    float  precio;
    int    stock;
    char   fechaRegistro[11];
    char   marca[50];
    char   modelo[50];
    char   talla[10];
    char   color[30];
    char   genero[20];
    char   material[50];
    float  precioCompra;
    int    stockMinimo;
    int    totalVendido;
    int    transIDs[MAX_HIST];
    int    cantTrans;
    bool   eliminado;
    time_t fechaCreacion;
    time_t fechaModif;

public:
    // CONSTRUCTORES //
    Producto();                          // constructor por defecto
    Producto(const char* codigo,
             const char* nombre,
             const char* marca,
             float precio,
             int stock,
             int idProveedor);           // constructor con parametros principales
    Producto(const Producto& otro);      // constructor de copia
    ~Producto();                         // destructor

    // GETTERS - todos const porque no modifican el objeto //
    int          getId()          const;
    const char*  getCodigo()      const;
    const char*  getNombre()      const;
    const char*  getDescripcion() const;
    int          getIdProveedor() const;
    float        getPrecio()      const;
    int          getStock()       const;
    const char*  getMarca()       const;
    const char*  getModelo()      const;
    const char*  getTalla()       const;
    const char*  getColor()       const;
    const char*  getGenero()      const;
    const char*  getMaterial()    const;
    float        getPrecioCompra()const;
    int          getStockMinimo() const;
    int          getTotalVendido()const;
    int          getCantTrans()   const;
    bool         isEliminado()    const;
    time_t       getFechaCreacion()const;
    time_t       getFechaModif()  const;
    const int*   getTransIDs()    const;

    // SETTERS - con validacion interna //
    void setId(int id);
    void setCodigo(const char* codigo);
    void setNombre(const char* nombre);
    void setDescripcion(const char* descripcion);
    void setIdProveedor(int idProveedor);
    bool setPrecio(float precio);          // retorna false si precio <= 0
    bool setStock(int stock);              // retorna false si stock < 0
    void setMarca(const char* marca);
    void setModelo(const char* modelo);
    void setTalla(const char* talla);
    void setColor(const char* color);
    void setGenero(const char* genero);
    void setMaterial(const char* material);
    bool setPrecioCompra(float precioCompra); // retorna false si <= 0
    bool setStockMinimo(int stockMinimo);     // retorna false si < 0
    void setEliminado(bool eliminado);
    void setFechaCreacion(time_t fecha);
    void setFechaModif(time_t fecha);

    // METODOS DE GESTION DE RELACIONES //
    bool agregarTransaccionID(int idTransaccion); // agrega al arreglo transIDs
    void incrementarVendido(int cantidad);
    bool stockSuficiente(int cantidad) const;     // verifica si hay stock para cantidad
    bool stockCritico() const;                    // stock <= stockMinimo

    // METODOS DE PRESENTACION //
    void mostrarInformacionBasica()   const;
    void mostrarInformacionCompleta() const;

    // METODO ESTATICO - retorna sizeof para calcular offsets en archivos //
    static int obtenerTamano();
};

#endif