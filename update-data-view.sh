#! /bin/dash

ROOT=$(pwd)

BASE="$ROOT/esrocos/types-base/asn/Eigen.asn \
    $ROOT/esrocos/types-base/asn/Point.asn \
    $ROOT/esrocos/types-base/asn/taste-extended.asn \
    $ROOT/esrocos/types-base/asn/taste-types.asn \
    $ROOT/esrocos/types-base/asn/Temperature.asn \
    $ROOT/esrocos/types-base/asn/Time.asn"

export FILES="$BASE \
$ROOT/SensorSuite.asn"

taste-update-data-view $FILES
