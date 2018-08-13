//Implement a C function called ex5 such that it achieves 
//the same functionality as the machine code of objs/ex5_sol.o
//Note: you need to figure out ex5()'s function signature yourself; 
//the signature is not void ex5(node *n). However, as a hint, 
//ex5 does take node * as an argument.
 
#include <assert.h>
#include <stdlib.h>
#include "ex5.h"

/*
   0:	48 85 ff             	test   %rdi,%rdi
   3:	75 0d                	jne    12 <ex5+0x12>
   5:	eb 1a                	jmp    21 <ex5+0x21>
	//LOOP
   7:	48 8b 17             	mov    (%rdi),%rdx
   a:	48 89 c7             	mov    %rax,%rdi
   d:	48 3b 10             	cmp    (%rax),%rdx or (rdi)
  10:	7f 15                	jg     27 <ex5+0x27>
	//if NOT NULL
  12:	48 8b 47 08          	mov    0x8(%rdi),%rax
  16:	48 85 c0             	test   %rax,%rax
  19:	75 ec                	jne    7 <ex5+0x7>
  1b:	b8 01 00 00 00       	mov    $0x1,%eax
  20:	c3                   	retq   
	//if NULL
  21:	b8 01 00 00 00       	mov    $0x1,%eax
  26:	c3                   	retq   

  27:	b8 00 00 00 00       	mov    $0x0,%eax
  2c:	c3                   	retq   
*/

long
ex5(node *rdi, long rsi, int rdx) {
	if(rdi!=NULL){
		node *rax=rdi->next;

		while(rax!=NULL){
			if(rdi->value>rax->value)
				return 0;
			rax=rax->next;
		}
		return 1;
	}
	else
		return 1;
}
