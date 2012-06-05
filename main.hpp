#ifndef MAIN_HPP_
#define MAIN_HPP_

class Board;

int Atoi(const char *str);

int testing();

void process(std::string fname, uint width, uint height, uint turns,
		bool wrapx, bool wrapy, bool expand, bool pict);

#endif /* MAIN_HPP_ */
int main(int argc, char **argv);
