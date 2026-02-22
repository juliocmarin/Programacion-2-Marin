#include <iostream>
#include <fstream>
#include <sstream>
using namespace std;

struct Producto
{
    int id;
    string nombre;
    string codigo;
    string descripcion;
    int idProveedor;
    float precio;
    int stock;
    string fechaRegistro;

};

int main(){

    ifstream archivo (" csv_productos.csv");
    string linea;
if (archivo.is_open()){

    cout <<"EL ARCHIVO FUE ABIERTO CORRECTAMENTE." <<endl;
}
while (getline(archivo, linea))
{
    
}


 {archivo.close();
    return 0;

}
}