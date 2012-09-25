full:
	cd bin; \
	cmake ..; \
	rm -rf ./Spike; \
	make -j4; \
	./Spike
quick:
	cd bin; \
	rm -rf ./Spike; \
	make -j4; \
	./Spike

clean:
	rm -rf bin; \
	mkdir bin;



