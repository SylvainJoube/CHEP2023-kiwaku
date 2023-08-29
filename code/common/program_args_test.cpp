#include "program_args.hpp"

int main(int argc, char** argv)
{
  program_args::parse(argc, argv, true);
}