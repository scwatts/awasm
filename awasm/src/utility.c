#include "utility.h"


void fssync_block() {
  // Async call
  EM_ASM(
    {
      Module.fsync = 1;
      FS.syncfs(!!1, function(err) {
        // TODO: error handling
        Module.fsync = 0;
      });
    }
  );
  // Block
  while (emscripten_run_script_int("Module.fsync") == 1) {
    emscripten_sleep(5);
  }
}


int setup_idbfs() {
  char object_store[] = "/IDBFS";
  DIR *dir = opendir(object_store);
  if (dir) {
    closedir(dir);
  } else if (ENOENT == errno) {
    EM_ASM(
      {
        FS.mkdir(Module.UTF8ToString($0));
        FS.mount(IDBFS, {}, Module.UTF8ToString($0));
      },
      object_store
    );
    fssync_block();
  } else {
    return 1;
  }
  return 0;
}


