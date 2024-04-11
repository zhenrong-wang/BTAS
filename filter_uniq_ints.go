package main

import (
	"fmt"
	"math/rand"
	"os"
	"time"
)

func filterUniqueElements(input []int) []int {
	var output []int
	for _, elem := range input {
		found := false
		for _, val := range output {
			if val == elem {
				found = true
				break
			}
		}
		if !found {
			output = append(output, elem)
		}
	}
	return output
}

func filterUniqueElementsImproved(input []int) []int {
	var output []int
	var max, min int

	for _, elem := range input {
		// Update max and min if necessary
		if elem > max {
			max = elem
		}
		if elem < min || len(output) == 0 {
			min = elem
		}

		found := false
		for _, val := range output {
			if val == elem {
				found = true
				break
			}
		}
		if !found {
			output = append(output, elem)
		}
	}
	return output
}

func filterUniqueElementsHashTable(input []int) []int {
	seen := make(map[int]bool)
	var output []int

	for _, elem := range input {
		if !seen[elem] {
			seen[elem] = true
			output = append(output, elem)
		}
	}
	return output
}

type hashTableBaseNode struct {
	branchSizeP uint32
	branchSizeN uint32
	ptrBranchP  []int
	ptrBranchN  []int
}

func newHashTableBaseNode(branchSizeP, branchSizeN uint32) *hashTableBaseNode {
	return &hashTableBaseNode{
		branchSizeP: branchSizeP,
		branchSizeN: branchSizeN,
		ptrBranchP:  make([]int, branchSizeP),
		ptrBranchN:  make([]int, branchSizeN),
	}
}

func filterUniqueElementsDynamicHashTable(input []int) []int {
	const (
		initialSize = 32
		modValue    = 65536
	)

	hashTable := make([]*hashTableBaseNode, initialSize)

	var output []int

	for _, elem := range input {
		hashIndex := elem % initialSize
		if hashIndex < 0 {
			hashIndex = -hashIndex
		}

		if hashTable[hashIndex] == nil {
			hashTable[hashIndex] = newHashTableBaseNode(modValue, modValue)
		}

		var ptrBranch []int
		if elem >= 0 {
			ptrBranch = hashTable[hashIndex].ptrBranchP
		} else {
			ptrBranch = hashTable[hashIndex].ptrBranchN
		}

		modIndex := elem % modValue
		if modIndex < 0 {
			modIndex = -modIndex
		}

		if ptrBranch[modIndex] == 0 {
			output = append(output, elem)
			ptrBranch[modIndex] = 1
		}
	}

	return output
}

type bitHashTableNode struct {
	branchSizeP uint32
	branchSizeN uint32
	ptrBranchP  []uint8
	ptrBranchN  []uint8
}

func newBitHashTableNode(branchSizeP, branchSizeN uint32) *bitHashTableNode {
	return &bitHashTableNode{
		branchSizeP: branchSizeP,
		branchSizeN: branchSizeN,
		ptrBranchP:  make([]uint8, (branchSizeP+7)/8),
		ptrBranchN:  make([]uint8, (branchSizeN+7)/8),
	}
}

func flipBit(byteArray []uint8, bitPosition int) {
	byteIndex := bitPosition / 8
	bitOffset := uint8(1 << uint(bitPosition%8))
	byteArray[byteIndex] |= bitOffset
}

func checkBit(byteArray []uint8, bitPosition int) bool {
	byteIndex := bitPosition / 8
	bitOffset := uint8(1 << uint(bitPosition%8))
	return (byteArray[byteIndex] & bitOffset) != 0
}

func filterUniqueElementsBitHashTable(input []int) []int {
	const (
		modValue = 65536
	)

	hashTable := make([]*bitHashTableNode, modValue)

	var output []int

	for _, elem := range input {
		hashIndex := elem % modValue
		if hashIndex < 0 {
			hashIndex = -hashIndex
		}

		if hashTable[hashIndex] == nil {
			hashTable[hashIndex] = newBitHashTableNode(modValue, modValue)
		}

		var ptrBranch []uint8
		if elem >= 0 {
			ptrBranch = hashTable[hashIndex].ptrBranchP
		} else {
			ptrBranch = hashTable[hashIndex].ptrBranchN
		}

		modIndex := elem % modValue
		if modIndex < 0 {
			modIndex = -modIndex
		}

		if !checkBit(ptrBranch, modIndex) {
			output = append(output, elem)
			flipBit(ptrBranch, modIndex)
		}
	}

	return output
}

func generateRandomInputArr(arr []int, numElems, randMax int) error {
	if arr == nil {
		return fmt.Errorf("array is nil")
	}
	if numElems < 1 {
		return fmt.Errorf("number of elements is less than 1")
	}
	if randMax < 1 {
		return fmt.Errorf("randMax is less than 1")
	}

	rand.Seed(time.Now().UnixNano())
	for i := 0; i < numElems; i++ {
		signFlag := rand.Intn(2)
		randNum := rand.Intn(randMax)
		if signFlag%2 == 0 {
			arr[i] = randNum
		} else {
			arr[i] = -randNum
		}
	}
	return nil
}

func generateGrowingArr(arr []int, numElems int) error {
	if arr == nil {
		return fmt.Errorf("array is nil")
	}
	if numElems < 1 {
		return fmt.Errorf("number of elements is less than 1")
	}

	for i := 0; i < numElems; i++ {
		arr[i] = i
	}
	return nil
}

/* New Integer Generators */

func runBenchmark() {
	// Array sizes to test
	// sizes := []int{10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 50000, 100000, 200000, 300000, 400000, 500000, 600000, 700000, 800000, 900000, 1000000, 2000000, 3000000, 4000000, 5000000, 6000000, 7000000, 8000000, 9000000, 10000000}
	sizes := []int{10, 20, 30, 40, 50, 60, 70, 80, 90, 100, 200, 300, 400, 500, 600, 700, 800, 900, 1000, 2000, 3000, 4000, 5000, 6000, 7000, 8000, 9000, 10000, 50000, 100000, 200000, 300000, 400000, 500000}

	// Filter functions to test
	filterFuncs := []struct {
		name string
		fn   func([]int) []int
	}{
		{"Naive", filterUniqueElements},
		{"Improved", filterUniqueElementsImproved},
		{"HashTable", filterUniqueElementsHashTable},
		{"DynamicHashTable", filterUniqueElementsDynamicHashTable},
		{"BitHashTable", filterUniqueElementsBitHashTable},
	}

	// Open a file to write the results
	file, err := os.Create("benchmark_results.txt")
	if err != nil {
		fmt.Println("Error creating file:", err)
		return
	}
	defer file.Close()

	// Write header to the file
	fmt.Fprintf(file, "Benchmark results\n")
	fmt.Fprintf(file, "Date: %s\n", time.Now().Format(time.RFC3339))
	fmt.Fprintf(file, "Author: Junior ADI\n")

	fmt.Fprintf(file, "---------------------------------------\n")

	// Loop over each array size
	for _, size := range sizes {
		fmt.Fprintf(file, "Benchmark for array size %d\n", size)
		var input []int
		if size == 10 {
			input = []int{16, 17, 2, 17, 4, 2, 97, 4, 17, 56}
		} else {
			input = make([]int, size)
			generateRandomInputArr(input, size, size*10)
		}

		// Loop over each filter function
		for _, filter := range filterFuncs {
			fmt.Fprintf(file, "Benchmark for %s algorithm\n", filter.name)
			// Capture the time before executing the filter function
			startTime := time.Now()

			// Execute the filter function
			_ = filter.fn(input)

			// Capture the time after executing the filter function
			endTime := time.Now()

			// Calculate the execution duration
			duration := endTime.Sub(startTime)

			// Write results to the file
			fmt.Fprintf(file, "Execution time: %v\n", duration)
		}
		fmt.Fprintf(file, "---------------------------------------\n")
	}
	fmt.Println("Benchmark results saved in benchmark_results.txt")
}

func main() {

	runBenchmark()

}
