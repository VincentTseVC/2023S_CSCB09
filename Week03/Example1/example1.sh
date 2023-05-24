for tex_file in *.tex
do
    # echo "$tex_file"

    # A1.tex -> A1
    file_name=`basename "$tex_file" .tex`
    #file_name=$(basename "$tex_file" .tex)
    # echo "$file_name"

    # A1 -> A1.pdf
    pdf_file="$file_name".pdf
    echo "$pdf_file"

    if [ ! -f "$pdf_file" ]; then
        pdflatex "$tex_file"
    elif [ "$tex_file" -nt "$pdf_file" ]; then
        pdflatex "$tex_file"
    fi

    #[ ! -f "$pdf_file" -o "$tex_file" -nt "$pdf_file" ] && pdflatex "$tex_file"

done