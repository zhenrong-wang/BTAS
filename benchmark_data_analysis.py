# /******************************************************************************

#                             Author: Junior ADI
# 				Description: Brief description of the code file
# 				    Date: April 8th 2024, 12:17 AM GMT
# 						Location: Abidjan, Cote d'Ivoire.
# 						e-mail: rootoor.projects@gmail.com
# 						Github: https://github.com/junior-adi/

#     Original repository: https://github.com/zhenrong-wang/filter-uniq-ints.git

# 					This code is licensed under the MIT License.

#                         Copyright (c) 2024, Junior ADI

# *******************************************************************************/
import matplotlib.pyplot as plt

# Function to parse benchmark results from a file
def parse_benchmark_results(filename):
    with open(filename, 'r') as file:
        lines = file.readlines()

    results = {}
    current_size = None

    # Loop over each line in the file
    for line in lines:
        if "Benchmark for array size" in line:
            # Extract the array size
            current_size = int(line.split()[-1])
            results[current_size] = {}
        elif "Benchmark for" in line:
            # Extract the algorithm name and execution time
            algorithm = line.split("Benchmark for")[-1].strip()
            time_str = lines[lines.index(line) + 1].split(":")[-1].strip()
            time, unit = float(time_str[:-2]), time_str[-2:]
            # Convert execution time to microseconds if necessary
            if unit == 'ms':
                time *= 1000  # convert milliseconds to microseconds
            elif unit == 'ns':
                time /= 1000  # convert nanoseconds to microseconds
            elif unit == 'µs':
                pass  # time is already in microseconds
            elif unit == 's':
                time *= 1000000  # convert seconds to microseconds
            
            results[current_size][algorithm] = time

    return results

# Parse the benchmark results
filename = "./benchmark_results.txt"
results = parse_benchmark_results(filename)

# Write the results to a file
with open('./results.txt', 'w') as f:
    for size, algorithms in results.items():
        f.write(f"Array size: {size}\n")
        for algorithm, time in algorithms.items():
            f.write(f"{algorithm}: {time}µs\n")
        f.write("-----------------------------\n")

# Print the results to the console
for size, algorithms in results.items():
    print(f"Array size: {size}")
    for algorithm, time in algorithms.items():
        print(f"{algorithm}: {time}µs")
    print("-----------------------------")

# Plot the performance of each algorithm
plt.figure(figsize=(10, 6))

for algorithm in results[next(iter(results))].keys():
    sizes = sorted(results.keys())
    times = [results[size][algorithm] for size in sizes]
    plt.plot(sizes, times, marker='o', label=algorithm)

plt.xlabel('Array size')
plt.ylabel('Execution time (µs)')
plt.title('Algorithm Performance')
plt.legend()
plt.grid(True)
plt.show()
