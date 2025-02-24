/*
Code automatically generated by asn1scc tool
*/
#include <limits.h>
#include <string.h>
#include <math.h>
#include "taste-types.h"



void T_Int32_Initialize(T_Int32* pVal)
{

	(*(pVal)) = 0;
}

flag T_Int32_IsConstraintValid(const T_Int32* pVal, int* pErrCode)
{
    flag ret = TRUE;
	
    ret = ((-2147483648LL <= (*(pVal))) && ((*(pVal)) <= 2147483647LL));
    *pErrCode = ret ? 0 :  ERR_T_INT32; 

	return ret;
}

flag T_Int32_Encode(const T_Int32* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;
	ret = bCheckConstraints ? T_Int32_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret) {
	    BitStream_EncodeConstraintWholeNumber(pBitStrm, (*(pVal)), -2147483648LL, 2147483647LL);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag T_Int32_Decode(T_Int32* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;

	ret = BitStream_DecodeConstraintWholeNumber(pBitStrm, pVal, -2147483648LL, 2147483647LL);
	*pErrCode = ret ? 0 : ERR_UPER_DECODE_T_INT32;

	return ret  && T_Int32_IsConstraintValid(pVal, pErrCode);
}



void T_UInt32_Initialize(T_UInt32* pVal)
{

	(*(pVal)) = 0;
}

flag T_UInt32_IsConstraintValid(const T_UInt32* pVal, int* pErrCode)
{
    flag ret = TRUE;
	
    ret = ((*(pVal)) <= 4294967295UL);
    *pErrCode = ret ? 0 :  ERR_T_UINT32; 

	return ret;
}

flag T_UInt32_Encode(const T_UInt32* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;
	ret = bCheckConstraints ? T_UInt32_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret) {
	    BitStream_EncodeConstraintPosWholeNumber(pBitStrm, (*(pVal)), 0, 4294967295LL);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag T_UInt32_Decode(T_UInt32* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;

	ret = BitStream_DecodeConstraintPosWholeNumber(pBitStrm, pVal, 0, 4294967295LL);
	*pErrCode = ret ? 0 : ERR_UPER_DECODE_T_UINT32;

	return ret  && T_UInt32_IsConstraintValid(pVal, pErrCode);
}



void T_Int8_Initialize(T_Int8* pVal)
{

	(*(pVal)) = 0;
}

flag T_Int8_IsConstraintValid(const T_Int8* pVal, int* pErrCode)
{
    flag ret = TRUE;
	
    ret = ((-128LL <= (*(pVal))) && ((*(pVal)) <= 127LL));
    *pErrCode = ret ? 0 :  ERR_T_INT8; 

	return ret;
}

flag T_Int8_Encode(const T_Int8* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;
	ret = bCheckConstraints ? T_Int8_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret) {
	    BitStream_EncodeConstraintWholeNumber(pBitStrm, (*(pVal)), -128, 127);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag T_Int8_Decode(T_Int8* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;

	ret = BitStream_DecodeConstraintWholeNumber(pBitStrm, pVal, -128, 127);
	*pErrCode = ret ? 0 : ERR_UPER_DECODE_T_INT8;

	return ret  && T_Int8_IsConstraintValid(pVal, pErrCode);
}



void T_UInt8_Initialize(T_UInt8* pVal)
{

	(*(pVal)) = 0;
}

flag T_UInt8_IsConstraintValid(const T_UInt8* pVal, int* pErrCode)
{
    flag ret = TRUE;
	
    ret = ((*(pVal)) <= 255UL);
    *pErrCode = ret ? 0 :  ERR_T_UINT8; 

	return ret;
}

flag T_UInt8_Encode(const T_UInt8* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;
	ret = bCheckConstraints ? T_UInt8_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret) {
	    BitStream_EncodeConstraintPosWholeNumber(pBitStrm, (*(pVal)), 0, 255);
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag T_UInt8_Decode(T_UInt8* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;

	ret = BitStream_DecodeConstraintPosWholeNumber(pBitStrm, pVal, 0, 255);
	*pErrCode = ret ? 0 : ERR_UPER_DECODE_T_UINT8;

	return ret  && T_UInt8_IsConstraintValid(pVal, pErrCode);
}



void T_Boolean_Initialize(T_Boolean* pVal)
{

	(*(pVal)) = FALSE;
}

flag T_Boolean_IsConstraintValid(const T_Boolean* pVal, int* pErrCode)
{
    flag ret = TRUE;
	
    ret = TRUE;
    *pErrCode = 0;

	return ret;
}

flag T_Boolean_Encode(const T_Boolean* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints)
{
    flag ret = TRUE;
	ret = bCheckConstraints ? T_Boolean_IsConstraintValid(pVal, pErrCode) : TRUE ;
	if (ret) {
	    BitStream_AppendBit(pBitStrm,(*(pVal)));
    } /*COVERAGE_IGNORE*/

	
    return ret;
}

flag T_Boolean_Decode(T_Boolean* pVal, BitStream* pBitStrm, int* pErrCode)
{
    flag ret = TRUE;

	ret = BitStream_ReadBit(pBitStrm, pVal);
	*pErrCode = ret ? 0 : ERR_UPER_DECODE_T_BOOLEAN;

	return ret  && T_Boolean_IsConstraintValid(pVal, pErrCode);
}

