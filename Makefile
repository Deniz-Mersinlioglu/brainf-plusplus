all:
	#i know, bad Makefile habits. i dont really care though since theres only one file anyways.
	gcc main.c -o brainf-plusplus
	#And that's all, folks!
	#And no, you shouldn't install this.
install: brainf-plusplus
	#You don't even get to choose where to install to. That's how bad this Makefile is.
	cp brainf-plusplus /usr/bin
