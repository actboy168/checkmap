#include <stdio.h>
#include "checkmap.h"

int main()
{
  char MapPath[] = "D:\\Warcraft III_122\\Maps\\(2)BootyBay.w3m";

  printf("%s\n", CheckMap(MapPath)?"TRUE":"FALSE");

  return 0;
}