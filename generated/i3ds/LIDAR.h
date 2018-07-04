#ifndef GENERATED_ASN1SCC_LIDAR_H
#define GENERATED_ASN1SCC_LIDAR_H
/*
Code automatically generated by asn1scc tool
*/
#include "Point.h"
#include "taste-types.h"
#include "Region.h"
#include "SampleAttribute.h"
#include "asn1crt.h"

#ifdef  __cplusplus
extern "C" {
#endif



typedef struct
{
  int nCount;

  Point arr[200000];
} LIDARMeasurement200K_points;

#define LIDARMeasurement200K_points_REQUIRED_BYTES_FOR_ENCODING       7850006
#define LIDARMeasurement200K_points_REQUIRED_BITS_FOR_ENCODING        62800048
#define LIDARMeasurement200K_points_REQUIRED_BYTES_FOR_ACN_ENCODING   7850003
#define LIDARMeasurement200K_points_REQUIRED_BITS_FOR_ACN_ENCODING    62800018
#define LIDARMeasurement200K_points_REQUIRED_BYTES_FOR_XER_ENCODING   45600059

void LIDARMeasurement200K_points_Initialize(LIDARMeasurement200K_points* pVal);
flag LIDARMeasurement200K_points_IsConstraintValid(const LIDARMeasurement200K_points* val, int* pErrCode);
#ifndef ERR_LIDARMeasurement200K_points
#define ERR_LIDARMeasurement200K_points		1001  /*(SIZE(1 .. 200000))*/
#endif

typedef struct
{
  SampleAttributes attributes;
  LIDARMeasurement200K_points points;
  PolarRegion region;
} LIDARMeasurement200K;

#define LIDARMeasurement200K_REQUIRED_BYTES_FOR_ENCODING       7850125
#define LIDARMeasurement200K_REQUIRED_BITS_FOR_ENCODING        62800993
#define LIDARMeasurement200K_REQUIRED_BYTES_FOR_ACN_ENCODING   7850121
#define LIDARMeasurement200K_REQUIRED_BITS_FOR_ACN_ENCODING    62800963
#define LIDARMeasurement200K_REQUIRED_BYTES_FOR_XER_ENCODING   45601304

void LIDARMeasurement200K_Initialize(LIDARMeasurement200K* pVal);
flag LIDARMeasurement200K_IsConstraintValid(const LIDARMeasurement200K* val, int* pErrCode);


typedef struct
{
  T_Boolean enable;
  PolarRegion region;
} LIDARRegion;

#define LIDARRegion_REQUIRED_BYTES_FOR_ENCODING       53
#define LIDARRegion_REQUIRED_BITS_FOR_ENCODING        417
#define LIDARRegion_REQUIRED_BYTES_FOR_ACN_ENCODING   53
#define LIDARRegion_REQUIRED_BITS_FOR_ACN_ENCODING    417
#define LIDARRegion_REQUIRED_BYTES_FOR_XER_ENCODING   350

void LIDARRegion_Initialize(LIDARRegion* pVal);
flag LIDARRegion_IsConstraintValid(const LIDARRegion* val, int* pErrCode);


typedef struct
{
  T_Boolean region_enabled;
  PolarRegion region;
} LIDARConfiguration;

#define LIDARConfiguration_REQUIRED_BYTES_FOR_ENCODING       53
#define LIDARConfiguration_REQUIRED_BITS_FOR_ENCODING        417
#define LIDARConfiguration_REQUIRED_BYTES_FOR_ACN_ENCODING   53
#define LIDARConfiguration_REQUIRED_BITS_FOR_ACN_ENCODING    417
#define LIDARConfiguration_REQUIRED_BYTES_FOR_XER_ENCODING   380

void LIDARConfiguration_Initialize(LIDARConfiguration* pVal);
flag LIDARConfiguration_IsConstraintValid(const LIDARConfiguration* val, int* pErrCode);




/* ================= Encoding/Decoding function prototypes =================
 * These functions are placed at the end of the file to make sure all types
 * have been declared first, in case of parameterized ACN encodings
 * ========================================================================= */

flag LIDARMeasurement200K_points_Encode(const LIDARMeasurement200K_points* val, BitStream* pBitStrm, int* pErrCode,
                                        flag bCheckConstraints);
flag LIDARMeasurement200K_points_Decode(LIDARMeasurement200K_points* pVal, BitStream* pBitStrm, int* pErrCode);
flag LIDARMeasurement200K_Encode(const LIDARMeasurement200K* val, BitStream* pBitStrm, int* pErrCode,
                                 flag bCheckConstraints);
flag LIDARMeasurement200K_Decode(LIDARMeasurement200K* pVal, BitStream* pBitStrm, int* pErrCode);
flag LIDARRegion_Encode(const LIDARRegion* val, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);
flag LIDARRegion_Decode(LIDARRegion* pVal, BitStream* pBitStrm, int* pErrCode);
flag LIDARConfiguration_Encode(const LIDARConfiguration* val, BitStream* pBitStrm, int* pErrCode,
                               flag bCheckConstraints);
flag LIDARConfiguration_Decode(LIDARConfiguration* pVal, BitStream* pBitStrm, int* pErrCode);


#ifdef  __cplusplus
}

#endif

#endif
