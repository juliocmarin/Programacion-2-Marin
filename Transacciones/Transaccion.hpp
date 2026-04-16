#ifndef TRANSACCION_HPP
#define TRANSACCION_HPP

#include <ctime>
#include "../Persistencias/Constantes.hpp"

// struct auxiliar para cada item de la transaccion
// se mantiene como struct porque es solo un contenedor de datos
struct ItemTransaccion {
    int   idProducto;
    int   cantidad;
    float precioUnitario;
    float subTotal;
};

class Transaccion {
private:
    int             id;
    char            tipo[10];        // "VENTA" o "COMPRA"
    int             idRelacionado;   // idCliente si VENTA, idProveedor si COMPRA
    ItemTransaccion items[MAX_ITEMS];
    int             cantItems;
    float           total;
    char            descripcion[200];
    bool            cancelada;
    time_t          fechaCreacion;
    time_t          fechaModif;

public:
    // CONSTRUCTORES //
    Transaccion();
    Transaccion(const char* tipo, int idRelacionado);
    Transaccion(const Transaccion& otro);
    ~Transaccion();

    // GETTERS //
    int                   getId()            const;
    const char*           getTipo()          const;
    int                   getIdRelacionado() const;
    int                   getCantItems()     const;
    float                 getTotal()         const;
    const char*           getDescripcion()   const;
    bool                  isCancelada()      const;
    time_t                getFechaCreacion() const;
    time_t                getFechaModif()    const;
    const ItemTransaccion* getItems()        const;

    // SETTERS //
    void setId(int id);
    void setTipo(const char* tipo);
    void setIdRelacionado(int id);
    void setDescripcion(const char* descripcion);
    void setCancelada(bool cancelada);
    void setFechaCreacion(time_t fecha);
    void setFechaModif(time_t fecha);

    // METODOS DE GESTION //
    bool agregarItem(int idProducto, int cantidad, float precioUnitario);
    bool esVenta()   const;  // retorna true si tipo == "VENTA"
    bool esCompra()  const;  // retorna true si tipo == "COMPRA"

    // METODOS DE PRESENTACION //
    void mostrarInformacionBasica()   const;
    void mostrarInformacionCompleta() const;

    // METODO ESTATICO //
    static int obtenerTamano();
};

#endif 