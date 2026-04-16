#ifndef PROVEEDOR_HPP
#define PROVEEDOR_HPP

#include <ctime>
#include "../Persistencias/Constantes.hpp"


class Proveedor {
private:
    int    id;
    char   nombre[100];
    char   rif[20];
    char   telefono[20];
    char   email[100];
    char   direccion[200];
    int    productosIds[MAX_PRODS];
    int    cantProductos;
    int    transIDs[MAX_HIST];
    int    cantTrans;
    float  totalComprado;
    bool   eliminado;
    time_t fechaCreacion;
    time_t fechaModif;

public:
    // CONSTRUCTORES //
    Proveedor();
    Proveedor(const char* nombre, const char* rif, const char* telefono, const char* email);
    Proveedor(const Proveedor& otro);
    ~Proveedor();

    // GETTERS //
    int         getId()            const;
    const char* getNombre()        const;
    const char* getRif()           const;
    const char* getTelefono()      const;
    const char* getEmail()         const;
    const char* getDireccion()     const;
    int         getCantProductos() const;
    int         getCantTrans()     const;
    float       getTotalComprado() const;
    bool        isEliminado()      const;
    time_t      getFechaCreacion() const;
    time_t      getFechaModif()    const;
    const int*  getProductosIds()  const;
    const int*  getTransIDs()      const;

    // SETTERS //
    void setId(int id);
    void setNombre(const char* nombre);
    void setRif(const char* rif);
    void setTelefono(const char* telefono);
    bool setEmail(const char* email);       // retorna false si email invalido
    void setDireccion(const char* direccion);
    void setEliminado(bool eliminado);
    void setFechaCreacion(time_t fecha);
    void setFechaModif(time_t fecha);

    // METODOS DE GESTION DE RELACIONES //
    bool agregarProductoID(int idProducto);
    bool agregarTransaccionID(int idTransaccion);
    void sumarTotalComprado(float monto);

    // METODOS DE PRESENTACION //
    void mostrarInformacionBasica()   const;
    void mostrarInformacionCompleta() const;

    // METODO ESTATICO //
    static int obtenerTamano();
};

#endif