/*
 *
 * MIT License
 * Copyright (c) 2025 ARM Limited.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */


#include <stdio.h>
#include "callback_ns.h"
#include "interface.h"


int main(void)
{
  /* ===========================================
    * This Non-secure example demonstrates function calls from Non-secure to Secure state:
    * Implement a Non-secure function and pass the function pointer to Secure side
    * Call Secure functions
     =========================================== */

  S test;
  float   fresult = 0.0f;
  float  fdata = 5.8f;
  int32_t result = 0;
  uint32_t data[]={1,2,3,4,5};
  int32_t len = 0;

  test.a = 1;
  test.b = 2;
  test.c = 3;
  test.d = 4;
  test.e = 5;

  printf("NS: Hello World in Non-secure State \n\r");
  printf("NS: call Secure function\n\r");
  fresult = ns_callable_fn1(test.a, test.b, test.c, fdata);
  printf("NS: get add result from Secure side: %d + %d + %d + %f = %f\n\r",
         test.a,test.b,test.c,fdata,fresult);


  printf("NS: call Secure function with more input parameters\n\r");
  result = ns_callable_fn2(&test);
  printf("NS: get add result from Secure side: %d + %d + %d + %d + %d = %d\n\r",
         test.a,test.b,test.c,test.d, test.e,result);


  printf("NS: call Secure function with Non-secure function pointer as input parameter \n\r");
  ns_callable_init(func_ns);

  printf("NS: call Secure function with Non-secure data pointer as input parameter \n\r");
  len = sizeof(data)/sizeof(uint32_t);
  ns_callable_fn3(data,len);

  printf("Example Project: security-func-call-params-passing End\n");

  return 0;

}

