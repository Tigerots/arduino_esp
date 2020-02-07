


#ifndef  __MY_LED_H__
#define  __MY_LED_H__
 

#ifndef  __MY_LED_APP
#define	 __MY_LED_EXT__  extern
#else
#define	 __MY_LED_EXT__ 
#endif

__MY_LED_EXT__ int led_sta;
__MY_LED_EXT__ void my_led_run(int sta);
__MY_LED_EXT__ void my_led_init(void);

#endif

