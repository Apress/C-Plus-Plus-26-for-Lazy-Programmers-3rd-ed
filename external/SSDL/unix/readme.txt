To install SSDL for Unix, run make in this directory.

It currently makes one version of the library:  shared (.so).

To run an SSDL program in Unix, you need this in the library path. In the sample code, ./runx uses a script that knows to find it. You could also put it in a folder where the operating system can find it:  maybe the project folder. Better yet, wherever libSDL2.so[.*] may be (possibly under /usr/lib), or export LD_LIBRARY_PATH=<where you put it>; and then just run the executable, often a.out.

The makefile creates a tester program shared-test; you can run it with the script runx, to verify that make worked (that is, that the library built and the program runs). Run it thus: ./runx

If you opt to try a static version, uncomment the rest of the line for all in the Makefile; run it with ./run-static-test.

