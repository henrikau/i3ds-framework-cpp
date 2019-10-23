#!/bin/bash
set -e
SUITE=../services/i3ds_suite_emulator
RECORD=../tools/i3ds_record
REPLAY=../tools/i3ds_replay
TMP_FILE=tmp_file.log
TMP_FILE2=tmp_file2.log
EXIT_CODE=1

echo "Starting sensor emulator:"
$SUITE > /dev/null &
suite_pid=$!
echo "Started suite with PID: ${suite_pid}"
count=$(ps | grep "${suite_pid}[^[]" | wc -l)
if [[ ${count} -eq 0 ]]; then
    echo "Could not start suite emulator"
    exit 1
fi
echo "Started suite."

function cleanup() {
    echo "Cleaning up... looking for pid ${suite_pid}"
    if [[ -d /prod/${suite_pid}} ]]; then
	kill -INT ${suite_pid}
	while kill -0 ${suite_pid} 2>/dev/null; do
	    sleep 0.1
	done
	exit ${EXIT_CODE}
    fi
}
# we've addet 'set -e', so trap cleanup to make sure we kill off
# i3ds_suite_emulator
trap cleanup EXIT

echo "Activating sensors: TIR, HR and Stereo camera"
NODES=(10 11 12)
for node in ${NODES[@]}; do
    ../tools/i3ds_configure_sensor --node $node --activate
done

echo "Starting recording script"
node_param=""
for n in ${NODES[@]}; do
    node_param+="--node=$n ";
done
$RECORD $node_param -o $TMP_FILE -v &

record_pid=$!
echo "Started recording with PID: ${record_pid}"
count=$(ps | grep "${record_pid}[^[]" | wc -l)
if [[ ${count} -eq 0 ]]; then
    echo "Could not start recorder"
    EXIT_CODE=1
    cleanup
fi

echo "Starting nodes"
for node in ${NODES[@]}; do
    ../tools/i3ds_configure_sensor --node $node --start
done

sleep 3

echo "Stopping recording"
kill -INT ${record_pid}
while kill -0 ${record_pid} 2>/dev/null; do
    sleep 0.1
done

echo "Stopping nodes"
for node in ${NODES[@]}; do
    ../tools/i3ds_configure_sensor --node $node --stop
done

echo "Deactivating nodes"
for node in ${NODES[@]}; do
    ../tools/i3ds_configure_sensor --node $node --deactivate
done

echo "Killing suite"
kill -INT ${suite_pid}
while kill -0 ${suite_pid} 2>/dev/null; do
    sleep 0.1
done

echo "Restarting recording suite"
$RECORD $node_param -o $TMP_FILE2 -v &
record_pid=$!
echo "Started recording with PID: ${record_pid}"
count=$(ps | grep "${record_pid}[^[]" | wc -l)
if [[ ${count} -eq 0 ]]; then
    echo "Could not start recorder"
    rm -f $TMP_FILE
    exit 1
fi

sleep 1

echo "Starting replay"
$REPLAY -i $TMP_FILE > /dev/null &
replay_pid=$!
echo "Started replayer with PID: ${replay_pid}"
count=$(ps | grep "${replay_pid}[^[]" | wc -l)
if [[ ${count} -eq 0 ]]; then
    echo "Could not start recorder"
    rm -f $TMP_FILE
    kill ${record_pid}
    exit 1
fi

echo "Waiting for replay to finish"
while kill -0 ${replay_pid} 2>/dev/null; do
    sleep 0.1
done

sleep 1

echo "Stopping recording"
kill -INT ${record_pid} 2>/dev/null
while kill -0 ${record_pid} 2>/dev/null; do
    sleep 0.1
done

FILE1_SIZE=$(stat ${TMP_FILE} -c %s)
FILE2_SIZE=$(stat ${TMP_FILE2} -c %s)
if [ $FILE1_SIZE -eq $FILE2_SIZE ]; then
    echo "File sizes equal!"
    EXIT_CODE=0
else
    echo "Did not produce same file size from playback."
    EXIT_CODE=1
fi

rm ${TMP_FILE} ${TMP_FILE2}

# cleanup() will return with EXIT_CODE
