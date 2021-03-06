/*---------------------------------------------------------------------------/
/  FatFs - FAT file system module include file  R0.09a    (C)ChaN, 2012
/----------------------------------------------------------------------------/
/ FatFs module is a generic FAT file system module for small embedded systems.
/ This is a free software that opened for education, research and commercial
/ developments under license policy of following terms.
/
/  Copyright (C) 2012, ChaN, all right reserved.
/
/ * The FatFs module is a free software and there is NO WARRANTY.
/ * No restriction on use. You can use, modify and redistribute it for
/   personal, non-profit or commercial product UNDER YOUR RESPONSIBILITY.
/ * Redistributions of source code must retain the above copyright notice.
/
/----------------------------------------------------------------------------*/

#ifndef _SDFS_FATFS
#define _SDFS_FATFS  4004    /* Revision ID */

#ifdef __cplusplus
extern "C" {
#endif

#include "sdfs_integer.h"    /* Basic integer types */
#include "ffconf.h"     /* FatFs configuration options */

#if _SDFS_FATFS != _FFCONF
#error Wrong configuration file (ffconf.h).
#endif

namespace SDFS
{

/* Definitions of volume management */

#if _MULTI_PARTITION        /* Multiple partition configuration */
typedef struct {
    BYTE pd;    /* Physical drive number */
    BYTE pt;    /* Partition: 0:Auto detect, 1-4:Forced partition) */
} PARTITION;
extern PARTITION VolToPart[];   /* Volume - Partition resolution table */
#ifdef LD2PD
#undef LD2PD
#endif
#define LD2PD(vol) (VolToPart[vol].pd)  /* Get physical drive number */
#ifdef LD2PT
#undef LD2PT
#endif
#define LD2PT(vol) (VolToPart[vol].pt)  /* Get partition index */

#else                           /* Single partition configuration */
#ifdef LD2PD
#undef LD2PD
#endif
#define LD2PD(vol) (BYTE)(vol)  /* Each logical drive is bound to the same physical drive number */
#ifdef LD2PT
#undef LD2PT
#endif
#define LD2PT(vol) 0            /* Always mounts the 1st partition or in SFD */

#endif



/* Type of path name strings on FatFs API */

#if _LFN_UNICODE            /* Unicode string */
#if !_USE_LFN
#error _LFN_UNICODE must be 0 in non-LFN cfg.
#endif
#ifndef _INC_TCHAR
typedef WCHAR TCHAR;
#ifdef _T
#undef _T
#endif
#define _T(x) L ## x
#ifdef _TEXT
#undef _TEXT
#endif
#define _TEXT(x) L ## x
#endif

#else                       /* ANSI/OEM string */
#ifndef _INC_TCHAR
typedef char TCHAR;
#ifdef _T
#undef _T
#endif
#define _T(x) x
#ifdef _TEXT
#undef _TEXT
#endif
#define _TEXT(x) x
#endif

#endif



/* File system object structure (FATFS) */

typedef struct {
    BYTE    fs_type;        /* FAT sub-type (0:Not mounted) */
    BYTE    drv;            /* Physical drive number */
    BYTE    csize;          /* Sectors per cluster (1,2,4...128) */
    BYTE    n_fats;         /* Number of FAT copies (1,2) */
    BYTE    wflag;          /* win[] dirty flag (1:must be written back) */
    BYTE    fsi_flag;       /* fsinfo dirty flag (1:must be written back) */
    WORD    id;             /* File system mount ID */
    WORD    n_rootdir;      /* Number of root directory entries (FAT12/16) */
#if _MAX_SS != 512
    WORD    ssize;          /* Bytes per sector (512, 1024, 2048 or 4096) */
#endif
#if _FS_REENTRANT
    _SYNC_t sobj;           /* Identifier of sync object */
#endif
#if !_FS_READONLY
    DWORD   last_clust;     /* Last allocated cluster */
    DWORD   free_clust;     /* Number of free clusters */
    DWORD   fsi_sector;     /* fsinfo sector (FAT32) */
#endif
#if _FS_RPATH
    DWORD   cdir;           /* Current directory start cluster (0:root) */
#endif
    DWORD   n_fatent;       /* Number of FAT entries (= number of clusters + 2) */
    DWORD   fsize;          /* Sectors per FAT */
    DWORD   fatbase;        /* FAT start sector */
    DWORD   dirbase;        /* Root directory start sector (FAT32:Cluster#) */
    DWORD   database;       /* Data start sector */
    DWORD   winsect;        /* Current sector appearing in the win[] */
    BYTE    win[_MAX_SS];   /* Disk access window for Directory, FAT (and Data on tiny cfg) */
} FATFS;



/* File object structure (FIL) */

typedef struct {
    FATFS*  fs;             /* Pointer to the related file system object */
    WORD    id;             /* File system mount ID of the related file system object */
    BYTE    flag;           /* File status flags */
    BYTE    pad1;
    DWORD   fptr;           /* File read/write pointer (0ed on file open) */
    DWORD   fsize;          /* File size */
    DWORD   sclust;         /* File data start cluster (0:no data cluster, always 0 when fsize is 0) */
    DWORD   clust;          /* Current cluster of fpter */
    DWORD   dsect;          /* Current data sector of fpter */
#if !_FS_READONLY
    DWORD   dir_sect;       /* Sector containing the directory entry */
    BYTE*   dir_ptr;        /* Pointer to the directory entry in the window */
#endif
#if _USE_FASTSEEK
    DWORD*  cltbl;          /* Pointer to the cluster link map table (null on file open) */
#endif
#if _FS_LOCK
    UINT    lockid;         /* File lock ID (index of file semaphore table Files[]) */
#endif
#if !_FS_TINY
    BYTE    buf[_MAX_SS];   /* File data read/write buffer */
#endif
} FIL;



/* Directory object structure (DIR) */

typedef struct {
    FATFS*  fs;             /* Pointer to the owner file system object */
    WORD    id;             /* Owner file system mount ID */
    WORD    index;          /* Current read/write index number */
    DWORD   sclust;         /* Table start cluster (0:Root dir) */
    DWORD   clust;          /* Current cluster */
    DWORD   sect;           /* Current sector */
    BYTE*   dir;            /* Pointer to the current SFN entry in the win[] */
    BYTE*   fn;             /* Pointer to the SFN (in/out) {file[8],ext[3],status[1]} */
#if _USE_LFN
    WCHAR*  lfn;            /* Pointer to the LFN working buffer */
    WORD    lfn_idx;        /* Last matched LFN index number (0xFFFF:No LFN) */
#endif
} FATFS_DIR;



/* File status structure (FILINFO) */

typedef struct {
    DWORD   fsize;          /* File size */
    WORD    fdate;          /* Last modified date */
    WORD    ftime;          /* Last modified time */
    BYTE    fattrib;        /* Attribute */
    TCHAR   fname[13];      /* Short file name (8.3 format) */
#if _USE_LFN
    TCHAR*  lfname;         /* Pointer to the LFN buffer */
    UINT    lfsize;         /* Size of LFN buffer in TCHAR */
#endif
} FILINFO;



/* File function return code (FRESULT) */

typedef enum {
    FR_OK = 0,              /* (0) Succeeded */
    FR_DISK_ERR,            /* (1) A hard error occurred in the low level disk I/O layer */
    FR_INT_ERR,             /* (2) Assertion failed */
    FR_NOT_READY,           /* (3) The physical drive cannot work */
    FR_NO_FILE,             /* (4) Could not find the file */
    FR_NO_PATH,             /* (5) Could not find the path */
    FR_INVALID_NAME,        /* (6) The path name format is invalid */
    FR_DENIED,              /* (7) Access denied due to prohibited access or directory full */
    FR_EXIST,               /* (8) Access denied due to prohibited access */
    FR_INVALID_OBJECT,      /* (9) The file/directory object is invalid */
    FR_WRITE_PROTECTED,     /* (10) The physical drive is write protected */
    FR_INVALID_DRIVE,       /* (11) The logical drive number is invalid */
    FR_NOT_ENABLED,         /* (12) The volume has no work area */
    FR_NO_FILESYSTEM,       /* (13) There is no valid FAT volume */
    FR_MKFS_ABORTED,        /* (14) The f_mkfs() aborted due to any parameter error */
    FR_TIMEOUT,             /* (15) Could not get a grant to access the volume within defined period */
    FR_LOCKED,              /* (16) The operation is rejected according to the file sharing policy */
    FR_NOT_ENOUGH_CORE,     /* (17) LFN working buffer could not be allocated */
    FR_TOO_MANY_OPEN_FILES, /* (18) Number of open files > _FS_SHARE */
    FR_INVALID_PARAMETER    /* (19) Given parameter is invalid */
} FRESULT;



/*--------------------------------------------------------------*/
/* FatFs module application interface                           */

FRESULT f_mount (BYTE, FATFS*);                     /* Mount/Unmount a logical drive */
FRESULT f_open (FIL*, const TCHAR*, BYTE);          /* Open or create a file */
FRESULT f_read (FIL*, void*, UINT, UINT*);          /* Read data from a file */
FRESULT f_lseek (FIL*, DWORD);                      /* Move file pointer of a file object */
FRESULT f_close (FIL*);                             /* Close an open file object */
FRESULT f_opendir (FATFS_DIR*, const TCHAR*);             /* Open an existing directory */
FRESULT f_readdir (FATFS_DIR*, FILINFO*);                 /* Read a directory item */
FRESULT f_stat (const TCHAR*, FILINFO*);            /* Get file status */
FRESULT f_write (FIL*, const void*, UINT, UINT*);   /* Write data to a file */
FRESULT f_getfree (const TCHAR*, DWORD*, FATFS**);  /* Get number of free clusters on the drive */
FRESULT f_truncate (FIL*);                          /* Truncate file */
FRESULT f_sync (FIL*);                              /* Flush cached data of a writing file */
FRESULT f_unlink (const TCHAR*);                    /* Delete an existing file or directory */
FRESULT f_mkdir (const TCHAR*);                     /* Create a new directory */
FRESULT f_chmod (const TCHAR*, BYTE, BYTE);         /* Change attribute of the file/dir */
FRESULT f_utime (const TCHAR*, const FILINFO*);     /* Change times-tamp of the file/dir */
FRESULT f_rename (const TCHAR*, const TCHAR*);      /* Rename/Move a file or directory */
FRESULT f_chdrive (BYTE);                           /* Change current drive */
FRESULT f_chdir (const TCHAR*);                     /* Change current directory */
FRESULT f_getcwd (TCHAR*, UINT);                    /* Get current directory */
FRESULT f_forward (FIL*, UINT(*)(const BYTE*,UINT), UINT, UINT*);   /* Forward data to the stream */
FRESULT f_mkfs (BYTE, BYTE, UINT);                  /* Create a file system on the drive */
FRESULT f_fdisk (BYTE, const DWORD[], void*);       /* Divide a physical drive into some partitions */
int f_putc (TCHAR, FIL*);                           /* Put a character to the file */
int f_puts (const TCHAR*, FIL*);                    /* Put a string to the file */
int f_printf (FIL*, const TCHAR*, ...);             /* Put a formatted string to the file */
TCHAR* f_gets (TCHAR*, int, FIL*);                  /* Get a string from the file */

#ifdef f_eof
#undef f_eof
#endif
#define f_eof(fp) (((fp)->fptr == (fp)->fsize) ? 1 : 0)
#ifdef f_error
#undef f_error
#endif
#define f_error(fp) (((fp)->flag & FA__ERROR) ? 1 : 0)
#ifdef f_tell
#undef f_tell
#endif
#define f_tell(fp) ((fp)->fptr)
#ifdef f_size
#undef f_size
#endif
#define f_size(fp) ((fp)->fsize)

#ifndef EOF
#ifdef EOF
#undef EOF
#endif
#define EOF (-1)
#endif




/*--------------------------------------------------------------*/
/* Additional user defined functions                            */

/* RTC function */
#if !_FS_READONLY
DWORD get_fattime (void);
#endif

/* Unicode support functions */
#if _USE_LFN                        /* Unicode - OEM code conversion */
WCHAR ff_convert (WCHAR, UINT);     /* OEM-Unicode bidirectional conversion */
WCHAR ff_wtoupper (WCHAR);          /* Unicode upper-case conversion */
#if _USE_LFN == 3                   /* Memory functions */
void* ff_memalloc (UINT);           /* Allocate memory block */
void ff_memfree (void*);            /* Free memory block */
#endif
#endif

/* Sync functions */
#if _FS_REENTRANT
int ff_cre_syncobj (BYTE, _SYNC_t*);/* Create a sync object */
int ff_req_grant (_SYNC_t);         /* Lock sync object */
void ff_rel_grant (_SYNC_t);        /* Unlock sync object */
int ff_del_syncobj (_SYNC_t);       /* Delete a sync object */
#endif




/*--------------------------------------------------------------*/
/* Flags and offset address                                     */


/* File access control and file status flags (FIL.flag) */

#ifdef FA_READ
#undef FA_READ
#endif
#define FA_READ             0x01
#ifdef FA_OPEN_EXISTING
#undef FA_OPEN_EXISTING
#endif
#define FA_OPEN_EXISTING    0x00
#ifdef FA__ERROR
#undef FA__ERROR
#endif
#define FA__ERROR           0x80

#if !_FS_READONLY
#ifdef FA_WRITE
#undef FA_WRITE
#endif
#define FA_WRITE            0x02
#ifdef FA_CREATE_NEW
#undef FA_CREATE_NEW
#endif
#define FA_CREATE_NEW       0x04
#ifdef FA_CREATE_ALWAYS
#undef FA_CREATE_ALWAYS
#endif
#define FA_CREATE_ALWAYS    0x08
#ifdef FA_OPEN_ALWAYS
#undef FA_OPEN_ALWAYS
#endif
#define FA_OPEN_ALWAYS      0x10
#ifdef FA__WRITTEN
#undef FA__WRITTEN
#endif
#define FA__WRITTEN         0x20
#ifdef FA__DIRTY
#undef FA__DIRTY
#endif
#define FA__DIRTY           0x40
#endif


/* FAT sub type (FATFS.fs_type) */

#ifdef FS_FAT12
#undef FS_FAT12
#endif
#define FS_FAT12    1
#ifdef FS_FAT16
#undef FS_FAT16
#endif
#define FS_FAT16    2
#ifdef FS_FAT32
#undef FS_FAT32
#endif
#define FS_FAT32    3


/* File attribute bits for directory entry */

#ifdef AM_RDO
#undef AM_RDO
#endif
#define AM_RDO  0x01    /* Read only */
#ifdef AM_HID
#undef AM_HID
#endif
#define AM_HID  0x02    /* Hidden */
#ifdef AM_SYS
#undef AM_SYS
#endif
#define AM_SYS  0x04    /* System */
#ifdef AM_VOL
#undef AM_VOL
#endif
#define AM_VOL  0x08    /* Volume label */
#ifdef AM_LFN
#undef AM_LFN
#endif
#define AM_LFN  0x0F    /* LFN entry */
#ifdef AM_DIR
#undef AM_DIR
#endif
#define AM_DIR  0x10    /* Directory */
#ifdef AM_ARC
#undef AM_ARC
#endif
#define AM_ARC  0x20    /* Archive */
#ifdef AM_MASK
#undef AM_MASK
#endif
#define AM_MASK 0x3F    /* Mask of defined bits */


/* Fast seek feature */
#ifdef CREATE_LINKMAP
#undef CREATE_LINKMAP
#endif
#define CREATE_LINKMAP  0xFFFFFFFF



/*--------------------------------*/
/* Multi-byte word access macros  */

#if _WORD_ACCESS == 1   /* Enable word access to the FAT structure */
#ifdef LD_WORD
#undef LD_WORD
#endif
#define LD_WORD(ptr)        (WORD)(*(WORD*)(BYTE*)(ptr))
#ifdef LD_DWORD
#undef LD_DWORD
#endif
#define LD_DWORD(ptr)       (DWORD)(*(DWORD*)(BYTE*)(ptr))
#ifdef ST_WORD
#undef ST_WORD
#endif
#define ST_WORD(ptr,val)    *(WORD*)(BYTE*)(ptr)=(WORD)(val)
#ifdef ST_DWORD
#undef ST_DWORD
#endif
#define ST_DWORD(ptr,val)   *(DWORD*)(BYTE*)(ptr)=(DWORD)(val)
#else                   /* Use byte-by-byte access to the FAT structure */
#ifdef LD_WORD
#undef LD_WORD
#endif
#define LD_WORD(ptr)        (WORD)(((WORD)*((BYTE*)(ptr)+1)<<8)|(WORD)*(BYTE*)(ptr))
#ifdef LD_DWORD
#undef LD_DWORD
#endif
#define LD_DWORD(ptr)       (DWORD)(((DWORD)*((BYTE*)(ptr)+3)<<24)|((DWORD)*((BYTE*)(ptr)+2)<<16)|((WORD)*((BYTE*)(ptr)+1)<<8)|*(BYTE*)(ptr))
#ifdef ST_WORD
#undef ST_WORD
#endif
#define ST_WORD(ptr,val)    *(BYTE*)(ptr)=(BYTE)(val); *((BYTE*)(ptr)+1)=(BYTE)((WORD)(val)>>8)
#ifdef ST_DWORD
#undef ST_DWORD
#endif
#define ST_DWORD(ptr,val)   *(BYTE*)(ptr)=(BYTE)(val); *((BYTE*)(ptr)+1)=(BYTE)((WORD)(val)>>8); *((BYTE*)(ptr)+2)=(BYTE)((DWORD)(val)>>16); *((BYTE*)(ptr)+3)=(BYTE)((DWORD)(val)>>24)
#endif

#ifdef __cplusplus
}
#endif

} // namespace SDFS

#endif /* _SDFS_FATFS */
