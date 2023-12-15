compiler=g++
FLAGS=-Wall -g
c11 = -std=c++11

output: social_network.o network.o user.o post.o
	$(compiler) $(FLAGS) $(c11) $^ -o $@	#compiled everything

test: test.o network.o user.o post.o
	$(compiler) $(FLAGS) $(c11) $^ -o $@	

%.o: %.cpp
	$(compiler) $(FLAGS) $(c11) -c $< -o $@      

clean:
	rm output
	rm *.o
	rm test