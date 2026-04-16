#ifndef CLIENTE_HPP
#define CLIENTE_HPP

#include <ctime>
#include "../Persistencias/Constantes.hpp"

class Cliente {
private:
    int    id;
    char   nombre[100];
    char   cedula[20];
    char   telefono[20];
    char   email[100];
    char   direccion[200];
    int    comprasIDs[MAX_HIST];
    int    cantCompras;
    float  totalGastado;
    bool   eliminado;
    time_t fechaCreacion;
    time_t fechaModif;

public:
    // CONSTRUCTORES //
    Cliente();
    Cliente(const char* nombre, const char* cedula, const char* telefono, const char* email);
    Cliente(const Cliente& otro);
    ~Cliente();

    // GETTERS //
    int         getId()           const;
    const char* getNombre()       const;
    const char* getCedula()       const;
    const char* getTelefono()     const;
    const char* getEmail()        const;
    const char* getDireccion()    const;
    int         getCantCompras()  const;
    float       getTotalGastado() const;
    bool        isEliminado()     const;
    time_t      getFechaCreacion()const;
    time_t      getFechaModif()   const;
    const int*  getComprasIDs()   const;

    // SETTERS //
    void setId(int id);
    void setNombre(const char* nombre);
    void setCedula(const char* cedula);
    void setTelefono(const char* telefono);
    bool setEmail(const char* email);
    void setDireccion(const char* direccion);
    void setEliminado(bool eliminado);
    void setFechaCreacion(time_t fecha);
    void setFechaModif(time_t fecha);

    // METODOS DE GESTION DE RELACIONES //
    bool agregarCompraID(int idTransaccion);
    void sumarTotalGastado(float monto);

    // METODOS DE PRESENTACION //
    void mostrarInformacionBasica()   const;
    void mostrarInformacionCompleta() const;

    // METODO ESTATICO //
    static int obtenerTamano();
};

#endif