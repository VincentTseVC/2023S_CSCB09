x=3

if [ $x -lt 3 ] ; then
    echo hello
elif [ $x -lt 2 ]
then
    echo bye
else
    echo fk
fi

[ $x -eq 1 ] && echo hello

echo `expr $x + 2`


for f in *.txt
do
    echo $f
done

for i in `seq 0 10`; do
    echo $i
done

for i in `yes vc | head -n 3`; do
    echo $i
done


for f in *
do

    case $f in
        *.txt)
            echo $f is a text file
            ;;
        *.sh)
            echo $f is ashell script
            ;;
        *)
            echo $f is a other files
            ;;
    esac
done