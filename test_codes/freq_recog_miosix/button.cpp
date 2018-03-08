
#include "button.h"
#include <miosix.h>
#include <miosix/kernel/scheduler/scheduler.h>

using namespace miosix;

typedef Gpio<GPIOA_BASE,0> button;

static Thread *waiting=0; // Global because interrupts cannot take params, they must use global variables to access outside info

void __attribute__((naked)) EXTI0_IRQHandler() //interrupt handler function
{
    saveContext();
    asm volatile("bl _Z16EXTI0HandlerImplv"); //calls a function in assembly
    restoreContext();
}

void __attribute__((used)) EXTI0HandlerImpl()
{
    EXTI->PR=EXTI_PR_PR0; //clears pending flag
    
    if(waiting==0) return; //if no one is waiting
    waiting->IRQwakeup(); //if there is someone waiting, wake up
    if(waiting->IRQgetPriority()>Thread::IRQgetCurrentThread()->IRQgetPriority())
		Scheduler::IRQfindNextThread(); //get the priority of the waiting thread: if it is higher than the cuttent thread's priority, call the scheduler
    waiting=0;
}

void configureButtonInterrupt()
{
    button::mode(Mode::INPUT_PULL_DOWN); //pull the button pin down
    EXTI->IMR |= EXTI_IMR_MR0; 
    EXTI->RTSR |= EXTI_RTSR_TR0; //rising edge (from one to zero = interrupt)
    NVIC_EnableIRQ(EXTI0_IRQn); //set the interrupt
    NVIC_SetPriority(EXTI0_IRQn,15); //set Low priority for the interrupt
}

void waitForButton()
{
    FastInterruptDisableLock dLock; //disable interrupts since this is a critical section
    waiting=Thread::IRQgetCurrentThread(); //IRQ isa prefix for functions designed to be called when interrupts are disabled
    while(waiting)
    {
        Thread::IRQwait(); //tells the OS that this thread must block
	FastInterruptEnableLock eLock(dLock); //re-enables the interrupts at the end of this scope (next '}')
        Thread::yield();
    }
}
