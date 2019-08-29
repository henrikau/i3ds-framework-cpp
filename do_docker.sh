#!/bin/bash
set -e
BPATH="$(dirname `realpath $0`)"

# Options:
# 1) Only compile
# 2) Compile and run tests (default)
# 3) Run tests only (assumes compile has been done previously)
# 4) Create tar-archive for install
# 5) Clean
usage()
{
    echo "Usage: $0 [-a] [-A] [-c] [-C] [-D] [-t] [-T testname]" 1>&2;
    cat<<EOF
Foo

  -a   All, compile and run tests
  -A   Create archive of components for local install (assumes successful build)
  -c   Force compile project (includes clean)
  -D   Docker: Do *not* create and update docker image, assume image is OK
  -t   Run tests only (assumes successful build)
  -T   Run specific test
  -?   Show this help
EOF
    exit 1;
}

SPECIFIC_TEST=""
params=""
while getopts "aAcDtT:?h" o; do
    case "${o}" in
	a)
	    params="-C -t all"
	    ;;
	A)
	    archive="yes"
	    ;;
	c)
	    params="${params} -c"
	    ;;
	D)
	    nodocker="yes"
	    # trigger fast exec as well
	    params="${params} -f"
	    ;;
	t)
	    params="${params} -t all"
	    ;;
	T)
	    params="${params} -t ${OPTARG}"
	    ;;
	?|h)
	usage
	;;
	*)
	    usage
	    ;;
    esac
done

if [[ ! "${nodocker}" == "yes" ]]; then
    docker build --tag=hostbuild .
fi

docker run -v ${BPATH}/:/app \
       --cap-add=sys_nice --ulimit rtprio=20 \
       -u $(id -u ${USER}):$(id -g ${USER}) \
       hostbuild ${params}
