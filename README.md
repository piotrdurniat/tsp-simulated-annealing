# Held-Karp algorithm solving the Travelling Salesman Problem

## Measure RAM usage over time:

```bash
./valgrind-mem.sh <massif_out_file> <csv_out_file>

```

example:

```bash
./valgrind-mem.sh massif.out mem.csv

```

### Ms print

```bash
ms_print <out_file>
```

## Get max RAM usage

Read "Maximum resident set size (kbytes)" value from:

```bash
/usr/bin/time -v ./bin/main
```
