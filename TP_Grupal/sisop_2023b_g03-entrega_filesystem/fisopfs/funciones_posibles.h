#ifndef FUNCIONES_POSIBLES_H
#define FUNCIONES_POSIBLES_H

#define FUSE_USE_VERSION 30

#include <fuse.h>

#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <time.h>

// ********* Implementadas *********

int fisopfs_create(const char *path, mode_t mode, struct fuse_file_info *fi);

int fisopfs_getattr(const char *path, struct stat *st);

int fisopfs_readdir(const char *path,
                    void *buffer,
                    fuse_fill_dir_t filler,
                    off_t offset,
                    struct fuse_file_info *fi);

int fisopfs_read(const char *path,
                 char *buffer,
                 size_t size,
                 off_t offset,
                 struct fuse_file_info *fi);

void *fisopfs_init(struct fuse_conn_info *conn);

int fisopfs_mkdir(const char *path, mode_t mode);

int fisopfs_flush(const char *path, struct fuse_file_info *fi);

void fisopfs_destroy(void *private_data);

// ********* Posibles a implementar *********

int fisopfs_readlink(const char *path, char *buffer, size_t size);

int fisopfs_mknod(const char *path, mode_t mode, dev_t rdev);

int fisopfs_unlink(const char *path);

int fisopfs_rmdir(const char *path);

int fisopfs_symlink(const char *to, const char *from);

int fisopfs_rename(const char *from, const char *to);

int fisopfs_link(const char *from, const char *to);

int fisopfs_chmod(const char *path, mode_t mode);

int fisopfs_chown(const char *path, uid_t uid, gid_t gid);

int fisopfs_truncate(const char *path, off_t size);

int fisopfs_open(const char *path, struct fuse_file_info *fi);

int fisopfs_write(const char *path,
                  const char *buf,
                  size_t size,
                  off_t offset,
                  struct fuse_file_info *fi);

int fisopfs_statfs(const char *path, struct statvfs *stbuf);


int fisopfs_release(const char *path, struct fuse_file_info *fi);

int fisopfs_fsync(const char *path, int is_data_sync, struct fuse_file_info *fi);

int fisopfs_setxattr(const char *path,
                     const char *name,
                     const char *value,
                     size_t size,
                     int flags);

int fisopfs_getxattr(const char *path, const char *name, char *value, size_t flags);

int fisopfs_listxattr(const char *path, char *list, size_t size);

int fisopfs_removexattr(const char *path, const char *name);

int fisopfs_opendir(const char *path, struct fuse_file_info *fi);

int fisopfs_releasedir(const char *path, struct fuse_file_info *fi);

int fisopfs_fsyncdir(const char *path, int datasync, struct fuse_file_info *fi);


int fisopfs_access(const char *path, int mask);

int fisopfs_ftruncate(const char *path, off_t size, struct fuse_file_info *fi);

int fisopfs_fgetattr(const char *path, struct stat *buf, struct fuse_file_info *fi);

int fisopfs_lock(const char *path,
                 struct fuse_file_info *fi,
                 int cmd,
                 struct flock *lock);

int fisopfs_utimens(const char *path, const struct timespec tv[2]);

int fisopfs_bmap(const char *path, size_t blocksize, uint64_t *idx);

int fisopfs_ioctl(const char *path,
                  int cmd,
                  void *arg,
                  struct fuse_file_info *fi,
                  unsigned int flags,
                  void *data);

int fisopfs_poll(const char *path,
                 struct fuse_file_info *fi,
                 struct fuse_pollhandle *ph,
                 unsigned *reventsp);

int fisopfs_write_buf(const char *path,
                      struct fuse_bufvec *buf,
                      off_t off,
                      struct fuse_file_info *fi);

int fisopfs_read_buf(const char *path,
                     struct fuse_bufvec **bufp,
                     size_t size,
                     off_t off,
                     struct fuse_file_info *fi);

int fisopfs_flock(const char *path, struct fuse_file_info *fi, int op);

int fisopfs_fallocate(const char *path,
                      int mode,
                      off_t offset,
                      off_t length,
                      struct fuse_file_info *fi);

#endif