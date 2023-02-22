#ifndef _PINCTRL_TEST_H_
#define _PINCTRL_TEST_H_

#include <linux/mutex.h>

enum {
  PINCTRL_GPIOH_4_ON = 0,
  PINCTRL_GPIOH_4_OFF,
};

struct aml_pinctl_gpio {
  const char* name;
  unsigned int active_low;
  unsigned int pin;
  unsigned int state;
};
// pinctrl_dev设备实体
struct aml_pinctrl_dev {
  struct aml_pinctl_gpio d;
  struct mutex lock;
};


#endif