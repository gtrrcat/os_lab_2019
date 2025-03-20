#include "swap.h"

void Swap(char *left, char *right)
{
	char enter = *left;
	*left = *right;
	*right = enter;
}
