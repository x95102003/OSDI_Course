/* This file use for NCTU OSDI course */

// It's handel the file system APIs 
#include <inc/stdio.h>
#include <inc/syscall.h>
#include <fs.h>

/*TODO: Lab7, file I/O system call interface.*/
// Below is POSIX like I/O system call 
int sys_open(const char *file, int flags, int mode)
{
    //We dont care the mode.
/* TODO */
	int fd, err, i;
	struct fs_fd *vfs_fd = NULL;
	fd = fd_new();
	if(fd != -1)
	vfs_fd = fd_get(fd);
	err = file_open(vfs_fd, file, flags);
	return fd;
}

int sys_close(int fd)
{
/* TODO */
	struct fs_fd *vfs_fd;
	vfs_fd = fd_get(fd);
	if (!vfs_fd)
		return -1;
	return file_close(vfs_fd);
}
int sys_read(int fd, void *buf, size_t len)
{
/* TODO */
	struct fs_fd *vfs_fd;
	vfs_fd = fd_get(fd);
	if (!vfs_fd){
		printk("sys_read vfs error\n");
		return -1;
	}
	return file_read(vfs_fd, buf, len);
}
int sys_write(int fd, const void *buf, size_t len)
{
/* TODO */
	struct fs_fd *vfs_fd;
	vfs_fd = fd_get(fd);
	if (!vfs_fd)
		return -1;
	return 	file_write(vfs_fd, buf, len);
}

off_t sys_lseek(int fd, off_t offset, int whence)
{
/* TODO */
	struct fs_fd *vfs_fd;
	vfs_fd = fd_get(fd);
	if (!vfs_fd)
		return -1;
	if (whence == 0)
		vfs_fd->pos = whence;
	if (whence == 2)
		vfs_fd->pos = vfs_fd->size;
	return file_lseek(vfs_fd, offset);
}

int sys_unlink(const char *pathname)
{
/* TODO */ 
	return file_unlink(pathname);
}


              

