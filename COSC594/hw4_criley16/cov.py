s1_file = "S1.txt"       # reference start
e1_file = "E1.txt"       # reference end
s2_file = "S2.txt"       # contig start
e2_file = "E2.txt"       # contig end
lenq_file = "LENQ.txt"   # contig lengths
lenr_file = "LENR.txt"   # reference lengths
contig_file = "contigs.txt"

with open(s1_file) as f: s1 = [int(line.strip()) for line in f]
with open(e1_file) as f: e1 = [int(line.strip()) for line in f]
with open(s2_file) as f: s2 = [int(line.strip()) for line in f]
with open(e2_file) as f: e2 = [int(line.strip()) for line in f]
with open(lenq_file) as f: lenq = [int(line.strip()) for line in f]
with open(lenr_file) as f: lenr = [int(line.strip()) for line in f]
with open(contig_file) as f: contigs = [line.strip() for line in f]

from collections import defaultdict

blocks_per_contig = defaultdict(list)
blocks_per_ref = []

len_contig = {}
ref_len = lenr[0]  
for c, start_c, end_c, start_r, end_r, lq in zip(contigs, s2, e2, s1, e1, lenq):

    start_c, end_c = min(start_c, end_c), max(start_c, end_c)
    start_r, end_r = min(start_r, end_r), max(start_r, end_r)


    blocks_per_contig[c].append((start_c, end_c))
    len_contig[c] = lq


    blocks_per_ref.append((start_r, end_r))


def merge_intervals(intervals):
    if not intervals:
        return []
    intervals.sort(key=lambda x: x[0])
    merged = [intervals[0]]
    for curr_start, curr_end in intervals[1:]:
        last_start, last_end = merged[-1]
        if curr_start <= last_end:  
            merged[-1] = (last_start, max(last_end, curr_end))
        else:
            merged.append((curr_start, curr_end))
    return merged


print("Per-contig coverage (%):")
for c, blocks in blocks_per_contig.items():
    merged_blocks = merge_intervals(blocks)
    total_bases = sum(end - start + 1 for start, end in merged_blocks)
    coverage = total_bases / len_contig[c] * 100
    print(f"{c}\t{coverage:.2f}%")


merged_ref = merge_intervals(blocks_per_ref)
total_aligned_ref = sum(end - start + 1 for start, end in merged_ref)
ref_coverage = total_aligned_ref / ref_len * 100
print(f"\nTotal reference coverage: {ref_coverage:.2f}%")
