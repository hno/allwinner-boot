#ifndef __WBOOT_MP_H__
#define __WBOOT_MP_H__

#ifndef __BSP_DRV_MP_H__
#define __BSP_DRV_MP_H__

typedef enum
{
	MP_WIN_OP_NOP = 0,
	MP_WIN_OP_MIRROR_LR = 1,
	MP_WIN_OP_MIRROR_UD = 2,
	MP_WIN_OP_ROTATE_180 = 3,
	MP_WIN_OP_MIRROR_135_DEGREE = 4,
	MP_WIN_OP_ROTATE_270 = 5,
	MP_WIN_OP_ROTATE_90 = 6,
	MP_WIN_OP_MIRROR_45_DEGREE = 7,
}__mp_win_op_t;

typedef enum
{
	MP_ROP_AND = 0,		//Logical And by bit
	MP_ROP_OR = 4, 		//Logical Or by bit
	MP_ROP_ALPHA = 14, 
	MP_ROP_COLORKEY = 15, 
}__mp_layers_op_t;

typedef enum
{
	MP_LAYER_OP_INVERT = 0,
	MP_LAYER_OP_FILL   = 1,
	MP_LAYER_OP_NOP    = 5,
}__mp_layer_op_t;

typedef enum
{
	MP_PIXEL_MONO_1BPP=0,
	MP_PIXEL_MONO_2BPP,
	MP_PIXEL_MONO_4BPP,
	MP_PIXEL_MONO_8BPP,
	MP_PIXEL_COLOR_RGB655,
	MP_PIXEL_COLOR_RGB565,
	MP_PIXEL_COLOR_RGB556,
	MP_PIXEL_COLOR_ARGB1555,
	MP_PIXEL_COLOR_ARGB8888=9,
}__MP_pixel_rgbfmt_t;

typedef enum
{
	MP_OUT_ALPHA_0XFF = 0,
	MP_OUT_ALPHA_FORMULA = 1,//layer0_alpha + layer1_alpha*(1-layer0_alpha)
	MP_OUT_ALPHA_LAYER0_ALPHA = 2,
	MP_OUT_ALPHA_LAYER1_ALPHA = 3,	
}__MP_output_alpha_mode_t;

typedef enum
{
	MP_IN_ALPHA_PIXEL_ALPHA = 1,
	MP_IN_ALPHA_GLOBAL_ALPHA = 2,
	MP_IN_ALPHA_PIXEL_MULTIPLY_GLOBAL_ALPHA = 3,
}__MP_in_alpha_mode_t;

typedef enum
{
	MP_MODE_SRC_TO_DST = 0,
	MP_MODE_SRC_OP_DST = 1,
	MP_MODE_SRC0_OP_SRC1_TO_DST = 2,
}__mp_mix_mode_t;

typedef struct
{
	__u8 alpha;
	__u8 red;
	__u8 green;
	__u8 blue;
}__mp_color_t;// 32-bit (ARGB) color

typedef struct
{
	__s32 x; 
	__s32 y; 
	__u32 width; 
	__u32 height;
}__mp_rect_t;          //rect attrib

typedef struct
{
	__MP_pixel_rgbfmt_t pixelfmt;
	__u32 addr[3];
	__u32 line_width;
	__u8 byte_seq;//0:argb; 1:bgra

	__u8 alpha;
	__MP_in_alpha_mode_t in_alpha_mode;

	__mp_rect_t src_rect;	//framebuffer中需要blit的偏移和大小
	__mp_rect_t dest_rect;	//only used x,y temply
	
	__mp_win_op_t 	win_op;	//mirror/ rotate
	__mp_layer_op_t layer_op;	//invert/ fill/ complement/ condition fill/ shift/ nop
	__mp_color_t 	fill_color;
}__mp_layer_t;

typedef struct
{
	__u32 addr[3];
	__u32 line_width;
	__mp_rect_t window_rect;
}__mp_dest_t;

typedef struct
{
	__u32 red_ctrl;		//0:in area; 1:out area
	__u32 green_ctrl; 	//0:in area; 1:out area
	__u32 blue_ctrl; 	//0:in area; 1:out area
	__mp_color_t min_color;
	__mp_color_t max_color;
	__u8 ky_type;	//0:source color key; 1:destination color key
}__mp_ck_ctrl_t;

typedef struct
{
//must
	__mp_mix_mode_t mix_mode;//0:layer0->dst, 1:layer0 op dst->dst, 2:layer0 op layer1 -> dst
	__mp_layer_t layer0;
	__mp_dest_t  dest;

//only used when mix_mode is  MP_MODE_SRC0_OP_SRC1_TO_DST
	__mp_layer_t layer1;

//only used when mix_mode is MP_MODE_SRC_OP_DST or MP_MODE_SRC0_OP_SRC1_TO_DST
	__mp_layers_op_t layers_op;

//only used when alpha blending
	__u32 img_area_out_alpha_ctl;//0:A0/A1/A2 single area control ; 1:using the img_area_out_alpha_value
	__u32 img_area_out_alpha_value;
	__MP_output_alpha_mode_t A0_out_alpha_mode;//MP_OUT_ALPHA_LAYER0_ALPHA or MP_OUT_ALPHA_VALUE
	__MP_output_alpha_mode_t A1_out_alpha_mode;//MP_OUT_ALPHA_LAYER1_ALPHA or MP_OUT_ALPHA_VALUE
	__MP_output_alpha_mode_t A2_out_alpha_mode;//all
	
	__u32 non_img_area_out_alpha_value;

//only used when ck is enable
	__mp_ck_ctrl_t ck_ctrl;
}__mp_mix_t;

typedef struct
{
	__u32 				layer_addr[3];
	__u32 				layer_line_width;
	__MP_pixel_rgbfmt_t	layer_pixelfmt;
	__u8				layer_byte_seq;
	__mp_rect_t			layer_src_rect;	//framebuffer中需要blit的偏移和大小
	__mp_rect_t			layer_dest_rect;	//only use x,y temply

	__u32 				dst_addr[3];
	__u32 				dst_line_width;
	__mp_rect_t			dst_window_rect;

	__mp_win_op_t 		win_op;	//mirror/ rotate
}__mp_rotate_mirror_t;


typedef struct
{
	__u32 				layer_addr[3];
	__u32 				layer_line_width;
	__MP_pixel_rgbfmt_t	layer_pixelfmt;
	__u8 				layer_byte_seq;
	__mp_rect_t 		layer_src_rect;	//framebuffer中需要blit的偏移和大小
	__mp_rect_t 		layer_dest_rect;	//only use x,y temply

	__u32 				dst_addr[3];
	__u32 				dst_line_width;
	__mp_rect_t			dst_window_rect;
}__mp_copy_t;


typedef struct
{
	__u32 			dst_addr[3];
	__u32 			dst_line_width;
	__mp_rect_t		dst_window_rect;

	__mp_color_t 	fill_color;
}__mp_fill_t;


typedef struct
{
	__u32 				layer_addr[3];
	__u32 				layer_line_width;
	__MP_pixel_rgbfmt_t	layer_pixelfmt;
	__u8 				layer_byte_seq;
	__mp_rect_t 		layer_src_rect;	//framebuffer中需要blit的偏移和大小
	__mp_rect_t 		layer_dest_rect;	//only use x,y temply

	__u32 				dst_addr[3];
	__u32 				dst_line_width;
	__mp_rect_t			dst_window_rect;

	__u32 						img_area_out_alpha_ctl;//0:A0/A1/A2 single area control ; 1:using the img_area_out_alpha_value
	__u32 						img_area_out_alpha_value;
	__MP_output_alpha_mode_t	A0_out_alpha_mode;//MP_OUT_ALPHA_LAYER0_ALPHA or MP_OUT_ALPHA_VALUE
	__MP_output_alpha_mode_t 	A1_out_alpha_mode;//MP_OUT_ALPHA_LAYER1_ALPHA or MP_OUT_ALPHA_VALUE
	__MP_output_alpha_mode_t 	A2_out_alpha_mode;//all
}__mp_alpha_blend_t;

#endif



typedef enum
{
	MP_CMD_REQUEST =	0x0,
	MP_CMD_RELEASE =	0x1,
	MP_CMD_EXCUSE	= 0x2,
}__mp_cmd_t;

#endif


