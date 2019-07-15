#ifndef GENERATED_ASN1SCC_Common_H
#define GENERATED_ASN1SCC_Common_H
/*
Code automatically generated by asn1scc tool
*/
#include "taste-extended.h"
#include "asn1crt.h"

#ifdef  __cplusplus
extern "C" {
#endif


typedef T_Int64 Timepoint;


void Timepoint_Initialize(Timepoint* pVal);

#define ERR_TIMEPOINT		688  /**/
flag Timepoint_IsConstraintValid(const Timepoint* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_TIMEPOINT_2		695  /**/
#define Timepoint_REQUIRED_BYTES_FOR_ENCODING       8 
#define Timepoint_REQUIRED_BITS_FOR_ENCODING        64

flag Timepoint_Encode(const Timepoint* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_TIMEPOINT_2		696  /**/
flag Timepoint_Decode(Timepoint* pVal, BitStream* pBitStrm, int* pErrCode);
typedef asn1SccUint NodeID;


void NodeID_Initialize(NodeID* pVal);

#define ERR_NODEID		674  /**/
flag NodeID_IsConstraintValid(const NodeID* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_NODEID		675  /**/
#define NodeID_REQUIRED_BYTES_FOR_ENCODING       3 
#define NodeID_REQUIRED_BITS_FOR_ENCODING        24

flag NodeID_Encode(const NodeID* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_NODEID		676  /**/
flag NodeID_Decode(NodeID* pVal, BitStream* pBitStrm, int* pErrCode);
typedef asn1SccUint EndpointID;


void EndpointID_Initialize(EndpointID* pVal);

#define ERR_ENDPOINTID		681  /**/
flag EndpointID_IsConstraintValid(const EndpointID* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_ENDPOINTID		682  /**/
#define EndpointID_REQUIRED_BYTES_FOR_ENCODING       1 
#define EndpointID_REQUIRED_BITS_FOR_ENCODING        8

flag EndpointID_Encode(const EndpointID* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_ENDPOINTID		683  /**/
flag EndpointID_Decode(EndpointID* pVal, BitStream* pBitStrm, int* pErrCode);
typedef enum {
    success = 0,
    error_node_id = 1,
    error_endpoint_id = 2,
    error_unsupported = 3,
    error_state = 4,
    error_value = 5,
    error_other = 6
} ResultCode;

// please use the following macros to avoid breaking code.
#define ResultCode_success success
#define ResultCode_error_node_id error_node_id
#define ResultCode_error_endpoint_id error_endpoint_id
#define ResultCode_error_unsupported error_unsupported
#define ResultCode_error_state error_state
#define ResultCode_error_value error_value
#define ResultCode_error_other error_other

void ResultCode_Initialize(ResultCode* pVal);

#define ERR_RESULTCODE		699  /**/
flag ResultCode_IsConstraintValid(const ResultCode* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_RESULTCODE		700  /**/
#define ResultCode_REQUIRED_BYTES_FOR_ENCODING       1 
#define ResultCode_REQUIRED_BITS_FOR_ENCODING        3

flag ResultCode_Encode(const ResultCode* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_RESULTCODE		701  /**/
flag ResultCode_Decode(ResultCode* pVal, BitStream* pBitStrm, int* pErrCode);
/*-- CommandResponse --------------------------------------------*/
typedef struct {
    ResultCode result;
    T_String message;

} CommandResponse;

void CommandResponse_Initialize(CommandResponse* pVal);

#define ERR_COMMANDRESPONSE		728  /**/
#define ERR_COMMANDRESPONSE_RESULT		706  /**/
#define ERR_COMMANDRESPONSE_MESSAGE		717  /**/
flag CommandResponse_IsConstraintValid(const CommandResponse* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_COMMANDRESPONSE		729  /**/
#define ERR_UPER_ENCODE_COMMANDRESPONSE_RESULT_2		713  /**/
#define ERR_UPER_ENCODE_COMMANDRESPONSE_MESSAGE_2		724  /**/
#define CommandResponse_REQUIRED_BYTES_FOR_ENCODING       258 
#define CommandResponse_REQUIRED_BITS_FOR_ENCODING        2060

flag CommandResponse_Encode(const CommandResponse* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_COMMANDRESPONSE		730  /**/
#define ERR_UPER_DECODE_COMMANDRESPONSE_RESULT_2		714  /**/
#define ERR_UPER_DECODE_COMMANDRESPONSE_MESSAGE_2		725  /**/
flag CommandResponse_Decode(CommandResponse* pVal, BitStream* pBitStrm, int* pErrCode);

 

/* ================= Encoding/Decoding function prototypes =================
 * These functions are placed at the end of the file to make sure all types
 * have been declared first, in case of parameterized ACN encodings
 * ========================================================================= */

 


#ifdef  __cplusplus
}

#endif

#endif
