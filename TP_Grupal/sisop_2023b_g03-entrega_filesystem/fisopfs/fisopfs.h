#ifndef FISOPFS_H
#define FISOPFS_H

#define FUSE_USE_VERSION 30

#include <fuse.h>

#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <errno.h>
#include <time.h>
#include <stdbool.h>

#include "funciones_posibles.h"

#define MAX_TAMANIO_NOMBRE 50
#define MAX_CANTIDAD_INODOS_FS 200
#define MAX_TAMANIO_FILE 4096
#define NOMBRE_ARCHIVO_DUMP "dump_fs.fisopfs"

#define FILE_T 0
#define DIR_T 1

#define LIBRE 0
#define OCUPADO 1

// archivo | directorio
typedef struct inodo {
	char nombre[MAX_TAMANIO_NOMBRE];
	int tipo;  // FILE_T | DIR_T
	size_t tamanio;
	char datos[MAX_TAMANIO_FILE];
	mode_t permisos;  // mode
	time_t fecha_creacion;
	time_t fecha_ultimo_acceso;  // en caso de lectura.
	time_t fecha_modificacion;
} inodo_t;

typedef struct file_system {
	int cantidad_inodos;
	inodo_t inodos[MAX_CANTIDAD_INODOS_FS];

	// Marca los inodos como ocupados ó no
	// 0: LIBRE
	// 1: OCUPADO
	int bitmap_inodos[MAX_CANTIDAD_INODOS_FS];
} file_system_t;

static struct fuse_operations operations = {
	.getattr = fisopfs_getattr,
	.readlink = fisopfs_readlink,
	.mknod = fisopfs_mknod,
	.mkdir = fisopfs_mkdir,
	.unlink = fisopfs_unlink,
	.rmdir = fisopfs_rmdir,
	.symlink = fisopfs_symlink,
	.rename = fisopfs_rename,
	.link = fisopfs_link,
	.chmod = fisopfs_chmod,
	.chown = fisopfs_chown,
	.truncate = fisopfs_truncate,
	.open = fisopfs_open,
	.read = fisopfs_read,
	.write = fisopfs_write,
	.statfs = fisopfs_statfs,
	.flush = fisopfs_flush,
	.release = fisopfs_release,
	.fsync = fisopfs_fsync,
	.setxattr = fisopfs_setxattr,
	.getxattr = fisopfs_getxattr,
	.listxattr = fisopfs_listxattr,
	.removexattr = fisopfs_removexattr,
	.opendir = fisopfs_opendir,
	.readdir = fisopfs_readdir,
	.releasedir = fisopfs_releasedir,
	.fsyncdir = fisopfs_fsyncdir,
	.init = fisopfs_init,
	.destroy = fisopfs_destroy,
	.access = fisopfs_access,
	.create = fisopfs_create,
	.lock = fisopfs_lock,
	.utimens = fisopfs_utimens,
	.bmap = fisopfs_bmap,
	.ioctl = fisopfs_ioctl,
	.poll = fisopfs_poll,
	// comentado p/ que funcione la lectura y escritura de archivos
	//.write_buf = fisopfs_write_buf,
	//.read_buf = fisopfs_read_buf,
	//.ftruncate = fisopfs_ftruncate,
	//.fgetattr = fisopfs_fgetattr,
	.flock = fisopfs_flock,
	.fallocate = fisopfs_fallocate
};


#endif