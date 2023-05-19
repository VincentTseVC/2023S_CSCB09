echo $#
echo $0
echo $1
echo $2
echo $3

echo $*

echo $@

for var in "$*"
do
    echo $var
done

for var in "$@"
do
    echo $var
done

echo --------

echo $1
echo $2
shift
echo $1
echo $2
shift
echo $1
echo $2