#!/bin/sh
# SPDX-License-Identifier: CC0
fixup() {
	if [ "$AS_ARGS" == "1" ]; then
		sed -e 's/^/-I/g' | tr '\n' ' '
		echo
	else
		cat
	fi
}

if [ ! -z "$SYSINCLUDE" ]; then
	echo "$SYSINCLUDE" | fixup
	exit 0
fi

if [ -z "$CC" ]; then
	CC="cc"
fi

if [ ! -z "$1" ]; then
	if [ "$1" == "--as-args" ]; then
		AS_ARGS=1
		if [ ! -z "$2" ]; then
			CC="$2"
		fi
	else
		CC="$1"
		if [ "$2" == "--as-args" ]; then
			AS_ARGS=1
		fi
	fi
fi

"$CC" -xc /dev/null -E -Wp,-v 2>&1 | sed -n 's,^ ,,p' | fixup
exit $?
