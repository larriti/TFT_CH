/*-----------------------------------------------------------------------*/
/* Low level disk I/O module skeleton for FatFs     (C)ChaN, 2014        */
/*-----------------------------------------------------------------------*/
/* If a working storage control module is available, it should be        */
/* attached to the FatFs via a glue function rather than modifying it.   */
/* This is an example of glue functions to attach various exsisting      */
/* storage control modules to the FatFs module with a defined API.       */
/*-----------------------------------------------------------------------*/

#include "diskio.h"		/* FatFs lower layer API */
#include "stm32f10x.h"
#include "sdio_function.h"

#define BLOCK_SIZE 		512

/*-----------------------------------------------------------------------*/
/* Get Drive Status                                                      */
/*-----------------------------------------------------------------------*/

DSTATUS disk_status (
	BYTE pdrv		/* Physical drive nmuber to identify the drive */
)
{
	
	return RES_OK;
}



/*-----------------------------------------------------------------------*/
/* Inidialize a Drive                                                    */
/*-----------------------------------------------------------------------*/

DSTATUS disk_initialize (
	BYTE pdrv				/* Physical drive nmuber to identify the drive */
)
{
	SD_Error Status;
	if(pdrv)
	{
		return STA_NOINIT;
	}
	Status = SD_Init();
	if(Status != SD_OK)
	{
		return STA_NOINIT;
	}
	else
	{
		return RES_OK;
	}
}



/*-----------------------------------------------------------------------*/
/* Read Sector(s)                                                        */
/*-----------------------------------------------------------------------*/

DRESULT disk_read (
	BYTE pdrv,		/* Physical drive nmuber to identify the drive */
	BYTE *buff,		/* Data buffer to store read data */
	DWORD sector,	/* Sector address in LBA */
	UINT count		/* Number of sectors to read */
)
{
	if(count>1)
	{
		SD_ReadMultiBlocks(buff, sector*BLOCK_SIZE, BLOCK_SIZE, count);
		// check transfer finished
		SD_WaitReadOperation();
		// wait the DMA transfer finished
		while(SD_GetStatus() != SD_TRANSFER_OK);
	}
	else
	{
		SD_ReadBlock(buff, sector*BLOCK_SIZE, BLOCK_SIZE);
		// check transfer finished
		SD_WaitReadOperation();
		// wait the DMA transfer finished
		while(SD_GetStatus() != SD_TRANSFER_OK);

	}
	return RES_OK;	
}



/*-----------------------------------------------------------------------*/
/* Write Sector(s)                                                       */
/*-----------------------------------------------------------------------*/

#if _USE_WRITE
DRESULT disk_write (
	BYTE pdrv,			/* Physical drive nmuber to identify the drive */
	const BYTE *buff,	/* Data to be written */
	DWORD sector,		/* Sector address in LBA */
	UINT count			/* Number of sectors to write */
)
{
	if(count>1)
	{
		SD_WriteMultiBlocks((uint8_t *)buff, sector*BLOCK_SIZE, BLOCK_SIZE, count);
		// check transfer finished
		SD_WaitWriteOperation();
		// wait the DMA transfer finished
		while(SD_GetStatus() != SD_TRANSFER_OK);
	}
	else
	{
		SD_WriteBlock((uint8_t *)buff, sector*BLOCK_SIZE, BLOCK_SIZE);
		// check transfer finished
		SD_WaitReadOperation();
		// wait the DMA transfer finished
		while(SD_GetStatus() != SD_TRANSFER_OK);

	}

	return RES_OK;
}
#endif


/*-----------------------------------------------------------------------*/
/* Miscellaneous Functions                                               */
/*-----------------------------------------------------------------------*/

#if _USE_IOCTL
DRESULT disk_ioctl (
	BYTE pdrv,		/* Physical drive nmuber (0..) */
	BYTE cmd,		/* Control code */
	void *buff		/* Buffer to send/receive control data */
)
{
	return RES_OK;
}

/*-----------------------------------------------------------------------*/
/* get Current time                                                      */
/*-----------------------------------------------------------------------*/

DWORD get_fattime(void)
{
	return 0;
}

#endif
