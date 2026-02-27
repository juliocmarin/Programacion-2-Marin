#  Sistema de Gestión De Inventarios de Productos
## Realizado por: Julio Marin y Hanna Fuentes

## 1. Descripción del Proyecto

### 🎯 Objetivo General
Desarrollar un sistema completo de gestión de inventario para una zapatería que permita administrar productos, proveedores, clientes y transacciones comerciales, demostrando dominio de punteros, memoria dinámica y estructuras de datos en C++, mediante la implementación de operaciones CRUD completas, manejo de memoria con redimensionamiento automático de arrays,algoritmos de búsqueda y validación eficientes.

### ⚙️ Funcionalidades Principales
- CRUD completo de Productos
- CRUD completo de Proveedores
- CRUD completo de clientes 
- Transacciones (Compra y venta)
- Busquedas por ID, nombre, codido, entre otros.
- Validaciones (email, precio, stock, duplicados)
- Memoria dinamica con new/delete
- Redimensionamiento de arrays

---

## 2. Instrucciónes de compilacion.
- Identificar ruta en consola.
- Colocar la siguiente expresión: g++ -std=c++11 star_shoes.cpp -o star_shoes 

## 3. Instrucciones de ejecución:
 - Identificar ruta en consola
 - Colocar la expresión ./star_shoes.exe para ejecutar

## 4. Estructuras de Datos

### 👟 `Producto`
Almacena información de cada calzado:
- ID automático (autoincremental)
- Código único (ej. "CAL-001")
- Nombre, descripción
- ID del proveedor asociado
- Precio de compra y venta
- Stock actual y stock mínimo (alerta)
- Fecha de registro (YYYY-MM-DD)
- Campos extra para zapateria: marca, modelo, talla, color, género, material

### 🚚 `Proveedor`
Contiene datos del proveedor de calzados:
- ID automatico
- Nombre comercial
- RIF (unico)
- Telefono, email, dirección
- Fecha de registro

### 👤 `Cliente`
Almacena información del cliente:
- ID automatico
- Nombre completo
- Cedula/RIF
- Teléfono, email, direccion
- Fecha de registro

### 💰 `Transaccion`
Representa una operación comercial:
- ID automatico
- Tipo: "COMPRA" o "VENTA"
- ID del producto involucrado
- ID relacionado (proveedor para compras, cliente para ventas)
- Cantidad, precio unitario, total
- Fecha, descripcion
- Estado (activa o cancelada)

### 🏪 `Tienda`
Estructura principal que centraliza todo:
- Nombre y RIF de la tienda
- Arrays dinamicos de Producto, Proveedor, Cliente, Transaccion
- Contadores de elementos y capacidades
- Contadores de IDs autoincrementales

### 🔗 Relaciones entre Estructuras
- `Producto` está vinculado a un Proveedor por ID.
- `Transaccion` enlaza Producto con Proveedor (compra) o Cliente (venta).
- `Tienda` contiene y gestiona todos los arrays dinámicos.
- El stock se ajusta automáticamente según las transacciones.

---

## 5. Funciones Principales

### 👤 Modulo de Productos

| Funcion | Descripcion |
|--------|-------------|
| `crearProducto` | Crea y valida un nuevo producto. |
| `buscarProducto` / `Por ID` / `Por nombre` / `Por marca` / `Por codigo`/  Búsqueda por distintos criterios. |
| `actualizarProducto` | Modifica datos como marca, precio, stock. |
| `actualizarStockProducto` | Ajuste manual de stock (+/-) con validación de no negativo. | 
| `eliminarProducto` | Libera memoria y compacta el array. |
| `listarProducto` | Muestra tabla con datos importantes. |
| `filtrarPorTalla` | Extra: Muestra calzados disponibles por talla específica. |
| `filtrarPorMarca` | Extra: Muestra calzados de una marca específica. |
| `alertasStockBajo` | Extra: Lista productos con stock <= mínimo configurado. |

### 🚚 Modulo de Proveedores

| Funcion | Descripcion |
|--------|-------------|
| `crearProveedor` | Registra un nuevo proveedor con validaciones. |
| `buscarProveedor`| Búsqueda por ID, nombre o rif. |
| `actualizarProveedor` | Modifica datos de contacto con confirmacion. |
| `listarProveedores` | Muestra tabla con datos importantes. |
| `eliminarProveedor` | Elimina proveedor (advertencia si tiene productos asociados). |

### 👤 Módulo de Clientes

| Funcion | Descripcion |
|--------|-------------|
| `crearCliente` | Registra cliente validando email y cedula. |
| `buscarCliente` | Busqueda por ID, nombre o cedula. |
| `actualizarCliente` | Modifica datos de contacto. |
| `listarClientes` | Muestra tabla con ID, nombre, cédula. |
| `eliminarCliente` | Elimina cliente con confirmacion. |

### 💰 Modulo de Transacciones

| Funcion | Descripcion |
|--------|-------------|
| `registrarCompra` | Registra compra a proveedor, incrementa stock automaticamente. |
| `registrarVenta` | Registra venta a cliente, decrementa stock (valida stock suficiente). |
| `buscarTransacciones` | Busqueda por ID transaccion, producto, cliente, proveedor, fecha o tipo. |
| `listarTransacciones` | Muestra tabla con ID, tipo, cantidad, total, fecha. |
| `cancelarTransaccion` | Anula transaccion y revierte stock (compra resta, venta suma). |
| `reporteGanancias` | Extra: Calcula total ventas - total compras = ganancia neta. |



### 🏪 Inicializacion y Memoria

| Funcion | Descripcion |
|--------|-------------|
| `inicializarTienda` | Crea tienda con capacidad inicial de 5 elementos por array. |
| `redimensionarProductos` | Duplica capacidad de productos cuando se llena. |
| `redimensionarProveedores` | Duplica capacidad de proveedores cuando se llena. |
| `redimensionarClientes` | Duplica capacidad de clientes cuando se llena. |
| `redimensionarTransacciones` | Duplica capacidad de transacciones cuando se llena. |
| `liberarTienda` | Libera toda la memoria dinamica con delete[] antes de salir. |

---

## 6. Decisiones de Diseño

### 📌 Uso de Arrays Dinamicos
Se eligieron arrays dinamicos para:
- Controlar el crecimiento de datos sin límite fijo.
- Evitar uso de STL para mantener compatibilidad académica.
- Permitir redimensionamiento manual y control de memoria.

### 🔁 Estrategia de Redimensionamiento
- Cada array se duplica cuando se alcanza su capacidad.
- Se copian los elementos existentes al nuevo array.
- Se libera la memoria anterior con `delete[]`.

### 🧠 Manejo de Memoria
- Asignacion: new para todos los arrays dinámicos en `inicializarTienda()`.
- Liberacion: `delete[]` en `liberarTienda()` para los 4 arrays principales.
- Seguridad: Después de cada `delete[]`, los punteros se == a `nullptr` para evitar punteros colgantes.
- Sin fugas: Cada new tiene su `delete[]` correspondiente.

---
