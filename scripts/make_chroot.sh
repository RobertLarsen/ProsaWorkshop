#!/bin/bash

# For user and group change to work these files from /etc must be copied (or created) too:
# group  nsswitch.conf  passwd  shadow

verbose=1
SCRIPT=${0}
if [ "${1}" == "-q" ]; then
    verbose=0
    shift
fi

ROOT=${1}
test -d ${ROOT} || mkdir -p ${ROOT}

dir=$(dirname ${2})
if [ "${dir}" != "." ]; then
    dir=${ROOT}${dir}
    test -d ${dir} || mkdir -p ${dir}
else
    dir="${ROOT}"
fi

while test -n "${2}"; do
    dir=$(dirname ${2})
    if [ "${dir}" != "." ]; then
        dir=${ROOT}${dir}
        test -d ${dir} || mkdir -p ${dir}
    else
        dir=${ROOT}
    fi
    cp ${2} ${dir}
    
    ldd ${2} | grep -v linux-vdso | grep -v statically | while read line; do
        if echo "${line}" | grep '=>' >/dev/null; then
            file=$(echo "${line}" | awk '{print $3}')
        else
            file=$(echo "${line}" | awk '{print $1}')
        fi
        test -f "${file}" && ${SCRIPT} -q ${ROOT} ${file}
    done
    shift
done

if test ${verbose} -eq 1; then
    echo "chroot is ready."
    echo "If you need to switch user inside the jail"
    echo "you need to copy/create the following files:"
    echo "/etc/group"
    echo "/etc/nsswitch.conf"
    echo "/etc/passwd"
    echo "/etc/shadow"
fi
