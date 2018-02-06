/*
 * test_module.h
 *
 *  Created on: 5 lut 2018
 *      Author: miecznik
 */

#ifndef TEST_MODULE_H_
#define TEST_MODULE_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#ifdef AVR_SUPPORT
#include <avr/pgmspace.h>
#endif

#define	Q(x)	#x
#ifdef AVR_SUPPORT
#define FUNC_NAME_GENERATOR_PGM(_NAME)		const char PROGMEM _NAME ## _name[] = Q(_NAME)
#endif
#define FUNC_NAME_GENERATOR(_NAME)		char _NAME ## _name[] = Q(_NAME)

struct UnitTest{
	const char* name;
	void (*unit_test)();
};

class Test{
private:
	template <typename t>
	bool is_mem_equal(t& a, t& b, uint16_t size=0);
	template <typename t>
	bool is_mem_equal(t* a, t* b, uint16_t size=0);
	void passed();
	void failed();
	UnitTest* unit_tests;
	uint16_t unit_tests_cnt;
	UnitTest* alloc_test();
	static uint16_t test_count;
	static uint16_t tests_passed;
	static uint16_t tests_failed;
public:
	//what kind of data is compared
	enum assert_type{
		numeric,
		character,
		string,
		array
	};
	template <typename t>
	void assertEqual(t& a, t& b, uint16_t size=0, assert_type type=numeric);
	template <typename t>
	void assertEqual(t* a, t* b, uint16_t size=0, assert_type type=numeric);
	void add_test(UnitTest& test);
	Test();
	 ~Test();
};

UnitTest* Test::alloc_test(){
	unit_tests_cnt++;
	unit_tests = (UnitTest*)realloc(unit_tests, unit_tests_cnt*sizeof(UnitTest));
	return unit_tests + unit_tests_cnt;
}

void Test::add_test(UnitTest& test){

	*alloc_test() = test;
}

template <typename t>
void Test:: assertEqual (t& a, t& b, uint16_t size, assert_type type){
	/*
	 * template for numeric types
	 */
	if(size > 0 and type != array)
		type = string;
	switch (type) {
		case numeric:
			printf("assert equal %ld=%ld ", (long int)a, (long int)b);
			break;
		case character:
			printf("assert equal %c=%c ", a, b);
			break;
		case string:
			printf("assert equal %s=%s ", (char*)a, (char*)b);
			break;
		default:
			break;
	}
	is_mem_equal(a, b, size) ? passed() : failed();
}

template <typename t>
void Test:: assertEqual (t* a, t* b, uint16_t size, assert_type type){
	if(size > 0 and type != array)
		type = string;
	switch (type) {
		case numeric:
			printf("assert equal %ld=%ld ", (long int)*a, (long int)*b);
			break;
		case character:
			printf("assert equal %c=%c ", *a, *b);
			break;
		case string:
			printf("assert equal %s=%s ", (char*)a, (char*)b);
			break;
		case array:
			printf("assert equal ");
			for(uint16_t i=0; i<size/sizeof(t); i++){
				printf("%d, ", a[i]);
			}
			printf(" = ");
			for(uint16_t i=0; i<size/sizeof(t); i++){
				printf("%d, ", b[i]);
			}
			break;
		default:
			break;
	}
	is_mem_equal(a, b, size) ? passed() : failed();
}

template <typename t>
bool Test::is_mem_equal(t& a, t& b, uint16_t size){
	if(not size)
		size = sizeof(t);
	test_count++;
	uint16_t memcmp_result = memcmp(&a, &b, size);
	if(memcmp_result == 0){
		tests_passed++;
		return true;
	}
	else{
		tests_failed++;
		return false;
	}
}

template <typename t>
bool Test::is_mem_equal(t* a, t* b, uint16_t size){
	if(not size)
		size = sizeof(t);
	test_count++;
	uint16_t memcmp_result = memcmp(a, b, size);
	if(memcmp_result == 0){
		tests_passed++;
		return true;
	}
	else{
		tests_failed++;
		return false;
	}
}

Test::Test(){
	printf("Initializing internal test module\n");
	test_count = 0;
	tests_passed = 0;
	tests_failed = 0;
	unit_tests_cnt = 0;
	unit_tests = (UnitTest*)malloc(unit_tests_cnt * sizeof(UnitTest));
};

Test::~Test(){
	printf("Test results:\n");
	printf("passed: %d \n", tests_passed);
	printf("failed: %d \n", tests_failed);
	printf("out of total tests: %d \n", test_count);
	free(unit_tests);
}

uint16_t Test::test_count = 0;
uint16_t Test::tests_passed = 0;
uint16_t Test::tests_failed = 0;

void Test::passed(){
	printf("passed\n\n");
}

void Test::failed(){
	printf("failed\n\n");
}

#endif /* TEST_MODULE_H_ */
