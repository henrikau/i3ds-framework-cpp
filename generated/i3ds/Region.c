/*
Code automatically generated by asn1scc tool
*/
#include <limits.h>
#include <string.h>
#include <math.h>
#include "Region.h"



void PlanarRegion_Initialize(PlanarRegion* pVal)
{


	/*set offset_x */
	T_UInt16_Initialize((&(pVal->offset_x)));
	/*set offset_y */
	T_UInt16_Initialize((&(pVal->offset_y)));
	/*set size_x */
	T_UInt16_Initialize((&(pVal->size_x)));
	/*set size_y */
	T_UInt16_Initialize((&(pVal->size_y)));
}

flag PlanarRegion_IsConstraintValid(const PlanarRegion* pVal, int* pErrCode)
{
    flag ret = TRUE;
	
    ret = (pVal->offset_x <= 65535UL);
    *pErrCode = ret ? 0 :  ERR_PLANARREGION_OFFSET_X; 
    if (ret) {
        ret = (pVal->offset_y <= 65535UL);
        *pErrCode = ret ? 0 :  ERR_PLANARREGION_OFFSET_Y; 
        if (ret) {
            ret = (pVal->size_x <= 65535UL);
            *pErrCode = ret ? 0 :  ERR_PLANARREGION_SIZE_X; 
            if (ret) {
                ret = (pVal->size_y <= 65535UL);
                *pErrCode = ret ? 0 :  ERR_PLANARREGION_SIZE_Y; 
            }
        }
    }

	return ret;
}

flag PlanarRegion_Encode(const PlanarRegion* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;
	ret = bCheckConstraints ? PlanarRegion_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret) {
	    /*Encode offset_x */
	    ret = T_UInt16_Encode((&(pVal->offset_x)), pBitStrm, pErrCode, FALSE);
	    if (ret) {
	        /*Encode offset_y */
	        ret = T_UInt16_Encode((&(pVal->offset_y)), pBitStrm, pErrCode, FALSE);
	        if (ret) {
	            /*Encode size_x */
	            ret = T_UInt16_Encode((&(pVal->size_x)), pBitStrm, pErrCode, FALSE);
	            if (ret) {
	                /*Encode size_y */
	                ret = T_UInt16_Encode((&(pVal->size_y)), pBitStrm, pErrCode, FALSE);
	            }
	        }
	    }
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag PlanarRegion_Decode(PlanarRegion* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;

	/*Decode offset_x */
	ret = T_UInt16_Decode((&(pVal->offset_x)), pBitStrm, pErrCode);
	if (ret) {
	    /*Decode offset_y */
	    ret = T_UInt16_Decode((&(pVal->offset_y)), pBitStrm, pErrCode);
	    if (ret) {
	        /*Decode size_x */
	        ret = T_UInt16_Decode((&(pVal->size_x)), pBitStrm, pErrCode);
	        if (ret) {
	            /*Decode size_y */
	            ret = T_UInt16_Decode((&(pVal->size_y)), pBitStrm, pErrCode);
	        }
	    }
	}

	return ret  && PlanarRegion_IsConstraintValid(pVal, pErrCode);
}



void PolarRegion_Initialize(PolarRegion* pVal)
{


	/*set offset_x */
	T_Float_Initialize((&(pVal->offset_x)));
	/*set offset_y */
	T_Float_Initialize((&(pVal->offset_y)));
	/*set size_x */
	T_Float_Initialize((&(pVal->size_x)));
	/*set size_y */
	T_Float_Initialize((&(pVal->size_y)));
}

flag PolarRegion_IsConstraintValid(const PolarRegion* pVal, int* pErrCode)
{
    flag ret = TRUE;
	
    ret = ((-3.40282346600000020000E+038 <= pVal->offset_x) && (pVal->offset_x <= 3.40282346600000020000E+038));
    *pErrCode = ret ? 0 :  ERR_POLARREGION_OFFSET_X; 
    if (ret) {
        ret = ((-3.40282346600000020000E+038 <= pVal->offset_y) && (pVal->offset_y <= 3.40282346600000020000E+038));
        *pErrCode = ret ? 0 :  ERR_POLARREGION_OFFSET_Y; 
        if (ret) {
            ret = ((-3.40282346600000020000E+038 <= pVal->size_x) && (pVal->size_x <= 3.40282346600000020000E+038));
            *pErrCode = ret ? 0 :  ERR_POLARREGION_SIZE_X; 
            if (ret) {
                ret = ((-3.40282346600000020000E+038 <= pVal->size_y) && (pVal->size_y <= 3.40282346600000020000E+038));
                *pErrCode = ret ? 0 :  ERR_POLARREGION_SIZE_Y; 
            }
        }
    }

	return ret;
}

flag PolarRegion_Encode(const PolarRegion* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;
	ret = bCheckConstraints ? PolarRegion_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret) {
	    /*Encode offset_x */
	    ret = T_Float_Encode((&(pVal->offset_x)), pBitStrm, pErrCode, FALSE);
	    if (ret) {
	        /*Encode offset_y */
	        ret = T_Float_Encode((&(pVal->offset_y)), pBitStrm, pErrCode, FALSE);
	        if (ret) {
	            /*Encode size_x */
	            ret = T_Float_Encode((&(pVal->size_x)), pBitStrm, pErrCode, FALSE);
	            if (ret) {
	                /*Encode size_y */
	                ret = T_Float_Encode((&(pVal->size_y)), pBitStrm, pErrCode, FALSE);
	            }
	        }
	    }
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag PolarRegion_Decode(PolarRegion* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;

	/*Decode offset_x */
	ret = T_Float_Decode((&(pVal->offset_x)), pBitStrm, pErrCode);
	if (ret) {
	    /*Decode offset_y */
	    ret = T_Float_Decode((&(pVal->offset_y)), pBitStrm, pErrCode);
	    if (ret) {
	        /*Decode size_x */
	        ret = T_Float_Decode((&(pVal->size_x)), pBitStrm, pErrCode);
	        if (ret) {
	            /*Decode size_y */
	            ret = T_Float_Decode((&(pVal->size_y)), pBitStrm, pErrCode);
	        }
	    }
	}

	return ret  && PolarRegion_IsConstraintValid(pVal, pErrCode);
}

