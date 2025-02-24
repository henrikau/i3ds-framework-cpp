#ifndef GENERATED_ASN1SCC_StarTracker_H
#define GENERATED_ASN1SCC_StarTracker_H
/*
Code automatically generated by asn1scc tool
*/
#include "base.h"
#include "SampleAttribute.h"
#include "asn1crt.h"

#ifdef  __cplusplus
extern "C" {
#endif


/*-- StarTrackerMeasurement --------------------------------------------*/
typedef struct {
    SampleAttributes attributes;
    Wrappers_Quaterniond position;

} StarTrackerMeasurement;

void StarTrackerMeasurement_Initialize(StarTrackerMeasurement* pVal);

#define ERR_STARTRACKERMEASUREMENT		11226  /**/
#define ERR_STARTRACKERMEASUREMENT_ATTRIBUTES		11175  /**/
#define ERR_STARTRACKERMEASUREMENT_ATTRIBUTES_TIMESTAMP		11065  /**/
#define ERR_STARTRACKERMEASUREMENT_ATTRIBUTES_VALIDITY		11080  /**/
#define ERR_STARTRACKERMEASUREMENT_ATTRIBUTES_ATTRIBUTES		11168  /**/
#define ERR_STARTRACKERMEASUREMENT_ATTRIBUTES_ATTRIBUTES_ELM		11157  /**/
#define ERR_STARTRACKERMEASUREMENT_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_KEY		11091  /**/
#define ERR_STARTRACKERMEASUREMENT_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE		11146  /**/
#define ERR_STARTRACKERMEASUREMENT_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_BOOLEAN_VALUE		11102  /**/
#define ERR_STARTRACKERMEASUREMENT_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_DISCRETE_VALUE		11113  /**/
#define ERR_STARTRACKERMEASUREMENT_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_REAL_VALUE		11124  /**/
#define ERR_STARTRACKERMEASUREMENT_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_STRING_VALUE		11135  /**/
#define ERR_STARTRACKERMEASUREMENT_POSITION		11215  /**/
#define ERR_STARTRACKERMEASUREMENT_POSITION_IM		11197  /**/
#define ERR_STARTRACKERMEASUREMENT_POSITION_IM_ELM		11186  /**/
#define ERR_STARTRACKERMEASUREMENT_POSITION_RE		11204  /**/
flag StarTrackerMeasurement_IsConstraintValid(const StarTrackerMeasurement* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_STARTRACKERMEASUREMENT		11227  /**/
#define ERR_UPER_ENCODE_STARTRACKERMEASUREMENT_ATTRIBUTES_2		11182  /**/
#define ERR_UPER_ENCODE_STARTRACKERMEASUREMENT_POSITION_2		11222  /**/
#define StarTrackerMeasurement_REQUIRED_BYTES_FOR_ENCODING       118 
#define StarTrackerMeasurement_REQUIRED_BITS_FOR_ENCODING        943

flag StarTrackerMeasurement_Encode(const StarTrackerMeasurement* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_STARTRACKERMEASUREMENT		11228  /**/
#define ERR_UPER_DECODE_STARTRACKERMEASUREMENT_ATTRIBUTES_2		11183  /**/
#define ERR_UPER_DECODE_STARTRACKERMEASUREMENT_POSITION_2		11223  /**/
flag StarTrackerMeasurement_Decode(StarTrackerMeasurement* pVal, BitStream* pBitStrm, int* pErrCode);

 

/* ================= Encoding/Decoding function prototypes =================
 * These functions are placed at the end of the file to make sure all types
 * have been declared first, in case of parameterized ACN encodings
 * ========================================================================= */

 


#ifdef  __cplusplus
}

#endif

#endif
