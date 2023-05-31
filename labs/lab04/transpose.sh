if [ $# -lt 2 ]; then
  echo "Missing Arguments" 1>&2
  exit 1
fi

if [ -d "$2" ]; then
  echo "target path already exists" 1>&2
  exit 2
fi

mkdir "$2"

for dir1 in "$1"/*; do
  d1=`basename "$dir1"` # $(......)
  # echo "$d1"
  for dir2 in "$dir1"/*; do
    d2=`basename "$dir2"`
    # echo "$d2"

    # only create if not exist
    if [ ! -d "$2"/"$d2" ]; then
      mkdir "$2"/"$d2"
    fi

    mkdir "$2"/"$d2"/"$d1"

    cp -r "$dir2"/* "$2"/"$d2"/"$d1"

  done

done