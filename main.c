
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void VFlipArray (uint8_t *a, int w, int h) {
	uint8_t b;

	for (int i=0; i<h/2; i++) {
		uint8_t *c, *d;
		c = a + i*w;
		d = a + (h-i-1)*w;

		for (int j=0; j<w; j++) {
			b = c[j];
			c[j] = d[j];
			d[j] = b;
		}
	}
}

int Get16 (FILE *f) {
	return getc(f) | (getc(f)<<8);
}

int Get32 (FILE *f) {
	return getc(f) | (getc(f)<<8) | (getc(f)<<16) | (getc(f)<<24);
}

int FileIsBMP (FILE *f) {
	fseek(f, 0, SEEK_SET);
	
	return (getc(f) == 'B' && getc(f) == 'M');
}

int GetHeight (FILE *f) {
	fseek(f, 22, SEEK_SET);
	return Get32(f);
}

int GetOffset (FILE *f) {
	fseek(f, 10, SEEK_SET);
	
	return Get16(f);
}

int GetData (FILE *f, uint8_t *a) {
	fseek(f, GetOffset(f), SEEK_SET);

	return fread(a, 1, 0x800, f);
}

char nibToString[16][5] = {
	"----","---#","--#-","--##",
	"-#--","-#-#","-##-","-###",
	"#---","#--#","#-#-","#-##",
	"##--","##-#","###-","####"
};

void Output(uint8_t *a, FILE *f) {
	for (int i=0; i<16; i++) {
		uint8_t *b;
		b = a + i*128;
		for (int j=0; j<16; j++) {
			uint8_t *c;
			c = b+j;
			for (int k=0; k<8; k++) {
				uint8_t d;
				d = c[k*16];
				//printf("%s%s\n",nibToString[d>>4],nibToString[d&15]);
				putc(d,f);
			}
		}
	}

}

int main(int argc, char **argv) {
	if (argc < 3) {
		printf("Not enough arguments\n"
			"Usage: G2.exe in.bmp out.bin\n"
			);
		return 0;
	}

	FILE *fb, *fout;
	fb = fopen(argv[1], "rb");
	fout = fopen(argv[2], "wb");

	if (!FileIsBMP(fb)) {
		printf("Error: Not a BMP file.\n");
		return 0;
	}
	
	uint8_t *a;

	a = malloc(0x800);
	GetData(fb, a);

	// is image flipped ?
	if (GetHeight(fb) >= 0) {
		// 16 bytes wide by 128 rows
		VFlipArray(a,16,128);
	}

	fclose(fb);


	Output(a, fout);
	fclose(fout);

	free(a);
	
	return 0;
}

