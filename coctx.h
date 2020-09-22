/*
* Tencent is pleased to support the open source community by making Libco available.

* Copyright (C) 2014 THL A29 Limited, a Tencent company. All rights reserved.
*
* Licensed under the Apache License, Version 2.0 (the "License"); 
* you may not use this file except in compliance with the License. 
* You may obtain a copy of the License at
*
*	http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, 
* software distributed under the License is distributed on an "AS IS" BASIS, 
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. 
* See the License for the specific language governing permissions and 
* limitations under the License.
*/

#ifndef __CO_CTX_H__
#define __CO_CTX_H__
#include <stdlib.h>
typedef void* (*coctx_pfn_t)( void* s, void* s2 );
struct coctx_param_t
{
	const void *s1;
	const void *s2;
};

/*
 * x86-64体系寄存器, 向下兼容(https://wiki.osdev.org/CPU_Registers_x86-64)
 * 63                 31       15   7    0(bit position)
 * |------------------|--------|----|----|  x86_64
 *                    |--------|----|----|  i386/x86
 *                             |---------|  8086
 *                             |----|----|  8086
 * |---------------- RAX ----------------| (1. Main Register: Accumulator Register)
 *                    |------- EAX ------| 
 *                             |-- AX ---|
 *                             |-AH-|-AL-|
 * |---------------- RBX ----------------| (2. Main Register: Base Register)
 *                    |------- EBX ------|
 *                             |-- BX ---|
 *                             |-BH-|-BL-|
 * |---------------- RCX ----------------| (3. Main Register: Count Register)
 *                    |------- ECX ------| 
 *                             |-- CX ---|
 *                             |-CH-|-CL-|
 * |---------------- RDX ----------------| (4. Main Register: Data Register)
 *                    |------- EDX ------| 
 *                             |-- DX ---|
 *                             |-DH-|-DL-|
 * |---------------- RSI ----------------| (5. Index Register: Source Index)
 *                    |------- ESI ------| 
 *                             |-- SI ---| 
 *                                  |SIL-| 
 * |---------------- RDI ----------------| (6. Index Register: Destination Index)
 *                    |------- EDI ------|
 *                             |-- DI ---| 
 *                                  |DIL-| 
 * |---------------- RBP ----------------| (7. Index Register: Base Pointer)
 *                    |------- EBP ------|
 *                             |-- BP ---| 
 *                                  |BPL-| 
 * |---------------- RSP ----------------| (8. Index Register: Stack Pointer)
 *                    |------- ESP ------|
 *                             |-- SP ---| 
 *                                  |SPL-| 
 * |---------------- R8 -----------------| (9. 只在x86-84存在)
 *                    |------- R8D ------|
 *                             |-- R8W --| 
 *                                  |R8B-| 
 *                  ...
 * |---------------- R15 ----------------| (16. 只在x86-84存在)
 *                    |------- R15D -----|
 *                             |-- R15W -| 
 *                                  |R15B| 
 * |---------------- RIP ----------------| (17. Program Counter)
 *                    |------- EIP ------| 
 *                             |-- IP ---| 
 * |-------------- EFlags ---------------| (18. EFlags)
 *                    |----- EFlags -----| 
 *                             |-- CS ---| (19. Segment Register: Code Segment)
 *                             |-- DS ---| (20. Segment Register: Data Segment)
 *                             |-- ES ---| (21. Segment Register: Extra Segment(used for string operations))
 *                             |-- FS ---| (22. Segment Register: General-purpose Segment)
 *                             |-- GS ---| (23. Segment Register: General-purpose Segment)
 *                             |-- SS ---| (24. Segment Register: Stack Segment)
 *
 * Others: Control Registers(CR0-CR4, CR8), Model-Specific Register, Debug Registers(DR0-DR3, DR6, DR7), Test Registers(TR3-TR5, TR6, TR7), GDTR, LDTR, TR, IDTR
 *
 */
struct coctx_t
{
#if defined(__i386__)
	void *regs[ 8 ];    //详见coctx.cpp
#else
	void *regs[ 14 ];   //详见coctx.cpp, R10, R11为callee saved register, 由被调用函数保存
#endif
	size_t ss_size;     //协程栈剩余大小
	char *ss_sp;        //协程栈栈底地址
	
};

int coctx_init( coctx_t *ctx );

/*
 * @param
 * ctx  :指向上下文结构体
 * pfn  :指向调用函数
 * s    :参数1
 * s1   :参数2           
 */
int coctx_make( coctx_t *ctx,coctx_pfn_t pfn,const void *s,const void *s1 );
#endif
