#!/bin/sh

# Apply project-wide indentation rules to the C source code.

astyle --mode=c --style=gnu -r -c -n "*.c" "*.h"
exit $?
