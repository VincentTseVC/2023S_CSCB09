
d_flag=
for p in "$@"
do
    # echo $p
    if [ "$p" != "-d" ]; then

        if [ "$d_flag" = "-d" ]; then

            if [ -d "$p" ]; then
                echo "$p" is a directory
            else
                echo "$p" is not a directory
            fi
        else

            if [ -e "$p" ]; then
                echo "$p" exists
            else
                echo "$p" does not exists
            fi
        fi
    fi
    d_flag="$p"
done



