#ifndef GENERATED_ASN1SCC_Radar_H
#define GENERATED_ASN1SCC_Radar_H
/*
Code automatically generated by asn1scc tool
*/
#include "DepthMap.h"
#include "taste-extended.h"
#include "SampleAttribute.h"
#include "Region.h"
#include "taste-types.h"
#include "asn1crt.h"

#ifdef  __cplusplus
extern "C" {
#endif


/*-- RadarMeasurement400K --------------------------------------------*/


typedef struct {
    int nCount; 
    
    T_Float arr[400000];
} RadarMeasurement400K_distances;


typedef struct {
    int nCount; 
    
    Depth_validity_t arr[400000];
} RadarMeasurement400K_validity;
typedef struct {
    SampleAttributes attributes;
    RadarMeasurement400K_distances distances;
    RadarMeasurement400K_validity validity;
    PlanarRegion region;

} RadarMeasurement400K;

void RadarMeasurement400K_distances_Initialize(RadarMeasurement400K_distances* pVal);
void RadarMeasurement400K_validity_Initialize(RadarMeasurement400K_validity* pVal);
void RadarMeasurement400K_Initialize(RadarMeasurement400K* pVal);

#define ERR_RADARMEASUREMENT400K		2673  /**/
#define ERR_RADARMEASUREMENT400K_ATTRIBUTES		2571  /**/
#define ERR_RADARMEASUREMENT400K_ATTRIBUTES_TIMESTAMP		2461  /**/
#define ERR_RADARMEASUREMENT400K_ATTRIBUTES_VALIDITY		2476  /**/
#define ERR_RADARMEASUREMENT400K_ATTRIBUTES_ATTRIBUTES		2564  /**/
#define ERR_RADARMEASUREMENT400K_ATTRIBUTES_ATTRIBUTES_ELM		2553  /**/
#define ERR_RADARMEASUREMENT400K_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_KEY		2487  /**/
#define ERR_RADARMEASUREMENT400K_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE		2542  /**/
#define ERR_RADARMEASUREMENT400K_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_BOOLEAN_VALUE		2498  /**/
#define ERR_RADARMEASUREMENT400K_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_DISCRETE_VALUE		2509  /**/
#define ERR_RADARMEASUREMENT400K_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_REAL_VALUE		2520  /**/
#define ERR_RADARMEASUREMENT400K_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_STRING_VALUE		2531  /**/
#define ERR_RADARMEASUREMENT400K_DISTANCES		2593  /**/
#define ERR_RADARMEASUREMENT400K_DISTANCES_ELM		2582  /**/
#define ERR_RADARMEASUREMENT400K_VALIDITY		2611  /**/
#define ERR_RADARMEASUREMENT400K_VALIDITY_ELM		2600  /**/
#define ERR_RADARMEASUREMENT400K_REGION		2662  /**/
#define ERR_RADARMEASUREMENT400K_REGION_OFFSET_X		2618  /**/
#define ERR_RADARMEASUREMENT400K_REGION_OFFSET_Y		2629  /**/
#define ERR_RADARMEASUREMENT400K_REGION_SIZE_X		2640  /**/
#define ERR_RADARMEASUREMENT400K_REGION_SIZE_Y		2651  /**/
flag RadarMeasurement400K_IsConstraintValid(const RadarMeasurement400K* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_RADARMEASUREMENT400K		2674  /**/
#define ERR_UPER_ENCODE_RADARMEASUREMENT400K_ATTRIBUTES_2		2578  /**/
#define ERR_UPER_ENCODE_RADARMEASUREMENT400K_DISTANCES		2594  /**/
#define ERR_UPER_ENCODE_RADARMEASUREMENT400K_DISTANCES_ELM_2		2589  /**/
#define ERR_UPER_ENCODE_RADARMEASUREMENT400K_VALIDITY		2612  /**/
#define ERR_UPER_ENCODE_RADARMEASUREMENT400K_VALIDITY_ELM_2		2607  /**/
#define ERR_UPER_ENCODE_RADARMEASUREMENT400K_REGION_2		2669  /**/
#define RadarMeasurement400K_REQUIRED_BYTES_FOR_ENCODING       5300092 
#define RadarMeasurement400K_REQUIRED_BITS_FOR_ENCODING        42400733

flag RadarMeasurement400K_Encode(const RadarMeasurement400K* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_RADARMEASUREMENT400K		2675  /**/
#define ERR_UPER_DECODE_RADARMEASUREMENT400K_ATTRIBUTES_2		2579  /**/
#define ERR_UPER_DECODE_RADARMEASUREMENT400K_DISTANCES		2595  /**/
#define ERR_UPER_DECODE_RADARMEASUREMENT400K_DISTANCES_ELM_2		2590  /**/
#define ERR_UPER_DECODE_RADARMEASUREMENT400K_VALIDITY		2613  /**/
#define ERR_UPER_DECODE_RADARMEASUREMENT400K_VALIDITY_ELM_2		2608  /**/
#define ERR_UPER_DECODE_RADARMEASUREMENT400K_REGION_2		2670  /**/
flag RadarMeasurement400K_Decode(RadarMeasurement400K* pVal, BitStream* pBitStrm, int* pErrCode);
/*-- RadarRegion --------------------------------------------*/
typedef struct {
    T_Boolean enable;
    PlanarRegion region;

} RadarRegion;

void RadarRegion_Initialize(RadarRegion* pVal);

#define ERR_RADARREGION		2381  /**/
#define ERR_RADARREGION_ENABLE		2315  /**/
#define ERR_RADARREGION_REGION		2370  /**/
#define ERR_RADARREGION_REGION_OFFSET_X		2326  /**/
#define ERR_RADARREGION_REGION_OFFSET_Y		2337  /**/
#define ERR_RADARREGION_REGION_SIZE_X		2348  /**/
#define ERR_RADARREGION_REGION_SIZE_Y		2359  /**/
flag RadarRegion_IsConstraintValid(const RadarRegion* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_RADARREGION		2382  /**/
#define ERR_UPER_ENCODE_RADARREGION_ENABLE_2		2322  /**/
#define ERR_UPER_ENCODE_RADARREGION_REGION_2		2377  /**/
#define RadarRegion_REQUIRED_BYTES_FOR_ENCODING       9 
#define RadarRegion_REQUIRED_BITS_FOR_ENCODING        65

flag RadarRegion_Encode(const RadarRegion* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_RADARREGION		2383  /**/
#define ERR_UPER_DECODE_RADARREGION_ENABLE_2		2323  /**/
#define ERR_UPER_DECODE_RADARREGION_REGION_2		2378  /**/
flag RadarRegion_Decode(RadarRegion* pVal, BitStream* pBitStrm, int* pErrCode);
/*-- RadarConfiguration --------------------------------------------*/
typedef struct {
    T_Boolean region_enabled;
    PlanarRegion region;

} RadarConfiguration;

void RadarConfiguration_Initialize(RadarConfiguration* pVal);

#define ERR_RADARCONFIGURATION		2454  /**/
#define ERR_RADARCONFIGURATION_REGION_ENABLED		2388  /**/
#define ERR_RADARCONFIGURATION_REGION		2443  /**/
#define ERR_RADARCONFIGURATION_REGION_OFFSET_X		2399  /**/
#define ERR_RADARCONFIGURATION_REGION_OFFSET_Y		2410  /**/
#define ERR_RADARCONFIGURATION_REGION_SIZE_X		2421  /**/
#define ERR_RADARCONFIGURATION_REGION_SIZE_Y		2432  /**/
flag RadarConfiguration_IsConstraintValid(const RadarConfiguration* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_RADARCONFIGURATION		2455  /**/
#define ERR_UPER_ENCODE_RADARCONFIGURATION_REGION_ENABLED_2		2395  /**/
#define ERR_UPER_ENCODE_RADARCONFIGURATION_REGION_2		2450  /**/
#define RadarConfiguration_REQUIRED_BYTES_FOR_ENCODING       9 
#define RadarConfiguration_REQUIRED_BITS_FOR_ENCODING        65

flag RadarConfiguration_Encode(const RadarConfiguration* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_RADARCONFIGURATION		2456  /**/
#define ERR_UPER_DECODE_RADARCONFIGURATION_REGION_ENABLED_2		2396  /**/
#define ERR_UPER_DECODE_RADARCONFIGURATION_REGION_2		2451  /**/
flag RadarConfiguration_Decode(RadarConfiguration* pVal, BitStream* pBitStrm, int* pErrCode);

 

/* ================= Encoding/Decoding function prototypes =================
 * These functions are placed at the end of the file to make sure all types
 * have been declared first, in case of parameterized ACN encodings
 * ========================================================================= */

 


#ifdef  __cplusplus
}

#endif

#endif
