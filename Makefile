processflow:
	gcc processflow.c -o processflow

clean:
	rm -f processflow

run: processflow
	./processflow