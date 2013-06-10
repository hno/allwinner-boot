/*
*******************************************************************************
*                                            Aone project ---- usb peripheral module
*
* File Name 	: encrypt.c
*
* Author 		: javen
*
* Version 		: 1.0
*
* Date 			: 2008.04.24
*
* Description 	: 加密和解密
*
* History 		:
*
********************************************************************************************************************
*/

#include    "twofish/twofish.h"
#include    "encrypt.h"

#define     CODE_KEY_LEN    32

HTF    handle = NULL;
int    encrypt_flag = 0;

/*
***************************************************************
*                    get_code_key
*
* Description:
*     获得密匙
* Parameters:
*     void
* Return value:
*     0		:   success
*     !0    :   fail
* History:
*
***************************************************************
*/
__s32  get_code_key(__u32 * key_new, __u32 keylen)
{
    __u32  i = 0;

    //暂时用明文的key，等找到好的算法以后再替换
 //   _mem_cpy(key_new, key_old, keylen);

    key_new[0] = 5;
	key_new[1] = 4;
	for(i = 2; i < keylen; i++){
		key_new[i] = key_new[i-1] + key_new[i-2];
	}

    return 0;
}

/*
***************************************************************
*                    init_code
*
* Description:
*     加密算法初始化
* Parameters:
*     void
* Return value:
*     0		:   success
*     !0    :   fail
* History:
*
***************************************************************
*/
__s32  init_code(__s32 flag)
{
    __s32  ret = 0;
	__u32  key_new[CODE_KEY_LEN];

	if(0 == flag)
	{
		encrypt_flag = 1;

		return 0;
	}
	memset((void *)key_new, 0, CODE_KEY_LEN * sizeof(__u32));
	encrypt_flag = 0;

	ret = get_code_key(key_new, CODE_KEY_LEN);
    if(ret != 0){
		__wrn("ERR: init_code, get_code_key failed\n");
		return -1;
	}

    handle = TFInitial(key_new, CODE_KEY_LEN);
	if(handle == NULL){
		__wrn("ERR: init_code, TFInitial failed\n");
		return -1;
	}

    return 0;
}

/*
***************************************************************
*                    encode
*
* Description:
*     加密
* Parameters:
*     ibuf  :  input.  originality buffer
*     obuf  :  output. cryptograph buffer
*     len   :  input.  buffer lenght
* Return value:
*     0		:   success
*     !0    :   fail
* History:
*
***************************************************************
*/
__s32  encode(void * ibuf, void * obuf, __u32 len)
{
    __wrn("ERR: encode not support\n");

    return 0;
}

/*
***************************************************************
*                    decode
*
* Description:
*     解密
* Parameters:
*     ibuf  :  input.  originality buffer
*     obuf  :  output. cryptograph buffer
*     len   :  input.  buffer lenght
* Return value:
*     0		:   success
*     !0    :   fail
* History:
*
***************************************************************
*/
//__s32  decode(void * ibuf, void * obuf, __u32 len)
//{
//	return TFDecode(handle, ibuf, len, obuf);
//}
__s32  decode(__u32 src_buf, __u32 dest_buf, __u32 len, __u32 *buf_addr)
{
	void *ibuf, *obuf;

	if(1 == encrypt_flag)
	{
		*buf_addr = src_buf;

		return 0;
	}
	ibuf = (void *)src_buf;
	obuf = (void *)dest_buf;

	TFDecode(handle, ibuf, len, obuf);
	*buf_addr = dest_buf;

	return 0;
}

/*
***************************************************************
*                    exit_code
*
* Description:
*     关闭
* Parameters:
*     void
* Return value:
*     0		:   success
*     !0    :   fail
* History:
*
***************************************************************
*/
__s32  exit_code(void)
{
    return 0;
}
