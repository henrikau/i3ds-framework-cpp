#! /bin/dash

ROOT=$(pwd)
BASE=$(ls esrocos/types-base/asn/*.asn)

export FILES="$BASE SensorSuite.asn"

taste-update-data-view $FILES
