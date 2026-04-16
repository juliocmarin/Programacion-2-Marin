#ifndef OPERACIONES_PRODUCTOS_HPP
#define OPERACIONES_PRODUCTOS_HPP

// OPERACIONES DE PRODUCTOS //
// contiene toda la logica de negocio de productos
// solicita datos al usuario, valida, llama a GestorArchivos y muestra resultados
// la clase Producto NO sabe nada de esto, solo representa los datos

void registrarProducto();
void buscarProducto();
void actualizarProducto();
void listarProductos();
void eliminarProducto();
void ajustarStock();
void filtrarPorTalla();
void filtrarPorMarca();

#endif