// package main

// import (
// 	"errors"
// 	"strconv"
// )

// const HASH_TABLE_SIZE = 32769
// const MOD_TABLE_SIZE = 65536
// const BITMAP_INIT_LENGTH = 256
// const BIT_MOD_TABLE_SIZE = 256
// const BIT_MOD_DIV_FACTOR = 256
// const BITMAP_LENGTH_MAX = 8192
// const NEGATIVE_START_POS = 128
// const HT_DYN_INI_SIZE = 1024

// type htableBase struct {
// 	branchSizeP uint32
// 	branchSizeN uint32
// 	ptrBranchP  *uint8
// 	ptrBranchN  *uint8
// }

// type bitmapBase struct {
// 	ptrBranch *uint8
// }

// type bitmapIdxBase struct {
// 	ptrBitBranch *uint8
// 	ptrIdxBranch *bmapIdxBrch
// }

// type bmapIdxBrch struct {
// 	byteIndex   uint16
// 	bitPosition uint8
// 	rawIndex    uint32
// 	ptrNext     *bmapIdxBrch
// }

// type dupIdxList struct {
// 	indexA  uint32
// 	indexB  uint32
// 	ptrNext *dupIdxList
// }

// func convertStringToPositiveNum(string string) (uint32, error) {
// 	if string == "" {
// 		return 0, errors.New("empty string")
// 	}
// 	for _, c := range string {
// 		if c < '0' || c > '9' {
// 			return 0, errors.New("invalid character in string")
// 		}
// 	}
// 	result, err := strconv.ParseUint(string, 10, 32)
// 	if err != nil {
// 		return 0, err
// 	}
// 	return uint32(result), nil
// }

// func fuiBrute(inputArr []int, numElems uint32) ([]int, uint32, error) {
// 	if inputArr == nil {
// 		return nil, 0, errors.New("nil input array")
// 	}
// 	if numElems == 0 {
// 		return nil, 0, errors.New("empty input array")
// 	}
// 	outputArr := make([]int, numElems)
// 	outputArr[0] = inputArr[0]
// 	numOutputElems := uint32(1)
// 	for i := uint32(1); i < numElems; i++ {
// 		found := false
// 		for j := uint32(0); j < numOutputElems; j++ {
// 			if inputArr[i] == outputArr[j] {
// 				found = true
// 				break
// 			}
// 		}
// 		if !found {
// 			outputArr[numOutputElems] = inputArr[i]
// 			numOutputElems++
// 		}
// 	}
// 	outputArr = outputArr[:numOutputElems]
// 	return outputArr, numOutputElems, nil
// }

// func fuiBruteOpt(inputArr []int, numElems uint32) ([]int, uint32, error) {
// 	if inputArr == nil {
// 		return nil, 0, errors.New("nil input array")
// 	}
// 	if numElems == 0 {
// 		return nil, 0, errors.New("empty input array")
// 	}
// 	outputArr := make([]int, 0, numElems)
// 	outputArr = append(outputArr, inputArr[0])
// 	numOutputElems := uint32(1)
// 	for i := uint32(1); i < numElems; i++ {
// 		found := false
// 		for j := uint32(0); j < numOutputElems; j++ {
// 			if inputArr[i] == outputArr[j] {
// 				found = true
// 				break
// 			}
// 		}
// 		if !found {
// 			outputArr = append(outputArr, inputArr[i])
// 			numOutputElems++
// 		}
// 	}
// 	return outputArr, numOutputElems, nil
// }

// func freeHashTable(hashTable [][]uint8, numElems uint32) {
// 	for i := uint32(0); i < numElems; i++ {
// 		if hashTable[i] != nil {
// 			hashTable[i] = nil
// 		}
// 	}
// }

package main

import (
	"errors"
	"fmt"
	"math"
	"math/rand"
	"strconv"
	"time"
)

const (
	HASH_TABLE_SIZE = 32769
	MOD_TABLE_SIZE  = 65536
	HT_DYN_INI_SIZE = 32
)

type hashTableBase struct {
	branchSizeP int
	branchSizeN int
	ptrBranchP  []uint8
	ptrBranchN  []uint8
}

func convertStringToPositiveNum(str string) (uint32, error) {
	if str == "" {
		return 0, errors.New("empty string")
	}
	for _, c := range str {
		if c < '0' || c > '9' {
			return 0, errors.New("invalid character in string")
		}
	}
	result, err := strconv.ParseUint(str, 10, 32)
	if err != nil {
		return 0, err
	}
	return uint32(result), nil
}

func fuiBrute(inputArr []int, numElems uint32) ([]int, uint32, error) {
	if inputArr == nil {
		return nil, 0, errors.New("nil input array")
	}
	if numElems == 0 {
		return nil, 0, errors.New("empty input array")
	}
	outputArr := make([]int, numElems)
	outputArr[0] = inputArr[0]
	numOutputElems := uint32(1)
	for i := uint32(1); i < numElems; i++ {
		found := false
		for j := uint32(0); j < numOutputElems; j++ {
			if inputArr[i] == outputArr[j] {
				found = true
				break
			}
		}
		if !found {
			outputArr[numOutputElems] = inputArr[i]
			numOutputElems++
		}
	}
	outputArr = outputArr[:numOutputElems]
	return outputArr, numOutputElems, nil
}

func fuiBruteOpt(inputArr []int, numElems uint32) ([]int, uint32, error) {
	if inputArr == nil {
		return nil, 0, errors.New("nil input array")
	}
	if numElems == 0 {
		return nil, 0, errors.New("empty input array")
	}
	outputArr := make([]int, 0, numElems)
	outputArr = append(outputArr, inputArr[0])
	numOutputElems := uint32(1)
	for i := uint32(1); i < numElems; i++ {
		found := false
		for j := uint32(0); j < numOutputElems; j++ {
			if inputArr[i] == outputArr[j] {
				found = true
				break
			}
		}
		if !found {
			outputArr = append(outputArr, inputArr[i])
			numOutputElems++
		}
	}
	return outputArr, numOutputElems, nil
}

func freeHashTable(hashTable [][]uint8, numElems uint32) {
	for i := uint32(0); i < numElems; i++ {
		if hashTable[i] != nil {
			hashTable[i] = nil
		}
	}
}

func freeHashTableNew(hashTable []hashTableBase, numElems uint32) {
	for i := uint32(0); i < numElems; i++ {
		if hashTable[i].ptrBranchP != nil {
			hashTable[i].ptrBranchP = nil
		}
		if hashTable[i].ptrBranchN != nil {
			hashTable[i].ptrBranchN = nil
		}
	}
}

func fuiHtableDtree(inputArr []int, numElems uint32) ([]int, uint32, error) {
	if inputArr == nil {
		return nil, 0, errors.New("nil input array")
	}
	if numElems == 0 {
		return nil, 0, errors.New("empty input array")
	}
	hashTableBaseP := make([][]uint8, HASH_TABLE_SIZE)
	hashTableBaseN := make([][]uint8, HASH_TABLE_SIZE)
	outputArr := make([]int, 0, numElems)
	for i := uint32(0); i < numElems; i++ {
		tmp := inputArr[i]
		tmpQuotient := uint32(math.Abs(float64(tmp))) / MOD_TABLE_SIZE
		tmpMod := uint32(math.Abs(float64(tmp))) % MOD_TABLE_SIZE
		if tmp > 0 {
			if hashTableBaseP[tmpQuotient] == nil {
				hashTableBaseP[tmpQuotient] = make([]uint8, MOD_TABLE_SIZE)
			}
			if hashTableBaseP[tmpQuotient][tmpMod] != 0 {
				continue
			}
		} else {
			if hashTableBaseN[tmpQuotient] == nil {
				hashTableBaseN[tmpQuotient] = make([]uint8, MOD_TABLE_SIZE)
			}
			if hashTableBaseN[tmpQuotient][tmpMod] != 0 {
				continue
			}
		}
		outputArr = append(outputArr, tmp)
		if tmp > 0 {
			hashTableBaseP[tmpQuotient][tmpMod] = 1
		} else {
			hashTableBaseN[tmpQuotient][tmpMod] = 1
		}
	}
	freeHashTable(hashTableBaseP, HASH_TABLE_SIZE)
	freeHashTable(hashTableBaseN, HASH_TABLE_SIZE)
	return outputArr, uint32(len(outputArr)), nil
}

func fuiHtableStree(inputArr []int, numElems uint32) ([]int, uint32, error) {
	if inputArr == nil {
		return nil, 0, errors.New("nil input array")
	}
	if numElems == 0 {
		return nil, 0, errors.New("empty input array")
	}
	hashTable := make([]hashTableBase, HASH_TABLE_SIZE)
	outputArr := make([]int, 0, numElems)
	for i := uint32(0); i < numElems; i++ {
		tmp := inputArr[i]
		tmpQuotient := int(math.Abs(float64(tmp))) / MOD_TABLE_SIZE
		tmpMod := int(math.Abs(float64(tmp))) % MOD_TABLE_SIZE
		if tmp > 0 {
			if hashTable[tmpQuotient].ptrBranchP == nil {
				hashTable[tmpQuotient].ptrBranchP = make([]uint8, tmpMod+1)
				hashTable[tmpQuotient].branchSizeP = tmpMod + 1
			} else if hashTable[tmpQuotient].branchSizeP < tmpMod+1 {
				newBranch := make([]uint8, tmpMod+1)
				copy(newBranch, hashTable[tmpQuotient].ptrBranchP)
				hashTable[tmpQuotient].ptrBranchP = newBranch
				hashTable[tmpQuotient].branchSizeP = tmpMod + 1
			}
			if hashTable[tmpQuotient].ptrBranchP[tmpMod] != 0 {
				continue
			}
		} else {
			if hashTable[tmpQuotient].ptrBranchN == nil {
				hashTable[tmpQuotient].ptrBranchN = make([]uint8, tmpMod+1)
				hashTable[tmpQuotient].branchSizeN = tmpMod + 1
			} else if hashTable[tmpQuotient].branchSizeN < tmpMod+1 {
				newBranch := make([]uint8, tmpMod+1)
				copy(newBranch, hashTable[tmpQuotient].ptrBranchN)
				hashTable[tmpQuotient].ptrBranchN = newBranch
				hashTable[tmpQuotient].branchSizeN = tmpMod + 1
			}
			if hashTable[tmpQuotient].ptrBranchN[tmpMod] != 0 {
				continue
			}
		}
		outputArr = append(outputArr, tmp)
		if tmp > 0 {
			hashTable[tmpQuotient].ptrBranchP[tmpMod] = 1
		} else {
			hashTable[tmpQuotient].ptrBranchN[tmpMod] = 1
		}
	}
	freeHashTableNew(hashTable, HASH_TABLE_SIZE)
	return outputArr, uint32(len(outputArr)), nil
}

func fuiHtableStreeDyn(inputArr []int, numElems uint32) ([]int, uint32, error) {
	if inputArr == nil {
		return nil, 0, errors.New("nil input array")
	}
	if numElems == 0 {
		return nil, 0, errors.New("empty input array")
	}
	hashTable := make([]hashTableBase, HT_DYN_INI_SIZE)
	outputArr := make([]int, 0, numElems)
	htBaseLength := HT_DYN_INI_SIZE
	for i := uint32(0); i < numElems; i++ {
		tmp := inputArr[i]
		tmpQuotient := int(math.Abs(float64(tmp))) / MOD_TABLE_SIZE
		tmpMod := int(math.Abs(float64(tmp))) % MOD_TABLE_SIZE
		if (tmpQuotient + 1) > len(hashTable) {
			newHashTable := make([]hashTableBase, tmpQuotient+1)
			copy(newHashTable, hashTable)
			hashTable = newHashTable
			htBaseLength = tmpQuotient + 1
		}
		if tmp > 0 {
			if hashTable[tmpQuotient].ptrBranchP == nil {
				hashTable[tmpQuotient].ptrBranchP = make([]uint8, tmpMod+1)
				hashTable[tmpQuotient].branchSizeP = tmpMod + 1
			} else if hashTable[tmpQuotient].branchSizeP < tmpMod+1 {
				newBranch := make([]uint8, tmpMod+1)
				copy(newBranch, hashTable[tmpQuotient].ptrBranchP)
				hashTable[tmpQuotient].ptrBranchP = newBranch
				hashTable[tmpQuotient].branchSizeP = tmpMod + 1
			}
			if hashTable[tmpQuotient].ptrBranchP[tmpMod] != 0 {
				continue
			}
		} else {
			if hashTable[tmpQuotient].ptrBranchN == nil {
				hashTable[tmpQuotient].ptrBranchN = make([]uint8, tmpMod+1)
				hashTable[tmpQuotient].branchSizeN = tmpMod + 1
			} else if hashTable[tmpQuotient].branchSizeN < tmpMod+1 {
				newBranch := make([]uint8, tmpMod+1)
				copy(newBranch, hashTable[tmpQuotient].ptrBranchN)
				hashTable[tmpQuotient].ptrBranchN = newBranch
				hashTable[tmpQuotient].branchSizeN = tmpMod + 1
			}
			if hashTable[tmpQuotient].ptrBranchN[tmpMod] != 0 {
				continue
			}
		}
		outputArr = append(outputArr, tmp)
		if tmp > 0 {
			hashTable[tmpQuotient].ptrBranchP[tmpMod] = 1
		} else {
			hashTable[tmpQuotient].ptrBranchN[tmpMod] = 1
		}
	}
	freeHashTableNew(hashTable, uint32(htBaseLength))
	return outputArr, uint32(len(outputArr)), nil
}

func printArr(arr []int, numElems uint32, maxElems uint32) {
	if arr == nil || numElems < 1 {
		fmt.Println("ERROR: NULL array input.")
		return
	}

	var i uint32
	for i = 0; i < numElems && i < maxElems; i++ {
		fmt.Printf("%d\t", arr[i])
		if (i+1)%10 == 0 {
			fmt.Println()
		}
	}

	if numElems != maxElems && i == maxElems {
		fmt.Printf("... Remaining %d elements not printed ...\n", numElems-maxElems)
	}

	fmt.Println()
}

func compareArr(arrA, arrB []int, numElems int) int {
	if arrA == nil || arrB == nil {
		return -3
	}
	if numElems < 1 {
		return -1
	}
	for i := 0; i < numElems; i++ {
		if arrA[i] != arrB[i] {
			return 1
		}
	}
	return 0
}

func generateRandomInputArr(arr []int, numElems int, randMax int) int {
	if arr == nil {
		return -5
	}
	if numElems < 1 {
		return -3
	}
	if randMax < 1 {
		return -1
	}
	rand.Seed(time.Now().UnixNano())
	for i := 0; i < numElems; i++ {
		signFlag := rand.Int()
		randNum := rand.Intn(randMax)
		if signFlag%2 == 0 {
			arr[i] = randNum
		} else {
			arr[i] = -randNum
		}
	}
	return 0
}

func generateGrowingArr(arr []int, numElems int) int {
	if arr == nil {
		return -5
	}
	if numElems < 1 {
		return -3
	}
	for i := 0; i < numElems; i++ {
		arr[i] = i
	}
	return 0
}

// main
