#!/bin/sh

SRC='console.c error.c expressions.c flow_visit.c format_visit.c fs.c hash.c hashmap.c main.c options.c osstream.c parser.c pre_expressions.c token.c tokenizer.c type.c visit.c'
INC='ownership.h hash.h hashmap.h console.h osstream.h options.h token.h type.h error.h fs.h tokenizer.h parser.h visit.h flow_visit.h pre_expressions.h expressions.h format_visit.h'

SYSINC='assert.h ctype.h errno.h limits.h stdarg.h stdbool.h stddef.h stdint.h stdio.h stdlib.h string.h time.h'
UNXINC='sys/ioctl.h sys/stat.h sys/types.h termios.h unistd.h dirent.h fcntl.h'
WININC='windows.h conio.h'

get_sys_include() {
	for x in $SYSINC; do
	echo "#include <$x>"
	done

	echo '#ifdef _WIN32'
	for x in $WININC; do
	echo "#include <$x>"
	done
	echo '#else'
	for x in $UNXINC; do
	echo "#include <$x>"
	done
	echo '#endif'
}

get_include() {
	for x in $INC; do
		echo "/* $x */"
		cat $x | grep -vE '^#pragma once' | grep -vE '^#include'
	done
}

get_src() {
	for x in $SRC; do
		echo "/* $x */"
		grep -vE '^#include' $x
	done
}

get_sys_include
get_include
get_src
