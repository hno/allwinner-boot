#ifndef  __DEC_IMG_H__
#define  __DEC_IMG_H__

#include  "../card_sprite_i.h"

//------------------------------------------------------------------------------------------------------------
// image ½âÎö½Ó¿Ú
//------------------------------------------------------------------------------------------------------------
typedef void * 		HIMAGE;

typedef void * 		HIMAGEITEM;

extern HIMAGE 		Img_Open		(__u32 start_sector, int size);

extern HIMAGEITEM 	Img_OpenItem	(HIMAGE hImage, char * MainType, char * subType);

extern __u64		Img_GetItemSize	(HIMAGE hImage, HIMAGEITEM hItem);

extern __u32 		Img_ReadItemData(HIMAGE hImage, HIMAGEITEM hItem, void * buffer, __u64 Length);

extern __u32 		Img_CloseItem	(HIMAGE hImage, HIMAGEITEM hItem);

extern void  		Img_Close		(HIMAGE hImage);


#endif   //__DEC_IMG_H__

