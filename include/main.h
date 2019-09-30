
typedef struct words {
	char word[100];
}words;

typedef struct pixelMatrix{
	int n;
	int m;
	int matrix[600][600];
}pixelMatrix;

typedef struct floatPixelMatrix{
	int nearlyBlack;
	int n;
	int m;
	float matrix[600][600];
}floatPixelMatrix;

typedef struct kernelMatrix{
	int matrix[3][3];
}kernelMatrix;


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

//pixelMatrix pngRead(char * fileName);
//void pipeline(int cValue, char * mValue, int nValue, int bFlag);