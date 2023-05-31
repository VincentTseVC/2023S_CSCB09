editpath() {

  # if no arguments, return 1
  [ $# -eq 0 ] && return 1;

  option=
  # Parse options and arguments
  while [ $# -gt 0 ]; do
  # while getopts ":apd" opt: do

    # case ${opt} in
    #  a)

    #  p)


    case $1 in
      -a)
        option='a'
        shift
        ;;
      -p)
        option='p'
        shift
        ;;
      -d)
        option='d'
        shift
        ;;
      --)
        shift
        break
        ;;
      *)
        break
        ;;
    esac

  done

  # shift OP... 我忘了

  # Handle options
  case $option in
    a)
      for path in "$@"; do
        PATH="$PATH:$path"
      done
      ;;

    p)
      for path in "$@"; do
        PATH="$path:$PATH"
      done
      ;;

    d)
      # test="."
      # echo "$PATH" | tr ':' '\n' | grep -v ^\\"$test"$

      for path in "$@"; do
        # PATH=$(echo "$PATH" | tr ':' '\n' | grep -v ^\\"$path"$ | paste -sd ':' -)
        PATH=$(echo "$PATH" | tr ':' '\n' | grep -F -x -v "$path" | paste -sd ':' -)
        #                                       -Fvx -vFx -...
        # 可以把 paste 改成 tr | sed, 自己上網查怎麼用sed
      done

      ;;

    *)
      echo "Invalid option" >&2
      return 1;
      ;;
  esac
}


