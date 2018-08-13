//Implement a C function called ex1 such that it achieves 
//the same functionality as the machine code of objs/ex1_sol.o
//Note: you need to figure out ex1()'s function signature yourself; 
//the signature is not void ex1()
  
#include <assert.h>
/*
*  0:	48 01 fe             	add    %rdi,%rsi
   3:	48 39 d6             	cmp    %rdx,%rsi
   6:	75 06                	jne    e <ex1+0xe>
   8:	b8 01 00 00 00       	mov    $0x1,%eax
   d:	c3                   	retq   
   e:	b8 00 00 00 00       	mov    $0x0,%eax
  13:	c3                   	retq   
*/

long long
ex1(long long x, long long y, long long z) {
	return (x+y!=z)? 0:1;
}
