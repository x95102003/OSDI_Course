/* This file use for NCTU OSDI course */
#include <fs.h>
#include <fat/diskio.h>
#include <fat/ff.h>
#include <kernel/drv/disk.h>
#include <kernel/timer.h>
#define DISK_ID 1

/*TODO: Lab7, low level file operator.*/

/**
  * @brief  Initial IDE disk
  * @param  pdrv: disk type
  * @retval disk error status
  *         - RES_OK: Initial success
  *         - STA_NOINIT: Intial failed
  */
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

/**
  * @brief  Get disk current status
  * @param  pdrv: disk type
  * @retval disk status
  *         - RES_OK: Normal status
  *         - STA_NODISK: ...
  */
DSTATUS disk_status (BYTE pdrv)
{
	//printk("disk_status\n");
    /* TODO */
	if(!ide_devices[DISK_ID].Reserved)
		return RES_NOTRDY;
	return RES_OK;
    
}

/**
  * @brief  Read serval sector form a IDE disk
  * @param  pdrv: disk type
  * @param  buff: destination memory start address
  * @param  sector: start sector number
  * @param  count: number of sector
  * @retval disk status (See diskio.h)
  *         - RES_OK: success
  *         - < 0: failed
  */
DRESULT disk_read (BYTE pdrv, BYTE* buff, DWORD sector, UINT count)
{
    int err = 0;
    int i = count;
    BYTE *ptr = buff;
    UINT cur_sector = sector;
    //printk("disk_read %d sector %d count %d\n", pdrv, sector, count);
	err = ide_read_sectors(DISK_ID, i, cur_sector, (unsigned int)ptr);
	if(err == 0)
		return RES_OK;
	return -err;
    /* TODO */
}

/**
  * @brief  Write serval sector to a IDE disk
  * @param  pdrv: disk type
  * @param  buff: memory start address
  * @param  sector: destination start sector number
  * @param  count: number of sector
  * @retval disk status (See diskio.h)
  *         - RES_OK: success
  *         - < 0: failed
  */
DRESULT disk_write (BYTE pdrv, const BYTE* buff, DWORD sector, UINT count)
{
    int err = 0;
    int i = count;
    BYTE *ptr = buff;
    UINT cur_sector = sector;
    //printk("disk_write %d sector %d count %d\n", pdrv, sector, count);
	err = ide_write_sectors(DISK_ID, i, cur_sector, (unsigned int)ptr);	
	if(err == 0)
		return RES_OK;
	return -err;
    /* TODO */    

}

/**
  * @brief  Get disk information form disk
  * @param  pdrv: disk type
  * @param  cmd: disk control command (See diskio.h)
  *         - GET_SECTOR_COUNT
  *         - GET_BLOCK_SIZE (Same as sector size)
  * @param  buff: return memory space
  * @retval disk status (See diskio.h)
  *         - RES_OK: success
  *         - < 0: failed
  */
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


/**
  * @brief  Get OS timestamp
  * @retval tick of CPU
  */
DWORD get_fattime (void)
{
    /* TODO */
	return sys_get_ticks();
}
