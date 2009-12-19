{
  if (substr($0,1,4) ~ /^[0-9a-f][0-9a-f][0-9a-f][0-9a-f]$/) {
    vendor = $1
    printf ("%s\n", $0);
  } else {
    if (substr($0,1,4) ~ /^C [0-9a-f][0-9a-f]$/) {
      vendor = substr($0,1,4);
      printf ("%s\n", $0);
    } else {
      if (substr($0,1,1) == "\t") {
        printf ("%s%s\n", vendor, $0);
      } else {
        printf ("%s\n", $0);
      }
    }
  }
}
