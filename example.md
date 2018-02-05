/*
 * example.cpp
 *
 *  Created on: 5 lut 2018
 *      Author: miecznik
 */


#include "test_module.h"
#include <stdio.h>
#include <string.h>

class cbufferTest: Test{
public:
	void test1(){
		int a=1, b=1;
		//assertEqual<int>(a, b);
	}
};

int main(){
	Test t;
	uint16_t size = 100;
	char tmp[size] = "abc";
	char tmp2[size] = "abc";
	int64_t arr_a[10] = {1,2,3,4,5,6,7,8,9,10};
	int64_t arr_b[10] = {1,2,3,4,5,6,7,8,9,10};
	int64_t arr_c[10] = {2,2,3,4,5,6,7,8,9,10};
	int a=2, b=3;
	t.assertEqual(a, b);
	t.assertEqual(tmp, tmp2, strlen(tmp));
	t.assertEqual("rafal", "rafal", strlen("rafal"));
	t.assertEqual(arr_a, arr_c, sizeof(arr_a), Test::array);
	t.assertEqual(arr_a, arr_b, sizeof(arr_a), Test::array);
}
