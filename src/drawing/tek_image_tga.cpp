#include "tek_image.hpp"

#include <stdlib.h>
#include <string.h>


#pragma pack(push, 1)
typedef struct
{
	u8 id_length;
	u8 colormap_type;
	u8 image_type;

	short cm_first_entry;
	short cm_length;
	u8 cm_size;

	short x_origin;
	short y_origin;

	short width;
	short height;

	u8 pixel_depth;
	u8 image_descriptor;
}TgaHeader;
#pragma pack(pop)

typedef struct
{
	u32 width;
	u32 height;
	u8 *pixels;
	TekImageFormat format;
	int internal_format;
}TgaImage;

void get_image_info(const TgaHeader *header, TgaImage *img)
{
	img->width = header->width;
	img->height = header->height;

	switch (header->image_type)
	{
		case 3: //grayscale 8bits
		case 11: //grayscale 8bits(rle)
		{
			if (header->pixel_depth == 8)
			{
				img->format = TEK_IMG_NONE;
				img->internal_format = 1;
			}
			else //16bits
			{
				img->format = TEK_IMG_NONE;
				img->internal_format = 2;
			}
			break;
		}
		case 1: //8bits index
		case 2: //bgr 16-24-32bits
		case 9: //8bits index rle
		case 10: //bgr 16-24-32bits rle
		{
			if (header->pixel_depth <= 24)
			{
				img->format = TEK_IMG_RGB;
				img->internal_format = 3;
			}
			else //32bits
			{
				img->format = TEK_IMG_RGBA;
				img->internal_format = 4;
			}
			break;
		}
	}
}

void read_tga_24bits(FILE *fp, TgaImage *img)
{
	u32 i;
	for (i = 0; i < img->width * img->height; ++i)
	{
		//convert from bgr to rgb
		img->pixels[(i * 3) + 2] = (u8) fgetc(fp);
		img->pixels[(i * 3) + 1] = (u8) fgetc(fp);
		img->pixels[(i * 3) + 0] = (u8) fgetc(fp);
	}
}

void read_tga_32bits(FILE *fp, TgaImage *img)
{
	u32 i;
	for (i = 0; i < img->width * img->height; ++i)
	{
		//convert from bgra to rgba
		img->pixels[(i * 4) + 2] = (u8) fgetc(fp);
		img->pixels[(i * 4) + 1] = (u8) fgetc(fp);
		img->pixels[(i * 4) + 0] = (u8) fgetc(fp);
		img->pixels[(i * 4) + 3] = (u8) fgetc(fp);
	}
}

void read_tga_24bits_rle(FILE *fp, TgaImage *img)
{
	int i, size;
	u8 rgb[3];
	u8 packet_header;
	u8 *ptr = img->pixels;

	while (ptr < img->pixels + (img->width * img->height) * 3)
	{
		packet_header = (u8) fgetc(fp);
		size = 1 + (packet_header & 0x7f);

		if (packet_header & 0x80)
		{
			fread(rgb, sizeof(u8), 3, fp);

			for (i = 0; i < size; ++i, ptr += 3)
			{
				ptr[0] = rgb[2];
				ptr[1] = rgb[1];
				ptr[2] = rgb[0];
			}
		}
		else
		{
			for (i = 0; i < size; ++i, ptr += 3)
			{
				ptr[2] = (u8) fgetc(fp);
				ptr[1] = (u8) fgetc(fp);
				ptr[0] = (u8) fgetc(fp);
			}
		}
	}
}

void read_tga_32bits_rle(FILE *fp, TgaImage *img)
{
	int i, size;
	u8 rgba[4];
	u8 packet_header;
	u8 *ptr = img->pixels;

	while (ptr < img->pixels + (img->width * img->height) * 4)
	{
		packet_header = (u8) fgetc(fp);
		size = 1 + (packet_header & 0x7f);

		if (packet_header & 0x80)
		{
			fread(rgba, sizeof(u8), 4, fp);

			for (i = 0; i < size; ++i, ptr += 4)
			{
				ptr[0] = rgba[2];
				ptr[1] = rgba[1];
				ptr[2] = rgba[0];
				ptr[3] = rgba[3];
			}
		}
		else
		{
			for (i = 0; i < size; ++i, ptr += 4)
			{
				ptr[2] = (u8) fgetc(fp);
				ptr[1] = (u8) fgetc(fp);
				ptr[0] = (u8) fgetc(fp);
				ptr[3] = (u8) fgetc(fp);
			}
		}
	}
}

bool _tek_load_tga(TekImage *img, const char *filename)
{
	TgaHeader header;
	TgaImage image;

	FILE *file;

	printf("loading image %s\n", filename);

	file = fopen(filename, "rb");
	if (!file)
	{
		printf("cannot open %s\n", filename);
		return false;
	}

	//read header
	fread(&header, sizeof(TgaHeader), 1, file);

	//texinfo = (struct gl_texture_t *)malloc (sizeof (struct gl_texture_t));
	get_image_info(&header, &image);
	//GetTextureInfo (&header, texinfo);
	fseek(file, header.id_length, SEEK_CUR);

	//printf("test2 %u %u %d\n",img->width , img->height , img->internal_format);

	image.pixels = (u8*)tek_malloc(sizeof(u8) * image.width * image.height * image.internal_format);

	//printf("test3\n");

	//read image
	int success = 1;
	switch (header.image_type)
	{
		case 0: // nothing
			break;

		case 1: //8bits color index
			printf("tga 8bits color index format not supported\n");
			success = 0;
			break;

		case 2: //16-24-32
			switch (header.pixel_depth)
			{
				case 16:
					printf("tga 16bits format not supported\n");
					success = 0;
					break;

				case 24:
					//printf("tga 24bits\n");
					read_tga_24bits(file, &image);
					break;

				case 32:
					//printf("tga 32bits\n");
					read_tga_32bits(file, &image);
					break;
			}
			break;

		case 3: //grayscale 8-16bits
			if (header.pixel_depth == 8)
			{
				printf("tga gray 8bits not supported\n");
				success = 0;
			}
			else //16bits
			{
				printf("tga gray 16bits not supported\n");
				success = 0;
			}
			break;

		case 9: //8bits color index rle
			printf("tga 8bits rle no supported\n");
			success = 0;
			break;

		case 10: //16-24-32bits rle
			switch (header.pixel_depth)
			{
				case 16:
					printf("tga 16bits rle not supported\n");
					success = 0;
					break;

				case 24:
					//printf("tga 24bits rle\n");
					read_tga_24bits_rle(file, &image);
					break;

				case 32:
					//printf("tga 32bits rle\n");
					read_tga_32bits_rle(file, &image);
					break;
			}
			break;

		case 11: //8-16bits grayscale rle
			if (header.pixel_depth == 8)
			{
				printf("tga 8bits gray rle not supported\n");
				success = 0;
			}
			else //16bits
			{
				printf("tga 16bits gray rle not supported\n");
				success = 0;
			}
			break;
		default:
			printf("unknown TGA image type %i\n", header.image_type);
			success = 0;
			break;
	}

	fclose(file);

	img->width = image.width;
	img->height = image.height;
	img->format = image.format;
	img->internal_format = image.internal_format;
	img->pixels = (u8*)tek_malloc(sizeof(u8) * image.width * image.height * image.internal_format);
	memcpy(img->pixels, image.pixels, sizeof(u8) * image.width * image.height * image.internal_format);

	free(image.pixels);

	return success;
}

