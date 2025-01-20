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
#include <arm_cmse.h>
#include <interface.h>
#include <stdlib.h>

/* typedef for Non-secure callback function */
typedef funcptr funcptr_NS __attribute__((cmse_nonsecure_call));


funcptr_NS callback_NS = (funcptr_NS)0xFFFFFFFF;

/**
  \brief        secure function exported to Non-secure side
  \details      support four integer parameters, add the received parameters and return the
                result
  \param [in]   arg1-4 integer parameters
 */
float __attribute__((cmse_nonsecure_entry)) ns_callable_fn1(int32_t a, int32_t b, int32_t c,
         float d)
{
  float ret = 0.0f;
  ret  = a + b + c + d;
  return ret;
}

/**
  \brief        secure function exported to Non-secure side
  \details      support a structure pointer as an input parameter that points to a structure
                with more than four elements
                call a CMSE intrinsic to check the structure read/write permission from
                Non-secure state
                sum up the structure elements and return the sum result
  \param [in]   pInParams   a structure pointer
 */
int32_t __attribute__((cmse_nonsecure_entry)) ns_callable_fn2(S* ptr)
{

  int32_t ret = 0;
  S*  in_check = NULL;

  /* check the memory pointed to by the pointer support read/write permission from Non-secure
   * state   */

  in_check = cmse_check_pointed_object(ptr,CMSE_NONSECURE | CMSE_MPU_READ);
  if (in_check == NULL) {
    printf("S: Non-secure access to the data structure is not permitted \n\r");
    exit(0);
  }
  else {
    ret = in_check->a + in_check->b + in_check->c + in_check->d + in_check->e;
    return ret;
  }
}

/**
  \brief        secure function exported to Non-secure side
  \details      support a function pointer type parameter
                calls a CMSE intrinsic to create a Non-secure function pointer with the lease
                significant bit (LSB) cleared
                saves the Non-secure function pointer to a global callback function pointer
                (callback_NS)
  \param [in]   a function pointer type parameter
 */
void __attribute__((cmse_nonsecure_entry)) ns_callable_init(funcptr callback)
{
  callback_NS = (funcptr_NS)cmse_nsfptr_create(callback);
}


/**
  \brief        secure function exported to Non-secure side
  \details      supports a data pointer type and length parameters
                call a CMSE intrinsic to check the address range has read permission from
                Non-secure state process data block after checking
                call the Non-secure callback function to pass the processing result to
                Non-secure side
  \param [in]   pIn    data pointer
  \param [in]   size   data block length
  */
void  __attribute__((cmse_nonsecure_entry)) ns_callable_fn3(volatile uint32_t* ptr,
           uint32_t size)
{
  uint32_t* in_check = NULL;
  uint32_t i = 0;
  int32_t ret = 0;

  printf("S: check Non-secure permission to read the data region \n\r");

  /* check the address range has read permission from Non-secure state */
  in_check = cmse_check_address_range((void*)ptr,size,CMSE_NONSECURE | CMSE_MPU_READ);

  if (in_check == NULL) {
      printf("S: Non-secure read access to the data region 0x%x - 0x%x is not permitted \n\r",
            (unsigned int)ptr, ((unsigned int)(ptr + size))-1);
      exit(0);
  }
  else{
    printf("S: process Non-secure data in Secure side\n\r");
    for(i = 0 ; i < size; i++)
    {
        ret += *in_check;
        in_check++;
    }

    /* call the callback function to return the result back to Non-secure side */
    callback_NS(ret);
  }
}

