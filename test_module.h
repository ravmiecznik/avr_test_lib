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

#define	Q(x)	#x
#define FUNC_NAME_GENERATOR(_NAME)	char _NAME ## _name[] = Q(_NAME)

class Test{
private:
	template <typename t>
	bool is_mem_equal(t& a, t& b, uint16_t size=0);
	template <typename t>
	bool is_mem_equal(t* a, t* b, uint16_t size=0);
	void passed();
	void failed();
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
	Test();
	 ~Test();
};

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

#endif /* TEST_MODULE_H_ */
