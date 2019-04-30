CPPFLAGS = -std=gnu++98 -ggdb3 -pedantic -Wall -Werror
myShell: myShell.cpp cmdLine.h builtin.h envMap.h parse.h path.h
	g++ -o myShell $(CPPFLAGS) myShell.cpp
myProg: myProg.cpp
	g++ -o myProg $(CPPFLAGS) myProg.cpp
brokenProg: brokenProg.cpp
	g++ -o brokenProg $(CPPFLAGS) brokenProg.cpp
killProg: killProg.cpp
	g++ -o killProg $(CPPFLAGS) killProg.cpp
printInput: printInput.cpp
	g++ -o printInput $(CPPFLAGS) printInput.cpp
printArgs: printArgs.cpp
	g++ -o printArgs $(CPPFLAGS) printArgs.cpp
write: write.cpp
	g++ -o write $(CPPFLAGS) write.cpp
clean:
	rm *~
