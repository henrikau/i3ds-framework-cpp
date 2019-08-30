#! /bin/sh

NODE=$1
GENERATOR=1
WIDTH=100
BASE=5000
OFFSET=2000
DELTA=50

CAMERA_OUTPUT=$2
FLASH_OUTPUT=$3

i3ds_configure_trigger --node ${NODE} --generator ${GENERATOR} --trigger ${CAMERA_OUTPUT} --width ${WIDTH} --delay ${BASE}

while :
do
    let delay=$BASE-$OFFSET
    while [[ $delay -le $BASE ]]
    do
	let offset=$BASE-$delay
	echo "delay = ${delay}, offset = ${offset}"
	i3ds_configure_trigger --node ${NODE} --generator ${GENERATOR} --trigger ${FLASH_OUTPUT} --width ${WIDTH} --delay ${delay}
	sleep 5
	let delay=$delay+$DELTA
    done
done
