#!/bin/bash
set -e
BPATH="$(dirname `realpath $0`)"

gen_archive_name()
{
    dirty=`test -z "$(git status -s|grep ^\ M)" || echo "-dirty"`
    hash="$(git log --pretty=oneline -n1|awk '{$hash=substr($1, 0, 12); print $1}')"
    name="i3ds-$(date "+%Y-%m")-${hash}${dirty}.tar"
    echo ${name}
}

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
  -f   Run fast-ish (assume CMake is happy)
  -D   Docker: Do *not* create and update docker image, assume image is OK
  -t   Run tests only (assumes successful build)
  -T   Run specific test
  -?   Show this help
EOF
    exit 1;
}

SPECIFIC_TEST=""
params=""
testparams=""
while getopts "aAcDftT:v?h" o; do
    case "${o}" in
	a)
	    params="-C -t all"
	    ;;
	A)
	    params="${params} -A $(gen_archive_name) -I /tmp/foo"
	    ;;
	c)
	    params="${params} -c"
	    ;;
	f)
	    params="${params} -f"
	    ;;
	D)
	    nodocker=1
	    ;;
	t)
	    testparams="${testparams} -t all"
	    ;;
	T)
	    testparams="${testparams} -t ${OPTARG}"
	    ;;
	v)
	    RUN_VALGRIND=1
	    ;;
	?|h)
	usage
	;;
	*)
	    usage
	    ;;
    esac
done

if [[ -z ${nodocker} ]]; then
    docker build --tag=hostbuild .
fi

# Compile
docker run -v ${BPATH}/:/app \
       --cap-add=sys_nice --ulimit rtprio=20 \
       -u $(id -u ${USER}):$(id -g ${USER}) \
       hostbuild ${params}

# Run tests
if [[ ! -z ${testparams} ]]; then
    docker run -v ${BPATH}/:/app \
	   --cap-add=sys_nice --ulimit rtprio=20 \
	   -u $(id -u ${USER}):$(id -g ${USER}) \
	   hostbuild -f ${testparams}
fi

# Run valgrind
if [[ ! -z ${RUN_VALGRIND} ]]; then
    docker run -v ${BPATH}/:/app \
	   --cap-add=sys_nice --ulimit rtprio=20 \
	   -u $(id -u ${USER}):$(id -g ${USER}) \
	   hostbuild -f -v ${testparams}
fi
