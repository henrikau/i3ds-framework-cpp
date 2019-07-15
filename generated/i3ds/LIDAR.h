#ifndef GENERATED_ASN1SCC_LIDAR_H
#define GENERATED_ASN1SCC_LIDAR_H
/*
Code automatically generated by asn1scc tool
*/
#include "Point.h"
#include "SampleAttribute.h"
#include "Region.h"
#include "taste-types.h"
#include "asn1crt.h"

#ifdef  __cplusplus
extern "C" {
#endif


/*-- LIDARMeasurement750K --------------------------------------------*/


typedef struct {
    int nCount; 
    
    Vector3d arr[750000];
} LIDARMeasurement750K_points;
typedef struct {
    SampleAttributes attributes;
    LIDARMeasurement750K_points points;
    PolarRegion region;

} LIDARMeasurement750K;

void LIDARMeasurement750K_points_Initialize(LIDARMeasurement750K_points* pVal);
void LIDARMeasurement750K_Initialize(LIDARMeasurement750K* pVal);

#define ERR_LIDARMEASUREMENT750K		2308  /**/
#define ERR_LIDARMEASUREMENT750K_ATTRIBUTES		2209  /**/
#define ERR_LIDARMEASUREMENT750K_ATTRIBUTES_TIMESTAMP		2099  /**/
#define ERR_LIDARMEASUREMENT750K_ATTRIBUTES_VALIDITY		2114  /**/
#define ERR_LIDARMEASUREMENT750K_ATTRIBUTES_ATTRIBUTES		2202  /**/
#define ERR_LIDARMEASUREMENT750K_ATTRIBUTES_ATTRIBUTES_ELM		2191  /**/
#define ERR_LIDARMEASUREMENT750K_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_KEY		2125  /**/
#define ERR_LIDARMEASUREMENT750K_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE		2180  /**/
#define ERR_LIDARMEASUREMENT750K_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_BOOLEAN_VALUE		2136  /**/
#define ERR_LIDARMEASUREMENT750K_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_DISCRETE_VALUE		2147  /**/
#define ERR_LIDARMEASUREMENT750K_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_REAL_VALUE		2158  /**/
#define ERR_LIDARMEASUREMENT750K_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_STRING_VALUE		2169  /**/
#define ERR_LIDARMEASUREMENT750K_POINTS		2246  /**/
#define ERR_LIDARMEASUREMENT750K_POINTS_ELM		2231  /**/
#define ERR_LIDARMEASUREMENT750K_POINTS_ELM_ELM		2220  /**/
#define ERR_LIDARMEASUREMENT750K_REGION		2297  /**/
#define ERR_LIDARMEASUREMENT750K_REGION_OFFSET_X		2253  /**/
#define ERR_LIDARMEASUREMENT750K_REGION_OFFSET_Y		2264  /**/
#define ERR_LIDARMEASUREMENT750K_REGION_SIZE_X		2275  /**/
#define ERR_LIDARMEASUREMENT750K_REGION_SIZE_Y		2286  /**/
flag LIDARMeasurement750K_IsConstraintValid(const LIDARMeasurement750K* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_LIDARMEASUREMENT750K		2309  /**/
#define ERR_UPER_ENCODE_LIDARMEASUREMENT750K_ATTRIBUTES_2		2216  /**/
#define ERR_UPER_ENCODE_LIDARMEASUREMENT750K_POINTS		2247  /**/
#define ERR_UPER_ENCODE_LIDARMEASUREMENT750K_POINTS_ELM_2_2		2242  /**/
#define ERR_UPER_ENCODE_LIDARMEASUREMENT750K_REGION_2		2304  /**/
#define LIDARMeasurement750K_REQUIRED_BYTES_FOR_ENCODING       29437632 
#define LIDARMeasurement750K_REQUIRED_BITS_FOR_ENCODING        235501053

flag LIDARMeasurement750K_Encode(const LIDARMeasurement750K* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_LIDARMEASUREMENT750K		2310  /**/
#define ERR_UPER_DECODE_LIDARMEASUREMENT750K_ATTRIBUTES_2		2217  /**/
#define ERR_UPER_DECODE_LIDARMEASUREMENT750K_POINTS		2248  /**/
#define ERR_UPER_DECODE_LIDARMEASUREMENT750K_POINTS_ELM_2_2		2243  /**/
#define ERR_UPER_DECODE_LIDARMEASUREMENT750K_REGION_2		2305  /**/
flag LIDARMeasurement750K_Decode(LIDARMeasurement750K* pVal, BitStream* pBitStrm, int* pErrCode);
/*-- LIDARRegion --------------------------------------------*/
typedef struct {
    T_Boolean enable;
    PolarRegion region;

} LIDARRegion;

void LIDARRegion_Initialize(LIDARRegion* pVal);

#define ERR_LIDARREGION		2019  /**/
#define ERR_LIDARREGION_ENABLE		1953  /**/
#define ERR_LIDARREGION_REGION		2008  /**/
#define ERR_LIDARREGION_REGION_OFFSET_X		1964  /**/
#define ERR_LIDARREGION_REGION_OFFSET_Y		1975  /**/
#define ERR_LIDARREGION_REGION_SIZE_X		1986  /**/
#define ERR_LIDARREGION_REGION_SIZE_Y		1997  /**/
flag LIDARRegion_IsConstraintValid(const LIDARRegion* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_LIDARREGION		2020  /**/
#define ERR_UPER_ENCODE_LIDARREGION_ENABLE_2		1960  /**/
#define ERR_UPER_ENCODE_LIDARREGION_REGION_2		2015  /**/
#define LIDARRegion_REQUIRED_BYTES_FOR_ENCODING       53 
#define LIDARRegion_REQUIRED_BITS_FOR_ENCODING        417

flag LIDARRegion_Encode(const LIDARRegion* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_LIDARREGION		2021  /**/
#define ERR_UPER_DECODE_LIDARREGION_ENABLE_2		1961  /**/
#define ERR_UPER_DECODE_LIDARREGION_REGION_2		2016  /**/
flag LIDARRegion_Decode(LIDARRegion* pVal, BitStream* pBitStrm, int* pErrCode);
/*-- LIDARConfiguration --------------------------------------------*/
typedef struct {
    T_Boolean region_enabled;
    PolarRegion region;

} LIDARConfiguration;

void LIDARConfiguration_Initialize(LIDARConfiguration* pVal);

#define ERR_LIDARCONFIGURATION		2092  /**/
#define ERR_LIDARCONFIGURATION_REGION_ENABLED		2026  /**/
#define ERR_LIDARCONFIGURATION_REGION		2081  /**/
#define ERR_LIDARCONFIGURATION_REGION_OFFSET_X		2037  /**/
#define ERR_LIDARCONFIGURATION_REGION_OFFSET_Y		2048  /**/
#define ERR_LIDARCONFIGURATION_REGION_SIZE_X		2059  /**/
#define ERR_LIDARCONFIGURATION_REGION_SIZE_Y		2070  /**/
flag LIDARConfiguration_IsConstraintValid(const LIDARConfiguration* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_LIDARCONFIGURATION		2093  /**/
#define ERR_UPER_ENCODE_LIDARCONFIGURATION_REGION_ENABLED_2		2033  /**/
#define ERR_UPER_ENCODE_LIDARCONFIGURATION_REGION_2		2088  /**/
#define LIDARConfiguration_REQUIRED_BYTES_FOR_ENCODING       53 
#define LIDARConfiguration_REQUIRED_BITS_FOR_ENCODING        417

flag LIDARConfiguration_Encode(const LIDARConfiguration* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_LIDARCONFIGURATION		2094  /**/
#define ERR_UPER_DECODE_LIDARCONFIGURATION_REGION_ENABLED_2		2034  /**/
#define ERR_UPER_DECODE_LIDARCONFIGURATION_REGION_2		2089  /**/
flag LIDARConfiguration_Decode(LIDARConfiguration* pVal, BitStream* pBitStrm, int* pErrCode);

 

/* ================= Encoding/Decoding function prototypes =================
 * These functions are placed at the end of the file to make sure all types
 * have been declared first, in case of parameterized ACN encodings
 * ========================================================================= */

 


#ifdef  __cplusplus
}

#endif

#endif
