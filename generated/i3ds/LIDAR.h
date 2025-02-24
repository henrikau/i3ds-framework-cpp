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

#define ERR_LIDARREGION		10209  /**/
#define ERR_LIDARREGION_ENABLE		10143  /**/
#define ERR_LIDARREGION_REGION		10198  /**/
#define ERR_LIDARREGION_REGION_OFFSET_X		10154  /**/
#define ERR_LIDARREGION_REGION_OFFSET_Y		10165  /**/
#define ERR_LIDARREGION_REGION_SIZE_X		10176  /**/
#define ERR_LIDARREGION_REGION_SIZE_Y		10187  /**/
flag LIDARRegion_IsConstraintValid(const LIDARRegion* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_LIDARREGION		10210  /**/
#define ERR_UPER_ENCODE_LIDARREGION_ENABLE_2		10150  /**/
#define ERR_UPER_ENCODE_LIDARREGION_REGION_2		10205  /**/
#define LIDARRegion_REQUIRED_BYTES_FOR_ENCODING       53 
#define LIDARRegion_REQUIRED_BITS_FOR_ENCODING        417

flag LIDARRegion_Encode(const LIDARRegion* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_LIDARREGION		10211  /**/
#define ERR_UPER_DECODE_LIDARREGION_ENABLE_2		10151  /**/
#define ERR_UPER_DECODE_LIDARREGION_REGION_2		10206  /**/
flag LIDARRegion_Decode(LIDARRegion* pVal, BitStream* pBitStrm, int* pErrCode);
/*-- LIDARConfiguration --------------------------------------------*/
typedef struct {
    T_Boolean region_enabled;
    PolarRegion region;

} LIDARConfiguration;

void LIDARConfiguration_Initialize(LIDARConfiguration* pVal);

#define ERR_LIDARCONFIGURATION		10282  /**/
#define ERR_LIDARCONFIGURATION_REGION_ENABLED		10216  /**/
#define ERR_LIDARCONFIGURATION_REGION		10271  /**/
#define ERR_LIDARCONFIGURATION_REGION_OFFSET_X		10227  /**/
#define ERR_LIDARCONFIGURATION_REGION_OFFSET_Y		10238  /**/
#define ERR_LIDARCONFIGURATION_REGION_SIZE_X		10249  /**/
#define ERR_LIDARCONFIGURATION_REGION_SIZE_Y		10260  /**/
flag LIDARConfiguration_IsConstraintValid(const LIDARConfiguration* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_LIDARCONFIGURATION		10283  /**/
#define ERR_UPER_ENCODE_LIDARCONFIGURATION_REGION_ENABLED_2		10223  /**/
#define ERR_UPER_ENCODE_LIDARCONFIGURATION_REGION_2		10278  /**/
#define LIDARConfiguration_REQUIRED_BYTES_FOR_ENCODING       53 
#define LIDARConfiguration_REQUIRED_BITS_FOR_ENCODING        417

flag LIDARConfiguration_Encode(const LIDARConfiguration* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_LIDARCONFIGURATION		10284  /**/
#define ERR_UPER_DECODE_LIDARCONFIGURATION_REGION_ENABLED_2		10224  /**/
#define ERR_UPER_DECODE_LIDARCONFIGURATION_REGION_2		10279  /**/
flag LIDARConfiguration_Decode(LIDARConfiguration* pVal, BitStream* pBitStrm, int* pErrCode);

 

/* ================= Encoding/Decoding function prototypes =================
 * These functions are placed at the end of the file to make sure all types
 * have been declared first, in case of parameterized ACN encodings
 * ========================================================================= */

 


#ifdef  __cplusplus
}

#endif

#endif
