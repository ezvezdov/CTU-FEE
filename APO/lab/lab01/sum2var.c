//#include <stdio.h>

int var_a = 0x1234;
int var_b = 0x2222;

int var_c = 0x3333;

int main()
{
  var_c = var_a + var_b;

  //printf("sum %d + %d -> %d\n", var_a, var_b, var_c);

  //printf("sum 0x%x + 0x%x -> 0x%x\n", var_a, var_b, var_c);

  return 0;
}