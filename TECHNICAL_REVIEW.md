# TR: BTAS - A BitTree Algorithm Set to Process Large-Scale Dataset Efficiently

[Author: Zhenrong WANG](https://github.com/zhenrong-wang)

# 0. Abstract

This article reviewed a dynamic bitmap algorithm set - the **BitTree Algorithm Set (BTAS)** proposed and implemented in the C Programming Language by [Zhenrong WANG](https://github.com/zhenrong-wang). This article described its background, introduced the algorithm set in detail, and demonstrated the implementation and benchmark results. 

# 1. Backgrounds

## 1.1 Problem Description

This algorithm set originated from an engineering problem: removing duplicate elements from a given dataset. This problem is very important for processing or preprocessing various types of raw data. In many cases, duplicate elements undermine the data quality, distort the analysis results, and thus mislead technical or business decisions. Here is a simple example to demonstrate the problem:

Given an array: 

`{10, 38895, 775, *10*, *38895*, 77899023, 892, *38895*}`

The algorithm is expected to remove the duplicate elements and filter the unique elements out to:

`{10, 38895, 775, 77899023, 892}`

The real-world cases are much more complex and with much larger scale. **E.g. a group of 100 million 32-bit hash values or timestamps**.

We focus on integer dataset because other types of data can be mapped or hashed to the integer space in many ways. The raw integer data are usually unsorted and chaotic. 

## 1.2 The Current Methods

To remove duplicate elements, there are 2 main methods:

### 1.2.1 Sorting and Removal/Filter

Sort the raw data first. After sorting, duplicate elements would be adjacent, and removing them can be done by using a single loop. In-place sorting saves space, but it would corrupt the raw data, thus it is not recommended for most of the cases. Sorting to another dataset introduces both time complexity and space complexity.

### 1.2.2 Direct Removal/Filter

Direct removal/filter is feasible with a **brute method** (BRUTE).

It is obvious that by using a nested-loop, one can find out all the duplicate elements and remove/skip them during processing. The outer loop reads the raw elements one-by-one, while the inner loop checks whether the current raw element has occured (been recorded) or not. If yes, skip the element and go ahead to the next one; if no, save it to the target output array. 

The main drawback is the time complexity - O(n^2) - which makes it unacceptable when the raw data scales up.

# 2. Algorithm Description

The introduced **BitTree Algorithm Set (BTAS)** aims to solve this problem with O(n) time complexity, lower/fixed space complexity, and without sorting. The following contents would prove both the feasibility and superiority.

In order to record whether an element occurred or not previously, it is natural to design a seperate data structure, e.g. a Hash Table that transforms every raw element (in the integer case, is the whole integer space with 2^32 possibilities) to an index table so that the inner loop described above would be avoided.

The problem of Hash Table is the space efficiency. The Hash Table could be very sparse, especially when the raw data are constructed with large set of duplicate elements.

## 2.1 The BitTree: 

If we dive deeper, it is obvious that, in order to record whether an element already recorded or not, only 1 bit information is needed. Therefore, a bitmap is the perfect choice. In a bitmap, every bit is available to record a boolean result (true/false). Therefore, the memory usage would be significantly suppressed. It is easy to calculate the total memory needed for the integer space with 2^32 elements:

```2^32 = 4,294,967,296 bit = 512 MiB```

Compare with Hash Table with 4,294,967,296 integers, the bitmap can suppress the memory usage to 32 times smaller.

Meanwhle, it is unnecessary to allocate a full bitmap with 512 MiB. Using a dynamic method with growing trees and branches can further suppress the memory usage. Given that every integer `N` can be expressed as a quotient and a modulus to another given integer `M`:

`(a = N / M, b = N Mod M )`

Therefore, we can use the 2 indexes `a`, `b` to divide the integer space. Take signed integers for an example, if we let `M = 65536`, the absolute range of quotient would be `[0, 32768]`, and the absolute modulus would be `[0, 65535]`. So we can either choose a single-tree with the absolute quotient, or choose a double-tree with one for positive integers or 0 and the other for negative integers. 

- For a single-tree, the maximum length of the modulus branch would be `2*65536 = 131,072 bit = 16KiB` (NOTE: Here we have 1 bit unused).
- For a double-tree, the maximum length of the modulus branch would be `65536 bit = 8 KiB`. (As mentioned, we also wasted 1 bit).

With the method described above and the dynamic memory management, it is ready to implement in C or other programming languages. 

## 2.2 The BitTree with Adjacent Index Hashmap

It is obvious that the BitTree can only handle 1-demensional data because the information of the raw index is discarded. That is, for a filtered array with unique integers such as:

```
{10013, 19983, 2185, 3, ...}
```

We only know that the `10013, 19983 ...` are unique but we don't know where they are in the original dataset. Only by recording their raw index, the algorithm can be expanded to 2-dimensional.

In order to resolve this issue, we need an Adjacent Index Hashmap to record the index. Obviously, this would introduce tramendous extra space cost because the index is in the (unsigned) int32 space (Let's omit 64bit because that's another story). Therefore, we need to make the Adjacent Index Hashmap dynamic.

# 3. Implementation and Benchmark

## 3.1 Current Implementation

Currently, we implemented 4 algorithms that combined as the **BitTree Algorithm Set (BTAS)**, they are:

- BitTree with single-tree (BTS)
- BitTree with double-tree (BTD)
- BitTree with single-tree and an Adjacent Index Hashmap (BTSAIH)
- BitTree with double-tree and an Adjacent Index Hashmap (BTDAIH)

In order to save memory, the Adjacent Index Hashmap uses a double-tree architecture and it is also self-adjustable according to the size of the original array.

The diagram below shows the architecture of a single-tree BitTree:

```
Quotient: Stem[0] - Stem[1] - Stem[2] - Stem[3] - ... - Stem[32768]
               |         |         |         |                 |               
Modulus:      [1]       [1]       [1]       [1]               [1]
  Positive     |         |         |         |                 |
              [2]       [2]       [2]       [2]               [2]
              ...       ...       ...       ...               ...
            [65535]   [65535]   [65535]   [65535]           [65535]
----------     +         +         +         +                 +
Modulus:      [1]       [1]       [1]       [1]               [1]
  Negative     |         |         |         |                 |
              [2]       [2]       [2]       [2]               [2]
              ...       ...       ...       ...               ...
            [65535]   [65535]   [65535]   [65535]           [65535]
```

The growth stratety is also important for performance. Frequent memory (re)allocation would undermine the performance significantly. 

For the bitmap, each branch takes 8/16 KiB memory, therefore, in the current implementation, we allocate 8/16 KiB when needed. This means a branch either exists with its maximum length or doesn't exist. While for the quotient stem, we start from a minumum length `1024` (modifiable) and grow it exponentially until to the maximum length `32769`. That is, if `Stem[2048]` needs to be allocated, we directly allocate to `Stem[4096]`. 

But the Adjacent Index Hashmap is another story. It may need very large memory space so we need to design the growth strategy delibrately. For both the stem and branch array, we adopt the exponential growth stratey to balance memory usage and performance.

Please check the [repository](https://github.com/zhenrong-wang/filter-uniq-ints) for the source code implemented and maintained.

## 3.2 Preliminery Benchmark

### 3.2.1 Benchmark Settings

We did benchmark with 2 rounds:

- **RANDOM  round:** an array with randomly-generated signed integers.
- **GROWING round:** an perfectly sorted array {0, 1, 2, 3,...}.

There are 3 sets of benchmark environment:

**EnvSet A - Physical Machine:**

- **CPU:** AMD Ryzen 7 PRO 6850HS with Radeon Graphics, 3.20 GHz, 8 physical cores, 16 logic processors
- **Memory:** 16 GB
- **Operating System:** Windows 11 - 23H2
- **Compiler:** gcc (x86_64-posix-seh-rev0, Built by MinGW-W64 project) 8.1.0

**EnvSet B: - Virtual Machine1**

- **CPU:** AMD Ryzen 7 PRO 6850HS with Radeon Graphics, 3.20 GHz, 2 vCPUs
- **Memory:** Allocated 4 GB
- **Operating System:** Ubuntu 18.04 LTS
- **Compiler:** gcc (Ubuntu 7.5.0-3ubuntu1~18.04) 7.5.0

**EnvSet C: - Virtual Machine2**

- **CPU:** AMD Ryzen 7 PRO 6850HS with Radeon Graphics, 3.20 GHz, 2 vCPUs
- **Memory:** Allocated 8 GB
- **Operating System:** Ubuntu 22.04 LTS
- **Compiler:** gcc (Ubuntu 11.4.0-1ubuntu1~22.04) 11.4.0


EnvSet A is for larger dataset benchmark because it has more hardware resource available;

EnvSet B is for small dataset benchmark;

EnvSet C is for benchmarking against database engines.

**GCC Compiling options:** `-Ofast`

### 3.2 Benchmark Among Algorithms

Preliminery results is summarized as below. Each pair {A, B}, A is the size of the raw/original/input array. B is the space of randomness.

Round RANDOM (time in second):
```
            |  EnvSet B  |          EnvSet A        |
| Algorithm | {100k, 2M} | {10M, 200M} | {100M, 4B} |
| BTS       |  0.001030  |  0.470000   |  4.233000  |
| BTD       |  0.001934  |  0.783000   |  23.04000  |
| BTSAIH    |  0.007222  |  1.304000   |  57.31900  |
| BTDAIH    |  0.008861  |  1.393000   |  261.0660  |
| BRUTE     |  4.608351  |  INFINITE   |  INFINITE  |
 *UNIQUE*      97533        9766427       97624372
```

Round GROWING (time in second):
```
            |  EnvSet B  |          EnvSet A        |
| Algorithm | {100k, 2M} | {10M, 200M} | {100M, 4B} |
| BTS       |  0.000545  |  0.076000   |  0.439000  |
| BTD       |  0.000959  |  0.073000   |  0.450000  |
| BTSAIH    |  0.000830  |  0.110000   |  0.828000  |
| BTDAIH    |  0.000779  |  0.090000   |  0.788000  |
| BRUTE     |  4.587606  |  INFINITE   |  INFINITE  |
 *UNIQUE*        ALL           ALL           ALL
```

## 3.3 Benchmark Against Database Engines

**EnvSet C** was used to benchmark against DB Engines. Only RANDOM Round executed.

**Versions**:

- **Postgres**: psql (PostgreSQL) 14.11 (Ubuntu 14.11-0ubuntu0.22.04.1)
- **mariaDB**: mariadb  Ver 15.1 Distrib 10.6.16-MariaDB, for debian-linux-gnu (x86_64) using  EditLine wrapper
- **Redis**: Redis server v=6.0.16 sha=00000000:0 malloc=jemalloc-5.2.1 bits=64 build=a3fdef44459b3ad6

**Steps:**

- 1. Build the BTAS executable: `gcc -Wall -Ofast *.c -o btas.run`
- 2. Run BTAS executable with the 2 args and generate the `random.csv`: E.g(*NOTE*). `./btas.run 1000000 1000000`
- 3. Run command `cp -r random.csv /tmp/` to copy the dataset to /tmp/
- 4A. For **Postgres**:
    - **Enter local postgres**: `sudo -u postgres psql` 
    - **Create a table**: `CREATE TABLE table_name (col INT);`
    - **Copy data from /tmp/random.csv**: `\COPY table_name (col) FROM '/tmp/random.csv' WITH CSV;`
    - **Validate the result**: `SELECT COUNT(DISTINCT col) FROM TABLE_NAME;`
    - **Get the time elapsed**: `EXPLAIN ANALYZE SELECT COUNT(DISTINCT col) FROM table_name;`
- 4B. For **mariaDB**:
    - **Enter local mariaDB**: `sudo mysql -uroot -pxxxxxxxxxx`
    - **Change the database**: `use mysql;`
    - **Create a table**: `CREATE TABLE table_name (col INT);`
    - **Load data from /tmp/random.csv**: `LOAD DATA LOCAL INFILE '/tmp/random.csv' INTO TABLE table_name;`
    - **Get the result and time elapsed**: `SELECT COUNT(DISTINCT col) FROM table_name;`
- 4C. For **Redis**:
    - **Make sure the redis-python has installed**: `pip3 install redis`
    - **Run**: `python3 redis_benchmark.py`

*NOTE*: There are 4 groups, the command line args are:
```
{1M, 200k}: ./btas.run 1000000 100000
{10M, 20M}: ./btas.run 10000000 10000000
{100M, 20M}: ./btas.run 100000000 10000000
{100M, 200M}: ./btas.run 100000000 100000000
```

**Round RANDOM (time in second):**
```
               | MariaDB | Postgres |  Redis  |   BTS    | *UNIQUE* |
| {1M,  200k}  |  0.571  |  0.3874  |  8.98   | 0.005185 | 198658   |
| {10M, 20M}   |  14.863 |  4.3673  |  123.70 | 0.079195 | 7869066  |
| {100M, 20M}  |  172.51 |  85.042  |  OOM    | 0.763641 | 19865391 |
| {100M, 200M} |  193.10 |  117.289 |  OOM    | 2.480953 | 78686830 |
```

**Performance Comparison Summary**
```
DB engine time elapsed / BTS time elapsed:
               | MariaDB/ | Postgres/ |  Redis/ |  
| {1M,  200k}  |  110     |  75       |  1732   |
| {10M, 20M}   |  188     |  55       |  1562   |
| {100M, 20M}  |  226     |  111      |  N/A    |
| {100M, 200M} |  78      |  47       |  N/A    |

```

The superiority got proved by the benchmarks above.

# 4. Summary

The article described the **BitTree Algorithm Set (BTAS)** whose superiority has been proved by the benchmark above. As described in the Background part, this algorithm is able to be applied to solve many types of real-world problems with very high efficiency.

# 5. Future Work

The methods and implementations are not the best in terms of memory management and datastructure. Many works need to be done, including:

- **Parallelism:** this needs extra work on multi-thread design and coding
- **Multi-layer data structure:** Currently there is only stems with branches, it is possible that by adopting a leaf layer the algorithms could be better handling sparse values.
- **Extend to 64bit** 64bit migration is another story but it worths efforts because the widely-used hash algorithms start from 64bit up to 256 or even more. Therefore, extending to 64bit has big technical values.

Any review and feedback is highly appreciated and welcomed. Please check the [GitHub repository](https://github.com/zhenrong-wang/filter-uniq-ints) or contact me through X/twitter(@wangzhr4), or email (zhenrongwang@live.com).