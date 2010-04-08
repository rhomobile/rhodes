
#include "common/spirit_comtypes.h"
#include "common/spirit_memory.h"
#include <stdio.h>
#include "spirit_bmp.h"
#ifdef SPIRIT_OS_WIN32
#include <windows.h>
#endif
#include <memory.h>

#ifdef SPIRIT_OS_LINUX
#pragma pack(1)

#define WORD uint16 
#define DWORD uint32 
#define LONG int32

#define BI_RGB        0L
#define BI_RLE8       1L
#define BI_RLE4       2L
#define BI_BITFIELDS  3L
#define BI_JPEG       4L
#define BI_PNG        5L

typedef struct tagBITMAPFILEHEADER { 
  WORD    bfType; 
  DWORD   bfSize; 
  WORD    bfReserved1; 
  WORD    bfReserved2; 
  DWORD   bfOffBits; 
} BITMAPFILEHEADER, *PBITMAPFILEHEADER; 
typedef struct tagBITMAPINFOHEADER{
        DWORD      biSize;
        LONG       biWidth;
        LONG       biHeight;
        WORD       biPlanes;
        WORD       biBitCount;
        DWORD      biCompression;
        DWORD      biSizeImage;
        LONG       biXPelsPerMeter;
        LONG       biYPelsPerMeter;
        DWORD      biClrUsed;
        DWORD      biClrImportant;
} BITMAPINFOHEADER, *PBITMAPINFOHEADER;
#pragma pack()
#endif


SPIRIT_EXTERN_C tBool SpiritBitmap_LoadFromFile(tSpiritVideoFrame *pFrame,const char *filename)
{
  FILE *ifp;

  uint8 *pDataBuff =0;
  BITMAPFILEHEADER bfh;
  BITMAPINFOHEADER bih;
  uint32 buff_size;

  ifp = fopen(filename,"rb");
  if (!ifp) {
    fprintf(stderr,"%s: cannot open file '%s'\n",__FUNCTION__,filename);
    return 0;
  }

  if (sizeof(BITMAPFILEHEADER) !=14) {
    fprintf(stderr,"Warning: invalid BITMAPFILEHEADER structure size %i, awaiting 14\n",sizeof(BITMAPFILEHEADER));
  }
  if (1 != fread(&bfh,14/*sizeof(BITMAPFILEHEADER)*/,1,ifp)) {
    fprintf(stderr,"%s: error reading file\n",__FUNCTION__);
    fclose(ifp);
    return 0;
  }

  if (1 != fread(&bih,sizeof(BITMAPINFOHEADER),1,ifp)) {
    fprintf(stderr,"%s: error reading file\n",__FUNCTION__);
    fclose(ifp);
    return 0;
  }

  if (bfh.bfType != 0x4D42/* || bfh.bfSize != sizeof(BITMAPFILEHEADER) */|| bih.biSize != sizeof(BITMAPINFOHEADER)) {
    printf("Type =%X, %li(%i)\n",bfh.bfType,bih.biSize,sizeof(BITMAPINFOHEADER));
    fprintf(stderr,"%s: invalid bitmap format\n",__FUNCTION__);
    fclose(ifp);
    return 0;
  }

  if (bih.biCompression != BI_RGB || bih.biPlanes != 1 || bih.biBitCount !=32 || (bih.biWidth==0) || (bih.biHeight==0)) {
    fprintf(stderr,"%s: unsupported bitmap format \n",__FUNCTION__);
    fclose(ifp);
    return 0;
  }

  buff_size = bih.biWidth * bih.biHeight * 4;
  pDataBuff = (uint8*) SpiritMalloc(buff_size);
  if (!pDataBuff) {
    fprintf(stderr,"%s: cannot allocate data buffer (%lu bytes)\n",__FUNCTION__,buff_size);
    fclose(ifp); 
    return 0;
  }
  fseek(ifp,/*sizeof(BITMAPFILEHEADER) + */bfh.bfOffBits,SEEK_SET);
  if (buff_size != fread(pDataBuff,1,buff_size,ifp)) {
    fprintf(stderr,"%s: error reading RGB data\n",__FUNCTION__);
    SpiritFree(pDataBuff);
    fclose(ifp);
    return 0;
  }
  fclose(ifp);

  memset(pFrame,0,sizeof(tSpiritVideoFrame));
  pFrame->pPlane[0] = pDataBuff;
  pFrame->width = bih.biWidth;
  pFrame->height = bih.biHeight;
  pFrame->stride[0] = pFrame->width *4;
  pFrame->fmt = SPIRIT_VIDEO_FORMAT_RGB32;

  return 1;
}


//
//frees video buffer allocated by the SpiritBitmap_LoadFromFile function
SPIRIT_EXTERN_C void SpiritBitmap_FreeFrame(tSpiritVideoFrame *pFrame)
{

  if (!pFrame) return;
  if (pFrame->pPlane[0]) SpiritFree(pFrame->pPlane[0]);
  pFrame->pPlane[0] = 0;

}


