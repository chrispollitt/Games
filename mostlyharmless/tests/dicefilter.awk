# Remove the ">/dice off" block that walk.sh adds at the start of a walkthrough
# (transcripts have CRLF line ends, so compare with the \r removed).
BEGIN { skip = 0 }
{ line = $0; sub(/\r$/, "", line) }
skip == 0 && line == ">/dice off" { skip = 1; next }
skip == 1 { if (line ~ /^\(The dice are off\.\)/) { skip = 2; next } skip = 0 }
skip == 2 { skip = 3; if (line ~ /^[[:space:]]*$/) next }
{ print }
