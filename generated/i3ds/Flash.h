#ifndef GENERATED_ASN1SCC_Flash_H
#define GENERATED_ASN1SCC_Flash_H
/*
Code automatically generated by asn1scc tool
*/
#include "Camera.h"
#include "asn1crt.h"

#ifdef  __cplusplus
extern "C" {
#endif



typedef struct {
    ShutterTime duration;
    FlashStrength strength;
} FlashSetup;

#define FlashSetup_REQUIRED_BYTES_FOR_ENCODING       5 
#define FlashSetup_REQUIRED_BITS_FOR_ENCODING        40
#define FlashSetup_REQUIRED_BYTES_FOR_ACN_ENCODING   5 
#define FlashSetup_REQUIRED_BITS_FOR_ACN_ENCODING    40
#define FlashSetup_REQUIRED_BYTES_FOR_XER_ENCODING   107

void FlashSetup_Initialize(FlashSetup* pVal);
flag FlashSetup_IsConstraintValid(const FlashSetup* val, int* pErrCode);


 

/* ================= Encoding/Decoding function prototypes =================
 * These functions are placed at the end of the file to make sure all types
 * have been declared first, in case of parameterized ACN encodings
 * ========================================================================= */

flag FlashSetup_Encode(const FlashSetup* val, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);
flag FlashSetup_Decode(FlashSetup* pVal, BitStream* pBitStrm, int* pErrCode); 


#ifdef  __cplusplus
}

#endif

#endif
