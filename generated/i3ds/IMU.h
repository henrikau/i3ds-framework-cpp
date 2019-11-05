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

#define ERR_IMUSAMPLE		8640  /**/
#define ERR_IMUSAMPLE_AXIS_X_RATE		8574  /**/
#define ERR_IMUSAMPLE_AXIS_X_ACCELERATION		8585  /**/
#define ERR_IMUSAMPLE_AXIS_Y_RATE		8596  /**/
#define ERR_IMUSAMPLE_AXIS_Y_ACCELERATION		8607  /**/
#define ERR_IMUSAMPLE_AXIS_Z_RATE		8618  /**/
#define ERR_IMUSAMPLE_AXIS_Z_ACCELERATION		8629  /**/
flag IMUSample_IsConstraintValid(const IMUSample* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_IMUSAMPLE		8641  /**/
#define ERR_UPER_ENCODE_IMUSAMPLE_AXIS_X_RATE_2		8581  /**/
#define ERR_UPER_ENCODE_IMUSAMPLE_AXIS_X_ACCELERATION_2		8592  /**/
#define ERR_UPER_ENCODE_IMUSAMPLE_AXIS_Y_RATE_2		8603  /**/
#define ERR_UPER_ENCODE_IMUSAMPLE_AXIS_Y_ACCELERATION_2		8614  /**/
#define ERR_UPER_ENCODE_IMUSAMPLE_AXIS_Z_RATE_2		8625  /**/
#define ERR_UPER_ENCODE_IMUSAMPLE_AXIS_Z_ACCELERATION_2		8636  /**/
#define IMUSample_REQUIRED_BYTES_FOR_ENCODING       78 
#define IMUSample_REQUIRED_BITS_FOR_ENCODING        624

flag IMUSample_Encode(const IMUSample* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_IMUSAMPLE		8642  /**/
#define ERR_UPER_DECODE_IMUSAMPLE_AXIS_X_RATE_2		8582  /**/
#define ERR_UPER_DECODE_IMUSAMPLE_AXIS_X_ACCELERATION_2		8593  /**/
#define ERR_UPER_DECODE_IMUSAMPLE_AXIS_Y_RATE_2		8604  /**/
#define ERR_UPER_DECODE_IMUSAMPLE_AXIS_Y_ACCELERATION_2		8615  /**/
#define ERR_UPER_DECODE_IMUSAMPLE_AXIS_Z_RATE_2		8626  /**/
#define ERR_UPER_DECODE_IMUSAMPLE_AXIS_Z_ACCELERATION_2		8637  /**/
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

#define ERR_IMUMEASUREMENT20		8867  /**/
#define ERR_IMUMEASUREMENT20_ATTRIBUTES		8757  /**/
#define ERR_IMUMEASUREMENT20_ATTRIBUTES_TIMESTAMP		8647  /**/
#define ERR_IMUMEASUREMENT20_ATTRIBUTES_VALIDITY		8662  /**/
#define ERR_IMUMEASUREMENT20_ATTRIBUTES_ATTRIBUTES		8750  /**/
#define ERR_IMUMEASUREMENT20_ATTRIBUTES_ATTRIBUTES_ELM		8739  /**/
#define ERR_IMUMEASUREMENT20_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_KEY		8673  /**/
#define ERR_IMUMEASUREMENT20_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE		8728  /**/
#define ERR_IMUMEASUREMENT20_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_BOOLEAN_VALUE		8684  /**/
#define ERR_IMUMEASUREMENT20_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_DISCRETE_VALUE		8695  /**/
#define ERR_IMUMEASUREMENT20_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_REAL_VALUE		8706  /**/
#define ERR_IMUMEASUREMENT20_ATTRIBUTES_ATTRIBUTES_ELM_ATTRIBUTE_VALUE_STRING_VALUE		8717  /**/
#define ERR_IMUMEASUREMENT20_SAMPLES		8845  /**/
#define ERR_IMUMEASUREMENT20_SAMPLES_ELM		8834  /**/
#define ERR_IMUMEASUREMENT20_SAMPLES_ELM_AXIS_X_RATE		8768  /**/
#define ERR_IMUMEASUREMENT20_SAMPLES_ELM_AXIS_X_ACCELERATION		8779  /**/
#define ERR_IMUMEASUREMENT20_SAMPLES_ELM_AXIS_Y_RATE		8790  /**/
#define ERR_IMUMEASUREMENT20_SAMPLES_ELM_AXIS_Y_ACCELERATION		8801  /**/
#define ERR_IMUMEASUREMENT20_SAMPLES_ELM_AXIS_Z_RATE		8812  /**/
#define ERR_IMUMEASUREMENT20_SAMPLES_ELM_AXIS_Z_ACCELERATION		8823  /**/
#define ERR_IMUMEASUREMENT20_BATCH_SIZE		8852  /**/
flag IMUMeasurement20_IsConstraintValid(const IMUMeasurement20* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_IMUMEASUREMENT20		8868  /**/
#define ERR_UPER_ENCODE_IMUMEASUREMENT20_ATTRIBUTES_2		8764  /**/
#define ERR_UPER_ENCODE_IMUMEASUREMENT20_SAMPLES		8846  /**/
#define ERR_UPER_ENCODE_IMUMEASUREMENT20_SAMPLES_ELM_2		8841  /**/
#define ERR_UPER_ENCODE_IMUMEASUREMENT20_BATCH_SIZE_2_2		8863  /**/
#define IMUMeasurement20_REQUIRED_BYTES_FOR_ENCODING       1631 
#define IMUMeasurement20_REQUIRED_BITS_FOR_ENCODING        13042

flag IMUMeasurement20_Encode(const IMUMeasurement20* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_IMUMEASUREMENT20		8869  /**/
#define ERR_UPER_DECODE_IMUMEASUREMENT20_ATTRIBUTES_2		8765  /**/
#define ERR_UPER_DECODE_IMUMEASUREMENT20_SAMPLES		8847  /**/
#define ERR_UPER_DECODE_IMUMEASUREMENT20_SAMPLES_ELM_2		8842  /**/
#define ERR_UPER_DECODE_IMUMEASUREMENT20_BATCH_SIZE_2_2		8864  /**/
flag IMUMeasurement20_Decode(IMUMeasurement20* pVal, BitStream* pBitStrm, int* pErrCode);

 

/* ================= Encoding/Decoding function prototypes =================
 * These functions are placed at the end of the file to make sure all types
 * have been declared first, in case of parameterized ACN encodings
 * ========================================================================= */

 


#ifdef  __cplusplus
}

#endif

#endif
