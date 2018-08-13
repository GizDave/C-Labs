//Implement a C function called ex4 such that it achieves 
//the same functionality as the machine code of objs/ex4_sol.o
//Note: you need to figure out ex4()'s function signature yourself; 
//the signature is not void ex4()
  
#include <assert.h>

/*
   0:	48 83 ff 01          	cmp    $0x1,%rdi
   4:	7e 23                	jle    29 <ex4+0x29>
   6:	55                   	push   %rbp
   7:	53                   	push   %rbx
   8:	48 83 ec 08          	sub    $0x8,%rsp
   c:	48 89 fb             	mov    %rdi,%rbx
   f:	48 8d 7f ff          	lea    -0x1(%rdi),%rdi
  13:	e8 00 00 00 00       	callq  18 <ex4+0x18>
  18:	48 89 c5             	mov    %rax,%rbp
  1b:	48 8d 7b fe          	lea    -0x2(%rbx),%rdi
  1f:	e8 00 00 00 00       	callq  24 <ex4+0x24>
  24:	48 01 e8             	add    %rbp,%rax
  27:	eb 04                	jmp    2d <ex4+0x2d>
  29:	48 89 f8             	mov    %rdi,%rax
  2c:	c3                   	retq   
  2d:	48 83 c4 08          	add    $0x8,%rsp
  31:	5b                   	pop    %rbx
  32:	5d                   	pop    %rbp
  33:	c3                   	retq  
*/

long long
ex4(long long rdi) {
	return (rdi<=1)?rdi:(ex4(rdi-1)+ex4(rdi-2));
}
