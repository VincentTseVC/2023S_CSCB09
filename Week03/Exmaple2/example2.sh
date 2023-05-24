for f in "$@"; do

    case "$f" in
        *.pdf)
            [ -f "$f" -a -f "$f".gz ] && rm "$f"
            ;;

        *)
            ;;
    esac
done