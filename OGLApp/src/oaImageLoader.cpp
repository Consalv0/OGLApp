#include "oaImageLoader.h"
#include <libpng\png.h>

std::unordered_map<std::string, GLuint> oaImageLoader::imagesIDs = std::unordered_map<std::string, GLuint>();

GLuint oaImageLoader::loadImage(std::string & filePath, size_t & outWidth, size_t & outHeight, bool &outHasAlpha) {
	if (filePath.empty()) {
		return NULL;
	}

	if (imagesIDs.find(filePath) != imagesIDs.end()) {
		return imagesIDs.at(filePath);
	}

	if (filePath == "white") {
		unsigned char* data = new unsigned char[8 * sizeof(unsigned char)];

		for (int i = 0; i < (int)(8 * sizeof(unsigned char)); i++) {
			data[i] = 255;
		}

		GLuint whiteTextureID;
		glGenTextures(1, &whiteTextureID);
		glBindTexture(GL_TEXTURE_2D, whiteTextureID);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 1, 1, 0, GL_RGBA, GL_UNSIGNED_BYTE, (GLvoid *)data);
		glGenerateMipmap(GL_TEXTURE_2D);

		imagesIDs.insert({ filePath, whiteTextureID });
		return whiteTextureID;
	}

	GLuint textureDataID = loadPNG24(filePath.c_str(), outWidth, outHeight);
	if (textureDataID == NULL) {
		try {
			textureDataID = loadPNG(filePath.c_str(), outWidth, outHeight);
		} catch (...) {
			textureDataID = NULL;
		}
	}

	if (textureDataID == NULL) {
		printf("Unable to load '%s'\n", filePath.c_str());
		return NULL;
	}

	imagesIDs.insert({ filePath, textureDataID });

	printf("Image loaded : %s\n", filePath.c_str());
	return textureDataID;
}

GLuint oaImageLoader::loadPNG(
	const char *filePath, size_t &outWidth, size_t &outHeight) 
{
	png_structp png_ptr;
	png_infop info_ptr;
	unsigned int sig_read = 0;
	int color_type, interlace_type;
	FILE *fp;

	errno_t err = fopen_s(&fp, filePath, "rb");
	if (err)
		return NULL;

	/* Create and initialize the png_struct
	* with the desired error handler
	* functions.  If you want to use the
	* default stderr and longjump method,
	* you can supply NULL for the last
	* three parameters.  We also supply the
	* the compiler header file version, so
	* that we know if the application
	* was compiled with a compatible version
	* of the library.  REQUIRED
	*/
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING,
		NULL, NULL, NULL);

	if (png_ptr == NULL) {
		fclose(fp);
		return false;
	}

	/* Allocate/initialize the memory
	* for image information.  REQUIRED. */
	info_ptr = png_create_info_struct(png_ptr);
	if (info_ptr == NULL) {
		fclose(fp);
		png_destroy_read_struct(&png_ptr, NULL, NULL);
		return false;
	}

	/* Set error handling if you are
	* using the setjmp/longjmp method
	* (this is the normal method of
	* doing things with libpng).
	* REQUIRED unless you  set up
	* your own error handlers in
	* the png_create_read_struct()
	* earlier.
	*/
	if (setjmp(png_jmpbuf(png_ptr))) {
		/* Free all of the memory associated
		* with the png_ptr and info_ptr */
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);
		fclose(fp);
		/* If we get here, we had a
		* problem reading the file */
		return false;
	}

	/* Set up the output control if
	* you are using standard C streams */
	png_init_io(png_ptr, fp);

	/* If we have already
	* read some of the signature */
	png_set_sig_bytes(png_ptr, sig_read);

	/*
	* If you have enough memory to read
	* in the entire image at once, and
	* you need to specify only
	* transforms that can be controlled
	* with one of the PNG_TRANSFORM_*
	* bits (this presently excludes
	* dithering, filling, setting
	* background, and doing gamma
	* adjustment), then you can read the
	* entire image (including pixels)
	* into the info structure with this
	* call
	*
	* PNG_TRANSFORM_STRIP_16 |
	* PNG_TRANSFORM_PACKING  forces 8 bit
	* PNG_TRANSFORM_EXPAND forces to
	*  expand a palette into RGB
	*/
	png_read_png(png_ptr, info_ptr,
		PNG_TRANSFORM_STRIP_16 | PNG_TRANSFORM_PACKING | PNG_TRANSFORM_EXPAND, NULL);

	png_uint_32 width, height;
	int bit_depth;
	png_get_IHDR(png_ptr, info_ptr, &width, &height, &bit_depth, &color_type,
		&interlace_type, NULL, NULL);
	outWidth = width;
	outHeight = height;

	unsigned int row_bytes = png_get_rowbytes(png_ptr, info_ptr);
	GLubyte* outData = (unsigned char*)malloc(row_bytes * outHeight);

	png_bytepp row_pointers = png_get_rows(png_ptr, info_ptr);

	for (size_t i = 0; i < outHeight; i++) {
		// note that png is ordered top to
		// bottom, but OpenGL expect it bottom to top
		// so the order or swapped
		memcpy(outData + (row_bytes * (outHeight - 1 - i)), row_pointers[i], row_bytes);
	}

	/* Clean up after the read,
	* and free any memory allocated */
	png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

	GLuint textureDataID;
	// Creates the texture
	glGenTextures(1, &textureDataID);
	glBindTexture(GL_TEXTURE_2D, textureDataID);

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA,
		outWidth, outHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, outData);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR_MIPMAP_LINEAR);
	glGenerateMipmap(GL_TEXTURE_2D);

	/* Close the file */
	fclose(fp);
	return true;
}

GLuint oaImageLoader::loadPNG24(
	const char *filePath, size_t &outWidth, size_t &outHeight) 
{
	FILE *f;
	int is_png, bit_depth, color_type, row_bytes, i;
	png_infop info_ptr, end_info;
	png_uint_32 t_width, t_height;
	png_byte header[8], *image_data;
	png_bytepp row_pointers;
	png_structp png_ptr;
	GLuint texture;
	int alpha;

	errno_t err = fopen_s(&f, filePath, "rb");
	if (err) {
		return NULL;
	}

	fread(header, 1, 8, f);
	is_png = !png_sig_cmp(header, 0, 8);
	if (!is_png) {
		fclose(f);
		return NULL;
	}
	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL,
		NULL, NULL);
	if (!png_ptr) {
		fclose(f);
		return NULL;
	}
	info_ptr = png_create_info_struct(png_ptr);
	if (!info_ptr) {
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL,
			(png_infopp)NULL);
		fclose(f);
		return NULL;
	}
	end_info = png_create_info_struct(png_ptr);
	if (!end_info) {
		png_destroy_read_struct(&png_ptr, (png_infopp)NULL,
			(png_infopp)NULL);
		fclose(f);
		return NULL;
	}
	if (setjmp(png_jmpbuf(png_ptr))) {
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(f);
		return NULL;
	}
	png_init_io(png_ptr, f);
	png_set_sig_bytes(png_ptr, 8);
	png_read_info(png_ptr, info_ptr);
	png_get_IHDR(png_ptr, info_ptr, &t_width, &t_height, &bit_depth,
		&color_type, NULL, NULL, NULL);
	outWidth = t_width;
	outHeight = t_height;

	// set least one byte per channel
	if (png_get_bit_depth(png_ptr, info_ptr) < 8) {
		png_set_packing(png_ptr);
	}

	// if transparency, convert it to alpha
	if (png_get_valid(png_ptr, info_ptr, PNG_INFO_tRNS)) {
		png_set_tRNS_to_alpha(png_ptr);
	}

	switch (png_get_color_type(png_ptr, info_ptr)) {
	case PNG_COLOR_TYPE_RGBA:
		alpha = GL_RGBA;
		break;
	case PNG_COLOR_TYPE_RGB:
		alpha = GL_RGB;
		break;
	case PNG_COLOR_TYPE_GRAY:
		alpha = GL_RGB;
		png_set_gray_to_rgb(png_ptr);
		break;
	case PNG_COLOR_TYPE_GA:
		alpha = GL_RGBA;
		png_set_gray_to_rgb(png_ptr);
	default:
		printf("Color type %d not supported!, may produce errors\n",
		png_get_color_type(png_ptr, info_ptr));
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		return NULL;
	}

	png_read_update_info(png_ptr, info_ptr);
	row_bytes = png_get_rowbytes(png_ptr, info_ptr);
	image_data = (png_bytep)malloc(row_bytes * t_height * sizeof(png_byte));
	if (!image_data) {
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		fclose(f);
		return NULL;
	}
	row_pointers = (png_bytepp)malloc(t_height * sizeof(png_bytep));
	if (!row_pointers) {
		png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
		free(image_data);
		fclose(f);
		return NULL;
	}
	for (i = 0; i < (int)t_height; ++i) {
		row_pointers[t_height - 1 - i] = image_data + i * row_bytes;
	}

	png_read_image(png_ptr, row_pointers);

	glGenTextures(1, &texture);
	glBindTexture(GL_TEXTURE_2D, texture);
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, t_width, t_height, 0,
		alpha, GL_UNSIGNED_BYTE, (GLvoid *)image_data);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glGenerateMipmap(GL_TEXTURE_2D);

	png_destroy_read_struct(&png_ptr, &info_ptr, &end_info);
	free(image_data);
	free(row_pointers);
	fclose(f);
	return texture;
}