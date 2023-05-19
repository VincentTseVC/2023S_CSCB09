if [ -d $1 ]
then
    echo dir exist
elif [ -f $1 ]
then
    echo file exist
else
    echo not exist
fi