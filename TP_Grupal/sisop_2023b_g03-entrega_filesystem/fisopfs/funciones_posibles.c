#include "funciones_posibles.h"


// ************ Funciones posibles a implementar ************

int
fisopfs_readlink(const char *path, char *buffer, size_t size)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_readlink\n");
	return 0;
}

int
fisopfs_mknod(const char *path, mode_t mode, dev_t rdev)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_mknod\n");
	return 0;
}


int
fisopfs_symlink(const char *to, const char *from)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_symlink\n");
	return 0;
}

int
fisopfs_rename(const char *from, const char *to)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_rename\n");
	return 0;
}

int
fisopfs_link(const char *from, const char *to)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_link\n");
	return 0;
}

int
fisopfs_chmod(const char *path, mode_t mode)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_chmod\n");
	return 0;
}

int
fisopfs_chown(const char *path, uid_t uid, gid_t gid)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_chown\n");
	return 0;
}


int
fisopfs_open(const char *path, struct fuse_file_info *fi)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_open\n");
	return 0;
}

int
fisopfs_statfs(const char *path, struct statvfs *stbuf)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_statfs\n");
	return 0;
}


int
fisopfs_release(const char *path, struct fuse_file_info *fi)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_release - path: %s\n", path);
	return 0;
}

int
fisopfs_fsync(const char *path, int is_data_sync, struct fuse_file_info *fi)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_fsync\n");
	return 0;
}

int
fisopfs_setxattr(const char *path,
                 const char *name,
                 const char *value,
                 size_t size,
                 int flags)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_setxattr\n");
	return 0;
}

int
fisopfs_getxattr(const char *path, const char *name, char *value, size_t flags)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_getxattr\n");
	return 0;
}

int
fisopfs_listxattr(const char *path, char *list, size_t size)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_listxattr\n");
	return 0;
}

int
fisopfs_removexattr(const char *path, const char *name)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_removexattr\n");
	return 0;
}

int
fisopfs_opendir(const char *path, struct fuse_file_info *fi)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_opendir - path: %s\n", path);
	return 0;
}

int
fisopfs_releasedir(const char *path, struct fuse_file_info *fi)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_releasedir - path: %s\n", path);
	return 0;
}

int
fisopfs_fsyncdir(const char *path, int datasync, struct fuse_file_info *fi)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_fsyncdir\n");
	return 0;
}


int
fisopfs_access(const char *path, int mask)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_access\n");
	return 0;
}

int
fisopfs_ftruncate(const char *path, off_t size, struct fuse_file_info *fi)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_ftruncate\n");
	return 0;
}

int
fisopfs_fgetattr(const char *path, struct stat *buf, struct fuse_file_info *fi)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_fgetattr\n");
	return 0;
}

int
fisopfs_lock(const char *path, struct fuse_file_info *fi, int cmd, struct flock *lock)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_lock\n");
	return 0;
}

int
fisopfs_utimens(const char *path, const struct timespec tv[2])
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_utimens\n");
	return 0;
}

int
fisopfs_bmap(const char *path, size_t blocksize, uint64_t *idx)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_bmap\n");
	return 0;
}

int
fisopfs_ioctl(const char *path,
              int cmd,
              void *arg,
              struct fuse_file_info *fi,
              unsigned int flags,
              void *data)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_ioctl\n");
	return 0;
}

int
fisopfs_poll(const char *path,
             struct fuse_file_info *fi,
             struct fuse_pollhandle *ph,
             unsigned *reventsp)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisopfs_poll\n");
	return 0;
}

int
fisopfs_write_buf(const char *path,
                  struct fuse_bufvec *buf,
                  off_t off,
                  struct fuse_file_info *fi)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisops_write_buf\n");
	return 0;
}

int
fisopfs_read_buf(const char *path,
                 struct fuse_bufvec **bufp,
                 size_t size,
                 off_t off,
                 struct fuse_file_info *fi)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisops_read_buf\n");
	return 0;
}

int
fisopfs_flock(const char *path, struct fuse_file_info *fi, int op)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisops_flock\n");
	return 0;
}

int
fisopfs_fallocate(const char *path,
                  int mode,
                  off_t offset,
                  off_t length,
                  struct fuse_file_info *fi)
{
	// TODO: mejorar mensaje de debug
	printf("[debug] fisops_fallocate\n");
	return 0;
}
