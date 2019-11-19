#!/bin/bash
set -e
BUILDPATH="docker_build"
INSTALL_PATH=""

run_valgrind ()
{
    test -e ${2} || return 1
    echo "Running ${2} under valgrind"

    # For more info/noise, enable verbose:
    # --verbose \

    # If valgrind finds a problem in one of the core libraries, enable
    # gen-suppressions and add a new rule:
    # --gen-suppressions=all -v\
    LD_LIBRARY_PATH=${1}/libi3ds.so.1.0 valgrind \
    		   --leak-check=full \
    		   --show-leak-kinds=all \
		   --show-reachable=no \
    		   --track-origins=yes \
		   --suppressions=/valgrind-suppressions.supp \
		   ${2}
}

run_test ()
{
    test -e ${2} || return 1
    echo "Running testcase `basename ${2}`"
    LD_LIBRARY_PATH=${1}/libi3ds.so.1.0 ${2}
    res=$?
    echo "Test completed with ${res}"
    return $res
}

usage()
{
    echo "Usage: $0 [-A archive] [-c] [-t testtarget] [-f] [-v] [-?]"
    cat <<EOF
-A [name]   Create tar-archive with i3ds-files
-c          Force a full recompile of the entire project
-f          Fast, avoid running cmake for configuration, useful when only minor changes since last build
-I [PATH]   Set install-path
-t [target] Run tests, either 'all' or specific tests
-v 	    Valgrind, run on specified test-target (or 'all' if -t is not set)
-?          Show this help
EOF
    exit 1;
}

while getopts ":A:cfI:t:v?" o; do
    case "${o}" in
	A)
	    ARCHIVE=${OPTARG}
	    ;;
	c)
	    # Ensure clean compile, avoid stale objects
	    test -d ${BUILDPATH} && rm -rf ${BUILDPATH}/
	    ;;
	f)
	    FAST=1
	    ;;
	I)
	    INSTALL_PATH=${OPTARG}
	    ;;
	t)
	    TEST_NAME=${OPTARG}
	    ;;
	v)
	    RUN_VALGRIND=1
	    ;;
	?)
	usage
	;;
	*)
	    usage
	    ;;
    esac
done

test -z ${NUM_CPUS} && NUM_CPUS=$(grep ^cpu\ MHz /proc/cpuinfo | wc -l)

test -d ${BUILDPATH} || mkdir -p ${BUILDPATH}
pushd ${BUILDPATH} > /dev/null

if [[ -z ${FAST} ]]; then
    cmake .. -DCMAKE_C_COMPILER="gcc" \
	  -DCMAKE_CXX_COMPILER="g++" \
	  -DBUILD_EMBEDDED=OFF \
	  -DBUILD_DEBUG=OFF \
	  -DBUILD_TOOLS=ON \
	  -DBUILD_TESTS=ON \
	  -DBUILD_BINDINGS=ON \
	  -DCMAKE_INSTALL_PREFIX="${INSTALL_PATH}" \
	  -DNO_OPENCV=OFF
fi

# Always trigger build, regardless of $FAST
make -j${NUM_CPUS}

# Determine if we are running hostmake standalone or inside docker
if [[ -e "${0}" ]]; then
    BPATH="$(dirname `realpath $0`)/libi3ds"
    if [[ ${BPATH} == "//libi3ds" ]]; then
	BPATH="`dirname $(find /app/${BUILDPATH} -name 'libi3ds.so')`"
    fi
elif [[ -d "libi3ds/" ]]; then
    BPATH="`pwd`/libi3ds"
else
    echo "Could not determine directory, ${0} will abort"
    exit 1;
fi

# This is a *ugly* hack! Assume that stuff has been built, find all
# test-binaries in ${BUILDPATH}/libi3ds/ and try to match.
if [[ ! -z ${TEST_NAME} ]]; then
    echo "Running tests: ${TEST_NAME}"

    if [[ "${TEST_NAME}" == "list" ]]; then
	make help | grep test
    else
	if [[ ${TEST_NAME} == "all" || ${TEST_NAME} == "test" ]]; then
	    env CTEST_OUTPUT_ON_FAILURE=1 make test
	else
	    # need to jump through some hoops as some of the
	    # test-binaries are placed elsewhere (i.e. tools/)
	    make ${TEST_NAME} || { echo "Making ${TEST_NAME} FAILED"; exit 1; }
	    BINARY=$(find $(pwd) -name ${TEST_NAME})
	    pushd ${BPATH} > /dev/null
	    run_test ${BPATH} ${BINARY}
	    popd > /dev/null
	fi
    fi
fi

if [[ ! -z ${RUN_VALGRIND} ]]; then
    # If we list all (or no) tests, then run valgrind of all of them
    if [[ -z ${TEST_NAME} || ${TEST_NAME} == "all" || ${TEST_NAME} == "test" || ${TEST_NAME} == "list" ]]; then
	for tname in $(ls -1 ${BPATH}/*-test); do
	    echo "Running valgrind for ${tname}"
	    run_valgrind ${BPATH} ${tname}
	done
    else
	BINARY=$(find $(pwd) -name ${TEST_NAME})
	test ! -z ${BINARY} && run_valgrind ${BPATH} ${BINARY}
    fi
fi

if [[ ! -z ${ARCHIVE} ]]; then
    make VERBOSE=1 install DIR=${INSTALL_PATH}

    pushd ${INSTALL_PATH} > /dev/null
    tarname=$(basename ${ARCHIVE})
    tar cf /app/${tarname} . --owner=root --group=root &>/dev/null
    popd > /dev/null
    echo "${tarname} created"
fi

popd > /dev/null
