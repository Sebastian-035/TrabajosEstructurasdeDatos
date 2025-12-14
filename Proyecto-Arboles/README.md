Sistema de Archivos Jerárquico
- Interfaz de Consola-

-¿Qué es?-
Es una implementación de un sistema de archivos jerárquico simulado, gestionado a través de una interfaz de línea de comandos (CLI) en C++. El sistema se basa en una estructura de 
árbol (Arbol/ArbolGestor) donde los nodos representan archivos (`ARCHIVO`) o directorios/carpetas (`CARPETA`), incluyendo funcionalidades de gestión de archivos, búsquedas avanzadas
y una papelera de reciclaje.

-¿Para qué sirve?-
El propósito principal es:

1.-Simular la funcionalidad básica de un sistema operativo (SO): para la gestión de archivos y directorios (como las funciones `mkdir`, `touch`, `ls`, `rm`, `mv`, `cat`, etc.).
2.-Demostrar la implementación de estructuras de datos jerárquicas (árboles): para la organización y manipulación de datos.
3.-Proveer un entorno de prueba: para algoritmos de búsqueda (por nombre, por prefijo) y persistencia de datos (guardado/cargado en JSON).

-Funcionamiento General-
El sistema opera mediante un bucle de comandos en la clase `Consola`, que interactúa con la lógica central de gestión del árbol (`Arbol`).

1.-Inicio:Al ejecutarse, la consola muestra un mensaje de bienvenida y el prompt, indicando la ruta actual (generalmente la raíz `/`).

2.-Ingreso de Comandos: El usuario ingresa un comando (ej: `ls`, `mkdir mi_carpeta`, `exit`).

3.-Procesamiento:
    -La función `Consola::ejecutar()` lee la entrada.
    -La función `Consola::procesarComando()` extrae el comando principal y llama a la función específica (ej: `procesarLs`, `procesarMkdir`).
    -Las funciones de procesamiento extraen los argumentos, validan la entrada (por ejemplo, asegurando que un ID sea un número entero con `obtenerId`), 
    y llaman a los métodos correspondientes en la clase `Arbol` para modificar la estructura de archivos.
4.Manejo de Errores: Se utiliza un manejo robusto de excepciones (`try-catch`) para capturar errores del sistema de archivos 
(como `NodoNotFoundException`, `OperacionInvalidaException`) y mostrarlos al usuario de forma clara utilizando `ErrorHandler`.

5.-Papelera (`Trash`): El borrado (`rm`) mueve el nodo a una estructura separada (la papelera), permitiendo su restauración (`restore`) o eliminación permanente (`cleartrash`).

6.-Persistencia: El estado del sistema de archivos puede ser guardado (`save`) y cargado (`load`) desde un archivo JSON.

-Explicación de los Comandos-
A continuación, se detalla cada comando disponible en la consola:
|Comando     |      Sintaxis                 | Descripción                                                                                         |
| :---       | :---                          | :---                                                                                                |
|`mkdir`     | `mkdir <nombre>`              | Crea una nueva carpeta en el nodo actual, Soporta nombres con espacios.                             |
|`touch`     | `touch <nombre>`              | Crea un nuevo archivo en el nodo actual, Soporta nombres con espacios.                              |
|`ls`        | `ls [id/ruta]`                | Lista el contenido de una carpeta. Si no se da argumento, lista la raíz, Soporta rutas con espacios.|
|`cat`       | `cat <id/ruta>`               | Muestra el contenido completo de un archivo.                                                        |
|`rm`        | `rm <id/ruta>`                | Mueve un nodo (archivo o carpeta) a la papelera (borrado suave).                                    |
|`mv`        | `mv <id_origen> <id_destino>` | Mueve el nodo de `<id_origen>` a la carpeta especificada por `<id_destino>`.                        |
|`rename`    | `rename <id> <nuevo_nombre>`  | Renombra el nodo con el ID especificado, Soporta el nuevo nombre con espacios.                      |
|`path`      | `path <id/ruta>`              | Muestra la ruta completa del nodo.                                                                  |
|`preorden`  | `preorden`                    | Muestra un listado de todos los nodos del árbol siguiendo un recorrido en preorden.                 |
|`find`      | `find <nombre>`               | Busca nodos por nombre exacto, Soporta nombres con espacios.                                        |
|`search`    | `search <prefijo>`            | Busca nodos cuyos nombres comiencen con el prefijo dado.                                            |
|`suggest`   | `suggest <prefijo>`           | Muestra sugerencias de autocompletado de nombres basados en un prefijo.                             |
|`save`      | `save [archivo]`              | Guarda el estado actual del árbol en un archivo JSON (default: `datos.json`).                       |
|`load`      | `load [archivo]`              | Carga un árbol desde un archivo JSON, reemplazando el estado actual (requiere confirmación).        |
|`info`      | `info`                        | Muestra información general del sistema (tamaño, próximo ID, altura, tamaño de papelera).           |
|`clear`     | `clear`                       | Limpia la pantalla de la consola.                                                                   |
|`trash`     | `trash`                       | Lista el contenido de la papelera de reciclaje.                                                     |
|`restore`   | `restore <id>`                | Restaura un nodo de la papelera al nodo padre original.                                             |
|`cleartrash`| `cleartrash`                  | Vacía permanentemente la papelera (requiere confirmación).                                          |
|`help`      | `help`                        | Muestra la lista de comandos disponibles (esta ayuda).                                              |
|`exit`      | `exit`                        | Sale del sistema (requiere confirmación y guarda automáticamente).                                  |

-Salidas Esperadas-
Las salidas del sistema son formateadas para ser claras y legibles:

Comandos de Listado (`ls`, `preorden`, `trash`): Muestran tablas con columnas para ID,TIPO(`CARPETA` o `ARCHIVO`), y NOMBRE/RUTA COMPLETA.
    --- Contenido de /MiCarpeta (2 elementos) ---
    ID      TIPO           NOMBRE
    ----------------------------------------------------------------------
    2       CARPETA        Documentos
    3       ARCHIVO        Notas.txt
    ----------------------------------------------------------------------
    
Comandos de Búsqueda (`find`, `search`):Muestran listados de resultados con formato similar a `ls`/`preorden`.

Comandos de Información (`info`, `path`): Muestran el dato solicitado de forma específica.
    Ruta completa: /Carpeta-A/Subcarpeta-B/MiArchivo.dat
    
Mensajes de Éxito/Información: Utilizan `ErrorHandler::mostrarInfo`.
    [INFO] Carpeta creada.
    
Mensajes de Error: Utilizan `ErrorHandler::mostrarError` al capturar una excepción.
    [ERROR: NodoNotFoundException] No se encontró el nodo con ID: 99. 
    
Confirmaciones: Utilizan `ErrorHandler::confirmarAccion` para operaciones destructivas (`exit`, `rm`, `load`, `cleartrash`).

-Conclusión-
Esta implementación de la consola proporciona una interfaz de usuario completa y robusta para la gestión de un sistema de archivos jerárquico simulado. 
Su diseño se enfoca en el manejo de excepciones, la usabilidad a través de comandos familiares, y la implementación de funcionalidades avanzadas como la 
gestión de una papelera y búsquedas eficientes (presumiblemente usando estructuras de datos como Tries para `search` y `suggest` en la clase `Arbol`). 
El sistema es un excelente ejemplo de la aplicación de estructuras de datos tipo árbol en un contexto práctico de simulación de un SO.
