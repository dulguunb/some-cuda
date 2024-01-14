Bitmap.o: Bitmap.cpp
	nvcc -c Bitmap.cpp -o Bitmap.o -x cu
main: main.cpp Bitmap.o
	nvcc main.cpp Bitmap.o -o main
clean:
	rm *.o main