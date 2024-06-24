# fisop-fs

El archivo fisopfs.h contiene las definiciones y  estructuras necesarias para un sistema de archivos. Proporciona un conjunto de macros, definiciones de estructuras y operaciones necesarias para simular las funciones de un sistema de archivos a través de FUSE.

A continuación se detallan los componentes más relevantes:

## Macros y Definiciones:

- NOMBRE_ARCHIVO_DUMP: Indica el nombre del archivo donde se realizará el dump por defecto de el sistema de archivos.
- MAX_TAMANIO_NOMBRE: Establecen los tamaños máximos para nombres de archivo/directorio. En nuestro caso 50.
- MAX_CANTIDAD_INODOS_FS: cantidad de inodos.
- MAX_TAMANIO_FILE: tamaño de archivos.
- LIBRE: Indica que un inodo se encuentra libre para ser usado.
- OCUPADO: Indica que un inodo se encuentra ocupado.
- FILE_T: Indica si un inodo representa un archivo.
- DIR_T: Indica si un inodo representa un directorio

Como se puede ver, se decidió utilizar un tamaño máximo para la longitud del path (nombre) y en caso de que se supere devuelve ENAMETOOLONG. También se fijó un tamaño máximo para el sistema de archivos (cantidad de inodos), en caso de que se supere se devuelve ENOMEM.

## Estructuras de Datos:

**file_system_t**: Estructura que contiene la cantidad de inodos y un arreglo de inodos, además de un bitmap para marcar inodos como ocupados o libres.

**cantidad_inodos**: Número de inodos actualmente utilizados.
inodos: Arreglo que contiene todos los inodos del sistema de archivos.
bitmap_inodos: Indica si cada inodo está libre u ocupado en el sistema de archivos.


**inodo_t**: Estructura que define un inodo en el sistema de archivos, incluyendo la siguiente información:

- nombre: Nombre del archivo o directorio.
- tipo: Indica si el inodo representa un archivo (FILE_T) o un directorio (DIR_T).
- tamanio: Tamaño del archivo o directorio.
- datos: Almacena los datos del archivo.
- permisos: Permisos de acceso (lectura, escritura, ejecución).
- fecha_creacion, fecha_ultimo_acceso, fecha_modificacion: 
Marcas de tiempo para controlar la creación, acceso y modificación.


Funciones de FUSE
struct fuse_operations: Contiene un conjunto de operaciones FUSE que representan llamadas del sistema de archivos. Cada operación está asociada a una función específica (por ejemplo, mkdir, unlink, rmdir, etc.).

## Búsqueda de archivos
La función find_file busca un archivo en la estructura del sistema de archivos utilizando el nombre del archivo proporcionado a través del parámetro path.

Realiza un recorrido por todos los inodos presentes en el sistema de archivos y compara el nombre del archivo en cada inodo con el nombre proveniente de path. Si encuentra una coincidencia, devuelve el índice del inodo donde se encuentra ese archivo. Si no hay coincidencias, devuelve -1, indicando que el archivo no se encontró en el sistema simulado.

## El formato de serialización del sistema de archivos en disco

Cuando se utiliza init, lo que ocurre es que se abre un archivo en modo lectura/escritura.
Se lee la cantidad de inodos guardados en el archivo y se cargan en la estructura fs.cantidad_inodos.
Luego, se recorre cada inodo en el archivo y se leen los atributos de cada uno de ellos, incluyendo el nombre, tipo, tamaño, datos y permisos.

En el caso opuesto, lo que ocurre es que se abre un archivo en modo escritura y se guarda la cantidad de inodos. 
Se itera sobre cada inodo en el sistema de archivos y si está marcado como "ocupado" se guardan los atributos de ese inodo en el archivo.

El formato de serialización se basa en la lectura y escritura secuencial de los datos de cada inodo en el archivo de sistema. Esto significa que la información se guarda en el archivo en el orden en que se leen o escriben los atributos de los inodos, uno tras otro.

Creemos que de esta manera se asegura que la estructura y la información se recuperen correctamente durante las operaciones de lectura del archivo de sistema al iniciar el programa.

## Documentacion de persistencia en disco

Al lanzar el filesystem, se debe especificar un nombre de archivo, si no se hace, se elige uno por defecto.

En nuestro caso el archivo por defecto es: `dump_fs.fisopfs`

Caso contrario se puede elegir un nombre de archivo para hacer el dump de la siguiente forma:
```
./fisopfs -f dir_montaje/ nombre_archivo_dump.fisopfs
```