///
#define _POSIX_C_SOURCE 202001L
///

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<math.h>

///
#include <unistd.h>
#include <time.h>
///

#define OUTPUT_FILENAME "output.ppm"
#define HISTOGRAM_FILENAME "output.txt"

void error_file_format(){
    fprintf(stderr,"ERROR: Invalid image format!\n");
    exit(1); 
}

void error_input(){
    fprintf(stderr,"ERROR: Input error!\n");
    exit(2); 
}

typedef struct{
    unsigned char r,g,b;
} pixel;

typedef struct{
    int w,h,max_color;
    pixel *px;
} PPM_image;




PPM_image* alloc_img(int w, int h, int max_color){
    PPM_image *img = malloc(sizeof(PPM_image));
    img->px = malloc(w * h * sizeof(pixel));
    img->w = w;
    img->h = h;
    img->max_color = max_color;
    return img;
}

void free_img(PPM_image *img){
    free(img->px);
    free(img);
}



/*Loads image to file f*/
void write_image(FILE *f,PPM_image *img){

    fprintf(f, "P6\n");
    
    //image size
    fprintf(f, "%d %d\n",img->w,img->h);

    // rgb component depth
    fprintf(f, "%d\n",img->max_color);

    // pixel data
    fwrite(img->px, 3 * img->h, img->w, f);
    
}

void lightness(int y, int *arr){
    if(y <= 50){
        arr[0]++;
    }
    else if (y >= 51 && y <= 101)
    {
        arr[1]++;
    }
    else if (y >= 102 && y <= 152)
    {
        arr[2]++;
    }
    else if (y >= 153 && y <= 203){
        arr[3]++;
    }
    else if (y >= 204 && y <= 255)
    {
        arr[4]++;
    }
}


int main(int argc, char * *argv){
    ///
    struct timespec start, stop;
    clock_gettime( CLOCK_REALTIME, &start);
    ///

    if(argc <= 1){        
        error_input();
    }

    FILE *img_file = fopen(argv[1],"rb");
    FILE *img_file_out = fopen(OUTPUT_FILENAME,"wb");
    FILE *his_file = fopen(HISTOGRAM_FILENAME,"w");
    
    
    
    char format1,format2;
    int height,width,max_color;
    
    fscanf(img_file,"%c%c",&format1,&format2);
    if(format1 != 'P' || format2 != '6'){
        error_file_format();
    }
    fscanf(img_file,"%d %d %d",&width,&height,&max_color);
    while (fgetc(img_file) != '\n') ;



    PPM_image *img = alloc_img(width,height,max_color);
    fread(img->px, sizeof(pixel), img->w * img->h, img_file); //read image

    PPM_image *img_out = alloc_img(img->w,img->h,img->max_color);    
    memcpy(img_out->px,img->px,sizeof(pixel) * img->h * img->w);
    
    
    int hist[5] = {0};

    
    int diagonal = 2;
    int i = 1, j = 1;

    int r_c = 0, g_c = 0, b_c = 0;
    
    while (i < img->h-1 && j < img->w-1)
    {
        int right_bound = j + diagonal;
        int down_bound = i + diagonal;
        if((img->w-1) - right_bound < diagonal){
            right_bound = img->w-1;
        }
        if((img->h-1) - down_bound < diagonal){
            down_bound = img->h-1;
        }
        for(int k = i; k < down_bound; k++){
            for(int l = j; l < right_bound; l++){
                int grayscale = 0;
                //printf("%d %d\n", k,l);
                r_c = 0;
                g_c = 0;
                b_c = 0;
                
                r_c = -img->px[(k-1) * (img->w) + l].r - img->px[(k) * (img->w) + l-1].r -img->px[(k) * (img->w) + l+1].r -img->px[(k+1) * (img->w) + l].r + 5*img->px[(k) * (img->w) + l].r;
                r_c = (r_c < 0) ? 0 : r_c;
                r_c = (r_c > 255) ? 255 : r_c;

                img_out->px[(k) * (img->w) + l].r = r_c;
                grayscale+= 0.2126*img_out->px[(k) * (img->w) + l].r;

                g_c = -img->px[(k-1) * (img->w) + l].g - img->px[(k) * (img->w) + l-1].g - img->px[(k) * (img->w) + l+1].g - img->px[(k+1) * (img->w) + l].g +  5 * img->px[(k) * (img->w) + l].g;
                g_c = (g_c < 0) ? 0 : g_c;
                g_c = (g_c > 255) ? 255 : g_c;
                img_out->px[(k) * (img->w) + l].g = g_c;
                grayscale+= 0.7152*img_out->px[(k) * (img->w) + l].g ;

                b_c = -img->px[(k-1) * (img->w) + l].b - img->px[(k) * (img->w) + l-1].b - img->px[(k) * (img->w) + l+1].b - img->px[(k+1) * (img->w) + l].b + 5 * img->px[(k) * (img->w) + l].b;
                b_c = (b_c < 0) ? 0 : b_c;
                b_c = (b_c > 255) ? 255 : b_c;
                img_out->px[(k) * (img->w) + l].b = b_c;
                grayscale+= 0.0722*img_out->px[(k) * (img->w) + l].b;
                lightness(grayscale,hist);

                //lightness(round(0.2126*img_out->px[(k) * (img->w) + l].r + 0.7152*img_out->px[(k) * (img->w) + l].g + 0.0722*img_out->px[(k) * (img->w) + l].b),hist);                


                // r_c-= img->px[(k-1) * (img->w) + l].r;
                // g_c-= img->px[(k-1) * (img->w) + l].g; //up_color
                // b_c-= img->px[(k-1) * (img->w) + l].b;

                if(k == 1){
                    lightness(round(0.2126*img->px[l].r + 0.7152*img->px[l].g + 0.0722*img->px[l].b),hist);
                }

                // r_c-= img->px[(k) * (img->w) + l-1].r;
                // g_c-= img->px[(k) * (img->w) + l-1].g; //left_color
                // b_c-= img->px[(k) * (img->w) + l-1].b;

                if(l == 1){
                    lightness(round(0.2126*img->px[(k) * (img->w) + l-1].r + 0.7152*img->px[(k) * (img->w) + l-1].g + 0.0722*img->px[(k) * (img->w) + l-1].b),hist);
                }

                // r_c-= img->px[(k) * (img->w) + l+1].r;
                // g_c-= img->px[(k) * (img->w) + l+1].g; // right_color
                // b_c-= img->px[(k) * (img->w) + l+1].b;   
                if(l+1 == img->w-1){
                    lightness(round(0.2126*img->px[(k) * (img->w) + l+1].r + 0.7152*img->px[(k) * (img->w) + l+1].g + 0.0722*img->px[(k) * (img->w) + l+1].b),hist);
                }

                // r_c-= img->px[(k+1) * (img->w) + l].r;
                // g_c-= img->px[(k+1) * (img->w) + l].g; // down_color
                // b_c-= img->px[(k+1) * (img->w) + l].b;
                if(k+1 == img->h-1){
                    lightness(round(0.2126*img->px[(img->h-1) * (img->w) + l].r + 0.7152*img->px[(img->h-1) * (img->w) + l].g + 0.0722*img->px[(img->h-1) * (img->w) + l].b),hist);
                }
                
                // r_c+= 5 * img->px[(k) * (img->w) + l].r;
                // g_c+= 5 * img->px[(k) * (img->w) + l].g; // cur_color
                // b_c+= 5 * img->px[(k) * (img->w) + l].b;

                
                
                

                
                
                


                
                
                

                
                
            }
        }
        if(right_bound == img->w-1 && down_bound == img->h-1){
            break;
        }
        if(right_bound == img->w-1){
            i+=diagonal;
            j = 1;
        }
        else{
            j+=diagonal;
        }
    }
    
    //img_out->px[(i) * (img_out->w) + j].r = 255; //change red
    //img_out->px[(i) * (img_out->w) + j].g = 255; //change green
    //img_out->px[(i) * (img_out->w) + j].b = 255; //change blue

    lightness(round(0.2126*img->px[0].r + 0.7152*img->px[0].g + 0.0722*img->px[0].b),hist);                 //left up
    lightness(round(0.2126*img->px[img->w-1].r + 0.7152*img->px[img->w-1].g + 0.0722*img->px[img->w-1].b),hist);     //right up           
    lightness(round(0.2126*img->px[(img->h-1) * (img->w)].r + 0.7152*img->px[(img->h-1) * (img->w)].g + 0.0722*img->px[(img->h-1) * (img->w)].b),hist);
    lightness(round(0.2126*img->px[(img->h-1) * (img->w) + (img->w-1)].r + 0.7152*img->px[(img->h-1) * (img->w) + (img->w-1)].g + 0.0722*img->px[(img->h-1) * (img->w) + (img->w-1)].b),hist);


    
    


    //printing hist
    fprintf(his_file,"%d ",hist[0]);

    for(int i = 1; i < 5; i++){
        fprintf(his_file," %d",hist[i]);
    }
    


    write_image(img_file_out,img_out);

    free_img(img_out);
    free_img(img);
    
    fclose(img_file);
    fclose(img_file_out);
    fclose(his_file);

    ///
    clock_gettime( CLOCK_REALTIME, &stop);
    double accum = ( stop.tv_sec - start.tv_sec )*1000.0 + ( stop.tv_nsec - start.tv_nsec )/ 1000000.0;
    printf( "Time: %.6lf ms\n", accum );
    ///
    return 0;
}
