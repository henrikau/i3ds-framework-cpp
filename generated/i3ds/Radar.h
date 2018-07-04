#ifndef GENERATED_ASN1SCC_Radar_H
#define GENERATED_ASN1SCC_Radar_H
/*
Code automatically generated by asn1scc tool
*/
#include "taste-extended.h"
#include "taste-types.h"
#include "DepthMap.h"
#include "Region.h"
#include "SampleAttribute.h"
#include "asn1crt.h"

#ifdef  __cplusplus
extern "C" {
#endif



typedef struct
{
  int nCount;

  Depth_validity_t arr[400000];
} RadarMeasurement400K_validity;

#define RadarMeasurement400K_validity_REQUIRED_BYTES_FOR_ENCODING       100009
#define RadarMeasurement400K_validity_REQUIRED_BITS_FOR_ENCODING        800072
#define RadarMeasurement400K_validity_REQUIRED_BYTES_FOR_ACN_ENCODING   100003
#define RadarMeasurement400K_validity_REQUIRED_BITS_FOR_ACN_ENCODING    800019
#define RadarMeasurement400K_validity_REQUIRED_BYTES_FOR_XER_ENCODING   12000063

void RadarMeasurement400K_validity_Initialize(RadarMeasurement400K_validity* pVal);
flag RadarMeasurement400K_validity_IsConstraintValid(const RadarMeasurement400K_validity* val, int* pErrCode);
#ifndef ERR_RadarMeasurement400K_validity
#define ERR_RadarMeasurement400K_validity		1001  /*(SIZE(1 .. 400000))*/
#endif

typedef struct
{
  int nCount;

  T_Float arr[400000];
} RadarMeasurement400K_distances;

#define RadarMeasurement400K_distances_REQUIRED_BYTES_FOR_ENCODING       5200009
#define RadarMeasurement400K_distances_REQUIRED_BITS_FOR_ENCODING        41600072
#define RadarMeasurement400K_distances_REQUIRED_BYTES_FOR_ACN_ENCODING   5200003
#define RadarMeasurement400K_distances_REQUIRED_BITS_FOR_ACN_ENCODING    41600019
#define RadarMeasurement400K_distances_REQUIRED_BYTES_FOR_XER_ENCODING   27600065

void RadarMeasurement400K_distances_Initialize(RadarMeasurement400K_distances* pVal);
flag RadarMeasurement400K_distances_IsConstraintValid(const RadarMeasurement400K_distances* val, int* pErrCode);
#ifndef ERR_RadarMeasurement400K_distances
#define ERR_RadarMeasurement400K_distances		1002  /*(SIZE(1 .. 400000))*/
#endif

typedef struct
{
  SampleAttributes attributes;
  RadarMeasurement400K_distances distances;
  RadarMeasurement400K_validity validity;
  PlanarRegion region;
} RadarMeasurement400K;

#define RadarMeasurement400K_REQUIRED_BYTES_FOR_ENCODING       5300093
#define RadarMeasurement400K_REQUIRED_BITS_FOR_ENCODING        42400737
#define RadarMeasurement400K_REQUIRED_BYTES_FOR_ACN_ENCODING   5300079
#define RadarMeasurement400K_REQUIRED_BITS_FOR_ACN_ENCODING    42400631
#define RadarMeasurement400K_REQUIRED_BYTES_FOR_XER_ENCODING   39601211

void RadarMeasurement400K_Initialize(RadarMeasurement400K* pVal);
flag RadarMeasurement400K_IsConstraintValid(const RadarMeasurement400K* val, int* pErrCode);


typedef struct
{
  T_Boolean enable;
  PlanarRegion region;
} RadarRegion;

#define RadarRegion_REQUIRED_BYTES_FOR_ENCODING       9
#define RadarRegion_REQUIRED_BITS_FOR_ENCODING        65
#define RadarRegion_REQUIRED_BYTES_FOR_ACN_ENCODING   9
#define RadarRegion_REQUIRED_BITS_FOR_ACN_ENCODING    65
#define RadarRegion_REQUIRED_BYTES_FOR_XER_ENCODING   230

void RadarRegion_Initialize(RadarRegion* pVal);
flag RadarRegion_IsConstraintValid(const RadarRegion* val, int* pErrCode);


typedef struct
{
  T_Boolean region_enabled;
  PlanarRegion region;
} RadarConfiguration;

#define RadarConfiguration_REQUIRED_BYTES_FOR_ENCODING       9
#define RadarConfiguration_REQUIRED_BITS_FOR_ENCODING        65
#define RadarConfiguration_REQUIRED_BYTES_FOR_ACN_ENCODING   9
#define RadarConfiguration_REQUIRED_BITS_FOR_ACN_ENCODING    65
#define RadarConfiguration_REQUIRED_BYTES_FOR_XER_ENCODING   260

void RadarConfiguration_Initialize(RadarConfiguration* pVal);
flag RadarConfiguration_IsConstraintValid(const RadarConfiguration* val, int* pErrCode);




/* ================= Encoding/Decoding function prototypes =================
 * These functions are placed at the end of the file to make sure all types
 * have been declared first, in case of parameterized ACN encodings
 * ========================================================================= */

flag RadarMeasurement400K_validity_Encode(const RadarMeasurement400K_validity* val, BitStream* pBitStrm, int* pErrCode,
    flag bCheckConstraints);
flag RadarMeasurement400K_validity_Decode(RadarMeasurement400K_validity* pVal, BitStream* pBitStrm, int* pErrCode);
flag RadarMeasurement400K_distances_Encode(const RadarMeasurement400K_distances* val, BitStream* pBitStrm,
    int* pErrCode, flag bCheckConstraints);
flag RadarMeasurement400K_distances_Decode(RadarMeasurement400K_distances* pVal, BitStream* pBitStrm, int* pErrCode);
flag RadarMeasurement400K_Encode(const RadarMeasurement400K* val, BitStream* pBitStrm, int* pErrCode,
                                 flag bCheckConstraints);
flag RadarMeasurement400K_Decode(RadarMeasurement400K* pVal, BitStream* pBitStrm, int* pErrCode);
flag RadarRegion_Encode(const RadarRegion* val, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);
flag RadarRegion_Decode(RadarRegion* pVal, BitStream* pBitStrm, int* pErrCode);
flag RadarConfiguration_Encode(const RadarConfiguration* val, BitStream* pBitStrm, int* pErrCode,
                               flag bCheckConstraints);
flag RadarConfiguration_Decode(RadarConfiguration* pVal, BitStream* pBitStrm, int* pErrCode);


#ifdef  __cplusplus
}

#endif

#endif
