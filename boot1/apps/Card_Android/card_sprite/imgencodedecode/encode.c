/*
* (C) Copyright 2007-2013
* Allwinner Technology Co., Ltd. <www.allwinnertech.com>
* Martin zheng <zhengjiewen@allwinnertech.com>
*
* See file CREDITS for list of people who contributed to this
* project.
*
* This program is free software; you can redistribute it and/or
* modify it under the terms of the GNU General Public License as
* published by the Free Software Foundation; either version 2 of
* the License, or (at your option) any later version.
*
* This program is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.	 See the
* GNU General Public License for more details.
*
* You should have received a copy of the GNU General Public License
* along with this program; if not, write to the Free Software
* Foundation, Inc., 59 Temple Place, Suite 330, Boston,
* MA 02111-1307 USA
*/
//------------------------------------------------------------------------------------------------------------
//
//2009-04-18 16:02:07
//
//scott
//
//encode.cpp
//------------------------------------------------------------------------------------------------------------

#ifndef __ENCODE___CPP__
#define __ENCODE___CPP__

#include "encode.h"

//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
#define w 		32			// word size in bits
#define r 		20			// based on security estimates

#define P32 	0xB7E15163	// Magic constants for key setup
#define Q32 	0x9E3779B9

// derived constants
#define bytes   (w >> 3)					// bytes per word
#define c       ((b + bytes - 1) >> 2)	    // key in words, rounded up
#define R24     ((r<<1) + 4)
#define lgw     5                       	// log2(w) -- wussed out

// Rotations
#define ROTL(x,y) (((x)<<(y&(w-1))) | ((x)>>(w-(y&(w-1)))))
#define ROTR(x,y) (((x)>>(y&(w-1))) | ((x)<<(w-(y&(w-1)))))

//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
//typedef void * HENCODE;
typedef struct tag_RC6_HANDLE{
	//unsigned int S[R24 - 1];		// Key schedule
	unsigned int S[R24];
}RC6_HANDLE_t;

//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
HENCODE Initial(void * param0, void * param1, __u32 index)
{
	unsigned char *K;
	int b;//key len

	int i, j, s, v;
	unsigned int L[(32 + bytes - 1) >> 2]; // Big enough for max b
	unsigned int A, B;
	RC6_HANDLE_t * handle;

    handle = (RC6_HANDLE_t *)wBoot_malloc(sizeof(RC6_HANDLE_t));
    if(!handle)
    {
        return NULL;
    }

	//如果用户传入密码则采用用户的密码
	if (NULL == param0)
	{
		wBoot_free(handle);
		handle = NULL;
		return NULL;
	}
	else
	{
//	    int * pint = (int *)param1;
		K = (__u8 *)param0;
//		b = *pint;
		b = *(int *)param1;
	}

	//产生密钥的种子

	L[c - 1] = 0;
	for (i = b - 1; i >= 0; i--)
		L[i >> 2] = (L[i >> 2] << 8) + K[i];

	handle->S[0] = P32;
	for (i = 1; i <= 2 * r + 3; i++)
		handle->S[i] = handle->S[i - 1] + Q32;

	A = B = i = j = 0;
	v = R24;
	if (c > v) v = c;
	v *= 3;

	for (s = 1; s <= v; s++)
	{
		A = handle->S[i] = ROTL(handle->S[i] + A + B, 3);
		B = L[j] = ROTL(L[j] + A + B, A + B);
		i = (i + 1) % R24;
		j = (j + 1) % c;
	}

	return handle;
}
//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------

#if 0

u32     Encode(HENCODE hEncode, void * ibuf, void * obuf)
{
	RC6_HANDLE_t * handle = (RC6_HANDLE_t *)hEncode;
	unsigned int *ct;
	unsigned int *pt;
	unsigned int A, B, C, D, t, u, x;
	int i;

	if (NULL == hEncode || NULL == ibuf || NULL == obuf)
		return __LINE__;

	pt = (unsigned int *)ibuf;
	ct = (unsigned int *)obuf;

	A = pt[0];
	B = pt[1];
	C = pt[2];
	D = pt[3];
	B += handle->S[0];
	D += handle->S[1];
	for (i = 2; i <= 2 * r; i += 2)
	{
		t = ROTL(B * (2 * B + 1), lgw);
		u = ROTL(D * (2 * D + 1), lgw);
		A = ROTL(A ^ t, u) + handle->S[i];
		C = ROTL(C ^ u, t) + handle->S[i + 1];
		x = A;
		A = B;
		B = C;
		C = D;
		D = x;
	}
	A += handle->S[2 * r + 2];
	C += handle->S[2 * r + 3];
	ct[0] = A;
	ct[1] = B;
	ct[2] = C;
	ct[3] = D;

	return OK;
}

#endif //__RC_ENCODE__


//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
#if 1

__u32		Decode(HENCODE hEncode, void * ibuf, void * obuf)
{
	RC6_HANDLE_t * handle = (RC6_HANDLE_t *)hEncode;

	unsigned int *ct;
	unsigned int *pt;
	unsigned int A, B, C, D, t, u, x;
	int i;


	if (NULL == hEncode || NULL == ibuf || NULL == obuf)
		return __LINE__;

	ct = (unsigned int *)ibuf;
	pt = (unsigned int *)obuf;

	A = ct[0];
	B = ct[1];
	C = ct[2];
	D = ct[3];
	C -= handle->S[(r << 1) + 3];
	A -= handle->S[(r << 1) + 2];
	for (i = 2 * r; i >= 2; i -= 2)
	{
		x = D;
		D = C;
		C = B;
		B = A;
		A = x;
		u = ROTL(D * ((D << 1) + 1), lgw);
		t = ROTL(B * ((B << 1) + 1), lgw);
		C = ROTR(C - handle->S[i + 1], t) ^ u;
		A = ROTR(A - handle->S[i],     u) ^ t;
	}
	D -= handle->S[1];
	B -= handle->S[0];
	pt[0] = A;
	pt[1] = B;
	pt[2] = C;
	pt[3] = D;

	return OK;
}
#endif //__RC_DECODE__

//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------
__u32		UnInitial(HENCODE hEncode)
{
//	RC6_HANDLE_t * handle = (RC6_HANDLE_t *)hEncode;

	if (NULL == hEncode)
		return __LINE__;

	//memset(handle, 0, sizeof(RC6_HANDLE_t));
//	free(handle);
//	handle = NULL;
	return OK;
}
//------------------------------------------------------------------------------------------------------------
//
//------------------------------------------------------------------------------------------------------------

#endif //__ENCODE___CPP__


