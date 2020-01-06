#include "JumpAPP.H"
#include "stm32f10x.h"

pFunction Jump_To_Application;
uint32_t JumpAddress;

void JumpAPP(uint32_t Address)
{
  JumpAddress = *(__IO uint32_t*) (Address + 4);

  /* Jump to user application */
  Jump_To_Application = (pFunction) JumpAddress;
  /* Initialize user application's Stack Pointer */
  __set_MSP(*(__IO uint32_t*) Address);
  Jump_To_Application();
}
