/* This file use for NCTU OSDI course */
/* It's contants fat file system operators */

#include <inc/stdio.h>
#include <fs.h>
#include <fat/ff.h>
#include <diskio.h>

extern struct fs_dev fat_fs;

/*TODO: Lab7, fat level file operator.*/

int fat_mount(struct fs_dev *fs, unsigned long rwflag, const void* data)
{
	//fs->data = data;
	return f_mount(fs->data, &fs->path, 1 );	
}
int fat_mkfs(const char* device_name)
{

	if(!device_name)
		return -1;
	return f_mkfs(device_name, 0, 512);
}
int fat_open(struct fs_fd* file)
{

	FIL* fd = (FIL*)file->data;
	BYTE flag = 0x00;
	uint32_t fd_flag = file->flags;
	flag |=(!(fd_flag & O_RDONLY))?FA_READ:0;
	flag |=(fd_flag & O_WRONLY)?FA_WRITE:0;
	flag |=(fd_flag & O_RDWR)?(FA_WRITE|FA_READ):0;
	flag |=(fd_flag & O_CREAT)?FA_OPEN_ALWAYS:FA_OPEN_EXISTING;
	flag |=(fd_flag & O_TRUNC)?FA_CREATE_ALWAYS:FA_OPEN_EXISTING;
	if (fd_flag & O_APPEND)
	{
		file->pos = file->size;
	}
	fd->err = f_open(fd, &(file->path), flag);
	if(!fd->err)
		return 0;
	return -1;
}

int fat_read(struct fs_fd* file, void* buf, size_t count)
{
	FIL* fd = (FIL*)file->data;
	int retVal = 0;
	fd->err = (BYTE)f_read(fd, buf, count, &retVal);
	//printk("read error:%d, retVal:%d\n", fd->err, retVal);
	if(fd->err == 0)
		return retVal;
	return -1;
}
int fat_write(struct fs_fd* file, const void* buf, size_t count)
{
	FIL* fd = (FIL*)file->data;
	int retVal = 0;
	fd->err = f_write(fd, buf, count, &retVal);
	//printk("write error:%d retVal:%d\n", fd->err, retVal);
	if(!fd->err){
		file->pos = retVal;
		return retVal;
	}
	return -1;
}
int fat_lseek(struct fs_fd* file, off_t offset)
{
	FIL* fd = (FIL*)file->data;
	fd->err = (BYTE)f_lseek(fd, offset);
	if(!fd->err)
		return offset;
	return -1;

}
int fat_unlink(struct fs_fd* file, const char *pathname)
{
	FIL* fd = (FIL*)file->data;
	fd->err = (BYTE)f_unlink(pathname);
	if(!fd->err)
		return 0;
	return -1;
	
}
int fat_close(struct fs_fd* file)
{
	FIL* fd = (FIL*)file->data;
	fd->err = (BYTE)f_close(fd);
	if(!fd->err)
		return 0;
	return -1;
}

struct fs_ops elmfat_ops = {
    .dev_name = "elmfat",
    .mount = fat_mount,
    .mkfs = fat_mkfs,
    .open = fat_open,
    .close = fat_close,
    .read = fat_read,
    .write = fat_write,
    .lseek = fat_lseek
};



