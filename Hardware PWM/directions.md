# Hardware PWM
Now that you have done the software version of PWM, it is time to start leveraging other features of these Timer Modules.

## Task
Replicate the same behavior as in the software PWM, but by using the Timer Modules ability to directly output to a GPIO Pin instead of managing them in software. One way to think about this is: unless there are other functions running in your code, your system should initialize, set the Timer Modules, and then turn off the CPU.

## Deliverables
You will need to have two folders in this repository, one for each of the processors that you used for this part of the lab. Remember to replace this README with your own.

### Hints
Read up on the P1SEL registers as well as look at the Timer modules ability to multiplex.

## Extra Work
### Using ACLK
Some of these microprocessors have a built in ACLK which is extremely slow compared to the up to 25MHz available on some of them. What is the overall impact on the system when using this clock? Can you actually use your PWM code with a clock that slow?
