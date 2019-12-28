#ifndef __DISK_STREAMER_H__
#define __DISK_STREAMER_H__


#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>


#include "kseq.h"
#include "zlib.h"


#include <emscripten.h>


#include "utility.h"


#define BUFFER_SZ 1024 * 512


// Defined in external.js
extern void read_input_file_slice(int slice_start, int slice_end, char *reader_name);


struct FileReader {
  char *name;
  size_t position;
  size_t buffer_sz;
  size_t file_sz;
  unsigned char *buffer_shared;
  z_stream *decomper;
};


int inflate_slice(z_stream *stream);
int get_reader_data(char *reader_name, char *property);
struct FileReader * init_file_reader(char *reader_name, size_t buffer_sz);
int destroy_file_reader(struct FileReader *file_reader);
int get_slice(struct FileReader *file_reader);
int read_data(struct FileReader *file_reader, void *buffer, unsigned int length);


#endif
