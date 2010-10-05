/**
 * @page OperatingSystems What is a operating system?
 * @section WhatIs What is a operating system?
 *
 * A operating system is in theory, the layer of software between the hardware and the application. What this means is that a operating system (Henceforth referred to as OS) provides the code that handles the applications running on a system, and all the hardware connected to that system. They also (Usually) provide some way of interacting with the hardware and the end-user, in the case of Dawn this is done through the application programming interface (API).
 *
 *
 * Operating systems are notoriously difficult to write, not because they require drastically harder techniques and knowledge (Though some of the concepts that programmers of operating systems have to deal with will be very difficult), but mainly because prior to the actual implementation of the project there is no way to predict exactly what you will need to achieve your aims, as many seemingly unrelated areas of a operating system will come to rely on others. The other issue with the development of operating systems is that they are extremely difficult to test and debug, this is because unlike user-space applications (Programs which run on top of a operating system) there is no big brother maintaining the integrity, if something goes wrong it is likely the entire system will crash with no message as to what went wrong – even in emulated environments the most you can hope for is a nondescript error message and a dump of the state of the processor – often leaving the developer clueless as to what has gone wrong.
 *
 *
 *Another note is that operating systems usually have no clear-cut purpose. Most are designed to be as versatile as possible, able to handle multiple situations and run applications designed for industry, entertainment, enterprise etcetera. This leads to many interesting design decisions and layers of abstraction – more of which will be noted in individual sections of theory within this Wiki.
 *
 */
