all:
	cd src/; make; mv llama ../llama

rlwrap:
	cd rlwrap*/; ./configure; make
	cd src/; make; mv llama ../llama
	cd src/; cp llama-rlwrap.sh ../llama-rlwrap.sh

clean:
	cd src/; make clean
	rm -rf llama