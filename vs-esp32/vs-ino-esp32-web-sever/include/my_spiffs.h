



#ifndef  _MY_SPIFS_H_
#define  _MY_SPIFS_H_
 

#ifndef  __MY_SPIFFS_APP
#define	 __MY_SPIFFS_APP_EXT__  extern
#else
#define	 __MY_SPIFFS_APP_EXT__ 
#endif

#include "FS.h"
#include "SPIFFS.h"


__MY_SPIFFS_APP_EXT__ void my_spiffs_init(void);
__MY_SPIFFS_APP_EXT__ void my_spiffs_sample(void);

#endif

