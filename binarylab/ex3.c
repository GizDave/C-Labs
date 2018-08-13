//Implement a C function called ex3 such that it achieves 
//the same functionality as the machine code of objs/ex3_sol.o
//Note: you need to figure out ex3()'s function signature yourself; 
//the signature is not void ex3()
  
#include <assert.h>

/*
   0:	b9 00 00 00 00       	mov    $0x0,%ecx 
   5:	eb 1b                	jmp    22 <ex3+0x22> 
   7:	48 63 c1             	movslq %ecx,%rax 
   a:	4c 8d 04 07          	lea    (%rdi,%rax,1),%r8 
   e:	45 0f b6 08          	movzbl (%r8),%r9d 
  12:	48 01 f0             	add    %rsi,%rax 
  15:	44 0f b6 10          	movzbl (%rax),%r10d 
  19:	45 88 10             	mov    %r10b,(%r8) 
  1c:	44 88 08             	mov    %r9b,(%rax) 
  1f:	83 c1 01             	add    $0x1,%ecx 
  22:	39 d1                	cmp    %edx,%ecx 
  24:	7c e1                	jl     7 <ex3+0x7> 
  26:	f3 c3                	repz retq 
*/

char *
ex3(long long rdi, long long rsi, int edx, int ecx, char *r8, int r9, char r10) {
	char *rax;
	//r8=(int *)malloc(sizeof(int *));
	//rax=(int *)malloc(sizeof(int *));

	for(ecx=0; ecx<edx; ecx++){
		rax=ecx;
		r8=rdi+rax;
		r9=*r8;
		rax+=rsi;
		r10=*rax;
		*r8=r10;
		*rax=r9;
	}
	return rax;
}
