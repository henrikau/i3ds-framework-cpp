/*
Code automatically generated by asn1scc tool
*/
#include <limits.h>
#include <string.h>
#include <math.h>
#include "Frame.h"



void Frame_mode_t_Initialize(Frame_mode_t* pVal)
{

	(*(pVal)) = mode_undefined;
}

flag Frame_mode_t_IsConstraintValid(const Frame_mode_t* pVal, int* pErrCode)
{
    flag ret = TRUE;
	
    ret = ((((((((((((((((((((((((((((*(pVal)) == mode_undefined)) || (((*(pVal)) == mode_mono)))) || (((*(pVal)) == mode_rgb)))) || (((*(pVal)) == mode_bgr)))) || (((*(pVal)) == mode_uyvy)))) || (((*(pVal)) == mode_rgba)))) || (((*(pVal)) == mode_bayer_rggb)))) || (((*(pVal)) == mode_bayer_grbg)))) || (((*(pVal)) == mode_bayer_bggr)))) || (((*(pVal)) == mode_bayer_gbrg)))) || (((*(pVal)) == mode_pjpg)))) || (((*(pVal)) == mode_jpeg)))) || (((*(pVal)) == mode_png)))) || (((*(pVal)) == mode_tiff)));
    *pErrCode = ret ? 0 :  ERR_FRAME_MODE_T; 

	return ret;
}

flag Frame_mode_t_Encode(const Frame_mode_t* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;
	ret = bCheckConstraints ? Frame_mode_t_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret) {
	    switch((*(pVal))) 
	    {
	        case mode_undefined:   
	            BitStream_EncodeConstraintWholeNumber(pBitStrm, 0, 0, 13);
	        	break;
	        case mode_mono:   
	            BitStream_EncodeConstraintWholeNumber(pBitStrm, 1, 0, 13);
	        	break;
	        case mode_rgb:   
	            BitStream_EncodeConstraintWholeNumber(pBitStrm, 2, 0, 13);
	        	break;
	        case mode_bgr:   
	            BitStream_EncodeConstraintWholeNumber(pBitStrm, 3, 0, 13);
	        	break;
	        case mode_uyvy:   
	            BitStream_EncodeConstraintWholeNumber(pBitStrm, 4, 0, 13);
	        	break;
	        case mode_rgba:   
	            BitStream_EncodeConstraintWholeNumber(pBitStrm, 5, 0, 13);
	        	break;
	        case mode_bayer_rggb:   
	            BitStream_EncodeConstraintWholeNumber(pBitStrm, 6, 0, 13);
	        	break;
	        case mode_bayer_grbg:   
	            BitStream_EncodeConstraintWholeNumber(pBitStrm, 7, 0, 13);
	        	break;
	        case mode_bayer_bggr:   
	            BitStream_EncodeConstraintWholeNumber(pBitStrm, 8, 0, 13);
	        	break;
	        case mode_bayer_gbrg:   
	            BitStream_EncodeConstraintWholeNumber(pBitStrm, 9, 0, 13);
	        	break;
	        case mode_pjpg:   
	            BitStream_EncodeConstraintWholeNumber(pBitStrm, 10, 0, 13);
	        	break;
	        case mode_jpeg:   
	            BitStream_EncodeConstraintWholeNumber(pBitStrm, 11, 0, 13);
	        	break;
	        case mode_png:   
	            BitStream_EncodeConstraintWholeNumber(pBitStrm, 12, 0, 13);
	        	break;
	        case mode_tiff:   
	            BitStream_EncodeConstraintWholeNumber(pBitStrm, 13, 0, 13);
	        	break;
	        default:                    /*COVERAGE_IGNORE*/
	    	    *pErrCode = ERR_UPER_ENCODE_FRAME_MODE_T; /*COVERAGE_IGNORE*/
	    	    ret = FALSE;            /*COVERAGE_IGNORE*/
	    }
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag Frame_mode_t_Decode(Frame_mode_t* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;

	{
	    asn1SccSint enumIndex;
	    ret = BitStream_DecodeConstraintWholeNumber(pBitStrm, &enumIndex, 0, 13);
	    *pErrCode = ret ? 0 : ERR_UPER_DECODE_FRAME_MODE_T;
	    if (ret) {
	        switch(enumIndex) 
	        {
	            case 0: 
	                (*(pVal)) = mode_undefined;
	                break;
	            case 1: 
	                (*(pVal)) = mode_mono;
	                break;
	            case 2: 
	                (*(pVal)) = mode_rgb;
	                break;
	            case 3: 
	                (*(pVal)) = mode_bgr;
	                break;
	            case 4: 
	                (*(pVal)) = mode_uyvy;
	                break;
	            case 5: 
	                (*(pVal)) = mode_rgba;
	                break;
	            case 6: 
	                (*(pVal)) = mode_bayer_rggb;
	                break;
	            case 7: 
	                (*(pVal)) = mode_bayer_grbg;
	                break;
	            case 8: 
	                (*(pVal)) = mode_bayer_bggr;
	                break;
	            case 9: 
	                (*(pVal)) = mode_bayer_gbrg;
	                break;
	            case 10: 
	                (*(pVal)) = mode_pjpg;
	                break;
	            case 11: 
	                (*(pVal)) = mode_jpeg;
	                break;
	            case 12: 
	                (*(pVal)) = mode_png;
	                break;
	            case 13: 
	                (*(pVal)) = mode_tiff;
	                break;
	            default:                        /*COVERAGE_IGNORE*/
		            *pErrCode = ERR_UPER_DECODE_FRAME_MODE_T;     /*COVERAGE_IGNORE*/
		            ret = FALSE;                /*COVERAGE_IGNORE*/
	        }
	    } else {
	        (*(pVal)) = mode_undefined;             /*COVERAGE_IGNORE*/
	    }
	}

	return ret  && Frame_mode_t_IsConstraintValid(pVal, pErrCode);
}



void FrameDescriptor_Initialize(FrameDescriptor* pVal)
{


	/*set attributes */
	SampleAttributes_Initialize((&(pVal->attributes)));
	/*set frame_mode */
	Frame_mode_t_Initialize((&(pVal->frame_mode)));
	/*set data_depth */
	T_UInt8_Initialize((&(pVal->data_depth)));
	/*set pixel_size */
	T_UInt8_Initialize((&(pVal->pixel_size)));
	/*set region */
	PlanarRegion_Initialize((&(pVal->region)));
	/*set image_count */
	T_UInt8_Initialize((&(pVal->image_count)));
}

flag FrameDescriptor_IsConstraintValid(const FrameDescriptor* pVal, int* pErrCode)
{
    flag ret = TRUE;
    int i1;
	
    ret = (((((pVal->attributes.validity == sample_empty)) || ((pVal->attributes.validity == sample_valid)))) || ((pVal->attributes.validity == sample_invalid)));
    *pErrCode = ret ? 0 :  ERR_FRAMEDESCRIPTOR_ATTRIBUTES_VALIDITY; 
    if (ret) {
        ret = (pVal->attributes.attributes.nCount <= 4);
        *pErrCode = ret ? 0 :  ERR_FRAMEDESCRIPTOR_ATTRIBUTES_ATTRIBUTES; 
        if (ret) {
            for(i1 = 0; ret && i1 < pVal->attributes.attributes.nCount; i1++) 
            {
            	ret = (pVal->attributes.attributes.arr[i1].attribute_key <= 255UL);
            	*pErrCode = ret ? 0 :  ERR_FRAMEDESCRIPTOR_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_KEY; 
            	if (ret) {
            	    if (pVal->attributes.attributes.arr[i1].attribute_value.kind == real_value_PRESENT) {
            	    	ret = ((-1.79769313486231570000E+308 <= pVal->attributes.attributes.arr[i1].attribute_value.u.real_value) && (pVal->attributes.attributes.arr[i1].attribute_value.u.real_value <= 1.79769313486231570000E+308));
            	    	*pErrCode = ret ? 0 :  ERR_FRAMEDESCRIPTOR_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_REAL_VALUE; 
            	    }
            	    if (ret) {
            	        if (pVal->attributes.attributes.arr[i1].attribute_value.kind == string_value_PRESENT) {
            	        	ret = (pVal->attributes.attributes.arr[i1].attribute_value.u.string_value.nCount <= 8);
            	        	*pErrCode = ret ? 0 :  ERR_FRAMEDESCRIPTOR_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_STRING_VALUE; 
            	        }
            	    }
            	}
            }
        }
    }
    if (ret) {
        ret = (((((((((((((((((((((((((((pVal->frame_mode == mode_undefined)) || ((pVal->frame_mode == mode_mono)))) || ((pVal->frame_mode == mode_rgb)))) || ((pVal->frame_mode == mode_bgr)))) || ((pVal->frame_mode == mode_uyvy)))) || ((pVal->frame_mode == mode_rgba)))) || ((pVal->frame_mode == mode_bayer_rggb)))) || ((pVal->frame_mode == mode_bayer_grbg)))) || ((pVal->frame_mode == mode_bayer_bggr)))) || ((pVal->frame_mode == mode_bayer_gbrg)))) || ((pVal->frame_mode == mode_pjpg)))) || ((pVal->frame_mode == mode_jpeg)))) || ((pVal->frame_mode == mode_png)))) || ((pVal->frame_mode == mode_tiff)));
        *pErrCode = ret ? 0 :  ERR_FRAMEDESCRIPTOR_FRAME_MODE; 
        if (ret) {
            ret = (pVal->data_depth <= 255UL);
            *pErrCode = ret ? 0 :  ERR_FRAMEDESCRIPTOR_DATA_DEPTH; 
            if (ret) {
                ret = (pVal->pixel_size <= 255UL);
                *pErrCode = ret ? 0 :  ERR_FRAMEDESCRIPTOR_PIXEL_SIZE; 
                if (ret) {
                    ret = (pVal->region.offset_x <= 65535UL);
                    *pErrCode = ret ? 0 :  ERR_FRAMEDESCRIPTOR_REGION_OFFSET_X; 
                    if (ret) {
                        ret = (pVal->region.offset_y <= 65535UL);
                        *pErrCode = ret ? 0 :  ERR_FRAMEDESCRIPTOR_REGION_OFFSET_Y; 
                        if (ret) {
                            ret = (pVal->region.size_x <= 65535UL);
                            *pErrCode = ret ? 0 :  ERR_FRAMEDESCRIPTOR_REGION_SIZE_X; 
                            if (ret) {
                                ret = (pVal->region.size_y <= 65535UL);
                                *pErrCode = ret ? 0 :  ERR_FRAMEDESCRIPTOR_REGION_SIZE_Y; 
                            }
                        }
                    }
                    if (ret) {
                        ret = (pVal->image_count <= 255UL);
                        *pErrCode = ret ? 0 :  ERR_FRAMEDESCRIPTOR_IMAGE_COUNT; 
                    }
                }
            }
        }
    }

	return ret;
}

flag FrameDescriptor_Encode(const FrameDescriptor* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;
	ret = bCheckConstraints ? FrameDescriptor_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret) {
	    /*Encode attributes */
	    ret = SampleAttributes_Encode((&(pVal->attributes)), pBitStrm, pErrCode, FALSE);
	    if (ret) {
	        /*Encode frame_mode */
	        ret = Frame_mode_t_Encode((&(pVal->frame_mode)), pBitStrm, pErrCode, FALSE);
	        if (ret) {
	            /*Encode data_depth */
	            ret = T_UInt8_Encode((&(pVal->data_depth)), pBitStrm, pErrCode, FALSE);
	            if (ret) {
	                /*Encode pixel_size */
	                ret = T_UInt8_Encode((&(pVal->pixel_size)), pBitStrm, pErrCode, FALSE);
	                if (ret) {
	                    /*Encode region */
	                    ret = PlanarRegion_Encode((&(pVal->region)), pBitStrm, pErrCode, FALSE);
	                    if (ret) {
	                        /*Encode image_count */
	                        ret = T_UInt8_Encode((&(pVal->image_count)), pBitStrm, pErrCode, FALSE);
	                    }
	                }
	            }
	        }
	    }
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag FrameDescriptor_Decode(FrameDescriptor* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;

	/*Decode attributes */
	ret = SampleAttributes_Decode((&(pVal->attributes)), pBitStrm, pErrCode);
	if (ret) {
	    /*Decode frame_mode */
	    ret = Frame_mode_t_Decode((&(pVal->frame_mode)), pBitStrm, pErrCode);
	    if (ret) {
	        /*Decode data_depth */
	        ret = T_UInt8_Decode((&(pVal->data_depth)), pBitStrm, pErrCode);
	        if (ret) {
	            /*Decode pixel_size */
	            ret = T_UInt8_Decode((&(pVal->pixel_size)), pBitStrm, pErrCode);
	            if (ret) {
	                /*Decode region */
	                ret = PlanarRegion_Decode((&(pVal->region)), pBitStrm, pErrCode);
	                if (ret) {
	                    /*Decode image_count */
	                    ret = T_UInt8_Decode((&(pVal->image_count)), pBitStrm, pErrCode);
	                }
	            }
	        }
	    }
	}

	return ret  && FrameDescriptor_IsConstraintValid(pVal, pErrCode);
}

