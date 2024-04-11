/*
*****************************************************************************

	                            Author: Junior ADI
					Description: Brief description of the code file
					    Date: April 8th 2024, 12:17 AM GMT
							Location: Abidjan, Cote d'Ivoire.
							e-mail: rootoor.projects@gmail.com
							Github: https://github.com/junior-adi/

	    Original repository: https://github.com/zhenrong-wang/filter-uniq-ints.git

						This code is licensed under the MIT License.

	                        Copyright (c) 2024, Junior ADI

******************************************************************************
*/
package main

import (
	"errors"
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

// This function generates a random array of integers with specified number of elements, maximum random value, and number of duplicates.
//
// Parameters:
//   - arr: the array to generate random integers in
//   - numElems: the number of elements in the array
//   - randMax: the maximum value for random integers
//   - numDuplicates: the number of duplicates for each random integer
//
// Returns:
//
//	an error if the array is nil, numElems is less than 1, randMax is less than 1, or numDuplicates is less than 1
//
// Example:
//
//	arr := make([]int, 10)
//	err := generateRandomInputArr(arr, 10, 100, 5)
//	if err != nil {
//	    fmt.Println(err)
//	} else {
//	    fmt.Println(arr)
//	}
func generateRandomInputArrImproved1(arr []int, numElems, randMax, numDuplicates int) error {
	if arr == nil {
		return fmt.Errorf("array is nil")
	}
	if numElems < 1 {
		return fmt.Errorf("number of elements is less than 1")
	}
	if randMax < 1 {
		return fmt.Errorf("randMax is less than 1")
	}
	if numDuplicates < 1 {
		return fmt.Errorf("numDuplicates is less than 1")
	}

	rand.Seed(time.Now().UnixNano())
	for i := 0; i < numElems; i++ {
		signFlag := rand.Intn(2)
		randNum := rand.Intn(randMax / numDuplicates)
		if signFlag%2 == 0 {
			arr[i] = randNum
		} else {
			arr[i] = -randNum
		}
	}
	return nil
}

// this function generates a random array of integers with specified number of elements and duplicates.
//
// Parameters:
//   - numElems: the total number of elements in the generated array
//   - randMax: the maximum value for each generated integer (inclusive)
//   - numDuplicates: the number of allowed duplicates in the generated array
//
// Returns:
//   - ([]int, error): the generated array of integers and an error, if any
//   - The generated array of integers.
//   - An error if numElems is less than 0, randMax is less than or equal to 0, or numDuplicates is less than 0.
//
// Example:
//
//	arr, err := generateRandomInputArrImproved2(10, 100, 3)
//	if err != nil {
//	    fmt.Println("Error:", err)
//	} else {
//	    fmt.Println("Generated random array:", arr)
//	}
func generateRandomInputArrImproved2(numElems, randMax, numDuplicates int) ([]int, error) {
	if numElems < 0 || randMax <= 0 || numDuplicates < 0 {
		return nil, fmt.Errorf("invalid input: numElems, randMax, and numDuplicates must be non-negative")
	}

	rand.Seed(time.Now().UnixNano())

	var arr []int

	// Génération de numElems - numDuplicates nombres uniques
	uniqueCount := numElems - numDuplicates
	for i := 0; i < uniqueCount; i++ {
		randomNumber := rand.Intn(randMax + 1)
		arr = append(arr, randomNumber)
	}

	// Ajout de numDuplicates nombres dupliqués
	for i := 0; i < numDuplicates; i++ {
		randomIndex := rand.Intn(uniqueCount)
		duplicateNumber := arr[randomIndex]
		arr = append(arr, duplicateNumber)
	}

	// Mélange de l'ordre des éléments dans la liste
	rand.Shuffle(len(arr), func(i, j int) {
		arr[i], arr[j] = arr[j], arr[i]
	})

	return arr, nil
}

// this function generates a growing array of integers with a specified number of elements and duplicates.
//
// Parameters:
//   - arr: the array in which to generate the growing integers
//   - numElems: the number of elements in the array
//   - randMax: the maximum value that each number can have
//   - numDuplicates: the number of duplicates for each growing integer
//
// Returns:
//
//	an error if the array is nil, numElems is less than 1, or numDuplicates is less than 1
//
// Example:
//
//	arr := make([]int, 10)
//	err := generateRandomInputArrImproved2(&arr, 10, 100, 5)
//	if err != nil {
//	    fmt.Println(err)
//	} else {
//	    fmt.Println(arr)
//	}
func generateRandomInputArrImprovedWithZeros(arr *[]int, numElems, randMax, numDuplicates int) error {
	if numDuplicates > numElems {
		return errors.New("numDuplicates cannot be greater than numElems")
	}

	rand.Seed(time.Now().UnixNano())

	// Generate unique elements
	for i := 0; i < numElems-numDuplicates; i++ {
		*arr = append(*arr, rand.Intn(randMax))
	}

	// Generate duplicate elements
	for i := 0; i < numDuplicates; i++ {
		duplicateElem := (*arr)[rand.Intn(len(*arr))]
		*arr = append(*arr, duplicateElem)
	}

	return nil
}

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
	file, err := os.Create("./benchmark_results.txt")
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
			// input = make([]int, size)
			rand.Seed(time.Now().UnixNano())
			numDuplicates := rand.Intn(size) // Génération aléatoire du nombre de duplicatas
			generateRandomInputArrImproved2(size, size*10, numDuplicates)
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
	// Exemple d'utilisation
	// numElems := 10
	// randMax := 100
	// numDuplicates := 3

	// arr, err := generateRandomInputArrImproved(numElems, randMax, numDuplicates)
	// if err != nil {
	// 	fmt.Println("Erreur:", err)
	// 	return
	// }

	// fmt.Println("Liste aléatoire générée:", arr)
	// arr := make([]int, 10)
	// err := generateRandomInputArrImprovedWithZeros(&arr, 10, 100, 5)
	// if err != nil {
	// 	fmt.Println(err)
	// } else {
	// 	fmt.Println(arr)
	// }

}
