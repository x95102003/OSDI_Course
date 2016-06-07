/* This file use for NCTU OSDI course */
#include <fs.h>
#include <fat/diskio.h>
#include <fat/ff.h>
#include <kernel/drv/disk.h>
#include <kernel/timer.h>
#define DISK_ID 1

/*TODO: Lab7, low level file operator.*/
DSTATUS disk_initialize (BYTE pdrv)
{
    printk("disk_initialize %d\n", pdrv);
    /* TODO */
	if(!ide_devices[DISK_ID].Reserved){
		printk("Not found disk :%d\n", DISK_ID);
		return RES_NOTRDY;
	}
	//	disk_init();	
	return RES_OK;
}
DSTATUS disk_status (BYTE pdrv)
{
	//printk("disk_status\n");
    /* TODO */
	if(!ide_devices[DISK_ID].Reserved)
		return RES_NOTRDY;
	return RES_OK;
    
}
DRESULT disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count)
{
    int err = 0;
    int i = count;
    BYTE *ptr = buff;
    UINT cur_sector = sector;
    printk("disk_read %d sector %d count %d\n", pdrv, sector, count);
	err = ide_read_sectors(DISK_ID, i, cur_sector, (unsigned int)ptr);
	return err;
    /* TODO */
}
DRESULT disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count)
{
    int err = 0;
    int i = count;
    BYTE *ptr = buff;
    UINT cur_sector = sector;
    printk("disk_write %d sector %d count %d\n", pdrv, sector, count);
	err = ide_write_sectors(DISK_ID, i, cur_sector, (unsigned int)ptr);	
	return err;
    /* TODO */    

}
DRESULT disk_ioctl (BYTE pdrv, BYTE cmd, void* buff)
{
    uint32_t *retVal = (uint32_t *)buff;
	printk("disk_ioctl drv=%d cmd=%d\n", pdrv, cmd);
    switch(cmd){
		case CTRL_SYNC:
			return RES_OK;
			while (ide_read((unsigned int)ide_devices[DISK_ID].Channel, ATA_REG_STATUS) & ATA_SR_BSY)
				;
			break;
		case GET_SECTOR_COUNT:
			*retVal = ide_read(ide_devices[DISK_ID].Channel, ATA_REG_SECCOUNT0);
			break;
		case GET_SECTOR_SIZE:
			*retVal = 512;
			break;
	}
    /* TODO */
	return RES_OK;
}

DWORD get_fattime (void)
{
    /* TODO */
	return sys_get_ticks();
}
