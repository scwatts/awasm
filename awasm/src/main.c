#include <emscripten.h>


#include "disk_streamer.h"
#include "velveth.h"
#include "velvetg.h"


EMSCRIPTEN_KEEPALIVE
int run() {
  EM_ASM(Module.execution_api.update(1));

  setup_idbfs();
  char *prefix = "/IDBFS/velvet";

  EM_ASM(Module.step_api.update(Module.step_enum.velveth));
  velveth(prefix);

  EM_ASM(Module.step_api.update(Module.step_enum.velvetg));
  velvetg(prefix);

  EM_ASM(Module.step_api.update(Module.step_enum.complete));
  EM_ASM(Module.execution_api.update(0));

  return 0;
}
