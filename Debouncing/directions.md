# Software Debouncing
Debouncing a signal is a necessary step when adding mechanical switches to digital circuits so a single press doesn't appear like multiple presses. You're going to need this feature in a later portion of this lab as you'll be using a button press to change the duty cycle of your signal and you don't want to count the "false positives" as a button press. Debouncing is also only one of many applications which would require the use of built in Timers to allow for other processes to take place. Check out this nice tutorial on the subject. [http://www.labbookpages.co.uk/electronics/debounce.html]

## Task
You'll need to use the TIMER modules within the MSP430 processors to implement a debounced switch to control the state of an LED. You'll likely want to hook up your buttons on the development boards to an oscilloscope to see how much time it takes for the buttons to settle. The idea here is that your processor should be able to run other code, while relying on timers and interrupts to manage the debouncing in the background. *You should not be using polling techniques for this assignment.*

## Deliverables
You will need to have two folders in this repository, one for each of the processors that you used for this part of the lab. Remember to replace this README with your own.

### Hints
You need to take a look at how the P1IE and P1IES registers work and how to control them within an interrupt routine. Remember that the debouncing is not going to be the main process you are going to run by the end of the lab.

## Extra Work
### Low Power Modes
Go into the datasheets or look online for information about the low power modes of your processors and using Energy Trace, see what the lowest power consumption you can achieve while still running your debouncing code. Take a note when your processor is not driving the LED (or unplug the header connecting the LED and check) but running the interrupt routine for your debouncing.

### Double the fun
Can you expand your code to debounce two switches? Do you have to use two Timer peripherals to do this?
