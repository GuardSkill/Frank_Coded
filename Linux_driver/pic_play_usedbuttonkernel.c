#include <stdio.h>
#include "jpeglib.h"
#include <setjmp.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <linux/fb.h>
#include <dirent.h>
#include <string.h>
#include <pthread.h>

static struct fb_var_screeninfo var;
static unsigned char* fbmem = NULL;
static void show_pic(int x, int width, int height, char* buffer);
static void show_point(int x, int y, unsigned long color);
static int isjpgfile(char *filename);
static int display_pic(char* filename);
static void show_blank(int width,int height);

static int flag=1;
static int op=1;

void *inspect(void)
{
    int fd;
    fd = open("/dev/lsy_button", O_RDONLY);
    char key = 1;
    int ret = 0;
    while(1)
    {
        ret = read(fd, &key, 4);
        if(ret>0)
        {
            switch(key)
            {
            case 1:flag=1; op=1; break;
            case 2:flag=0; op=0; break;
            case 3:flag=0; op=1;break;
            case 4:flag=0; op=-1; break;
            }
        }
    }
}
int main()
{
    char *filename;
    DIR *dir;
    struct dirent *dirp;
    int first_flag=1;
    int ret;
    long offset[100]={0};
    int offset_n=0;
    int offset_max=0;

    if((dir=opendir("./"))==NULL)
    {
        printf("error when opendir\n");
        exit(-1);
    }
    do
    {
        if(first_flag==1)
        {

            while(1)
            {
                offset[offset_n]=telldir(dir);
                dirp=readdir(dir);
                if(dirp==NULL) break;
                filename=dirp->d_name;
                if(isjpgfile(filename)==1)
                {
                    /*if(display_pic(filename)!=1)
                                        {
                                        printf("display_pic error\n");
                                        closedir(dir);
                                        exit(-1);
                                        }*/
                    printf("%d's offset is :%ld\n",offset_n,offset[offset_n]);
                    offset_n++;
                }
            }
            first_flag=0;
            offset_max=offset_n;
            offset_n=0;

            pthread_t id;
            ret=pthread_create(&id,NULL,(void *)inspect,NULL);
            if(ret!=0)
            {
                printf("Create pthread error!\n");
                return -1;
            }

        }
        else
        {
            seekdir(dir,offset[offset_n]);
            while((dirp=readdir(dir))!=NULL)
            {
                filename=dirp->d_name;
                if(display_pic(filename)!=1)
                {
                    closedir(dir);
                    exit(-1);
                }
                if(flag==1) sleep(3);                //boarding
                while(flag==0&&op==0)                      //not boarding and pause
                {
                    usleep(1000);
                }
                offset_n=offset_n+op;			//set the next pic's position
                if(offset_n==-1) offset_n=offset_max-1;   //arrive at the  back of offset[]
                else if(offset_n==offset_max) offset_n=0;
                if(flag!=1) op=0;                //not boarding ,reset the operation
                seekdir(dir,offset[offset_n]);
            }
        }

    }while (1) ;
    closedir(dir);
}


static int  display_pic(char* filename)
{
    int fd = 0;
    fd = open("/dev/fb0", O_RDWR);
    if(fd < 0)
    {
        printf("open /dev/fb0 failed!\n");
        return -1;
    }
    ioctl(fd, FBIOGET_VSCREENINFO, &var);
    /*	printf("heigh = %d\n", var.xres);
        printf("wight = %d\n", var.yres);
        printf("bpp   = %d\n", var.bits_per_pixel);   */          //print the display information

    unsigned int fbsize = var.xres * var.yres * var.bits_per_pixel / 8;
    fbmem = (unsigned char*)mmap(NULL, fbsize, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    show_blank(var.xres,var.yres);
    struct jpeg_decompress_struct cinfo;
    struct jpeg_error_mgr jerr;

    cinfo.err = jpeg_std_error(&jerr);
    jpeg_create_decompress(&cinfo);

    FILE * infile;
    if ((infile = fopen(filename, "rb")) == NULL)
    {
        fprintf(stderr, "can't open %s\n", filename);
        exit(1);
    }
    jpeg_stdio_src(&cinfo, infile);         //specify the source
    jpeg_read_header(&cinfo, TRUE);            //read the pic info

    printf("width:  %d\n",cinfo.image_width);
    printf("height: %d\n",cinfo.image_height);
    printf("num_components: %d\n",cinfo.num_components);

    //printf("input the rate <x/y>: ");
    fflush(stdout);
    //	scanf("%d/%d", &(cinfo.scale_num), &(cinfo.scale_denom));
    cinfo.scale_num=1;
    cinfo.scale_denom=1;
    jpeg_start_decompress(&cinfo);
    /*
        printf("dwidth:	%d\n",cinfo.output_width);
        printf("dheight: %d\n",cinfo.output_height);
        printf("dnum_components: %d\n",cinfo.output_components);    //the rewind pic's info
        */
    JSAMPARRAY buffer;
    int row_stride;

    row_stride = cinfo.output_width * cinfo.output_components;

    buffer = (*cinfo.mem->alloc_sarray)
            ((j_common_ptr) &cinfo, JPOOL_IMAGE, row_stride, 1);

    while (cinfo.output_scanline < cinfo.output_height)
    {
        (void) jpeg_read_scanlines(&cinfo, buffer, 1);
        show_pic(0, cinfo.output_width, cinfo.output_scanline, *buffer);
    }

    jpeg_finish_decompress(&cinfo);

    jpeg_destroy_decompress(&cinfo);

    munmap(fbmem, fbsize);
    close(fd);
    return 1;
}

static void show_point(int x, int y, unsigned long color)
{
    unsigned long* pen_32 = (unsigned long*)(fbmem + x * var.bits_per_pixel/8 + y*var.xres*var.bits_per_pixel/8);
    *pen_32 = color;
}
static void show_blank(int width,int height)
{
       int i;
       int j;
        for(i=0;i<height;i++)
                {
                        for(j=0;j<width;j++)
                                {
                                        show_point(j,i,0x00000000);
                                }
                }
}
static void show_pic(int x, int width, int height, char* buffer)
{
    int i = 0;
    unsigned long color;

    for(i = x; i < x+width; i++)
    {
        color = ((*buffer << 16) | (*(buffer + 1) << 8) | *(buffer + 2));
        show_point(i, height, color);
        buffer = buffer + 3;
    }
}
static int isjpgfile(char *filename)
{
    int len;
    len=strlen(filename);
    if(len<5) return 0;
    else if((filename[len-1]!='G'&&filename[len-1]!='g')||
            (filename[len-2]!='p'&&filename[len-2]!='P')||
            (filename[len-3]!='J'&&filename[len-3]!='j')||filename[len-4]!='.')
        return 0;
    else return 1;
}
