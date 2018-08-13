//Implement a C function called ex2 such that it achieves 
//the same functionality as the machine code of objs/ex2_sol.o
//Note: you need to figure out ex2()'s function signature yourself; 
//the signature is not void ex2()
  
#include <assert.h>
#include <string.h>
/*
   0:	b8 00 00 00 00       	mov    $0x0,%eax
   5:	ba 00 00 00 00       	mov    $0x0,%edx
   a:	eb 0b                	jmp    17 <ex2+0x17>
   c:	80 f9 61             	cmp    $0x61,%cl
   f:	75 03                	jne    14 <ex2+0x14>
  11:	83 c0 01             	add    $0x1,%eax
  14:	83 c2 01             	add    $0x1,%edx
  17:	48 63 ca             	movslq %edx,%rcx
  1a:	0f b6 0c 0f          	movzbl (%rdi,%rcx,1),%ecx
  1e:	84 c9                	test   %cl,%cl
  20:	75 ea                	jne    c <ex2+0xc>
  22:	f3 c3                	repz retq 
*/

int
ex2(char *arr) {
	int len=strlen(arr);
	int num=0;

	for(int i=0;i<len;i++)
		if(arr[i]==0x61)
			num++;
    
	return num;
}
