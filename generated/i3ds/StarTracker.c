/*
Code automatically generated by asn1scc tool
*/
#include <limits.h>
#include <string.h>
#include <math.h>
#include "StarTracker.h"



void StarTrackerMeasurement_Initialize(StarTrackerMeasurement* pVal)
{


	/*set attributes */
	SampleAttributes_Initialize((&(pVal->attributes)));
	/*set position */
	Quaterniond_Initialize((&(pVal->position)));
}

flag StarTrackerMeasurement_IsConstraintValid(const StarTrackerMeasurement* pVal, int* pErrCode)
{
    flag ret = TRUE;
    int i1;
	
    ret = (-9223372036854775807LL <= pVal->attributes.timestamp);
    *pErrCode = ret ? 0 :  ERR_STARTRACKERMEASUREMENT_ATTRIBUTES_TIMESTAMP; 
    if (ret) {
        ret = (((((pVal->attributes.validity == sample_empty)) || ((pVal->attributes.validity == sample_valid)))) || ((pVal->attributes.validity == sample_invalid)));
        *pErrCode = ret ? 0 :  ERR_STARTRACKERMEASUREMENT_ATTRIBUTES_VALIDITY; 
        if (ret) {
            ret = (pVal->attributes.attributes.nCount <= 4);
            *pErrCode = ret ? 0 :  ERR_STARTRACKERMEASUREMENT_ATTRIBUTES_ATTRIBUTES; 
            if (ret) {
                for(i1 = 0; ret && i1 < pVal->attributes.attributes.nCount; i1++) 
                {
                	ret = (pVal->attributes.attributes.arr[i1].attribute_key <= 255UL);
                	*pErrCode = ret ? 0 :  ERR_STARTRACKERMEASUREMENT_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_KEY; 
                	if (ret) {
                	    if (pVal->attributes.attributes.arr[i1].attribute_value.kind == discrete_value_PRESENT) {
                	    	ret = (-9223372036854775807LL <= pVal->attributes.attributes.arr[i1].attribute_value.u.discrete_value);
                	    	*pErrCode = ret ? 0 :  ERR_STARTRACKERMEASUREMENT_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_DISCRETE_VALUE; 
                	    }
                	    if (ret) {
                	        if (pVal->attributes.attributes.arr[i1].attribute_value.kind == real_value_PRESENT) {
                	        	ret = ((-1.79769313486231570000E+308 <= pVal->attributes.attributes.arr[i1].attribute_value.u.real_value) && (pVal->attributes.attributes.arr[i1].attribute_value.u.real_value <= 1.79769313486231570000E+308));
                	        	*pErrCode = ret ? 0 :  ERR_STARTRACKERMEASUREMENT_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_REAL_VALUE; 
                	        }
                	        if (ret) {
                	            if (pVal->attributes.attributes.arr[i1].attribute_value.kind == string_value_PRESENT) {
                	            	ret = (pVal->attributes.attributes.arr[i1].attribute_value.u.string_value.nCount <= 8);
                	            	*pErrCode = ret ? 0 :  ERR_STARTRACKERMEASUREMENT_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_STRING_VALUE; 
                	            }
                	        }
                	    }
                	}
                }
            }
        }
    }
    if (ret) {
        ret = ((1 <= pVal->position.nCount) && (pVal->position.nCount <= 4));
        *pErrCode = ret ? 0 :  ERR_STARTRACKERMEASUREMENT_POSITION; 
        if (ret) {
            for(i1 = 0; ret && i1 < pVal->position.nCount; i1++) 
            {
            	ret = ((-1.79769313486231570000E+308 <= pVal->position.arr[i1]) && (pVal->position.arr[i1] <= 1.79769313486231570000E+308));
            	*pErrCode = ret ? 0 :  ERR_STARTRACKERMEASUREMENT_POSITION_ELM; 
            }
        }
    }

	return ret;
}

flag StarTrackerMeasurement_Encode(const StarTrackerMeasurement* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;
	ret = bCheckConstraints ? StarTrackerMeasurement_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret) {
	    /*Encode attributes */
	    ret = SampleAttributes_Encode((&(pVal->attributes)), pBitStrm, pErrCode, FALSE);
	    if (ret) {
	        /*Encode position */
	        ret = Quaterniond_Encode((&(pVal->position)), pBitStrm, pErrCode, FALSE);
	    }
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag StarTrackerMeasurement_Decode(StarTrackerMeasurement* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;

	/*Decode attributes */
	ret = SampleAttributes_Decode((&(pVal->attributes)), pBitStrm, pErrCode);
	if (ret) {
	    /*Decode position */
	    ret = Quaterniond_Decode((&(pVal->position)), pBitStrm, pErrCode);
	}

	return ret  && StarTrackerMeasurement_IsConstraintValid(pVal, pErrCode);
}

