#ifndef GENERATED_ASN1SCC_Eigen_H
#define GENERATED_ASN1SCC_Eigen_H
/*
Code automatically generated by asn1scc tool
*/
#include "taste-extended.h"
#include "taste-types.h"
#include "asn1crt.h"

#ifdef  __cplusplus
extern "C" {
#endif





typedef struct {
    int nCount; 
    
    T_Double arr[4];
} Affine3d_elem;

typedef struct {
    int nCount; 
    
    Affine3d_elem arr[4];
} Affine3d;

void Affine3d_elem_Initialize(Affine3d_elem* pVal);
void Affine3d_Initialize(Affine3d* pVal);

#define ERR_AFFINE3D		19  /**/
#define ERR_AFFINE3D_ELM		12  /**/
#define ERR_AFFINE3D_ELM_ELM		1  /**/
flag Affine3d_IsConstraintValid(const Affine3d* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_AFFINE3D		20  /**/
#define ERR_UPER_ENCODE_AFFINE3D_ELM		13  /**/
#define ERR_UPER_ENCODE_AFFINE3D_ELM_ELM_2		8  /**/
#define Affine3d_REQUIRED_BYTES_FOR_ENCODING       210 
#define Affine3d_REQUIRED_BITS_FOR_ENCODING        1674

flag Affine3d_Encode(const Affine3d* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_AFFINE3D		21  /**/
#define ERR_UPER_DECODE_AFFINE3D_ELM		14  /**/
#define ERR_UPER_DECODE_AFFINE3D_ELM_ELM_2		9  /**/
flag Affine3d_Decode(Affine3d* pVal, BitStream* pBitStrm, int* pErrCode);



typedef struct {
    int nCount; 
    
    T_Double arr[6];
} Matrix6d_elem;

typedef struct {
    int nCount; 
    
    Matrix6d_elem arr[6];
} Matrix6d;

void Matrix6d_elem_Initialize(Matrix6d_elem* pVal);
void Matrix6d_Initialize(Matrix6d* pVal);

#define ERR_MATRIX6D		44  /**/
#define ERR_MATRIX6D_ELM		37  /**/
#define ERR_MATRIX6D_ELM_ELM		26  /**/
flag Matrix6d_IsConstraintValid(const Matrix6d* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_MATRIX6D		45  /**/
#define ERR_UPER_ENCODE_MATRIX6D_ELM		38  /**/
#define ERR_UPER_ENCODE_MATRIX6D_ELM_ELM_2		33  /**/
#define Matrix6d_REQUIRED_BYTES_FOR_ENCODING       471 
#define Matrix6d_REQUIRED_BITS_FOR_ENCODING        3765

flag Matrix6d_Encode(const Matrix6d* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_MATRIX6D		46  /**/
#define ERR_UPER_DECODE_MATRIX6D_ELM		39  /**/
#define ERR_UPER_DECODE_MATRIX6D_ELM_ELM_2		34  /**/
flag Matrix6d_Decode(Matrix6d* pVal, BitStream* pBitStrm, int* pErrCode);



typedef struct {
    int nCount; 
    
    T_Double arr[3];
} Matrix3d_elem;

typedef struct {
    int nCount; 
    
    Matrix3d_elem arr[3];
} Matrix3d;

void Matrix3d_elem_Initialize(Matrix3d_elem* pVal);
void Matrix3d_Initialize(Matrix3d* pVal);

#define ERR_MATRIX3D		69  /**/
#define ERR_MATRIX3D_ELM		62  /**/
#define ERR_MATRIX3D_ELM_ELM		51  /**/
flag Matrix3d_IsConstraintValid(const Matrix3d* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_MATRIX3D		70  /**/
#define ERR_UPER_ENCODE_MATRIX3D_ELM		63  /**/
#define ERR_UPER_ENCODE_MATRIX3D_ELM_ELM_2		58  /**/
#define Matrix3d_REQUIRED_BYTES_FOR_ENCODING       118 
#define Matrix3d_REQUIRED_BITS_FOR_ENCODING        944

flag Matrix3d_Encode(const Matrix3d* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_MATRIX3D		71  /**/
#define ERR_UPER_DECODE_MATRIX3D_ELM		64  /**/
#define ERR_UPER_DECODE_MATRIX3D_ELM_ELM_2		59  /**/
flag Matrix3d_Decode(Matrix3d* pVal, BitStream* pBitStrm, int* pErrCode);


typedef struct {
    int nCount; 
    
    T_Double arr[6];
} Vector6d;

void Vector6d_Initialize(Vector6d* pVal);

#define ERR_VECTOR6D		87  /**/
#define ERR_VECTOR6D_ELM		76  /**/
flag Vector6d_IsConstraintValid(const Vector6d* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_VECTOR6D		88  /**/
#define ERR_UPER_ENCODE_VECTOR6D_ELM_2		83  /**/
#define Vector6d_REQUIRED_BYTES_FOR_ENCODING       79 
#define Vector6d_REQUIRED_BITS_FOR_ENCODING        627

flag Vector6d_Encode(const Vector6d* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_VECTOR6D		89  /**/
#define ERR_UPER_DECODE_VECTOR6D_ELM_2		84  /**/
flag Vector6d_Decode(Vector6d* pVal, BitStream* pBitStrm, int* pErrCode);


typedef struct {
    int nCount; 
    
    T_Double arr[3];
} Vector3d;

void Vector3d_Initialize(Vector3d* pVal);

#define ERR_VECTOR3D		105  /**/
#define ERR_VECTOR3D_ELM		94  /**/
flag Vector3d_IsConstraintValid(const Vector3d* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_VECTOR3D		106  /**/
#define ERR_UPER_ENCODE_VECTOR3D_ELM_2		101  /**/
#define Vector3d_REQUIRED_BYTES_FOR_ENCODING       40 
#define Vector3d_REQUIRED_BITS_FOR_ENCODING        314

flag Vector3d_Encode(const Vector3d* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_VECTOR3D		107  /**/
#define ERR_UPER_DECODE_VECTOR3D_ELM_2		102  /**/
flag Vector3d_Decode(Vector3d* pVal, BitStream* pBitStrm, int* pErrCode);



typedef struct {
    int nCount; 
    
    T_Double arr[20];
} MatrixXd_elem;

typedef struct {
    int nCount; 
    
    MatrixXd_elem arr[20];
} MatrixXd;

void MatrixXd_elem_Initialize(MatrixXd_elem* pVal);
void MatrixXd_Initialize(MatrixXd* pVal);

#define ERR_MATRIXXD		130  /**/
#define ERR_MATRIXXD_ELM		123  /**/
#define ERR_MATRIXXD_ELM_ELM		112  /**/
flag MatrixXd_IsConstraintValid(const MatrixXd* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_MATRIXXD		131  /**/
#define ERR_UPER_ENCODE_MATRIXXD_ELM		124  /**/
#define ERR_UPER_ENCODE_MATRIXXD_ELM_ELM_2		119  /**/
#define MatrixXd_REQUIRED_BYTES_FOR_ENCODING       5214 
#define MatrixXd_REQUIRED_BITS_FOR_ENCODING        41705

flag MatrixXd_Encode(const MatrixXd* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_MATRIXXD		132  /**/
#define ERR_UPER_DECODE_MATRIXXD_ELM		125  /**/
#define ERR_UPER_DECODE_MATRIXXD_ELM_ELM_2		120  /**/
flag MatrixXd_Decode(MatrixXd* pVal, BitStream* pBitStrm, int* pErrCode);


typedef struct {
    int nCount; 
    
    T_Double arr[4];
} Quaterniond;

void Quaterniond_Initialize(Quaterniond* pVal);

#define ERR_QUATERNIOND		148  /**/
#define ERR_QUATERNIOND_ELM		137  /**/
flag Quaterniond_IsConstraintValid(const Quaterniond* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_QUATERNIOND		149  /**/
#define ERR_UPER_ENCODE_QUATERNIOND_ELM_2		144  /**/
#define Quaterniond_REQUIRED_BYTES_FOR_ENCODING       53 
#define Quaterniond_REQUIRED_BITS_FOR_ENCODING        418

flag Quaterniond_Encode(const Quaterniond* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_QUATERNIOND		150  /**/
#define ERR_UPER_DECODE_QUATERNIOND_ELM_2		145  /**/
flag Quaterniond_Decode(Quaterniond* pVal, BitStream* pBitStrm, int* pErrCode);


typedef struct {
    int nCount; 
    
    T_Double arr[100];
} VectorXd;

void VectorXd_Initialize(VectorXd* pVal);

#define ERR_VECTORXD		166  /**/
#define ERR_VECTORXD_ELM		155  /**/
flag VectorXd_IsConstraintValid(const VectorXd* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_VECTORXD		167  /**/
#define ERR_UPER_ENCODE_VECTORXD_ELM_2		162  /**/
#define VectorXd_REQUIRED_BYTES_FOR_ENCODING       1301 
#define VectorXd_REQUIRED_BITS_FOR_ENCODING        10407

flag VectorXd_Encode(const VectorXd* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_VECTORXD		168  /**/
#define ERR_UPER_DECODE_VECTORXD_ELM_2		163  /**/
flag VectorXd_Decode(VectorXd* pVal, BitStream* pBitStrm, int* pErrCode);



typedef struct {
    int nCount; 
    
    T_Double arr[4];
} Isometry3d_elem;

typedef struct {
    int nCount; 
    
    Isometry3d_elem arr[4];
} Isometry3d;

void Isometry3d_elem_Initialize(Isometry3d_elem* pVal);
void Isometry3d_Initialize(Isometry3d* pVal);

#define ERR_ISOMETRY3D		191  /**/
#define ERR_ISOMETRY3D_ELM		184  /**/
#define ERR_ISOMETRY3D_ELM_ELM		173  /**/
flag Isometry3d_IsConstraintValid(const Isometry3d* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_ISOMETRY3D		192  /**/
#define ERR_UPER_ENCODE_ISOMETRY3D_ELM		185  /**/
#define ERR_UPER_ENCODE_ISOMETRY3D_ELM_ELM_2		180  /**/
#define Isometry3d_REQUIRED_BYTES_FOR_ENCODING       210 
#define Isometry3d_REQUIRED_BITS_FOR_ENCODING        1674

flag Isometry3d_Encode(const Isometry3d* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_ISOMETRY3D		193  /**/
#define ERR_UPER_DECODE_ISOMETRY3D_ELM		186  /**/
#define ERR_UPER_DECODE_ISOMETRY3D_ELM_ELM_2		181  /**/
flag Isometry3d_Decode(Isometry3d* pVal, BitStream* pBitStrm, int* pErrCode);



typedef struct {
    int nCount; 
    
    T_Double arr[2];
} Matrix2d_elem;

typedef struct {
    int nCount; 
    
    Matrix2d_elem arr[2];
} Matrix2d;

void Matrix2d_elem_Initialize(Matrix2d_elem* pVal);
void Matrix2d_Initialize(Matrix2d* pVal);

#define ERR_MATRIX2D		216  /**/
#define ERR_MATRIX2D_ELM		209  /**/
#define ERR_MATRIX2D_ELM_ELM		198  /**/
flag Matrix2d_IsConstraintValid(const Matrix2d* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_MATRIX2D		217  /**/
#define ERR_UPER_ENCODE_MATRIX2D_ELM		210  /**/
#define ERR_UPER_ENCODE_MATRIX2D_ELM_ELM_2		205  /**/
#define Matrix2d_REQUIRED_BYTES_FOR_ENCODING       53 
#define Matrix2d_REQUIRED_BITS_FOR_ENCODING        419

flag Matrix2d_Encode(const Matrix2d* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_MATRIX2D		218  /**/
#define ERR_UPER_DECODE_MATRIX2D_ELM		211  /**/
#define ERR_UPER_DECODE_MATRIX2D_ELM_ELM_2		206  /**/
flag Matrix2d_Decode(Matrix2d* pVal, BitStream* pBitStrm, int* pErrCode);


typedef struct {
    int nCount; 
    
    T_Double arr[3];
} Vector2d;

void Vector2d_Initialize(Vector2d* pVal);

#define ERR_VECTOR2D		234  /**/
#define ERR_VECTOR2D_ELM		223  /**/
flag Vector2d_IsConstraintValid(const Vector2d* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_VECTOR2D		235  /**/
#define ERR_UPER_ENCODE_VECTOR2D_ELM_2		230  /**/
#define Vector2d_REQUIRED_BYTES_FOR_ENCODING       40 
#define Vector2d_REQUIRED_BITS_FOR_ENCODING        314

flag Vector2d_Encode(const Vector2d* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_VECTOR2D		236  /**/
#define ERR_UPER_DECODE_VECTOR2D_ELM_2		231  /**/
flag Vector2d_Decode(Vector2d* pVal, BitStream* pBitStrm, int* pErrCode);



typedef struct {
    int nCount; 
    
    T_Double arr[4];
} Matrix4d_elem;

typedef struct {
    int nCount; 
    
    Matrix4d_elem arr[4];
} Matrix4d;

void Matrix4d_elem_Initialize(Matrix4d_elem* pVal);
void Matrix4d_Initialize(Matrix4d* pVal);

#define ERR_MATRIX4D		259  /**/
#define ERR_MATRIX4D_ELM		252  /**/
#define ERR_MATRIX4D_ELM_ELM		241  /**/
flag Matrix4d_IsConstraintValid(const Matrix4d* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_MATRIX4D		260  /**/
#define ERR_UPER_ENCODE_MATRIX4D_ELM		253  /**/
#define ERR_UPER_ENCODE_MATRIX4D_ELM_ELM_2		248  /**/
#define Matrix4d_REQUIRED_BYTES_FOR_ENCODING       210 
#define Matrix4d_REQUIRED_BITS_FOR_ENCODING        1674

flag Matrix4d_Encode(const Matrix4d* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_MATRIX4D		261  /**/
#define ERR_UPER_DECODE_MATRIX4D_ELM		254  /**/
#define ERR_UPER_DECODE_MATRIX4D_ELM_ELM_2		249  /**/
flag Matrix4d_Decode(Matrix4d* pVal, BitStream* pBitStrm, int* pErrCode);



typedef struct {
    int nCount; 
    
    T_Double arr[4];
} Transform3d_elem;

typedef struct {
    int nCount; 
    
    Transform3d_elem arr[4];
} Transform3d;

void Transform3d_elem_Initialize(Transform3d_elem* pVal);
void Transform3d_Initialize(Transform3d* pVal);

#define ERR_TRANSFORM3D		284  /**/
#define ERR_TRANSFORM3D_ELM		277  /**/
#define ERR_TRANSFORM3D_ELM_ELM		266  /**/
flag Transform3d_IsConstraintValid(const Transform3d* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_TRANSFORM3D		285  /**/
#define ERR_UPER_ENCODE_TRANSFORM3D_ELM		278  /**/
#define ERR_UPER_ENCODE_TRANSFORM3D_ELM_ELM_2		273  /**/
#define Transform3d_REQUIRED_BYTES_FOR_ENCODING       210 
#define Transform3d_REQUIRED_BITS_FOR_ENCODING        1674

flag Transform3d_Encode(const Transform3d* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_TRANSFORM3D		286  /**/
#define ERR_UPER_DECODE_TRANSFORM3D_ELM		279  /**/
#define ERR_UPER_DECODE_TRANSFORM3D_ELM_ELM_2		274  /**/
flag Transform3d_Decode(Transform3d* pVal, BitStream* pBitStrm, int* pErrCode);


typedef struct {
    int nCount; 
    
    T_Double arr[4];
} Vector4d;

void Vector4d_Initialize(Vector4d* pVal);

#define ERR_VECTOR4D		302  /**/
#define ERR_VECTOR4D_ELM		291  /**/
flag Vector4d_IsConstraintValid(const Vector4d* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_VECTOR4D		303  /**/
#define ERR_UPER_ENCODE_VECTOR4D_ELM_2		298  /**/
#define Vector4d_REQUIRED_BYTES_FOR_ENCODING       53 
#define Vector4d_REQUIRED_BITS_FOR_ENCODING        418

flag Vector4d_Encode(const Vector4d* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_VECTOR4D		304  /**/
#define ERR_UPER_DECODE_VECTOR4D_ELM_2		299  /**/
flag Vector4d_Decode(Vector4d* pVal, BitStream* pBitStrm, int* pErrCode);


typedef struct {
    int nCount; 
    
    T_Double arr[4];
} AngleAxisd;

void AngleAxisd_Initialize(AngleAxisd* pVal);

#define ERR_ANGLEAXISD		320  /**/
#define ERR_ANGLEAXISD_ELM		309  /**/
flag AngleAxisd_IsConstraintValid(const AngleAxisd* pVal, int* pErrCode);

#define ERR_UPER_ENCODE_ANGLEAXISD		321  /**/
#define ERR_UPER_ENCODE_ANGLEAXISD_ELM_2		316  /**/
#define AngleAxisd_REQUIRED_BYTES_FOR_ENCODING       53 
#define AngleAxisd_REQUIRED_BITS_FOR_ENCODING        418

flag AngleAxisd_Encode(const AngleAxisd* pVal, BitStream* pBitStrm, int* pErrCode, flag bCheckConstraints);

#define ERR_UPER_DECODE_ANGLEAXISD		322  /**/
#define ERR_UPER_DECODE_ANGLEAXISD_ELM_2		317  /**/
flag AngleAxisd_Decode(AngleAxisd* pVal, BitStream* pBitStrm, int* pErrCode);

 

/* ================= Encoding/Decoding function prototypes =================
 * These functions are placed at the end of the file to make sure all types
 * have been declared first, in case of parameterized ACN encodings
 * ========================================================================= */

 


#ifdef  __cplusplus
}

#endif

#endif
