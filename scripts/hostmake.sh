#!/bin/bash
set -e
BUILDPATH="docker_build"

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
    echo "Usage: $0 [-c] [-t testtarget] -f -?" 1>&2;
    cat <<EOF
-c          Force a full recompile of the entire project
-f          Fast, avoid running cmake for configuration, useful when only minor changes since last build
-t [target] Run tests, either 'all' or specific tests
-?          Show this help
EOF
    exit 1;
}

while getopts ":cft:?" o; do
    case "${o}" in
	c)
	    # Ensure clean compile, avoid stale objects
	    test -d ${BUILDPATH} && rm -rf ${BUILDPATH}/
	    ;;
	f)
	    FAST=1
	    ;;
	t)
	    TEST_NAME=${OPTARG}
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
    cmake --version
    cmake .. -DCMAKE_C_COMPILER="gcc" \
	  -DCMAKE_CXX_COMPILER="g++" \
	  -DBUILD_EMBEDDED=OFF \
	  -DBUILD_TOOLS=ON \
	  -DBUILD_TESTS=ON \
	  -DBUILD_BINDINGS=ON \
	  -DGENERATE_ASN=OFF \
	  -DNO_OPENCV=ON
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
	    make test
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
popd > /dev/null
