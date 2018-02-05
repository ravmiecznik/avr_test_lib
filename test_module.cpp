/*
 * test_,odule.cpp
 *
 *  Created on: 5 lut 2018
 *      Author: miecznik
 */

#include "test_module.h"

#include <stdio.h>
#include <string.h>

Test::Test(){
	printf("Initializing internal test module\n");
	test_count = 0;
	tests_passed = 0;
	tests_failed = 0;
};

Test::~Test(){
	printf("Test results:\n");
	printf("passed: %d \n", tests_passed);
	printf("failed: %d \n", tests_failed);
	printf("out of total tests: %d \n", test_count);
}

uint16_t Test::test_count = 0;
uint16_t Test::tests_passed = 0;
uint16_t Test::tests_failed = 0;

void Test::passed(){
	printf("passed\n");
}

void Test::failed(){
	printf("failed\n");
}
