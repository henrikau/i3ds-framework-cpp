#ifndef GENERATED_ASN1SCC_Frame_H
#define GENERATED_ASN1SCC_Frame_H
/*
Code automatically generated by asn1scc tool
*/
#include "Region.h"
#include "taste-types.h"
#include "SampleAttribute.h"
#include "taste-extended.h"
#include "asn1crt.h"

#ifdef  __cplusplus
extern "C" {
#endif


typedef enum {
    mode_undefined = 0,
    mode_mono = 1,
    mode_rgb = 2,
    mode_bgr = 3,
    mode_uyvy = 4,
    mode_rgba = 5,
    mode_bayer_rggb = 6,
    mode_bayer_grbg = 7,
    mode_bayer_bggr = 8,
    mode_bayer_gbrg = 9,
    mode_pjpg = 10,
    mode_jpeg = 11,
    mode_png = 12,
    mode_tiff = 13
} Frame_mode_t;

// please use the following macros to avoid breaking code.
#define Frame_mode_t_mode_undefined mode_undefined
#define Frame_mode_t_mode_mono mode_mono
#define Frame_mode_t_mode_rgb mode_rgb
#define Frame_mode_t_mode_bgr mode_bgr
#define Frame_mode_t_mode_uyvy mode_uyvy
#define Frame_mode_t_mode_rgba mode_rgba
#define Frame_mode_t_mode_bayer_rggb mode_bayer_rggb
#define Frame_mode_t_mode_bayer_grbg mode_bayer_grbg
#define Frame_mode_t_mode_bayer_bggr mode_bayer_bggr
#define Frame_mode_t_mode_bayer_gbrg mode_bayer_gbrg
#define Frame_mode_t_mode_pjpg mode_pjpg
#define Frame_mode_t_mode_jpeg mode_jpeg
#define Frame_mode_t_mode_png mode_png
#define Frame_mode_t_mode_tiff mode_tiff

void Frame_mode_t_Initialize(Frame_mode_t* pVal);

#define ERR_FRAME_MODE_T		9909  /**/
flag Frame_mode_t_IsConstraintValid(const Frame_mode_t* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_FRAME_MODE_T		9910  /**/
#define Frame_mode_t_REQUIRED_BYTES_FOR_ENCODING       1 
#define Frame_mode_t_REQUIRED_BITS_FOR_ENCODING        4

flag Frame_mode_t_Encode(const Frame_mode_t* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_FRAME_MODE_T		9911  /**/
flag Frame_mode_t_Decode(Frame_mode_t* pVal, BitStream* pBitStrm, int* pErrCode);
/*-- FrameDescriptor --------------------------------------------*/
typedef struct {
    SampleAttributes attributes;
    Frame_mode_t frame_mode;
    T_UInt8 data_depth;
    T_UInt8 pixel_size;
    PlanarRegion region;
    T_UInt8 image_count;

} FrameDescriptor;

void FrameDescriptor_Initialize(FrameDescriptor* pVal);

#define ERR_FRAMEDESCRIPTOR		10136  /**/
#define ERR_FRAMEDESCRIPTOR_ATTRIBUTES		10026  /**/
#define ERR_FRAMEDESCRIPTOR_ATTRIBUTES_TIMESTAMP		9916  /**/
#define ERR_FRAMEDESCRIPTOR_ATTRIBUTES_VALIDITY		9931  /**/
#define ERR_FRAMEDESCRIPTOR_ATTRIBUTES_ATTRIBUTES		10019  /**/
#define ERR_FRAMEDESCRIPTOR_ATTRIBUTES_ATTRIBUTES_ELM		10008  /**/
#define ERR_FRAMEDESCRIPTOR_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_KEY		9942  /**/
#define ERR_FRAMEDESCRIPTOR_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE		9997  /**/
#define ERR_FRAMEDESCRIPTOR_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_BOOLEAN_VALUE		9953  /**/
#define ERR_FRAMEDESCRIPTOR_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_DISCRETE_VALUE		9964  /**/
#define ERR_FRAMEDESCRIPTOR_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_REAL_VALUE		9975  /**/
#define ERR_FRAMEDESCRIPTOR_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_STRING_VALUE		9986  /**/
#define ERR_FRAMEDESCRIPTOR_FRAME_MODE		10037  /**/
#define ERR_FRAMEDESCRIPTOR_DATA_DEPTH		10048  /**/
#define ERR_FRAMEDESCRIPTOR_PIXEL_SIZE		10059  /**/
#define ERR_FRAMEDESCRIPTOR_REGION		10114  /**/
#define ERR_FRAMEDESCRIPTOR_REGION_OFFSET_X		10070  /**/
#define ERR_FRAMEDESCRIPTOR_REGION_OFFSET_Y		10081  /**/
#define ERR_FRAMEDESCRIPTOR_REGION_SIZE_X		10092  /**/
#define ERR_FRAMEDESCRIPTOR_REGION_SIZE_Y		10103  /**/
#define ERR_FRAMEDESCRIPTOR_IMAGE_COUNT		10125  /**/
flag FrameDescriptor_IsConstraintValid(const FrameDescriptor* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_FRAMEDESCRIPTOR		10137  /**/
#define ERR_UPER_ENCODE_FRAMEDESCRIPTOR_ATTRIBUTES_2		10033  /**/
#define ERR_UPER_ENCODE_FRAMEDESCRIPTOR_FRAME_MODE_2		10044  /**/
#define ERR_UPER_ENCODE_FRAMEDESCRIPTOR_DATA_DEPTH_2		10055  /**/
#define ERR_UPER_ENCODE_FRAMEDESCRIPTOR_PIXEL_SIZE_2		10066  /**/
#define ERR_UPER_ENCODE_FRAMEDESCRIPTOR_REGION_2		10121  /**/
#define ERR_UPER_ENCODE_FRAMEDESCRIPTOR_IMAGE_COUNT_2		10132  /**/
#define FrameDescriptor_REQUIRED_BYTES_FOR_ENCODING       78 
#define FrameDescriptor_REQUIRED_BITS_FOR_ENCODING        617

flag FrameDescriptor_Encode(const FrameDescriptor* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_FRAMEDESCRIPTOR		10138  /**/
#define ERR_UPER_DECODE_FRAMEDESCRIPTOR_ATTRIBUTES_2		10034  /**/
#define ERR_UPER_DECODE_FRAMEDESCRIPTOR_FRAME_MODE_2		10045  /**/
#define ERR_UPER_DECODE_FRAMEDESCRIPTOR_DATA_DEPTH_2		10056  /**/
#define ERR_UPER_DECODE_FRAMEDESCRIPTOR_PIXEL_SIZE_2		10067  /**/
#define ERR_UPER_DECODE_FRAMEDESCRIPTOR_REGION_2		10122  /**/
#define ERR_UPER_DECODE_FRAMEDESCRIPTOR_IMAGE_COUNT_2		10133  /**/
flag FrameDescriptor_Decode(FrameDescriptor* pVal, BitStream* pBitStrm, int* pErrCode);

 

/* ================= Encoding/Decoding function prototypes =================
 * These functions are placed at the end of the file to make sure all types
 * have been declared first, in case of parameterized ACN encodings
 * ========================================================================= */

 


#ifdef  __cplusplus
}

#endif

#endif
