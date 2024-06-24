#include "fisopfs.h"

file_system_t fs;
char nombre_archivo_dump[MAX_TAMANIO_NOMBRE];
bool tiene_nombre;


// Funciones auxiliares.

// Marca al bitmap como todos inodos libres.
void
inicializar_bitmap()
{
	for (size_t i = 0; i < MAX_CANTIDAD_INODOS_FS; i++) {
		fs.bitmap_inodos[i] = LIBRE;
	}
}

// se fija si el file_name esta en el directorio dir
int
is_in_directory(const char *dir, const char *file_name)
{
	char buf[MAX_TAMANIO_NOMBRE + 1];
	snprintf(buf, sizeof(buf), "%s%s", dir, "/");

	return (strncmp(buf, file_name, strlen(buf)) == 0);
}

// Se fija si un archivo esta en root o no
int
is_in_root(const char *file_name)
{
	for (size_t i = 0; i < strlen(file_name); i++) {
		if (file_name[i] == '/')
			return 0;
	}
	return 1;
}

int
find_file(const char *path)
{
	for (int i = 0; i < MAX_CANTIDAD_INODOS_FS; i++) {
		if (strcmp(fs.inodos[i].nombre, &path[1]) == 0)
			return i;
	}
	return -1;
}

int
find_free_inode()
{
	for (int i = 0; i < MAX_CANTIDAD_INODOS_FS; i++) {
		if (fs.bitmap_inodos[i] == LIBRE) {
			return i;
		}
	}
	return -1;
}

// devuelve el path absoluto donde se construirá el dump
char *
cargar_path(char *nombre_dump)
{
	// Obtener el directorio de trabajo actual
	char *directorio_actual = getcwd(NULL, 0);

	if (!directorio_actual) {
		perror("Error al obtener el directorio de trabajo actual");
		return NULL;
	}

	// Construir el path del archivo en el directorio actual
	char *path_archivo =
	        malloc(strlen(directorio_actual) + 1 + strlen(nombre_dump) + 1);

	if (!path_archivo) {
		perror("Error malloc");
		return NULL;
	}

	sprintf(path_archivo, "%s/%s", directorio_actual, nombre_dump);

	// libero memoria del directorio
	free(directorio_actual);

	return path_archivo;
}

void
get_data()
{
	int fd_dump;
	char *path_archivo;

	if (tiene_nombre) {
		path_archivo = cargar_path(nombre_archivo_dump);
	} else {
		path_archivo = cargar_path(NOMBRE_ARCHIVO_DUMP);
	}

	// char *path_archivo = cargar_path(NOMBRE_ARCHIVO_DUMP);

	fd_dump = open(path_archivo, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);

	if (fd_dump == -1) {
		perror("Error al crear el archivo\n");
		return;
	}

	// Lectura cantidad de inodos
	int cantidad_inodos_leidos;
	ssize_t r_bytes = read(fd_dump, &cantidad_inodos_leidos, sizeof(int));
	if (r_bytes == -1) {
		perror("Error en lectura: cantidad inodos\n");
	}

	if (r_bytes == 0) {
		printf("tengo 0 bytes leídos: OK\n");
		cantidad_inodos_leidos = 0;
	}
	printf("cantidad_inodos_leidos: %d\n", cantidad_inodos_leidos);
	fs.cantidad_inodos = cantidad_inodos_leidos;

	for (size_t i = 0; i < cantidad_inodos_leidos; i++) {
		// Lectura de nombre
		char nombre_inodo_leido[MAX_TAMANIO_NOMBRE];
		ssize_t r_bytes = read(fd_dump,
		                       &nombre_inodo_leido,
		                       sizeof(char) * MAX_TAMANIO_NOMBRE);
		if (r_bytes == -1) {
			perror("Error en lectura: nombre\n");
		}
		strcpy(fs.inodos[i].nombre, nombre_inodo_leido);

		// Lectura de tipo
		int tipo_leido;
		r_bytes = read(fd_dump, &tipo_leido, sizeof(int));
		if (r_bytes == -1) {
			perror("Error en lectura: tipo\n");
		}
		fs.inodos[i].tipo = tipo_leido;

		// Lectura de tamanio
		size_t tamanio_leido;
		r_bytes = read(fd_dump, &tamanio_leido, sizeof(size_t));
		if (r_bytes == -1) {
			perror("Error en lectura: tamaño\n");
		}
		fs.inodos[i].tamanio = tamanio_leido;

		// Lectura de datos
		char datos_leidos[MAX_TAMANIO_FILE];
		r_bytes = read(fd_dump,
		               &datos_leidos,
		               sizeof(char) * MAX_TAMANIO_FILE);
		if (r_bytes == -1) {
			perror("Error en lectura: datos\n");
		}
		strcpy(fs.inodos[i].datos, datos_leidos);

		// Lectura de permisos
		mode_t permisos_leidos;
		r_bytes = read(fd_dump, &permisos_leidos, sizeof(mode_t));
		if (r_bytes == -1) {
			perror("Error en lectura: permisos\n");
		}
		fs.inodos[i].permisos = permisos_leidos;

		// Lectura de fecha de creación
		time_t fecha_de_creacion_leida;
		r_bytes = read(fd_dump, &fecha_de_creacion_leida, sizeof(time_t));
		if (r_bytes == -1) {
			perror("Error en lectura: fecha creación\n");
		}
		fs.inodos[i].fecha_creacion = fecha_de_creacion_leida;

		// Lectura de fecha de ultimo acceso
		time_t fecha_ultimo_acceso_leida;
		r_bytes =
		        read(fd_dump, &fecha_ultimo_acceso_leida, sizeof(time_t));
		if (r_bytes == -1) {
			perror("Error en lectura: fecha último acceso\n");
		}
		fs.inodos[i].fecha_ultimo_acceso = fecha_ultimo_acceso_leida;

		// Lectura de fecha de modificación
		time_t fecha_modificacion_leida;
		r_bytes = read(fd_dump, &fecha_modificacion_leida, sizeof(time_t));
		if (r_bytes == -1) {
			perror("Error en lectura: fecha creación\n");
		}
		fs.inodos[i].fecha_modificacion = fecha_modificacion_leida;


		fs.bitmap_inodos[i] = OCUPADO;

		printf("----- Inodo ----- \n");
		printf("nombre: %s\n", nombre_inodo_leido);
		printf("tipo: %d\n", tipo_leido);
		printf("tamanio: %ld\n", tamanio_leido);
		printf("datos: %s\n", datos_leidos);
		printf("permisos: %d\n", permisos_leidos);
		printf("fecha creacion: %s", ctime(&fecha_de_creacion_leida));
		printf("fecha ultimo acceso: %s",
		       ctime(&fecha_ultimo_acceso_leida));
		printf("fecha modificación: %s", ctime(&fecha_modificacion_leida));
	}


	// Liberar la memoria del path generado
	free(path_archivo);

	close(fd_dump);
}


void
guardar_fs()
{
	int fd_dump;
	char *path_archivo;

	if (tiene_nombre) {
		path_archivo = cargar_path(nombre_archivo_dump);
	} else {
		path_archivo = cargar_path(NOMBRE_ARCHIVO_DUMP);
	}

	// Crear el archivo en modo de escritura / lectura con reemplazo de datos
	fd_dump =
	        open(path_archivo, O_RDWR | O_CREAT | O_TRUNC, S_IRUSR | S_IWUSR);

	if (fd_dump == -1) {
		perror("Error al crear el archivo\n");
		return;
	}

	// Guardo cantidad de inodos
	ssize_t r_bytes = write(fd_dump, &fs.cantidad_inodos, sizeof(int));

	if (r_bytes == -1) {
		perror("Error al escribir\n");
	}

	for (int i = 0; i < fs.cantidad_inodos; i++) {
		inodo_t inodo_actual = fs.inodos[i];
		if (fs.bitmap_inodos[i] == OCUPADO) {
			// guardo nombre
			ssize_t r_bytes =
			        write(fd_dump,
			              &inodo_actual.nombre,
			              sizeof(char) * MAX_TAMANIO_NOMBRE);
			if (r_bytes == -1) {
				perror("Error al escribir nombre\n");
			}

			// guardo tipo
			r_bytes = write(fd_dump, &inodo_actual.tipo, sizeof(int));
			if (r_bytes == -1) {
				perror("Error al escribir Tipo\n");
			}

			// guardo tamaño
			r_bytes = write(fd_dump,
			                &inodo_actual.tamanio,
			                sizeof(size_t));
			if (r_bytes == -1) {
				perror("Error al escribir tamaño\n");
			}

			// guardo datos archivo
			r_bytes = write(fd_dump,
			                &inodo_actual.datos,
			                sizeof(char) * MAX_TAMANIO_FILE);
			if (r_bytes == -1) {
				perror("Error al escribir tipo\n");
			}

			// guardo permisos
			r_bytes = write(fd_dump,
			                &inodo_actual.permisos,
			                sizeof(mode_t));
			if (r_bytes == -1) {
				perror("Error al escribir permisos\n");
			}

			// guardo fecha creacion
			r_bytes = write(fd_dump,
			                &inodo_actual.fecha_creacion,
			                sizeof(time_t));
			if (r_bytes == -1) {
				perror("Error al escribir fecha de creación\n");
			}

			// guardo fecha último acceso
			r_bytes = write(fd_dump,
			                &inodo_actual.fecha_ultimo_acceso,
			                sizeof(time_t));
			if (r_bytes == -1) {
				perror("Error al escribir fecha de ultimo "
				       "acceso\n");
			}

			// guardo fecha modificación
			r_bytes = write(fd_dump,
			                &inodo_actual.fecha_modificacion,
			                sizeof(time_t));
			if (r_bytes == -1) {
				perror("Error al escribir fecha de "
				       "modificacion\n");
			}
		}
	}

	printf("Dump generado con éxito en: %s\n", path_archivo);


	// Liberar la memoria del path
	free(path_archivo);

	close(fd_dump);
}

int
encontrar_directorio_padre(const char *path)
{
	char directorio[MAX_TAMANIO_NOMBRE];
	const char *ultimo_slash = strrchr(path, '/');

	if (ultimo_slash == NULL || ultimo_slash == path) {
		// No se encontró un directorio padre (el archivo está en la raíz)
		return -1;
	}


	strncpy(directorio, path, ultimo_slash - path);
	directorio[ultimo_slash - path] = '\0';


	for (int i = 0; i < MAX_CANTIDAD_INODOS_FS; i++) {
		if (strcmp(fs.inodos[i].nombre, &directorio[1]) == 0 &&
		    fs.inodos[i].tipo == DIR_T) {
			return i;  // Retorna el índice del directorio padre encontrado
		}
	}

	return -1;  // No se encontró el directorio padre
}


// ********* Funciones implementadas ************

int
fisopfs_create(const char *path, mode_t mode, struct fuse_file_info *fi)
{
	printf("[debug] fisopfs_create - path: %s -  mode: %d\n", path, mode);

	if (fs.cantidad_inodos >= MAX_CANTIDAD_INODOS_FS) {
		return -ENOMEM;
	}
	int index_inode = find_free_inode();
	if (index_inode < 0) {
		return -ENOMEM;
	}

	if (strlen(path) > MAX_TAMANIO_NOMBRE - 1) {
		return -ENAMETOOLONG;
	}

	strcpy(fs.inodos[index_inode].nombre, &path[1]);

	fs.inodos[index_inode].tipo = FILE_T;
	fs.inodos[index_inode].tamanio = 0;
	fs.inodos[index_inode].permisos = mode;
	fs.inodos[index_inode].datos[0] = '\0';  // Archivo vacío

	fs.inodos[index_inode].fecha_creacion = time(NULL);
	fs.inodos[index_inode].fecha_ultimo_acceso = time(NULL);
	fs.inodos[index_inode].fecha_modificacion = time(NULL);

	// Marco como ocupado.
	fs.bitmap_inodos[index_inode] = OCUPADO;

	fs.cantidad_inodos++;

	return 0;
}

int
fisopfs_mkdir(const char *path, mode_t mode)
{
	printf("[debug] fisopfs_mkdir - path: %s -  mode: %d\n", path, mode);

	if (fs.cantidad_inodos >= MAX_CANTIDAD_INODOS_FS) {
		return -ENOMEM;
	}
	int index_inode = find_free_inode();
	if (index_inode < 0) {
		return -ENOMEM;
	}

	if (strlen(path) > MAX_TAMANIO_NOMBRE - 1) {
		return -ENAMETOOLONG;
	}

	strcpy(fs.inodos[index_inode].nombre, &path[1]);

	fs.inodos[index_inode].tipo = DIR_T;
	fs.inodos[index_inode].tamanio = 0;
	fs.inodos[index_inode].permisos = mode;
	fs.inodos[index_inode].datos[0] = '\0';  // Archivo vacío

	fs.inodos[index_inode].fecha_creacion = time(NULL);
	fs.inodos[index_inode].fecha_ultimo_acceso = time(NULL);
	fs.inodos[index_inode].fecha_modificacion = time(NULL);

	// Marco como ocupado.
	fs.bitmap_inodos[index_inode] = OCUPADO;

	fs.cantidad_inodos++;

	return 0;
}

int
fisopfs_getattr(const char *path, struct stat *st)
{
	printf("[debug] fisopfs_getattr - path: %s\n", path);

	if (strcmp(path, "/") == 0) {
		st->st_uid = 1717;
		st->st_mode = __S_IFDIR | 0755;
		st->st_nlink = 2;
		return 0;
	}

	int file_pos = find_file(path);

	if (file_pos > -1) {
		if (fs.inodos[file_pos].tipo == FILE_T) {
			st->st_mode = __S_IFREG | fs.inodos[file_pos].permisos;
			st->st_nlink = 1;
		} else {  // Directorio
			st->st_mode = __S_IFDIR | fs.inodos[file_pos].permisos;
			st->st_nlink = 2;
		}

		st->st_size = fs.inodos[file_pos].tamanio;
		st->st_blocks = 8;  // 8 * 512 = 4096

		// Campos de access, update y change
		st->st_atime = fs.inodos[file_pos].fecha_ultimo_acceso;
		st->st_mtime =
		        fs.inodos[file_pos].fecha_modificacion;  // Changes to the data.
		st->st_ctime =
		        fs.inodos[file_pos].fecha_creacion;  // Changes to the metadata (for renaming, permissions, etc).

		// Group and user info.
		st->st_uid = getuid();
		st->st_gid = getgid();

		return 0;
	}

	return -ENOENT;
}

int
fisopfs_readdir(const char *path,
                void *buffer,
                fuse_fill_dir_t filler,
                off_t offset,
                struct fuse_file_info *fi)
{
	printf("[debug] fisopfs_readdir - path: %s\n", path);

	// Los directorios '.' y '..'
	filler(buffer, ".", NULL, 0);
	filler(buffer, "..", NULL, 0);

	// Si nos preguntan por el directorio raiz, entro aca
	if (strcmp(path, "/") == 0) {
		for (int i = 0; i < MAX_CANTIDAD_INODOS_FS; i++) {
			if (is_in_root(fs.inodos[i].nombre) &&
			    (strlen(fs.inodos[i].nombre) > 0)) {
				filler(buffer, fs.inodos[i].nombre, NULL, 0);
			}
		}
		return 0;
	}

	// si estoy aca es porque estoy busco algo dentro de un directorio
	const char *aux_path = path + 1;  // le saco el '/' al path
	for (int i = 0; i < MAX_CANTIDAD_INODOS_FS; i++) {
		if (strcmp(aux_path, fs.inodos[i].nombre) &&
		    (is_in_directory(aux_path, fs.inodos[i].nombre))) {
			char *nombre = fs.inodos[i].nombre + strlen(aux_path) + 1;
			filler(buffer, nombre, NULL, 0);
		}
	}
	return 0;

	return -ENOENT;
}

int
fisopfs_read(const char *path,
             char *buffer,
             size_t size,
             off_t offset,
             struct fuse_file_info *fi)
{
	printf("[debug] fisopfs_read - path: %s, offset: %lu, size: %lu\n",
	       path,
	       offset,
	       size);

	int pos_file = find_file(path);

	if (pos_file == -1)
		return -ENOENT;

	if (offset + size > strlen(fs.inodos[pos_file].datos))
		size = strlen(fs.inodos[pos_file].datos) - offset;

	size = size > 0 ? size : 0;

	strncpy(buffer, fs.inodos[pos_file].datos + offset, size);
	fs.inodos[pos_file].fecha_ultimo_acceso = time(NULL);
	return size;
}

int
fisopfs_write(const char *path,
              const char *buffer,
              size_t size,
              off_t offset,
              struct fuse_file_info *fi)
{
	printf("[debug] fisopfs_write - path: %s - buffer:%s - size: %zu - "
	       "offset: %ld\n",
	       path,
	       buffer,
	       size,
	       offset);

	int pos_file = find_file(path);

	if (pos_file == -1)
		return -ENOENT;

	strncpy(fs.inodos[pos_file].datos + offset, buffer, size);

	fs.inodos[pos_file].tamanio += size;
	fs.inodos[pos_file].fecha_modificacion = time(NULL);
	return size;
}

int
fisopfs_truncate(const char *path, off_t size)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_truncate - path: %s - size: %zu\n", path, size);

	int index = find_file(path);
	if (index < 0) {
		return 0;
	}

	memset(fs.inodos[index].datos + size,
	       0,
	       (MAX_TAMANIO_FILE - size) * sizeof(char));
	fs.inodos[index].tamanio = size;

	return 0;
}

void *
fisopfs_init(struct fuse_conn_info *conn)
{
	printf("[debug] fisop_init\n");

	get_data();

	void *resultado = 0;
	return resultado;
}

int
fisopfs_flush(const char *path, struct fuse_file_info *fi)
{
	printf("[debug] fisop_flush\n");
	guardar_fs();
	return 0;
}

void
fisopfs_destroy(void *private_data)
{
	printf("[debug] fisop_destroy\n");
	guardar_fs();
}

int
fisopfs_rmdir(const char *path)
{
	printf("[debug] fisopfs_rmdir\n");

	int i;

	for (i = 0; i < MAX_CANTIDAD_INODOS_FS; i++) {
		if (strcmp(fs.inodos[i].nombre, &path[1]) != 0 &&
		    strncmp(fs.inodos[i].nombre, &path[1], strlen(path) - 1) == 0) {
			return -ENOTEMPTY;  // directorio no vacio
		}
	}

	for (i = 0; i < MAX_CANTIDAD_INODOS_FS; i++) {
		if (strcmp(fs.inodos[i].nombre, &path[1]) == 0 &&
		    fs.inodos[i].tipo == DIR_T) {
			memset(fs.inodos[i].nombre, 0, sizeof(fs.inodos[i].nombre));
			fs.inodos[i].tamanio = 0;
			memset(fs.inodos[i].datos, 0, sizeof(fs.inodos[i].datos));
			fs.inodos[i].permisos = 0;
			fs.inodos[i].fecha_creacion = 0;
			fs.inodos[i].fecha_ultimo_acceso = 0;
			fs.inodos[i].fecha_modificacion = 0;

			fs.cantidad_inodos--;
			fs.bitmap_inodos[i] = LIBRE;  // marco como libre
			return 0;
		}
	}

	return -ENOENT;
}

int
fisopfs_unlink(const char *path)
{
	printf("[debug] fisopfs_unlink\n");
	int i;

	for (i = 0; i < MAX_CANTIDAD_INODOS_FS; i++) {
		if (strcmp(fs.inodos[i].nombre, &path[1]) == 0 &&
		    fs.inodos[i].tipo == FILE_T) {
			memset(fs.inodos[i].nombre, 0, sizeof(fs.inodos[i].nombre));
			fs.inodos[i].tamanio = 0;
			memset(fs.inodos[i].datos, 0, sizeof(fs.inodos[i].datos));
			fs.inodos[i].permisos = 0;
			fs.inodos[i].fecha_creacion = 0;
			fs.inodos[i].fecha_ultimo_acceso = 0;
			fs.inodos[i].fecha_modificacion = 0;
			fs.cantidad_inodos--;
			fs.bitmap_inodos[i] = LIBRE;  // marco como libre

			int directorio_padre = encontrar_directorio_padre(path);

			// Actualizar la fecha de modificación del directorio padre
			if (directorio_padre != -1) {
				fs.inodos[directorio_padre].fecha_modificacion =
				        time(NULL);
				return 0;
			}
			return 0;
		}
	}

	return -ENOENT;
}

int
main(int argc, char *argv[])
{
	if (argc < 3 || argc > 4) {
		printf("Uso: ./fisop -f mount_dir/ file_to_dump.fisopfs\n");
		return 0;
	}
	tiene_nombre = false;

	if (argc == 4) {
		strcpy(nombre_archivo_dump, argv[3]);
		tiene_nombre = true;
		argv[3] = NULL;
		argc--;
	}

	fs.cantidad_inodos = 0;
	inicializar_bitmap();
	return fuse_main(argc, argv, &operations, NULL);
}