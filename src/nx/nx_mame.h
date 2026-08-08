
#include "osdepend.h"
#include "osd_cpu.h"
#undef BIT

void nx_fileio_init(void);
void osd_print_error(const char *fmt, ...);
void RenderMessage(char *name);
struct rom_load_data;
void RenderProgress(const char *name, struct rom_load_data *romdata);
 