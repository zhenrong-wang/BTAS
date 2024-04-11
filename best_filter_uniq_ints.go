package main

import (
	"errors"
	"strconv"
)

const HASH_TABLE_SIZE = 32769
const MOD_TABLE_SIZE = 65536
const BITMAP_INIT_LENGTH = 256
const BIT_MOD_TABLE_SIZE = 256
const BIT_MOD_DIV_FACTOR = 256
const BITMAP_LENGTH_MAX = 8192
const NEGATIVE_START_POS = 128
const HT_DYN_INI_SIZE = 1024

type htableBase struct {
	branchSizeP uint32
	branchSizeN uint32
	ptrBranchP  *uint8
	ptrBranchN  *uint8
}

type bitmapBase struct {
	ptrBranch *uint8
}

type bitmapIdxBase struct {
	ptrBitBranch *uint8
	ptrIdxBranch *bmapIdxBrch
}

type bmapIdxBrch struct {
	byteIndex   uint16
	bitPosition uint8
	rawIndex    uint32
	ptrNext     *bmapIdxBrch
}

type dupIdxList struct {
	indexA  uint32
	indexB  uint32
	ptrNext *dupIdxList
}

func convertStringToPositiveNum(string string) (uint32, error) {
	if string == "" {
		return 0, errors.New("empty string")
	}
	for _, c := range string {
		if c < '0' || c > '9' {
			return 0, errors.New("invalid character in string")
		}
	}
	result, err := strconv.ParseUint(string, 10, 32)
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
