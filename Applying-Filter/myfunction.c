#include <stdbool.h>
#define MAX(a,b) (a > b ? a : b)
#define MIN(a,b) (a < b ? a : b)

// struct for pixel
typedef struct {
    unsigned char red;
    unsigned char green;
    unsigned char blue;
} pixel;

// struct for pixel_sum
typedef struct {
    int i_red;
    int i_green;
    int i_blue;
} pixel_sum;

/*
 * This function for each pixel in the relevant field
 * (in the target image is considered without the first and last rows and the first
 * and last columns) the nine pixels around it with the sharpen mask.
 * Let us note that the sharpen is built so that in this matrix all the indexes except
 * the middle one are minus one so we will subtract this manually from the amount
 * and add 9 times the middle index as the mask defines.
 */
void smoothSharp(pixel *src, pixel *dst) {

    pixel return_pixel2;
    pixel_sum sum2;

    pixel a, b, c, d, e, f, g, h, q;
    int i, j, k = m - 1;
    int spot = 0;
    int v;
    for (i = 1; i < k; ++i) {
        spot += m;
        for (j = 1; j < k; ++j) {
            v = spot + j;
            a = src[v - m - 1];
            b = src[v - m];
            c = src[v - m + 1];
            d = src[v -1];
            e = src[v];
            f = src[v + 1];
            g = src[v + m - 1];
            h = src[v + m];
            q = src[v + m + 1];

            sum2.i_red = sum2.i_green = sum2.i_blue = 0;

            sum2.i_red -= ((int) a.red + (int) b.red + (int) c.red + (int) d.red + (int) f.red
                           + (int) g.red + (int) h.red + (int) q.red);
            sum2.i_green -=  ((int) a.green + (int) b.green + (int) c.green + (int) d.green + (int) f.green
                              + (int) g.green + (int) h.green + (int) q.green);
            sum2.i_blue -=  ((int) a.blue + (int) b.blue + (int) c.blue + (int) d.blue + (int) f.blue
                             + (int) g.blue + (int) h.blue + (int) q.blue);

            sum2.i_red +=  (int) e.red * 9;
            sum2.i_green +=  (int) e.green * 9;
            sum2.i_blue +=  (int) e.blue * 9;

            if (sum2.i_red < 0)
                return_pixel2.red = (unsigned char) 0;
            else
                return_pixel2.red = (unsigned char) MIN(sum2.i_red, 255);

            if (sum2.i_green < 0)
                return_pixel2.green = (unsigned char) 0;
            else
                return_pixel2.green = (unsigned char) MIN(sum2.i_green, 255);

            if (sum2.i_blue < 0)
                return_pixel2.blue = (unsigned char) 0;
            else
                return_pixel2.blue = (unsigned char) MIN(sum2.i_blue, 255);
            dst[spot + j] = return_pixel2;
        }
    }
}

/*
 * This function works similar to the Bloor function but in addition it finds the pixel
 * whose value is lowest and highest in its RGB calculation. We will subtract these pixels
 * from the amount and thus judge the image with a filter.
 */
void smoothBlurFiltered(pixel *src, pixel *dst) {

    int spott;
    int maxi, mini, max_spot, min_spot;
    int one, two, three;
    pixel a, b, c, d, e, f, g, h, q;

    pixel return_pixel3;
    pixel_sum sum3;

    int i, j,v,  k = m - 1;
    int spot = 0;

    for (i = 1; i < k; ++i) {
        spot += m;
        for (j = 1; j < k; ++j) {
            spott = spot + j - m - 1;
            v = spot + j;
            a = src[v - m - 1];
            b = src[v - m];
            c = src[v - m + 1];
            d = src[v -1];
            e = src[v];
            f = src[v + 1];
            g = src[v + m - 1];
            h = src[v + m];
            q = src[v + m + 1];

            sum3.i_red = (int) a.red + (int) b.red + (int) c.red + (int) d.red + (int) e.red + (int) f.red
                         + (int) g.red + (int) h.red + (int) q.red;
            sum3.i_green = (int) a.green + (int) b.green + (int) c.green + (int) d.green + (int) e.green + (int) f.green
                           + (int) g.green + (int) h.green + (int) q.green;
            sum3.i_blue = (int) a.blue + (int) b.blue + (int) c.blue + (int) d.blue + (int) e.blue + (int) f.blue
                          + (int) g.blue + (int) h.blue + (int) q.blue;

            maxi = ((int) a.red + (int) a.green + (int) a.blue); // pixel 1
            mini = ((int) a.red + (int) a.green + (int) a.blue); // pixel 1
            max_spot = spott;
            min_spot = spott;

            two = ((int) b.red + (int) b.green + (int) b.blue); // pixel 2

            if (two > maxi) {
                maxi = two;
                max_spot = spott + 1;
            }
            if (two <= mini) {
                mini = two;
                min_spot = spott + 1;
            }

            three = ((int) c.red + (int) c.green + (int) c.blue); // pixel 3

            if (three > maxi) {
                maxi = three;
                max_spot = (spott + 2);
            }
            if (three <= mini) {
                mini = three;
                min_spot = (spott + 2);
            }

            spott += m;
            one = ((int) d.red + (int) d.green + (int) d.blue); // pixel 4

            if (one > maxi) {
                maxi = one;
                max_spot = spott;
            }
            if (one <= mini) {
                mini = one;
                min_spot = spott;
            }

            two = ((int) e.red + (int) e.green + (int) e.blue); // pixel 5

            if (two > maxi) {
                maxi = two;
                max_spot = (spott + 1);
            }
            if (two <= mini) {
                mini = two;
                min_spot = (spott + 1);
            }

            three = ((int) f.red + (int) f.green + (int) f.blue); // pixel 6

            if (three > maxi) {
                maxi = three;
                max_spot = (spott + 2);
            }
            if (three <= mini) {
                mini = three;
                min_spot = (spott + 2);
            }
            spott += m;

            one = ((int) g.red + (int) g.green + (int) g.blue); // pixel 7

            if (one > maxi) {
                maxi = one;
                max_spot = spott;
            }
            if (one <= mini) {
                mini = one;
                min_spot = spott;
            }

            two = ((int) h.red + (int) h.green + (int) h.blue); // pixel 8

            if (two > maxi) {
                maxi = two;
                max_spot = (spott + 1);
            }
            if (two <= mini) {
                mini = two;
                min_spot = (spott + 1);
            }

            three = ((int) q.red + (int) q.green + (int) q.blue); // pixel 9

            if (three > maxi) {
                max_spot = (spott + 2);
            }
            if (three <= mini) {
                min_spot = (spott + 2);
            }

            sum3.i_red -= ((int) src[max_spot].red + (int) src[min_spot].red);
            sum3.i_green -= ((int) src[max_spot].green + (int) src[min_spot].green);
            sum3.i_blue -= ((int) src[max_spot].blue + (int) src[min_spot].blue);

            sum3.i_red = sum3.i_red / 7;
            sum3.i_green = sum3.i_green / 7;
            sum3.i_blue = sum3.i_blue / 7;


            return_pixel3.red = (unsigned char) (MIN(MAX(sum3.i_red, 0), 255));
            return_pixel3.green = (unsigned char) (MIN(MAX(sum3.i_green, 0), 255));
            return_pixel3.blue = (unsigned char) (MIN(MAX(sum3.i_blue, 0), 255));

            dst[spot + j] = return_pixel3;
        }
    }
}

/*
 * This function calculates for the Blur mask the 9 pixels around each pixel
 * from the relevant pixels in the field less the first and last rows and the
 * first and last columns in the original image.
 * Since the mask is basically a matrix that everything is unity,
 * you can simply not double it in every pixel.
 * Therefore we will insert the schema in the pixel in the target image
 */
void smoothBlur(pixel *src, pixel *dst) {

    int i, j, v, k = m - 1;
    int spot = 0;

    pixel return_pixel;
    pixel_sum sum;

    pixel a, b, c, d, e, f, g, h, q;

    for (i = 1; i < k; ++i) {
        spot += m;
        for (j = 1; j < k; ++j) {
            v = spot + j;
            a = src[v - m - 1];
            b = src[v - m];
            c = src[v - m + 1];
            d = src[v -1];
            e = src[v];
            f = src[v + 1];
            g = src[v + m - 1];
            h = src[v + m];
            q = src[v + m + 1];

            sum.i_red = (int) a.red + (int) b.red + (int) c.red + (int) d.red + (int) e.red + (int) f.red
                        + (int) g.red + (int) h.red + (int) q.red;
            sum.i_green = (int) a.green + (int) b.green + (int) c.green + (int) d.green + (int) e.green + (int) f.green
                          + (int) g.green + (int) h.green + (int) q.green;
            sum.i_blue = (int) a.blue + (int) b.blue + (int) c.blue + (int) d.blue + (int) e.blue + (int) f.blue
                         + (int) g.blue + (int) h.blue + (int) q.blue;

            sum.i_red = sum.i_red / 9;
            sum.i_green = sum.i_green / 9;
            sum.i_blue = sum.i_blue / 9;

            return_pixel.red = (unsigned char) sum.i_red;
            return_pixel.green = (unsigned char) sum.i_green;
            return_pixel.blue = (unsigned char) sum.i_blue;
            dst[spot + j] = return_pixel;
        }
    }
}

/*
 * This function runs the entire program as it opens a target image and
 * allocates memory to it, as well as copying the contents of the source
 * image to the target image. From there it calls for
 * functions by choice of masks and filters.
 */
void myfunction(Image *image, char *srcImgpName, char *blurRsltImgName, char *sharpRsltImgName,
                char *filteredBlurRsltImgName, char *filteredSharpRsltImgName, char flag) {

    pixel *pixelsImg = (pixel*) image->data;
    pixel *backupOrg = (pixel*) malloc(m * n * sizeof(pixel));

    /*
     * blurKernel
    * [1, 1, 1]
    * [1, 1, 1]
    * [1, 1, 1]
    */

    /*
     * sharpKernel
    * [-1, -1, -1]
    * [-1, 9, -1]
    * [-1, -1, -1]
    */

    if (flag == '1') {
        // blur image
        memcpy(backupOrg, pixelsImg, m * n * sizeof(pixel));
        smoothBlur(backupOrg, pixelsImg);
        // write result image to file
        writeBMP(image, srcImgpName, blurRsltImgName);

        // sharpen the resulting image
        memcpy(backupOrg, pixelsImg, m * n * sizeof(pixel));
        smoothSharp(backupOrg, pixelsImg);
        // write result image to file
        writeBMP(image, srcImgpName, sharpRsltImgName);
    } else {
        // apply extermum filtered kernel to blur image
        memcpy(backupOrg, pixelsImg, m * n * sizeof(pixel));
        smoothBlurFiltered(backupOrg, pixelsImg);
        // write result image to file
        writeBMP(image, srcImgpName, filteredBlurRsltImgName);

        // sharpen the resulting image
        memcpy(backupOrg, pixelsImg, m * n * sizeof(pixel));
        smoothSharp(backupOrg, pixelsImg);
        // write result image to file
        writeBMP(image, srcImgpName, filteredSharpRsltImgName);
    }

    free(backupOrg);
}

