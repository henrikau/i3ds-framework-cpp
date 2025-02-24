#ifndef GENERATED_ASN1SCC_Radar_H
#define GENERATED_ASN1SCC_Radar_H
/*
Code automatically generated by asn1scc tool
*/
#include "Region.h"
#include "taste-types.h"
#include "asn1crt.h"

#ifdef  __cplusplus
extern "C" {
#endif


/*-- RadarRegion --------------------------------------------*/
typedef struct {
    T_Boolean enable;
    PlanarRegion region;

} RadarRegion;

void RadarRegion_Initialize(RadarRegion* pVal);

#define ERR_RADARREGION		10628  /**/
#define ERR_RADARREGION_ENABLE		10562  /**/
#define ERR_RADARREGION_REGION		10617  /**/
#define ERR_RADARREGION_REGION_OFFSET_X		10573  /**/
#define ERR_RADARREGION_REGION_OFFSET_Y		10584  /**/
#define ERR_RADARREGION_REGION_SIZE_X		10595  /**/
#define ERR_RADARREGION_REGION_SIZE_Y		10606  /**/
flag RadarRegion_IsConstraintValid(const RadarRegion* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_RADARREGION		10629  /**/
#define ERR_UPER_ENCODE_RADARREGION_ENABLE_2		10569  /**/
#define ERR_UPER_ENCODE_RADARREGION_REGION_2		10624  /**/
#define RadarRegion_REQUIRED_BYTES_FOR_ENCODING       9 
#define RadarRegion_REQUIRED_BITS_FOR_ENCODING        65

flag RadarRegion_Encode(const RadarRegion* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_RADARREGION		10630  /**/
#define ERR_UPER_DECODE_RADARREGION_ENABLE_2		10570  /**/
#define ERR_UPER_DECODE_RADARREGION_REGION_2		10625  /**/
flag RadarRegion_Decode(RadarRegion* pVal, BitStream* pBitStrm, int* pErrCode);
/*-- RadarConfiguration --------------------------------------------*/
typedef struct {
    T_Boolean region_enabled;
    PlanarRegion region;

} RadarConfiguration;

void RadarConfiguration_Initialize(RadarConfiguration* pVal);

#define ERR_RADARCONFIGURATION		10701  /**/
#define ERR_RADARCONFIGURATION_REGION_ENABLED		10635  /**/
#define ERR_RADARCONFIGURATION_REGION		10690  /**/
#define ERR_RADARCONFIGURATION_REGION_OFFSET_X		10646  /**/
#define ERR_RADARCONFIGURATION_REGION_OFFSET_Y		10657  /**/
#define ERR_RADARCONFIGURATION_REGION_SIZE_X		10668  /**/
#define ERR_RADARCONFIGURATION_REGION_SIZE_Y		10679  /**/
flag RadarConfiguration_IsConstraintValid(const RadarConfiguration* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_RADARCONFIGURATION		10702  /**/
#define ERR_UPER_ENCODE_RADARCONFIGURATION_REGION_ENABLED_2		10642  /**/
#define ERR_UPER_ENCODE_RADARCONFIGURATION_REGION_2		10697  /**/
#define RadarConfiguration_REQUIRED_BYTES_FOR_ENCODING       9 
#define RadarConfiguration_REQUIRED_BITS_FOR_ENCODING        65

flag RadarConfiguration_Encode(const RadarConfiguration* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_RADARCONFIGURATION		10703  /**/
#define ERR_UPER_DECODE_RADARCONFIGURATION_REGION_ENABLED_2		10643  /**/
#define ERR_UPER_DECODE_RADARCONFIGURATION_REGION_2		10698  /**/
flag RadarConfiguration_Decode(RadarConfiguration* pVal, BitStream* pBitStrm, int* pErrCode);

 

/* ================= Encoding/Decoding function prototypes =================
 * These functions are placed at the end of the file to make sure all types
 * have been declared first, in case of parameterized ACN encodings
 * ========================================================================= */

 


#ifdef  __cplusplus
}

#endif

#endif
