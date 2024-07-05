#include <stdio.h>
#include <stdlib.h>

int main(int argc, char *argv[])
{
  int width, height;
  
  int ret = scanf("%d %d", &width, &height);

  if(ret != 2){
    fprintf(stderr,"Error: Chybny vstup!\n");
    return 100;
  }
  if(width < 3 || width > 69 || height < 3 || height > 69){
    fprintf(stderr, "Error: Vstup mimo interval!\n");
    return 101;
  }
  if(width % 2 != 1){
    fprintf(stderr,"Error: Sirka neni liche cislo!\n");
    return 102;
  }

  int fence = -1;
  if(width == height){
    ret = scanf("%d", &fence);
    if(ret != 1){
      fprintf(stderr,"Error: Chybny vstup!\n");
      return 100;
    }
    if(fence < 1 || fence >= height){
      fprintf(stderr,"Error: Neplatna velikost plotu!\n");
      return 103;
    }
  }

  int roof_sp_left = width / 2, roof_sp_right = width / 2; //roof space outside and inside
  int fence_cnt = 0;

  
  for(int i = 0; i < height + width / 2 ; i++){
    for(int j = 0; j < width; j++)
    {
      if(j == roof_sp_left || j == roof_sp_right || i == width / 2 || i == height + width / 2 - 1)
      {
        printf("X");
        if( fence != -1 && j+1 == width && i >= width / 2 + (height - fence)){
          char space = ' ';
          if(i == height + width / 2 - 1 || i+1 == height + (width + 1) / 2 - fence){
            space = '-';
          }
          
          for(int k = fence % 2; k < fence; k++){
            if(k % 2 == 0){
              printf("%c",space);
            }
            else{
              printf("|");
            }
          }
          if(fence % 2 == 1){
            printf("|");
          }
          if(space == '-'){
            space = ' ';
          }
        }
        if(j == roof_sp_right){
          break;
        }
      }
      else if(i < width / 2 || fence == -1){
        printf(" ");
      }
      else{
        if(fence_cnt % 2 == 0){
          printf("o");
        }
        else{
          printf("*");
        }
        fence_cnt++;
      }

    }
    if(roof_sp_left != 0 ){
      roof_sp_left--;
      roof_sp_right++;
    }

    printf("\n");
  }


  return 0;
}

