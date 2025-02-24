#ifndef GENERATED_ASN1SCC_Sensor_H
#define GENERATED_ASN1SCC_Sensor_H
/*
Code automatically generated by asn1scc tool
*/
#include "taste-extended.h"
#include "taste-types.h"
#include "base.h"
#include "asn1crt.h"

#ifdef  __cplusplus
extern "C" {
#endif


typedef T_UInt32 SamplePeriod;


void SamplePeriod_Initialize(SamplePeriod* pVal);

#define ERR_SAMPLEPERIOD		9286  /**/
flag SamplePeriod_IsConstraintValid(const SamplePeriod* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_SAMPLEPERIOD_2		9293  /**/
#define SamplePeriod_REQUIRED_BYTES_FOR_ENCODING       4 
#define SamplePeriod_REQUIRED_BITS_FOR_ENCODING        32

flag SamplePeriod_Encode(const SamplePeriod* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_SAMPLEPERIOD_2		9294  /**/
flag SamplePeriod_Decode(SamplePeriod* pVal, BitStream* pBitStrm, int* pErrCode);
typedef T_UInt32 BatchSize;


void BatchSize_Initialize(BatchSize* pVal);

#define ERR_BATCHSIZE		9297  /**/
flag BatchSize_IsConstraintValid(const BatchSize* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_BATCHSIZE_2		9304  /**/
#define BatchSize_REQUIRED_BYTES_FOR_ENCODING       4 
#define BatchSize_REQUIRED_BITS_FOR_ENCODING        32

flag BatchSize_Encode(const BatchSize* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_BATCHSIZE_2		9305  /**/
flag BatchSize_Decode(BatchSize* pVal, BitStream* pBitStrm, int* pErrCode);
typedef T_UInt32 BatchCount;


void BatchCount_Initialize(BatchCount* pVal);

#define ERR_BATCHCOUNT		9308  /**/
flag BatchCount_IsConstraintValid(const BatchCount* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_BATCHCOUNT_2		9315  /**/
#define BatchCount_REQUIRED_BYTES_FOR_ENCODING       4 
#define BatchCount_REQUIRED_BITS_FOR_ENCODING        32

flag BatchCount_Encode(const BatchCount* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_BATCHCOUNT_2		9316  /**/
flag BatchCount_Decode(BatchCount* pVal, BitStream* pBitStrm, int* pErrCode);
/*-- SampleCommand --------------------------------------------*/
typedef struct {
    T_UInt32 period;
    T_UInt32 batch_size;
    T_UInt32 batch_count;

} SampleCommand;

void SampleCommand_Initialize(SampleCommand* pVal);

#define ERR_SAMPLECOMMAND		9364  /**/
#define ERR_SAMPLECOMMAND_PERIOD		9319  /**/
#define ERR_SAMPLECOMMAND_BATCH_SIZE		9334  /**/
#define ERR_SAMPLECOMMAND_BATCH_COUNT		9349  /**/
flag SampleCommand_IsConstraintValid(const SampleCommand* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_SAMPLECOMMAND		9365  /**/
#define ERR_UPER_ENCODE_SAMPLECOMMAND_PERIOD_2_2		9330  /**/
#define ERR_UPER_ENCODE_SAMPLECOMMAND_BATCH_SIZE_2_2		9345  /**/
#define ERR_UPER_ENCODE_SAMPLECOMMAND_BATCH_COUNT_2_2		9360  /**/
#define SampleCommand_REQUIRED_BYTES_FOR_ENCODING       12 
#define SampleCommand_REQUIRED_BITS_FOR_ENCODING        96

flag SampleCommand_Encode(const SampleCommand* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_SAMPLECOMMAND		9366  /**/
#define ERR_UPER_DECODE_SAMPLECOMMAND_PERIOD_2_2		9331  /**/
#define ERR_UPER_DECODE_SAMPLECOMMAND_BATCH_SIZE_2_2		9346  /**/
#define ERR_UPER_DECODE_SAMPLECOMMAND_BATCH_COUNT_2_2		9361  /**/
flag SampleCommand_Decode(SampleCommand* pVal, BitStream* pBitStrm, int* pErrCode);
/*-- SensorConfiguration --------------------------------------------*/
typedef struct {
    T_String device_name;
    T_UInt32 period;
    T_UInt32 batch_size;
    T_UInt32 batch_count;

} SensorConfiguration;

void SensorConfiguration_Initialize(SensorConfiguration* pVal);

#define ERR_SENSORCONFIGURATION		9427  /**/
#define ERR_SENSORCONFIGURATION_DEVICE_NAME		9371  /**/
#define ERR_SENSORCONFIGURATION_PERIOD		9382  /**/
#define ERR_SENSORCONFIGURATION_BATCH_SIZE		9397  /**/
#define ERR_SENSORCONFIGURATION_BATCH_COUNT		9412  /**/
flag SensorConfiguration_IsConstraintValid(const SensorConfiguration* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_SENSORCONFIGURATION		9428  /**/
#define ERR_UPER_ENCODE_SENSORCONFIGURATION_DEVICE_NAME_2		9378  /**/
#define ERR_UPER_ENCODE_SENSORCONFIGURATION_PERIOD_2_2		9393  /**/
#define ERR_UPER_ENCODE_SENSORCONFIGURATION_BATCH_SIZE_2_2		9408  /**/
#define ERR_UPER_ENCODE_SENSORCONFIGURATION_BATCH_COUNT_2_2		9423  /**/
#define SensorConfiguration_REQUIRED_BYTES_FOR_ENCODING       53 
#define SensorConfiguration_REQUIRED_BITS_FOR_ENCODING        422

flag SensorConfiguration_Encode(const SensorConfiguration* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_SENSORCONFIGURATION		9429  /**/
#define ERR_UPER_DECODE_SENSORCONFIGURATION_DEVICE_NAME_2		9379  /**/
#define ERR_UPER_DECODE_SENSORCONFIGURATION_PERIOD_2_2		9394  /**/
#define ERR_UPER_DECODE_SENSORCONFIGURATION_BATCH_SIZE_2_2		9409  /**/
#define ERR_UPER_DECODE_SENSORCONFIGURATION_BATCH_COUNT_2_2		9424  /**/
flag SensorConfiguration_Decode(SensorConfiguration* pVal, BitStream* pBitStrm, int* pErrCode);
typedef enum {
    inactive = 0,
    standby = 1,
    operational = 2,
    failure = 3
} SensorState;

// please use the following macros to avoid breaking code.
#define SensorState_inactive inactive
#define SensorState_standby standby
#define SensorState_operational operational
#define SensorState_failure failure

void SensorState_Initialize(SensorState* pVal);

#define ERR_SENSORSTATE		9232  /**/
flag SensorState_IsConstraintValid(const SensorState* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_SENSORSTATE		9233  /**/
#define SensorState_REQUIRED_BYTES_FOR_ENCODING       1 
#define SensorState_REQUIRED_BITS_FOR_ENCODING        2

flag SensorState_Encode(const SensorState* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_SENSORSTATE		9234  /**/
flag SensorState_Decode(SensorState* pVal, BitStream* pBitStrm, int* pErrCode);
/*-- SensorStatus --------------------------------------------*/
typedef struct {
    SensorState current_state;
    Base_Temperature device_temperature;

} SensorStatus;

void SensorStatus_Initialize(SensorStatus* pVal);

#define ERR_SENSORSTATUS		9279  /**/
#define ERR_SENSORSTATUS_CURRENT_STATE		9246  /**/
#define ERR_SENSORSTATUS_DEVICE_TEMPERATURE		9268  /**/
#define ERR_SENSORSTATUS_DEVICE_TEMPERATURE_KELVIN		9257  /**/
flag SensorStatus_IsConstraintValid(const SensorStatus* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_SENSORSTATUS		9280  /**/
#define ERR_UPER_ENCODE_SENSORSTATUS_CURRENT_STATE_2		9253  /**/
#define ERR_UPER_ENCODE_SENSORSTATUS_DEVICE_TEMPERATURE_2		9275  /**/
#define SensorStatus_REQUIRED_BYTES_FOR_ENCODING       14 
#define SensorStatus_REQUIRED_BITS_FOR_ENCODING        106

flag SensorStatus_Encode(const SensorStatus* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_SENSORSTATUS		9281  /**/
#define ERR_UPER_DECODE_SENSORSTATUS_CURRENT_STATE_2		9254  /**/
#define ERR_UPER_DECODE_SENSORSTATUS_DEVICE_TEMPERATURE_2		9276  /**/
flag SensorStatus_Decode(SensorStatus* pVal, BitStream* pBitStrm, int* pErrCode);
typedef enum {
    activate = 0,
    start = 1,
    stop = 2,
    deactivate = 3
} StateCommand;

// please use the following macros to avoid breaking code.
#define StateCommand_activate activate
#define StateCommand_start start
#define StateCommand_stop stop
#define StateCommand_deactivate deactivate

void StateCommand_Initialize(StateCommand* pVal);

#define ERR_STATECOMMAND		9239  /**/
flag StateCommand_IsConstraintValid(const StateCommand* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_STATECOMMAND		9240  /**/
#define StateCommand_REQUIRED_BYTES_FOR_ENCODING       1 
#define StateCommand_REQUIRED_BITS_FOR_ENCODING        2

flag StateCommand_Encode(const StateCommand* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_STATECOMMAND		9241  /**/
flag StateCommand_Decode(StateCommand* pVal, BitStream* pBitStrm, int* pErrCode);

 

/* ================= Encoding/Decoding function prototypes =================
 * These functions are placed at the end of the file to make sure all types
 * have been declared first, in case of parameterized ACN encodings
 * ========================================================================= */

 


#ifdef  __cplusplus
}

#endif

#endif
