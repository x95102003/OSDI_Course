/* This file use for NCTU OSDI course */
/* It's contants file operator's wapper API */
#include <fs.h>
#include <fat/ff.h>
#include <inc/string.h>
#include <inc/stdio.h>

/* Static file objects */
FIL file_objs[FS_FD_MAX];

/* Static file system object */
FATFS fat;

/* It file object table */
struct fs_fd fd_table[FS_FD_MAX];

/* File system operator, define in fs_ops.c */
extern struct fs_ops elmfat_ops; //We only one file system...

/* File system object, it record the operator and file system object(FATFS) */
struct fs_dev fat_fs = {
    .dev_id = 1, //In this lab we only use second IDE disk
    .path = {0}, // Not yet mount to any path
    .ops = &elmfat_ops,
    .data = &fat
};
    
/*TODO: Lab7, VFS level file API.*/
int fs_init()
{
    int res, i;
    
    /* Initial fd_tables */
    for (i = 0; i < FS_FD_MAX; i++)
    {
        fd_table[i].flags = 0;
        fd_table[i].size = 0;
        fd_table[i].pos = 0;
        fd_table[i].type = 0;
        fd_table[i].ref_count = 0;
        fd_table[i].data = &file_objs[i];
        fd_table[i].fs = &fat_fs;
    }
    
    /* Mount fat file system at "/" */
    /* Check need mkfs or not */
    if ((res = fs_mount("elmfat", "/", NULL)) != 0)
    {
        fat_fs.ops->mkfs("elmfat");
        res = fs_mount("elmfat", "/", NULL);
        return res;
    }
    return -STATUS_EIO;
       
}

/** Mount a file system by path 
*  Note: You need compare the device_name with fat_fs.ops->dev_name and find the file system operator
*        then call ops->mount().
*
*  @param data: If you have mutilple file system it can be use for pass the file system object pointer save in fat_fs->data. 
*/
int fs_mount(const char* device_name, const char* path, const void* data)
{
	*fat_fs.ops->dev_name = *device_name;
	if(path)
		strcpy(fat_fs.path, path);
	return fat_mount(&fat_fs , data); 
} 

int file_read(struct fs_fd* fd, void *buf, size_t len)
{
	return fat_read(fd, buf, len);
}

int file_write(struct fs_fd* fd, const void *buf, size_t len)
{
	return fat_write(fd, buf, len);
}

/* Note: Before call call fat_fs.ops->open() you may copy the path and flags parameters into fd object structure */
int file_open(struct fs_fd* fd, const char *path, int flags)
{

	strcpy(fd->path, path);
	fd->flags = flags;
	fd_get(fd);
	return fat_open(fd);

}

int file_close(struct fs_fd* fd)
{

	if(!fat_close(fd)){
		return 0;
	}
	return -1;
	
}
int file_lseek(struct fs_fd* fd, off_t offset)
{
	return fat_lseek(fd, offset);
}
int file_unlink(const char *path)
{
	int i;
	struct fs_fd *fd = NULL;
	for (i=0;i<FS_FD_MAX;i++){
		if (!(strcmp(fd_table[i].path, path))){
			fd = fd_get(i);
			break;
		}
	}
	if(fd)
		return fat_unlink(fd, path);
	else
		return -STATUS_ENOENT;
}
int file_opendir(struct fs_fd* fd, const char*path)
{
	return fat_opendir(fd, path);
}
int file_readdir(struct fs_fd* fd, struct dirent *dirp)
{
	return fat_readdir();
}
int file_getdents(struct fs_fd* fd, struct dirent *dirp, uint32_t count)
{
	return fat_getdents(fd, dirp, count);	
}
int file_closedir(struct fs_fd* fd)
{
	return fat_closedir(fd);
}
/**
 * @ingroup Fd
 * This function will allocate a file descriptor.
 *
 * @return -1 on failed or the allocated file descriptor.
 */
int fd_new(void)
{
	struct fs_fd* d;
	int idx;

	/* find an empty fd entry */
	for (idx = 0; idx < FS_FD_MAX && fd_table[idx].ref_count > 0; idx++);


	/* can't find an empty fd entry */
	if (idx == FS_FD_MAX)
	{
		idx = -1;
		goto __result;
	}

	d = &(fd_table[idx]);
	d->ref_count = 1;

__result:
	return idx;
}

/**
 * @ingroup Fd
 *
 * This function will return a file descriptor structure according to file
 * descriptor.
 *
 * @return NULL on on this file descriptor or the file descriptor structure
 * pointer.
 */
struct fs_fd* fd_get(int fd)
{
	struct fs_fd* d;

	if ( fd < 0 || fd > FS_FD_MAX ) return NULL;

	d = &fd_table[fd];

	/* increase the reference count */
	d->ref_count ++;

	return d;
}

/**
 * @ingroup Fd
 *
 * This function will put the file descriptor.
 */
void fd_put(struct fs_fd* fd)
{

	fd->ref_count --;

	/* clear this fd entry */
	if ( fd->ref_count == 0 )
	{
		memset(fd->data, 0, sizeof(FIL));
		//memset(fd, 0, sizeof(struct fs_fd));
	}
};


