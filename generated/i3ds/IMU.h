#ifndef GENERATED_ASN1SCC_IMU_H
#define GENERATED_ASN1SCC_IMU_H
/*
Code automatically generated by asn1scc tool
*/
#include "Sensor.h"
#include "SampleAttribute.h"
#include "taste-types.h"
#include "taste-extended.h"
#include "asn1crt.h"

#ifdef  __cplusplus
extern "C" {
#endif


/*-- IMUSample --------------------------------------------*/
typedef struct {
    T_Double axis_x_rate;
    T_Double axis_x_acceleration;
    T_Double axis_y_rate;
    T_Double axis_y_acceleration;
    T_Double axis_z_rate;
    T_Double axis_z_acceleration;

} IMUSample;

void IMUSample_Initialize(IMUSample* pVal);

#define ERR_IMUSAMPLE		808  /**/
#define ERR_IMUSAMPLE_AXIS_X_RATE		742  /**/
#define ERR_IMUSAMPLE_AXIS_X_ACCELERATION		753  /**/
#define ERR_IMUSAMPLE_AXIS_Y_RATE		764  /**/
#define ERR_IMUSAMPLE_AXIS_Y_ACCELERATION		775  /**/
#define ERR_IMUSAMPLE_AXIS_Z_RATE		786  /**/
#define ERR_IMUSAMPLE_AXIS_Z_ACCELERATION		797  /**/
flag IMUSample_IsConstraintValid(const IMUSample* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_IMUSAMPLE		809  /**/
#define ERR_UPER_ENCODE_IMUSAMPLE_AXIS_X_RATE_2		749  /**/
#define ERR_UPER_ENCODE_IMUSAMPLE_AXIS_X_ACCELERATION_2		760  /**/
#define ERR_UPER_ENCODE_IMUSAMPLE_AXIS_Y_RATE_2		771  /**/
#define ERR_UPER_ENCODE_IMUSAMPLE_AXIS_Y_ACCELERATION_2		782  /**/
#define ERR_UPER_ENCODE_IMUSAMPLE_AXIS_Z_RATE_2		793  /**/
#define ERR_UPER_ENCODE_IMUSAMPLE_AXIS_Z_ACCELERATION_2		804  /**/
#define IMUSample_REQUIRED_BYTES_FOR_ENCODING       78 
#define IMUSample_REQUIRED_BITS_FOR_ENCODING        624

flag IMUSample_Encode(const IMUSample* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_IMUSAMPLE		810  /**/
#define ERR_UPER_DECODE_IMUSAMPLE_AXIS_X_RATE_2		750  /**/
#define ERR_UPER_DECODE_IMUSAMPLE_AXIS_X_ACCELERATION_2		761  /**/
#define ERR_UPER_DECODE_IMUSAMPLE_AXIS_Y_RATE_2		772  /**/
#define ERR_UPER_DECODE_IMUSAMPLE_AXIS_Y_ACCELERATION_2		783  /**/
#define ERR_UPER_DECODE_IMUSAMPLE_AXIS_Z_RATE_2		794  /**/
#define ERR_UPER_DECODE_IMUSAMPLE_AXIS_Z_ACCELERATION_2		805  /**/
flag IMUSample_Decode(IMUSample* pVal, BitStream* pBitStrm, int* pErrCode);
/*-- IMUMeasurement20 --------------------------------------------*/


typedef struct {
    int nCount; 
    
    IMUSample arr[20];
} IMUMeasurement20_samples;
typedef struct {
    SampleAttributes attributes;
    IMUMeasurement20_samples samples;
    T_UInt32 batch_size;

} IMUMeasurement20;

void IMUMeasurement20_samples_Initialize(IMUMeasurement20_samples* pVal);
void IMUMeasurement20_Initialize(IMUMeasurement20* pVal);

#define ERR_IMUMEASUREMENT20		1035  /**/
#define ERR_IMUMEASUREMENT20_ATTRIBUTES		925  /**/
#define ERR_IMUMEASUREMENT20_ATTRIBUTES_TIMESTAMP		815  /**/
#define ERR_IMUMEASUREMENT20_ATTRIBUTES_VALIDITY		830  /**/
#define ERR_IMUMEASUREMENT20_ATTRIBUTES_ATTRIBUTES		918  /**/
#define ERR_IMUMEASUREMENT20_ATTRIBUTES_ATTRIBUTES_ELM		907  /**/
#define ERR_IMUMEASUREMENT20_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_KEY		841  /**/
#define ERR_IMUMEASUREMENT20_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE		896  /**/
#define ERR_IMUMEASUREMENT20_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_BOOLEAN_VALUE		852  /**/
#define ERR_IMUMEASUREMENT20_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_DISCRETE_VALUE		863  /**/
#define ERR_IMUMEASUREMENT20_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_REAL_VALUE		874  /**/
#define ERR_IMUMEASUREMENT20_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_STRING_VALUE		885  /**/
#define ERR_IMUMEASUREMENT20_SAMPLES		1013  /**/
#define ERR_IMUMEASUREMENT20_SAMPLES_ELM		1002  /**/
#define ERR_IMUMEASUREMENT20_SAMPLES_ELM_AXIS_X_RATE		936  /**/
#define ERR_IMUMEASUREMENT20_SAMPLES_ELM_AXIS_X_ACCELERATION		947  /**/
#define ERR_IMUMEASUREMENT20_SAMPLES_ELM_AXIS_Y_RATE		958  /**/
#define ERR_IMUMEASUREMENT20_SAMPLES_ELM_AXIS_Y_ACCELERATION		969  /**/
#define ERR_IMUMEASUREMENT20_SAMPLES_ELM_AXIS_Z_RATE		980  /**/
#define ERR_IMUMEASUREMENT20_SAMPLES_ELM_AXIS_Z_ACCELERATION		991  /**/
#define ERR_IMUMEASUREMENT20_BATCH_SIZE		1020  /**/
flag IMUMeasurement20_IsConstraintValid(const IMUMeasurement20* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_IMUMEASUREMENT20		1036  /**/
#define ERR_UPER_ENCODE_IMUMEASUREMENT20_ATTRIBUTES_2		932  /**/
#define ERR_UPER_ENCODE_IMUMEASUREMENT20_SAMPLES		1014  /**/
#define ERR_UPER_ENCODE_IMUMEASUREMENT20_SAMPLES_ELM_2		1009  /**/
#define ERR_UPER_ENCODE_IMUMEASUREMENT20_BATCH_SIZE_2_2		1031  /**/
#define IMUMeasurement20_REQUIRED_BYTES_FOR_ENCODING       1631 
#define IMUMeasurement20_REQUIRED_BITS_FOR_ENCODING        13042

flag IMUMeasurement20_Encode(const IMUMeasurement20* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_IMUMEASUREMENT20		1037  /**/
#define ERR_UPER_DECODE_IMUMEASUREMENT20_ATTRIBUTES_2		933  /**/
#define ERR_UPER_DECODE_IMUMEASUREMENT20_SAMPLES		1015  /**/
#define ERR_UPER_DECODE_IMUMEASUREMENT20_SAMPLES_ELM_2		1010  /**/
#define ERR_UPER_DECODE_IMUMEASUREMENT20_BATCH_SIZE_2_2		1032  /**/
flag IMUMeasurement20_Decode(IMUMeasurement20* pVal, BitStream* pBitStrm, int* pErrCode);

 

/* ================= Encoding/Decoding function prototypes =================
 * These functions are placed at the end of the file to make sure all types
 * have been declared first, in case of parameterized ACN encodings
 * ========================================================================= */

 


#ifdef  __cplusplus
}

#endif

#endif
