
/**
Copyright (c) 2016 Jason Ertel, Codesim LLC

Permission is hereby granted, free of charge, to any person obtaining a copy of
this software and associated documentation files (the "Software"), to deal in 
the Software without restriction, including without limitation the rights to 
use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies 
of the Software, and to permit persons to whom the Software is furnished to do 
so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all 
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR 
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, 
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE 
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER 
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, 
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE 
SOFTWARE.
*/
#include <endian.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


unsigned short read16(FILE* f, unsigned char offset) {
  fseek(f, offset, SEEK_SET);
  unsigned short s = 0;
  fread(&s, sizeof(unsigned short), 1, f);
  return htobe16(s);
}

unsigned char read8(FILE* f, unsigned char offset) {
  fseek(f, offset, SEEK_SET);
  unsigned char c = 0;
  fread(&c, sizeof(unsigned char), 1, f);
  return c;
}

unsigned char read1(FILE* f, unsigned char offset, unsigned char bit) {
  fseek(f, offset, SEEK_SET);
  unsigned char c = 0;
  fread(&c, sizeof(unsigned char), 1, f);
  return (c & (1 << bit)) > 0 ? 1 : 0;
}

void write8(FILE* f, unsigned char offset, const unsigned char value) {
  fseek(f, offset, SEEK_SET);
  unsigned char c = 0;
  fwrite(&value, sizeof(unsigned char), 1, f);
}

void write1(FILE* f, unsigned char offset, unsigned char bit, const unsigned char value) {
  unsigned char c = read8(f, offset);  
  fseek(f, offset, SEEK_SET);
  if (value > 0) {
    c |= (1 << bit);
  } else {
    c &= ~(1 << bit);
  }
  fwrite(&c, sizeof(unsigned char), 1, f);
}

FILE* initEc() {
  system("modprobe ec_sys write_support=1");
  return fopen("/sys/kernel/debug/ec/ec0/io", "r+");
}

void closeEc(FILE* ec) {
  fclose(ec);
}

void fail(const char* msg) {
  printf("ERROR: %s\n", msg);
  exit(1);
}

int main(int argc, char** args) {
  FILE* ec = initEc();
  if (!ec) fail("Unable to run initialize embedded controller; did you forget to use sudo?");

  if (argc == 3) {
    char* dotIdx = strchr(args[1], '.');
    if (dotIdx != NULL) {
      *dotIdx = '\0';
      dotIdx++;
      unsigned char offset = (unsigned char)strtol(args[1], NULL, 0);
      unsigned char bit = (unsigned char)atoi(dotIdx);
      unsigned char value = (unsigned char)strtol(args[2], NULL, 0);
      write1(ec, offset, bit, value);
    } else {
      unsigned char offset = (unsigned char)strtol(args[1], NULL, 0);
      unsigned char value = (unsigned char)strtol(args[2], NULL, 0);
      write8(ec, offset, value);
    }
  } else {
    // Setting the following bits causing the EC to activate (or at least activate fan controls)
    write8(ec, 0x01, 0xA3);

    printf("Usage: sudo %s [<hex-offset[.bit]> <hex-value>]\n", args[0]);
    printf("   Ex: sudo %s 0x01.6 0x07\n\n", args[0]);
    printf("Current Embedded Controller Values:\n");
    printf("  USB Charge During Sleep     [0x01.5]: %d\n", read1(ec, 0x01, 5));
    printf("  USB Charge During Hibernate [0x07.2]: %d\n", read1(ec, 0x07, 2));
    printf("  Camera Enabled              [0x01.6]: %d\n", read1(ec, 0x01, 6));
    printf("  Bluetooth Enabled           [0x01.7]: %d\n", read1(ec, 0x01, 7));
    printf("  WiFi Enabled                [0x02.6]: %d\n", read1(ec, 0x02, 6));
    printf("  Touchpad Enabled            [0x03.5]: %d\n", read1(ec, 0x03, 5));
    printf("  Ambient Light               [0x66]:   %d%%\n", read8(ec, 0x66));
    printf("  Screen Enabled              [0x09.3]: %d\n", read1(ec, 0x09, 3));
    printf("  Keyboard Backlight Mode     [0xD7]:   %d\n", read8(ec, 0xD7));
    printf("  CPU Temp                    [0x60]:   %d C\n", read8(ec, 0x60));
    printf("  GPU Temp                    [0x61]:   %d C\n", read8(ec, 0x61));
    printf("  MLB Temp                    [0x62]:   %d C\n", read8(ec, 0x62));
    printf("  Fan0 Speed                  [0xFC]:   %d RPM\n", read16(ec, 0xFC));
    printf("  Fan1 Speed                  [0xFE]:   %d RPM\n", read16(ec, 0xFE));
    printf("  Fan Control Enabled         [0x13.3]: %d\n", read1(ec, 0x13, 3));
    printf("  Fan Quiet Mode Enabled      [0x08.6]: %d\n", read1(ec, 0x08, 6));
    printf("  Fan Gaming Mode Enabled     [0x12.4]: %d\n", read1(ec, 0x12, 4));
    printf("  Fan Custom Mode Enabled     [0x13.0]: %d\n", read1(ec, 0x13, 0));
    printf("  Fan0 Custom Speed Setting   [0xB0]:   %d%%\n", (int)round(read8(ec, 0xB0) / 2.55));
    printf("  Fan1 Custom Speed Setting   [0xB1]:   %d%%\n", (int)round(read8(ec, 0xB1) / 2.55));
    printf("  Current Speed Setting       [0x64]:   %d\n", read8(ec, 0x64));
  }
  closeEc(ec);
  exit(0);
}
