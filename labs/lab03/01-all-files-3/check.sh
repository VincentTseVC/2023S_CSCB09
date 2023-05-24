
d_flag=false
while [ "$#" -ne 0 ]
do
    if [ "$1" = "-d" ]; then
        d_flag=true
    else

        if [ "$d_flag" = "true" ]; then

            if [ -d "$1" ]; then
                echo "$1" is a directory
            else
                echo "$1" is not a directory
            fi
        else

            if [ -e "$1" ]; then
                echo "$1" exists
            else
                echo "$1" does not exists
            fi
        fi

        d_flag=false
    fi
    shift
done



