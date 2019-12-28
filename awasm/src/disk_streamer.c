#include "disk_streamer.h"


int inflate_slice(z_stream *stream) {
  int ret;
  do {
    ret = inflate(stream, Z_NO_FLUSH);
    switch (ret) {
      case Z_NEED_DICT:
        ret = Z_DATA_ERROR;
      case Z_STREAM_ERROR:
      case Z_DATA_ERROR:
      case Z_MEM_ERROR:
        fprintf(stderr, "inflate error (%d)\n", ret);
        exit(1);
    }
  // Loop until output buffer full, input empty, end of stream, or some other error
  } while(stream->avail_out > 0 && stream->avail_in > 0 && ret != Z_STREAM_END && ret != Z_BUF_ERROR);
  return 0;
}


int get_reader_data(char *reader_name, char *property) {
  int data = EM_ASM_INT(
    {
      var reader_name = Module.UTF8ToString($0);
      var reader_property = Module.UTF8ToString($1);
      // Incredibly dangerous lol
      var tokens = reader_property.split('.');
      var ret = Module.readers[reader_name];
      for (var i = 0; i < tokens.length; i++) {
        ret = ret[tokens[i]];
      }
      return ret;
    },
    reader_name,
    property
  );
  return data;
}


struct FileReader * init_file_reader(char *reader_name, size_t buffer_sz) {
  // Setup base reader
  struct FileReader *file_reader = malloc(sizeof(struct FileReader));
  file_reader->name = malloc(sizeof(char) * 256);
  strcpy(file_reader->name, reader_name);
  file_reader->position = 0;
  file_reader->buffer_sz = buffer_sz;

  // Setup JS reader
  // Define input file and FileReader in js, and malloc buffer that js and c can access
  // This is dicey, would be good to have as function arguments, alas this doesn't seem possible rn
  EM_ASM(
    {
      if (!Module.hasOwnProperty('readers')) {
        Module.readers = Object();
      }
      var reader_name = Module.UTF8ToString($0);
      Module.readers[reader_name] = Object();
      Module.readers[reader_name].fob = document.getElementById(reader_name).files[0];
      Module.readers[reader_name].fdr = new FileReader();
      Module.readers[reader_name].buf = Module._malloc($0);
    },
    file_reader->name,
    file_reader->buffer_sz
  );
  const int buffer_address = get_reader_data(file_reader->name, "buf");
  file_reader->buffer_shared = (unsigned char *)(long)buffer_address;
  file_reader->file_sz = get_reader_data(file_reader->name, "fob.size");

  // Setup decompressor reader
  file_reader->decomper = malloc(sizeof(z_stream));
  file_reader->decomper->zalloc = Z_NULL;
  file_reader->decomper->zfree = Z_NULL;
  file_reader->decomper->opaque = Z_NULL;
  file_reader->decomper->avail_in = 0;
  file_reader->decomper->next_in = Z_NULL;
  int ret = inflateInit2(file_reader->decomper, 16+MAX_WBITS);
  if (ret != Z_OK) {
    exit(1);
  }
  file_reader->decomper->avail_out = file_reader->buffer_sz;

  return file_reader;
}


int destroy_file_reader(struct FileReader *file_reader) {
  free(file_reader->buffer_shared);
  inflateEnd(file_reader->decomper);
  free(file_reader);
  return 0;
}


int get_slice(struct FileReader *file_reader) {
  // Slice bounds
  int slice_end;
  if (file_reader->position + file_reader->buffer_sz < file_reader->file_sz) {
    slice_end = file_reader->position + file_reader->buffer_sz;
  } else {
    slice_end = file_reader->file_sz;
  }
  // Read via JS and wait for async call to complete
  // Data slice is placed into a shared buffer, accessible from both JS and C
  read_input_file_slice(file_reader->position, slice_end, file_reader->name);
  while (get_reader_data(file_reader->name, "fdr.readyState") != 2 /*FileReader.DONE*/) {
    emscripten_sleep(50);
  }
  // (Re)set input buffer position and amount consumed
  file_reader->decomper->next_in = file_reader->buffer_shared;
  file_reader->decomper->avail_in = slice_end - file_reader->position;
  file_reader->position = slice_end;
  return 0;
}


int read_data(struct FileReader *file_reader, void *buffer, unsigned int length) {
  // Loop until output if full or no input remaining
  file_reader->decomper->next_out = buffer;
  file_reader->decomper->avail_out = length;

  do {
    // Get next slice if all input buffer is consumed
    if (file_reader->decomper->avail_in == 0) {
      get_slice(file_reader);
    }
    // Decompress input buffer data, copying result into kseq buffer
    inflate_slice(file_reader->decomper);
  } while(file_reader->decomper->avail_out && file_reader->decomper->avail_in);

  int buffer_used = length - file_reader->decomper->avail_out;
  return buffer_used;
}
