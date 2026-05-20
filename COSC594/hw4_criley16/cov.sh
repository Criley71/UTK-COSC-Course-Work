awk -F '[ |]+' '
NR > 2 {                          # skip the first two header lines
    if (NF < 10) next             # skip any short/malformed lines

    contig = $NF                   # last field = contig tag
    len_q = $(NF-4)                # LEN Q
    len2 = $(NF-6)                 # LEN 2 (contig aligned length)
    len1 = $(NF-7)                 # LEN 1 (reference aligned length)
    len_r = $(NF-5)                # LEN R

    # accumulate per contig
    cov_contig[contig] += len2
    contig_len[contig] = len_q

    # accumulate reference
    total_aligned += len1
    ref_len = len_r
}
END {
    print "Per-contig coverage (%):"
    for (c in cov_contig) {
        coverage = (cov_contig[c]/contig_len[c])*100
        printf "%s\t%.2f%%\n", c, coverage
    }

    ref_coverage = (total_aligned/ref_len)*100
    printf "\nTotal reference coverage: %.2f%%\n", ref_coverage
}'  align.coords
