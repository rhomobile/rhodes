List of folders and files in APD Kit v2.19.06 
==============================================

Main folder:-
   Readme.txt		This file.
   APD Overview.pdf An overview of the Adaptive Printer Driver
   
   NOTE: It is assumed that, if you use the APD, you have read and accept the terms of the licence.


"APD_dll" folder:-
   PrtLib.h		Header file required for application development (not copied to mobile device).

  "ARMRel" folder (for mobile devices using StrongARM or X-Scale processors):-
	PrtLib.dll	Adaptive Printer Driver. Copy to "\Application\APD" and "\Windows" folders.
	PrtLib.lib	Library file required for application development (not copied to mobile device).
	APD-com5.reg	Registry entry to set "COM5" as the serial port on mobile device.
			This file is only required for MC50 terminal. Copy to "\Application" folder.

  "MIPSRel" folder (for older mobile devices using MIPS processors):-
	PrtLib.dll	Adaptive Printer Driver. Copy to "\Application\APD" and "\Windows" folders.
	PrtLib.lib	Library file required for application development (not copied to mobile device).


"Demo_app" folder:-
   APD-demo.cpy		'Copy' file, activated during a 'cold boot', that automatically transfers files from the 
			"\Application\APD" folder to their working folders.

  "ARMRel" folder (for mobile devices using StrongARM or X-Scale processors):-
	Demo_dll.exe	Demonstration/test application using the Adaptive Printer Driver. 
			Copy to "\Application\APD" and "\Program Files" folders.
	APD Print Demo	Shortcut to above. Copy to "\Application\APD" and "\Windows\Start Menu\Programs" folders.

  "MIPSRel" folder (for older mobile devices using MIPS processors):-
	Demo_dll.exe	Demonstration/test application using the Adaptive Printer Driver. 
			Copy to "\Application\APD" and "\Program Files" folders.
	APD Print Demo	Shortcut to above. Copy to "\Application\APD" and "\Windows\Start Menu\Programs" folders.


"Data" folder:-
   A number of sample data files are provided for use with the "APD Print Demo" application.
   Full details are given in the "Readme.txt" file within this folder.
  
  "Formats" folder:-
   Copy this folder and contents to "\Application\APD" folder on mobile device.

  "Template" folder:-
   Copy this folder and contents to "\Application\APD" folder on mobile device.

  "ID_Label" folder:-
   Copy this folder and contents to "\Application\APD" folder on mobile device if the
     mobile device is intended to use "SPAN" connectivity to the printer.

----
END.
====