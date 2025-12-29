all:
	if [ ! -f seqexec.ca.TCS ]; then ln -s seqexec.ca seqexec.ca.TCS; fi
	cd lib; gmake
	cd lib/gmos; gmake
	cd lib/niri; gmake

clean:
	rm seqexec.ca.TCS
	cd lib; gmake clean
	cd lib/gmos; gmake clean
	cd lib/niri; gmake clean
