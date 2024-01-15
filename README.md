# WebCam_ImageProcessing

This code is heavily based on code from kinddev813.

Here's a piece of the introduction comments from the mainfile WebCam_ImageProcessing.cpp

	8-Bits Of Image Processing You Should Know

	License (OLC-3)
	~~~~~~~~~~~~~~~
	Copyright 2018-2019 OneLoneCoder.com

**A single processor can now:**
* Register itself with typeid and factory method
* Return its name
* Process a image frame
* Write info to UI about self
* Process keyboard input
* Hold private frames for sequential image processing

**As a user, with this code running, you can:**
* Add one processor after another using the ENTER key
* Delete the last added with the DEL key
* The last processor is selectable from all available using the PGUP and PGDN keys
* The last added processors parameters, can be edited with the arrow keys, while viewing the image change

**As a developer you add new image processors by:**
* copy an existing processor file
* Replace the name throughout the file
* Replace the image processing code
* Add the processor with #include at the top of the main cpp file
