#ifndef __SETTINGS__
#define __SETTINGS__


#define CPU_CLOCK_MHZ                   80000000                                // частота CPU в МГц
#define EXT_GENERATOR_FREQ              16000000                                // частота внешнего генератора
#define CPU_PLL_KOEF                    ((CPU_CLOCK_MHZ/EXT_GENERATOR_FREQ)-1)  // коэф. умножения PLL


#define SELFLOADER_DEFAULT_NAME        ("Angle MK Z")
#define SELFLOADER_DEFAULT_ID          0x81



#endif //__SETTINGS__