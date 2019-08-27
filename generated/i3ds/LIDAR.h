#ifndef GENERATED_ASN1SCC_LIDAR_H
#define GENERATED_ASN1SCC_LIDAR_H
/*
Code automatically generated by asn1scc tool
*/
#include "Region.h"
#include "taste-types.h"
#include "asn1crt.h"

#ifdef  __cplusplus
extern "C" {
#endif


/*-- LIDARRegion --------------------------------------------*/
typedef struct {
    T_Boolean enable;
    PolarRegion region;

} LIDARRegion;

void LIDARRegion_Initialize(LIDARRegion* pVal);

#define ERR_LIDARREGION		2352  /**/
#define ERR_LIDARREGION_ENABLE		2286  /**/
#define ERR_LIDARREGION_REGION		2341  /**/
#define ERR_LIDARREGION_REGION_OFFSET_X		2297  /**/
#define ERR_LIDARREGION_REGION_OFFSET_Y		2308  /**/
#define ERR_LIDARREGION_REGION_SIZE_X		2319  /**/
#define ERR_LIDARREGION_REGION_SIZE_Y		2330  /**/
flag LIDARRegion_IsConstraintValid(const LIDARRegion* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_LIDARREGION		2353  /**/
#define ERR_UPER_ENCODE_LIDARREGION_ENABLE_2		2293  /**/
#define ERR_UPER_ENCODE_LIDARREGION_REGION_2		2348  /**/
#define LIDARRegion_REQUIRED_BYTES_FOR_ENCODING       53 
#define LIDARRegion_REQUIRED_BITS_FOR_ENCODING        417

flag LIDARRegion_Encode(const LIDARRegion* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_LIDARREGION		2354  /**/
#define ERR_UPER_DECODE_LIDARREGION_ENABLE_2		2294  /**/
#define ERR_UPER_DECODE_LIDARREGION_REGION_2		2349  /**/
flag LIDARRegion_Decode(LIDARRegion* pVal, BitStream* pBitStrm, int* pErrCode);
/*-- LIDARConfiguration --------------------------------------------*/
typedef struct {
    T_Boolean region_enabled;
    PolarRegion region;

} LIDARConfiguration;

void LIDARConfiguration_Initialize(LIDARConfiguration* pVal);

#define ERR_LIDARCONFIGURATION		2425  /**/
#define ERR_LIDARCONFIGURATION_REGION_ENABLED		2359  /**/
#define ERR_LIDARCONFIGURATION_REGION		2414  /**/
#define ERR_LIDARCONFIGURATION_REGION_OFFSET_X		2370  /**/
#define ERR_LIDARCONFIGURATION_REGION_OFFSET_Y		2381  /**/
#define ERR_LIDARCONFIGURATION_REGION_SIZE_X		2392  /**/
#define ERR_LIDARCONFIGURATION_REGION_SIZE_Y		2403  /**/
flag LIDARConfiguration_IsConstraintValid(const LIDARConfiguration* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_LIDARCONFIGURATION		2426  /**/
#define ERR_UPER_ENCODE_LIDARCONFIGURATION_REGION_ENABLED_2		2366  /**/
#define ERR_UPER_ENCODE_LIDARCONFIGURATION_REGION_2		2421  /**/
#define LIDARConfiguration_REQUIRED_BYTES_FOR_ENCODING       53 
#define LIDARConfiguration_REQUIRED_BITS_FOR_ENCODING        417

flag LIDARConfiguration_Encode(const LIDARConfiguration* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_LIDARCONFIGURATION		2427  /**/
#define ERR_UPER_DECODE_LIDARCONFIGURATION_REGION_ENABLED_2		2367  /**/
#define ERR_UPER_DECODE_LIDARCONFIGURATION_REGION_2		2422  /**/
flag LIDARConfiguration_Decode(LIDARConfiguration* pVal, BitStream* pBitStrm, int* pErrCode);

 

/* ================= Encoding/Decoding function prototypes =================
 * These functions are placed at the end of the file to make sure all types
 * have been declared first, in case of parameterized ACN encodings
 * ========================================================================= */

 


#ifdef  __cplusplus
}

#endif

#endif
