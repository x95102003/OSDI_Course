/* This file use for NCTU OSDI course */
/* It's contants fat file system operators */

#include <inc/stdio.h>
#include <fs.h>
#include <fat/ff.h>
#include <diskio.h>

extern struct fs_dev fat_fs;

/*TODO: Lab7, fat level file operator.
*       Implement below functions to support basic file system operators by using the elmfat's API(f_xxx).
*       Reference: http://elm-chan.org/fsw/ff/00index_e.html
*/

/* Note: 1. Get FATFS object from fs->data
*        2. Check fs->path parameter then call f_mount.
*/
int fat_mount(struct fs_dev *fs, const void* data)
{
	return f_mount(fs->data, &fs->path,1 );	
}

/* Note: Just call f_mkfs at root path '/' */
int fat_mkfs(const char* device_name)
{

	if(!device_name)
		return -1;
	return f_mkfs(device_name, 0, 512);
}

/* Note: Convert the POSIX's open flag to elmfat's flag.
*        Example: if file->flags == O_RDONLY then open_mode = FA_READ
*                 if file->flags & O_APPEND then f_seek the file to end after f_open
*        Reference: http://elm-chan.org/fsw/ff/en/open.html 
*/
int fat_open(struct fs_fd* file)
{

	FIL* fd = (FIL*)file->data;
	off_t offset = 0;
	BYTE flag = 0x00;
	uint32_t fd_flag = file->flags;
	flag |=(!(fd_flag & O_RDONLY))?FA_READ:0;
	flag |=(fd_flag & O_WRONLY)?FA_WRITE:0;
	flag |=(fd_flag & O_RDWR)?(FA_WRITE|FA_READ):0;
	flag |=(fd_flag & O_CREAT && !(fd_flag & O_TRUNC))?FA_CREATE_NEW :FA_OPEN_EXISTING;
	flag |=(fd_flag & O_TRUNC)?FA_CREATE_ALWAYS:FA_OPEN_EXISTING;
	fd->err = f_open(fd, &(file->path), flag);
	if (fd_flag & O_APPEND)
	{
		offset += fd->obj.objsize;
		printk("offset%d size:%d\n", offset, fd->obj.objsize);
		fat_lseek(file, offset);
	}


	switch(fd->err){
		case 0:
			return FR_OK;
			break;
		case 1:
			return -17;
			break;
		case 3:
			return -6;
			break;
		case 4:
			return -2;
			break;
		case 8:
			return -17;
			break;
		case 9:
			return -22;
			break;
		case 11:
			return -22;
			break;
		case 19:
			return -22;
			break;
		default:
			return fd->err;
			break;
	}

	//return fd->err;
	//if(!fd->err)
	//	return 0;
	//return -1;
}

int fat_read(struct fs_fd* file, void* buf, size_t count)
{
	FIL* fd = (FIL*)file->data;
	int retVal = 0;
	fd->err = (BYTE)f_read(fd, buf, count, &retVal);
	//printk("read error:%d, retVal:%d\n", fd->err, retVal);
	switch(fd->err){
		case 0:
			return retVal;
			break;
		case 1:
			return -5;
			break;
		case 3:
			return -6;
			break;
		case 4:
			return -2;
			break;
		case 9:
			return -22;
			break;
		case 11:
			return -22;
			break;
		case 19:
			return -22;
			break;
		default:
			return fd->err;
			break;
	}
	/*if(fd->err == 0)
		return retVal;
	return -1;*/
}
int fat_write(struct fs_fd* file, const void* buf, size_t count)
{
	FIL* fd = (FIL*)file->data;
	int retVal = 0;
	fd->err = f_write(fd, buf, count, &retVal);
	//printk("write error:%d retVal:%d\n", fd->err, retVal);
	switch(fd->err){
		case 0:
			file->pos = retVal;
			file->size += retVal;
			return retVal;
			break;
		case 1:
			return -5;
			break;
		case 3:
			return -6;
			break;
		case 4:
			return -2;
			break;
		case 9:
			return -22;
			break;
		case 11:
			return -22;
			break;
		case 19:
			return -22;
			break;
		default:
			return fd->err;
			break;
	}
}
int fat_lseek(struct fs_fd* file, off_t offset)
{
	FIL* fd = (FIL*)file->data;
	fd->err = (BYTE)f_lseek(fd, offset);
	return fd->err;
	if(!fd->err)
		return offset;
	return -1;

}
int fat_unlink(struct fs_fd* file, const char *pathname)
{
	FIL* fd = (FIL*)file->data;
	fd->err = (BYTE)f_unlink(pathname);
	switch(fd->err){
		case 0:
			return FR_OK;
			break;
		case 3:
			return -6;
			break;
		case 4:
			return -2;
			break;
		default:
			return fd->err;
			break;
	}
	
}
int fat_close(struct fs_fd* file)
{
	FIL* fd = (FIL*)file->data;
	fd->err = (BYTE)f_close(fd);
	switch(fd->err){
		case 0:
			return FR_OK;
			break;
		case 1:
			return -17;
			break;
		case 3:
			return -6;
			break;
		case 4:
			return -2;
			break;
		case 8:
			return -17;
			break;
		case 9:
			return -22;
			break;
		case 11:
			return -22;
			break;
		case 19:
			return -22;
			break;
		default:
			return fd->err;
	}
}
int fat_opendir(struct fs_fd* file, const char *path)
{
	DIR *dp = file->data;
	int err = f_opendir(dp, path);
	if(!err)
		return 0;
	return err;

}
int fat_readdir(struct fs_fd *file, struct dirent *dirp)
{
	DIR *dp = file->data;
	FILINFO fno;
	int err = f_readdir(dp, &fno);
	if(!err)
		return 0;
	return err;
}
int fat_getdents(struct fs_fd* file, struct dirent* dirp, uint32_t count)
{
	int err, err_size,i;
	FILINFO fno;
	DIR *dp = file->data;
	//printk("f_opendir error:%d\n", err);
	err_size = (count ==sizeof(struct dirent)) ? 0 : -1;
	if(!err){
		err = f_readdir(dp, &fno);
		if(!err) {
			dirp->d_type = fno.fattrib;
			dirp->d_namlen = strlen(fno.fname);
			strncpy(dirp->d_name, fno.fname, dirp->d_namlen);
			dirp->d_reclen = fno.fsize;
			//printk("Type :%x File name:%s  filesize:%d\n", fno.fattrib, fno.fname, fno.fsize);
			}
	}
	return err;
	//else
	//	return f_closedir(dp);
}
int fat_closedir(struct fs_fd* file)
{
	int err, err_size,i;
	FILINFO fno;
	DIR *dp = file->data;
	err = f_closedir(dp);
	return err;
	
}
struct fs_ops elmfat_ops = {
    .dev_name = "elmfat",
    .mount = fat_mount,
    .mkfs = fat_mkfs,
    .open = fat_open,
    .close = fat_close,
    .read = fat_read,
    .write = fat_write,
    .lseek = fat_lseek,
	.unlink = fat_unlink,
	.opendir = fat_opendir,
	.readdir = fat_readdir,
	.closedir = fat_closedir,
	.getdents = fat_getdents,
};



