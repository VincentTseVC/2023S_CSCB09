#!/bin/sh

. ./def-editpath.sh
PATH=/bin:/usr/bin:/usr/local/bin
editpath -a '/xxx   yyy' /opt/bin .
/usr/bin/printenv PATH
editpath -p 'Job$' '/M$Office' x
/usr/bin/printenv PATH
editpath -d . 'Job$' usr
/usr/bin/printenv PATH
