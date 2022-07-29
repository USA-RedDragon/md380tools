
{
    name=$3
    if (length($4) != 0) {
        name=$3 " " $4
    }
    print $1 FS $2 FS name FS $5 FS $6 FS $3 FS $7
}
