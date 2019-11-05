#!/bin/bash
# require: inotify-tools

# find directorires with files
pushd "$(dirname `realpath $0`)/../" > /dev/null

BUILDPATH=$(grep "BUILDPATH=" scripts/hostmake.sh | cut -d '=' -f2|sed 's/\"//g')
test -z ${NUM_CPUS} && NUM_CPUS=$(grep ^cpu\ MHz /proc/cpuinfo | wc -l)

dirs=$(for f in $(find . -name "*.[c|h][pp]?" -o -name "CMakeLists.txt"); do dirname ${f}; done|grep -v _build|sort|uniq)
dirs="${dirs} ./generated/i3ds/"

if [[ -z ${dirs} ]]; then
    echo "Could not find any source-files, try to move to the root of the project!"
    exit 1
fi

inotifywait -m ${dirs} -e modify -e create --exclude "\#" |
    # If buildpath isn't available, we have to run full compile anyway
    if [[ ! -d ${BUILDPATH} && ! -z ${action} ]]; then
	echo "time ./scripts/hostmake.sh -c -t"
    else
	while read path action file; do
	    echo "path: ${path}"
	    echo "action: ${action}"
	    echo "file: ${file}"
	    case "${action}" in
	    	MODIFY)
	    	    case ${file} in
	    		*.c[pp]?)
	    		echo "[M] Doing a fast compile"
			# workaround for libi3ds
			test -e ${path}/Makefile || path=$(dirname ${path})
			pushd ${BUILDPATH}/${path}
			time make VERBOSE=1 -j${NUM_CPUS}
			popd
	    		;;

	    		*.h[pp]?)
	    	        echo "[M] headers have changed, do a system-wide (fast) rebuild"
			pushd ${BUILDPATH}
			time make VERBOSE=1 -j${NUM_CPUS}
			popd

	    		;;
	    		CMakeLists.txt)
	    		    echo "${path}/${file} updated, trigger full rebuild"
	    		    time ./scripts/hostmake.sh -c
	    		    ;;
	    		*)
	    		    # unknown file, ignore
	    		    ;;
	    	    esac
	    	    ;;
	    	CREATE)
	    	    case ${file} in
	    		*.c[pp]?)
			test -e Makefile ${path} || path=$(dirname ${path})
			pushd ${BUILDPATH}/${path}
			time make VERBOSE=1 -j${NUM_CPUS}
			popd
	    		;;
	    		*.h[pp]?)
	    		echo "[C] new header created, trigger full rebuild"
	    		time ./scripts/hostmake.sh -c -t all
	    		;;
	    		CMakeLists.txt)
	    		    echo "[C] ${path}/${file} created, trigger full rebuild"
	    		    time ./scripts/hostmake.sh -c
	    		    ;;
	    		*)
	    		    # unknown, ignore
	    		    ;;
	    	    esac
	    	    ;;
	    esac
	done
    fi
popd > /dev/null
