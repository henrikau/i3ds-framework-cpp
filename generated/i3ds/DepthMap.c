/*
Code automatically generated by asn1scc tool
*/
#include <limits.h>
#include <string.h>
#include <math.h>
#include "DepthMap.h"



void Depth_format_t_Initialize(Depth_format_t* pVal)
{

	(*(pVal)) = depth_f;
}

flag Depth_format_t_IsConstraintValid(const Depth_format_t* pVal, int* pErrCode)
{
    flag ret = TRUE;
	
    ret = ((*(pVal)) == depth_f);
    *pErrCode = ret ? 0 :  ERR_DEPTH_FORMAT_T; 

	return ret;
}

flag Depth_format_t_Encode(const Depth_format_t* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;
	ret = bCheckConstraints ? Depth_format_t_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret) {
	    switch((*(pVal))) 
	    {
	        case depth_f:   
	            BitStream_EncodeConstraintWholeNumber(pBitStrm, 0, 0, 0);
	        	break;
	        default:                    /*COVERAGE_IGNORE*/
	    	    *pErrCode = ERR_UPER_ENCODE_DEPTH_FORMAT_T; /*COVERAGE_IGNORE*/
	    	    ret = FALSE;            /*COVERAGE_IGNORE*/
	    }
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag Depth_format_t_Decode(Depth_format_t* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;

	{
	    asn1SccSint enumIndex;
	    ret = BitStream_DecodeConstraintWholeNumber(pBitStrm, &enumIndex, 0, 0);
	    *pErrCode = ret ? 0 : ERR_UPER_DECODE_DEPTH_FORMAT_T;
	    if (ret) {
	        switch(enumIndex) 
	        {
	            case 0: 
	                (*(pVal)) = depth_f;
	                break;
	            default:                        /*COVERAGE_IGNORE*/
		            *pErrCode = ERR_UPER_DECODE_DEPTH_FORMAT_T;     /*COVERAGE_IGNORE*/
		            ret = FALSE;                /*COVERAGE_IGNORE*/
	        }
	    } else {
	        (*(pVal)) = depth_f;             /*COVERAGE_IGNORE*/
	    }
	}

	return ret  && Depth_format_t_IsConstraintValid(pVal, pErrCode);
}



void DepthMapDescriptor_Initialize(DepthMapDescriptor* pVal)
{


	/*set attributes */
	SampleAttributes_Initialize((&(pVal->attributes)));
	/*set depth_format */
	Depth_format_t_Initialize((&(pVal->depth_format)));
	/*set depth_size */
	T_UInt8_Initialize((&(pVal->depth_size)));
	/*set width */
	T_UInt16_Initialize((&(pVal->width)));
	/*set height */
	T_UInt16_Initialize((&(pVal->height)));
}

flag DepthMapDescriptor_IsConstraintValid(const DepthMapDescriptor* pVal, int* pErrCode)
{
    flag ret = TRUE;
    int i1;
	
    ret = (((((pVal->attributes.validity == sample_empty)) || ((pVal->attributes.validity == sample_valid)))) || ((pVal->attributes.validity == sample_invalid)));
    *pErrCode = ret ? 0 :  ERR_DEPTHMAPDESCRIPTOR_ATTRIBUTES_VALIDITY; 
    if (ret) {
        ret = (pVal->attributes.attributes.nCount <= 4);
        *pErrCode = ret ? 0 :  ERR_DEPTHMAPDESCRIPTOR_ATTRIBUTES_ATTRIBUTES; 
        if (ret) {
            for(i1 = 0; ret && i1 < pVal->attributes.attributes.nCount; i1++) 
            {
            	ret = (pVal->attributes.attributes.arr[i1].attribute_key <= 255UL);
            	*pErrCode = ret ? 0 :  ERR_DEPTHMAPDESCRIPTOR_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_KEY; 
            	if (ret) {
            	    if (pVal->attributes.attributes.arr[i1].attribute_value.kind == real_value_PRESENT) {
            	    	ret = ((-1.79769313486231570000E+308 <= pVal->attributes.attributes.arr[i1].attribute_value.u.real_value) && (pVal->attributes.attributes.arr[i1].attribute_value.u.real_value <= 1.79769313486231570000E+308));
            	    	*pErrCode = ret ? 0 :  ERR_DEPTHMAPDESCRIPTOR_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_REAL_VALUE; 
            	    }
            	    if (ret) {
            	        if (pVal->attributes.attributes.arr[i1].attribute_value.kind == string_value_PRESENT) {
            	        	ret = (pVal->attributes.attributes.arr[i1].attribute_value.u.string_value.nCount <= 8);
            	        	*pErrCode = ret ? 0 :  ERR_DEPTHMAPDESCRIPTOR_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_STRING_VALUE; 
            	        }
            	    }
            	}
            }
        }
    }
    if (ret) {
        ret = (pVal->depth_format == depth_f);
        *pErrCode = ret ? 0 :  ERR_DEPTHMAPDESCRIPTOR_DEPTH_FORMAT; 
        if (ret) {
            ret = (pVal->depth_size <= 255UL);
            *pErrCode = ret ? 0 :  ERR_DEPTHMAPDESCRIPTOR_DEPTH_SIZE; 
            if (ret) {
                ret = (pVal->width <= 65535UL);
                *pErrCode = ret ? 0 :  ERR_DEPTHMAPDESCRIPTOR_WIDTH; 
                if (ret) {
                    ret = (pVal->height <= 65535UL);
                    *pErrCode = ret ? 0 :  ERR_DEPTHMAPDESCRIPTOR_HEIGHT; 
                }
            }
        }
    }

	return ret;
}

flag DepthMapDescriptor_Encode(const DepthMapDescriptor* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;
	ret = bCheckConstraints ? DepthMapDescriptor_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret) {
	    /*Encode attributes */
	    ret = SampleAttributes_Encode((&(pVal->attributes)), pBitStrm, pErrCode, FALSE);
	    if (ret) {
	        /*Encode depth_format */
	        ret = Depth_format_t_Encode((&(pVal->depth_format)), pBitStrm, pErrCode, FALSE);
	        if (ret) {
	            /*Encode depth_size */
	            ret = T_UInt8_Encode((&(pVal->depth_size)), pBitStrm, pErrCode, FALSE);
	            if (ret) {
	                /*Encode width */
	                ret = T_UInt16_Encode((&(pVal->width)), pBitStrm, pErrCode, FALSE);
	                if (ret) {
	                    /*Encode height */
	                    ret = T_UInt16_Encode((&(pVal->height)), pBitStrm, pErrCode, FALSE);
	                }
	            }
	        }
	    }
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag DepthMapDescriptor_Decode(DepthMapDescriptor* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;

	/*Decode attributes */
	ret = SampleAttributes_Decode((&(pVal->attributes)), pBitStrm, pErrCode);
	if (ret) {
	    /*Decode depth_format */
	    ret = Depth_format_t_Decode((&(pVal->depth_format)), pBitStrm, pErrCode);
	    if (ret) {
	        /*Decode depth_size */
	        ret = T_UInt8_Decode((&(pVal->depth_size)), pBitStrm, pErrCode);
	        if (ret) {
	            /*Decode width */
	            ret = T_UInt16_Decode((&(pVal->width)), pBitStrm, pErrCode);
	            if (ret) {
	                /*Decode height */
	                ret = T_UInt16_Decode((&(pVal->height)), pBitStrm, pErrCode);
	            }
	        }
	    }
	}

	return ret  && DepthMapDescriptor_IsConstraintValid(pVal, pErrCode);
}

