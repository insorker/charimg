#pragma once
#include "charimg.h"

void test_chim_cmd() {
	FILE* fp;
	fopen_s(&fp, "img/test1.bmp", "rb");
	if (fp != NULL) {
		chim_cmd(fp, 1, 2);
		// chim_cmd(fp, 1, 2);
		// chim_cmd(fp, 3, 1);
		// chim_cmd(fp, 10, 3);
		fclose(fp);
	}
}

void test_chim_file() {
	FILE* fpin, * fpout;
	fopen_s(&fpin, "img/test1.bmp", "rb");
	fopen_s(&fpout, "txt/test1.txt", "wb");
	if (fpin != NULL && fpout != NULL) {
		chim_file(fpin, 1, 2, fpout);
		//chim_file(fpin, 1, 2, fpout);
		//chim_file(fpin, 3, 1, fpout);
		//chim_file(fpin, 10, 3, fpout);
		fclose(fpin), fclose(fpout);
	}
}