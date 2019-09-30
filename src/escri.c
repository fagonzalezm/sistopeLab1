#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <png.h>
#include <errno.h>
#include <stdarg.h>

typedef struct pixelMatrix{
    int n;
    int m;
    int matrix[600][600];
}pixelMatrix;

typedef struct
{
    u_int8_t color;
}
pixel_t;

    
typedef struct
{
    pixel_t *pixels;
    size_t width;
    size_t height;
}
bitmap_t;
    


static pixel_t * pixel_at (bitmap_t * bitmap, int x, int y)
{
    return bitmap->pixels + bitmap->width * y + x;
}

static int save_png_to_file (bitmap_t *bitmap, const char *path)
{
    FILE * fp;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    size_t x, y;
    png_byte ** row_pointers = NULL;
   
    int status = -1;
    
    int pixel_size = 3;
    int depth = 8;
    
    fp = fopen (path, "wb");
    if (! fp) {
        goto fopen_failed;
    }

    png_ptr = png_create_write_struct (PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (png_ptr == NULL) {
        goto png_create_write_struct_failed;
    }
    
    info_ptr = png_create_info_struct (png_ptr);
    if (info_ptr == NULL) {
        goto png_create_info_struct_failed;
    }
    

    if (setjmp (png_jmpbuf (png_ptr))) {
        goto png_failure;
    }
    

    png_set_IHDR (png_ptr,
                  info_ptr,
                  bitmap->width,
                  bitmap->height,
                  depth,
                  PNG_COLOR_TYPE_GRAY,
                  PNG_INTERLACE_NONE,
                  PNG_COMPRESSION_TYPE_DEFAULT,
                  PNG_FILTER_TYPE_DEFAULT);
    

    row_pointers = png_malloc (png_ptr, bitmap->height * sizeof (png_byte *));
    for (y = 0; y < bitmap->height; y++) {
        png_byte *row = 
            png_malloc (png_ptr, sizeof (u_int8_t) * bitmap->width);
        row_pointers[y] = row;
        for (x = 0; x < bitmap->width; x++) {
            pixel_t * pixel = pixel_at (bitmap, x, y);
            *row++ = pixel->color;
        }
    }
    

    png_init_io (png_ptr, fp);
    png_set_rows (png_ptr, info_ptr, row_pointers);
    png_write_png (png_ptr, info_ptr, PNG_TRANSFORM_IDENTITY, NULL);

    

    status = 0;
    
    for (y = 0; y < bitmap->height; y++) {
        png_free (png_ptr, row_pointers[y]);
    }
    png_free (png_ptr, row_pointers);
    
 png_failure:
 png_create_info_struct_failed:
    png_destroy_write_struct (&png_ptr, &info_ptr);
 png_create_write_struct_failed:
    fclose (fp);
 fopen_failed:
    return status;
}



static int pix (int value, int max)
{
    if (value < 0) {
        return 0;
    }
    return (int) (256.0 *((double) (value)/(double) max));
}


int main(){


    bitmap_t pngOut;
    int xg;
    int yg;
    int status;

    status = 0;

    /* Se crean las struct */

    pngOut.width = matrizPix.n;
    pngOut.height = matrizPix.m;

    pngOut.pixels = calloc (pngOut.width * pngOut.height, sizeof (pixel_t));

    if (! pngOut.pixels) {
    return -1;
    }
    //Se recorre y crea una matriz con pixeles
    for (yg = 0; yg < pngOut.height; yg++) {
        for (xg = 0; xg < pngOut.width; xg++) {
            pixel_t * pixel = pixel_at (& pngOut, xg, yg);
            pixel->color = matrizPix.matrix[yg][xg];
        }
    }

    /* Se escribe la imagen */

    if (save_png_to_file (& pngOut, "test.png")) {
        fprintf (stderr, "Error escribiendo archivo.\n");
        status = -1;
    }

    free (pngOut.pixels);
    return status;

}