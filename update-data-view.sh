#! /bin/dash

ROOT=$(pwd)

BASE="$ROOT/esrocos/types-base/asn/Eigen.asn \
    $ROOT/esrocos/types-base/asn/Point.asn \
    $ROOT/esrocos/types-base/asn/taste-extended.asn \
    $ROOT/esrocos/types-base/asn/taste-types.asn \
    $ROOT/esrocos/types-base/asn/Temperature.asn \
    $ROOT/esrocos/types-base/asn/Time.asn"

export FILES="$BASE \
$ROOT/Analog.asn \
$ROOT/DepthMap.asn \
$ROOT/IMU.asn \
$ROOT/PointCloud.asn \
$ROOT/SensorSuite.asn \
$ROOT/Camera.asn \
$ROOT/Frame.asn \
$ROOT/LIDAR.asn \
$ROOT/Radar.asn \
$ROOT/StarTracker.asn \
$ROOT/ToFCamera.asn"

taste-update-data-view $FILES
