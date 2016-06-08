/* This file use for NCTU OSDI course */

// It's handel the file system APIs 
#include <inc/stdio.h>
#include <inc/syscall.h>
#include <fs.h>

/*TODO: Lab7, file I/O system call interface.*/
/*NOte: Here you need handle the file system call from user.
*       1. When user open a new file, you can use the fd_new() to alloc a file object(struct fs_fd)
*       2. When user R/W the file, use the fd_get() to get file object.
*       3. After get file object call file_* functions into VFS level
*       4. Update the file objet's position or size when user R/W or seek the file.(You can find the useful marco in ff.h)
*       5. Handle the error code, for example, if user call open() but no fd slot can be use, sys_open should return -STATUS_ENOSPC.
*/

// Below is POSIX like I/O system call 
int sys_open(const char *file, int flags, int mode)
{
    //We dont care the mode.
/* TODO */
	int fd, err, i;
	struct fs_fd *vfs_fd = NULL;
	fd = fd_new();
	printk("fd%d\t", fd);
	if(fd < 0)
		return -1;
	vfs_fd = fd_get(fd);
	if(!vfs_fd)
		return STATUS_ENOENT;
	err = file_open(vfs_fd, file, flags);
	if(err == 0)
		return fd;
	else{
		while(vfs_fd && vfs_fd->ref_count >= 1)
			fd_put(vfs_fd);
		return err;
	}
}

int sys_close(int fd)
{
/* TODO */
	struct fs_fd *vfs_fd;
	int err = 0;
	vfs_fd = fd_get(fd);
	if (!vfs_fd){
		return -STATUS_EINVAL;
	}
	if(!(err=file_close(vfs_fd))){
		while(vfs_fd && vfs_fd->ref_count >= 1)
			fd_put(vfs_fd);
		return 0;
	}
	else{
		while(vfs_fd && vfs_fd->ref_count >= 1)
			fd_put(vfs_fd);
		return err;
	}

}
int sys_read(int fd, void *buf, size_t len)
{
/* TODO */
	struct fs_fd *vfs_fd;
	vfs_fd = fd_get(fd);
	if (!vfs_fd){
		return -STATUS_EBADF;
	}
	if(!buf || len < 0)
		return -STATUS_EINVAL;
	return file_read(vfs_fd, buf, len);
}
int sys_write(int fd, const void *buf, size_t len)
{
/* TODO */
	struct fs_fd *vfs_fd;
	vfs_fd = fd_get(fd);
	if (!vfs_fd){
		return -STATUS_EBADF;
	}
	if(!buf || len < 0)
		return -STATUS_EINVAL;
	return file_write(vfs_fd, buf, len);
}

/* Note: Check the whence parameter and calcuate the new offset value before do file_seek() */
off_t sys_lseek(int fd, off_t offset, int whence)
{
/* TODO */
	struct fs_fd *vfs_fd;
	vfs_fd = fd_get(fd);
	if (!vfs_fd)
		return -STATUS_EBADF;
	if (whence == 0)
		vfs_fd->pos = offset;
	if (whence == 2)
		offset += vfs_fd->pos;
	return file_lseek(vfs_fd, offset);
}

int sys_unlink(const char *pathname)
{
/* TODO */ 
	return file_unlink(pathname);
}
int sys_getdents(int fd, struct dirent *dirp, uint32_t count)
{
	struct fs_fd *vfs_fd;
	vfs_fd = fd_get(fd);
	if (!vfs_fd){
		return -STATUS_EBADF;
	}
	return file_getdents(vfs_fd, dirp, count);
}
int sys_opendir(const char* path)
{
	int fd, err, i;
	struct fs_fd *vfs_fd = NULL;
	fd = fd_new();
	if(fd < 0)
		return -1;
	vfs_fd = fd_get(fd);
	if(!vfs_fd)
		return STATUS_ENOENT;
	err = file_opendir(vfs_fd, path);
	if(err == 0)
		return fd;
	else{
		while(vfs_fd && vfs_fd->ref_count >= 1)
			fd_put(vfs_fd);
		return err;
	}
}

int sys_readdir()
{
	return file_readdir();
}
int sys_closedir(int fd)
{
	struct fs_fd *vfs_fd;
	int err = 0;
	vfs_fd = fd_get(fd);
	if (!vfs_fd){
		return -STATUS_EINVAL;
	}
	if(!(err=file_closedir(vfs_fd))){
		while(vfs_fd && vfs_fd->ref_count >= 1)
			fd_put(vfs_fd);
		return 0;
	}
	else{
		while(vfs_fd && vfs_fd->ref_count >= 1)
			fd_put(vfs_fd);
		return err;
	}
}

              

