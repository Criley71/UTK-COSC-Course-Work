total=$(awk '{s+=$1} END{print s}' contig_lengths.txt)
awk -v total="$total" '{c+=$1; if(c>=total/2){print $1; exit}}' contig_lengths_sorted.txt
