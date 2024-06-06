SRC =		src/*.cpp


NAME =		Brain

CXX =		g++ -g

CXXFLAGS = 	-W -Wall -Wextra -std=c++20 -g

LIBS = 		-lm -lsfml-graphics -lsfml-window -lsfml-system -pthread

OBJ = $(SRC:.cpp=.o)

all: $(NAME)

$(NAME): $(OBJ)
	$(CXX) $(OBJ) -o $(NAME) $(LIBS) 

clear:
	clear


SUPP = --suppressions=./valSup.supp #--gen-suppressions=all

valgrind: clean fclean all
	valgrind --track-origins=yes --leak-check=full --show-leak-kinds=all $(SUPP) ./$(NAME)

run: fclean clear re
	-./$(NAME)

time: fclean clear re
	echo "standar test" >> time
	time -o time -a -p ./$(NAME)
	echo "" >> time
	time -o time -a -p ./$(NAME)
	echo "" >> time
	time -o time -a -p ./$(NAME)
	echo "" >> time
	time -o time -a -p ./$(NAME)
	echo "" >> time
	time -o time -a -p ./$(NAME)
	echo "\n####################\n" >> time

timeO4: fclean clear allO4
	echo "O4 test" >> time
	time -o time -a -p ./$(NAME)
	echo "" >> time
	time -o time -a -p ./$(NAME)
	echo "" >> time
	time -o time -a -p ./$(NAME)
	echo "" >> time
	time -o time -a -p ./$(NAME)
	echo "" >> time
	time -o time -a -p ./$(NAME)
	echo "\n####################\n" >> time


allO4:
	g++ -o $(NAME) $(SRC) $(FLAGS) -O4 $(LIBS)

runO4: fclean clear allO4
	./$(NAME)


clean:
	$(RM) $(OBJ)

fclean: clean
	$(RM) $(NAME)

re: fclean all

.PHONY: all clean fclean re

callgrind: fclean
	rm -f callgrind.*
	clear
	g++ -g -o $(NAME) $(SRC) $(FLAGS) $(LIBS)
	-valgrind --tool=callgrind $(SUPP) ./$(NAME) #&> valgrind_log
	-kcachegrind callgrind.*

callgrindO6: fclean
	rm -f callgrind.*
	clear
	g++ -g -o $(NAME) $(SRC) $(FLAGS) -O6 $(LIBS)
	-valgrind --tool=callgrind $(SUPP) ./$(NAME) #&> valgrind_log
	-kcachegrind callgrind.*

hellgrind: fclean
	clear
	clear
	g++ -g -o $(NAME) $(SRC) $(FLAGS) $(LIBS)
	valgrind --tool=helgrind --read-var-info=yes ./$(NAME) | cat -e #&> valgrind_log
