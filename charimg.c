#include "charimg.h"
#include <windows.h>

#define BMP_TYPE	19778
#define RGB_TO_GRAY(r, g, b, gray)		do { (gray) = ((r) * 30 + (g) * 59 + (b) * 11) / 100; } while(0)
#define GRAY_TO_STR(gray, str)			do { (str) = chim_char.c[(gray) * chim_char.l / 256]; } while(0)

typedef struct {
	const char* c;
	int l;
}chim_character;
static chim_character chim_char = { "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/\\|()1{}[]?-_+~<>i!lI;:,\"^`. ", 69 };

enum {
	CHIM_BFH_OFFSET = 0L,	// 位图头
	CHIM_BIH_OFFSET = 14L,	// 位图信息头
	CHIM_BW_OFFSET = 18L,	// 位图宽度
	CHIM_BH_OFFSET = 22L,	// 位图高度
	CHIM_BD_OFFSET = 54L	// 位图数据
};

typedef struct {
	DWORD w;	// 字符画宽度
	DWORD h;	// 字符画高度
	DWORD size;	// 字符画大小
	char* str;	// 字符数组
}chim_data;

static int chim_is_bmpfile(FILE* fp) {
	WORD ft = 0;
	fseek(fp, CHIM_BFH_OFFSET, SEEK_SET);
	fread(&ft, 1, sizeof(WORD), fp);
	if (ft == BMP_TYPE)
		return 1;
	else
		return 0;
}

static void chim_set_data(FILE* fp, chim_data* cd) {
	fseek(fp, CHIM_BW_OFFSET, SEEK_SET);
	fread(&cd->w, 4, sizeof(WORD), fp);
	fseek(fp, CHIM_BH_OFFSET, SEEK_SET);
	fread(&cd->h, 4, sizeof(WORD), fp);
	cd->size = (cd->w + 1) * cd->h + 1;
	cd->str = (char*)malloc(cd->size);
}

static void chim_rgb_to_str(FILE* fp, chim_data* cd) {
	chim_set_data(fp, cd);
	fseek(fp, CHIM_BD_OFFSET, SEEK_SET);

	DWORD gray = 0;
	DWORD width_rgb = (cd->w + 1) * 3 / 4 * 4;
	BYTE* str_rgb = (BYTE*)malloc(width_rgb);
	
	for (DWORD i = cd->h - 1; i < (DWORD)(-1); i--) {
		fread(str_rgb, 1, width_rgb, fp);

		for (DWORD j = 0; j < cd->w; j++) {
			RGB_TO_GRAY(str_rgb[j * 3 + 2], str_rgb[j * 3 + 1], str_rgb[j * 3], gray);
			GRAY_TO_STR(gray, cd->str[i * (cd->w + 1) + j]);
		}
		cd->str[(i + 1) * (cd->w + 1) - 1] = '\n';
	}
	cd->str[cd->h * (cd->w + 1)] = '\0';

	free(str_rgb);
}

static void chim_compress(chim_data* cd, int dw, int dh) {
	DWORD nw = cd->w / dw;
	DWORD nh = cd->h / dh;
	DWORD nsize = (nw + 1) * nh + 1;
	char* nstr = (char*)malloc(nsize);

	for (DWORD i = 0; i < nh; i++) {
		for (DWORD j = 0; j < nw; j++) {
			nstr[i * (nw + 1) + j] = cd->str[i * dh * (cd->w + 1) + j * dw];
		}
		nstr[(i + 1) * (nw + 1) - 1] = '\n';
	}
	nstr[nh * (nw + 1)] = '\0';

	free(cd->str);
	cd->w = nw;
	cd->h = nh;
	cd->size = nsize;
	cd->str = nstr;
}

void chim_cmd(FILE* fp, int dw, int dh) {
	if (fp != NULL || chim_is_bmpfile(fp)) {
		chim_data cv;
		chim_rgb_to_str(fp, &cv);
		chim_compress(&cv, dw, dh);
		fprintf(stdout, "%s", cv.str);
		free(cv.str);
	}
}

void chim_file(FILE* fpin, int dw, int dh, FILE* fpout) {
	if (fpin != NULL || chim_is_bmpfile(fpin)) {
		chim_data cv;
		chim_rgb_to_str(fpin, &cv);
		chim_compress(&cv, dw, dh);
		fprintf(fpout, "%s", cv.str);
		free(cv.str);
	}
}
