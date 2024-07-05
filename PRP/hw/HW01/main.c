#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int in1 = 0, in2 = 0;
  scanf("%d %d", &in1, &in2);
  if(in1 > 10000 || in1 < -10000 || in2 > 10000 || in2 < -10000){
    printf("Vstup je mimo interval!\n");
    return 0;
  }
  printf("Desitkova soustava: %d %d\n", in1, in2);
  printf("Sestnactkova soustava: %x %x\n", in1, in2);
  printf("Soucet: %d + %d = %d\n", in1, in2, in1+in2);
  printf("Rozdil: %d - %d = %d\n", in1, in2, in1-in2);
  printf("Soucin: %d * %d = %d\n",in1, in2, in1 * in2);
  if( in2 != 0){
    printf("Podil: %d / %d = %d\n", in1, in2, in1 / in2);
  }
  else{
    printf("Nedefinovany vysledek!\n");
  }
  double prumer = (in1 + in2) / 2.0;
  printf("Prumer: %.1f\n", prumer);

  return 0;
}
