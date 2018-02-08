/*
 * test_module.h
 *
 *  Created on: 5 lut 2018
 *      Author: miecznik
 */

#ifndef TEST_MODULE_H_
#define TEST_MODULE_H_

#define AVR_SUPPORT

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "stdlib.h"

#ifdef AVR_SUPPORT
#include <avr/pgmspace.h>
#endif

#define	Q(x)	#x
#ifdef AVR_SUPPORT
#define FUNC_NAME_GENERATOR_PGM(_NAME)		const char PROGMEM _NAME ## _name[] = Q(_NAME)
#endif
#define FUNC_NAME_GENERATOR(_NAME)		char _NAME ## _name[] = Q(_NAME)

void print_test_name(const char* test_name){
#ifdef AVR_SUPPORT
	printf_P(test_name);
#else
	printf(test_name);
#endif
	printf("\n");
}

struct UnitTest{
	const char* name;
	void (*unit_test)();
};

class Test{
public:
	//what kind of data is compared
	enum variable_type{
		numeric,
		character,
		string,
		array
	};
	template <typename t>
	void assertEqual(const char* test_name, t* a, t* b, variable_type type=numeric, uint16_t size=0);
	template <typename t>
	void assertEqual(const char* test_name, t a, t b, variable_type type=numeric, uint16_t size=0);
	template <typename t>
	void assertNotEqual(const char* test_name, t* a, t* b, variable_type type=numeric, uint16_t size=0);
	template <typename t>
	void assertNotEqual(const char* test_name, t a, t b, variable_type type=numeric, uint16_t size=0);
	void add_test(UnitTest& test);
	Test();
	 ~Test();
private:
	template <typename t>
	bool is_mem_equal(t& a, t& b, uint16_t size=0);
	template <typename t>
	bool is_mem_equal(t* a, t* b, uint16_t size=0);
	UnitTest* unit_tests;
	uint16_t unit_tests_cnt;
	UnitTest* alloc_test();
	static uint16_t test_count;
	static uint16_t tests_passed;
	static uint16_t tests_failed;
	//const strings
	static const char passed_s[];
	static const char failed_s[];
	static const char assert_eq_cond_s[];
	static const char assert_not_eq_cond_s[];
	template <typename t>
	void print_ut_result(t& val_a, t& val_b, char* assertion_type, char* assert_eq_cond, char* result, variable_type type, uint16_t size=0);
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
void Test:: assertEqual (const char* test_name, t* a, t* b, variable_type type, uint16_t size){
	print_test_name(test_name);
	char* result;
	char* assert_eq_cond;
	char assertion_type[] = "assert equal";
	if(is_mem_equal(a, b, size)){
		result=(char*)passed_s;
		assert_eq_cond=(char*)assert_eq_cond_s;
		tests_passed++;
	}
	else{
		result=(char*)failed_s;
		assert_eq_cond=(char*)assert_not_eq_cond_s;
		tests_failed++;
	}
	if(size > 0 and type != array)
		type = string;
	if(type != array)
		print_ut_result(a, b, assertion_type, assert_eq_cond, result, type, size);
	else
		print_ut_result( a[0], b[0], assertion_type, assert_eq_cond, result, type, size);
}

template <typename t>
void Test:: assertEqual (const char* test_name, t a, t b, variable_type type, uint16_t size){
	print_test_name(test_name);
	char* result;
	char* assert_eq_cond;
	char assertion_type[] = "assert equal";
	if(is_mem_equal(a, b, size)){
		result=(char*)passed_s;
		assert_eq_cond=(char*)assert_eq_cond_s;
		tests_passed++;
	}
	else{
		result=(char*)failed_s;
		assert_eq_cond=(char*)assert_not_eq_cond_s;
		tests_failed++;
	}
	if(size > 0 and type != array)
		type = string;
	print_ut_result(a, b, assertion_type, assert_eq_cond, result, type, size);
}

template <typename t>
void Test:: assertNotEqual (const char* test_name, t* a, t* b, variable_type type, uint16_t size){
	print_test_name(test_name);
	char* result;
	char* assert_eq_cond;
	char assertion_type[] = "assert not equal";
	if(not is_mem_equal(a, b, size)){
		result=(char*)passed_s;
		assert_eq_cond=(char*)assert_not_eq_cond_s;
		tests_passed++;
	}
	else{
		result=(char*)failed_s;
		assert_eq_cond=(char*)assert_eq_cond_s;
		tests_failed++;
	}
	if(size > 0 and type != array)
		type = string;
	if(type != array)
		print_ut_result(a, b, assertion_type, assert_eq_cond, result, type, size);
	else
		print_ut_result(a, b, assertion_type, assert_eq_cond, result, type, size);
}

template <typename t>
void Test:: assertNotEqual (const char* test_name, t a, t b, variable_type type, uint16_t size){
	print_test_name(test_name);
	char* result;
	char* assert_eq_cond;
	char assertion_type[] = "assert not equal";
	if(not is_mem_equal(a, b, size)){
		result=(char*)passed_s;
		assert_eq_cond=(char*)assert_eq_cond_s;
		tests_passed++;
	}
	else{
		result=(char*)failed_s;
		assert_eq_cond=(char*)assert_not_eq_cond_s;
		tests_failed++;
	}
	if(size > 0 and type != array)
		type = string;
	print_ut_result(a, b, assertion_type, assert_eq_cond, result, type, size);
}

template <typename t>
bool Test::is_mem_equal(t& a, t& b, uint16_t size){
	if(not size)
		size = sizeof(t);
	test_count++;
	uint16_t memcmp_result = memcmp(&a, &b, size);
	if(memcmp_result == 0){
		return true;
	}
	else{
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
		return true;
	}
	else{
		return false;
	}
}

template <typename t>
void Test::print_ut_result(t& val_a, t& val_b, char* assertion_type, char* assert_eq_cond, char* result, variable_type type, uint16_t size){
	switch (type) {
		case numeric:
			printf("%s %ld %s %ld ", assertion_type, (long int)val_a, assert_eq_cond, (long int)val_b);
			break;
		case character:
			printf("%s %c %s %c ", assertion_type, val_a, assert_eq_cond, val_b);
			break;
		case string:
			printf("%s %s %s %s ", assertion_type, val_a, assert_eq_cond, val_b);
			break;
		case array:
			{	t* pa = &val_a;
				t* pb = &val_b;
				printf("%s ", assertion_type);
				for(uint16_t i=0; i<size/sizeof(t); i++){
					printf("%d, ", pa[i]);
				}
				printf(" %s ", assert_eq_cond);
				for(uint16_t i=0; i<size/sizeof(t); i++){
					printf("%d, ", pb[i]);
				}
			}
			break;
		default:
			break;
	}
	printf("%s\n\n", result);
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
const char Test::passed_s[] = "passed";
const char Test::failed_s[] = "failed";
const char Test::assert_eq_cond_s[] = "==";
const char Test::assert_not_eq_cond_s[] = "!=";

#endif /* TEST_MODULE_H_ */
