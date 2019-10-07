.RECIPEPREFIX = +

CFLAGS = -g -std=c99 -Wall -Werror

coder: coder.o confab.o
+gcc coder.o confab.o -o coder -lm

coder.o: coder.c confab.h
+gcc $(CFLAGS) -c coder.c

confab.o: confab.c confab.h
+gcc $(CFLAGS) -c confab.c

clean:
+rm *.o
