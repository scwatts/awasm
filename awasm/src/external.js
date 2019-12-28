mergeInto(LibraryManager.library, {
  read_input_file_slice: function(slice_start, slice_end, reader_name_cstr) {
    // Define call back once data is processed
    var reader_name =  Module.UTF8ToString(reader_name_cstr);
    Module.readers[reader_name].fdr.onloadend = function(evt) {
      if (evt.target.readyState == FileReader.DONE) {
        // Cast to uint8 array and copy to shared memory, previously malloc'ed
        const data = new Uint8Array(evt.target.result);
        Module.HEAPU8.set(data, Module.readers[reader_name].buf);
      }
    };
    // Process
    var slice_data = Module.readers[reader_name].fob.slice(slice_start, slice_end);
    Module.readers[reader_name].fdr.readAsArrayBuffer(slice_data);
  },
});
