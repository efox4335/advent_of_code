#include <stdlib.h>

typedef struct{
	long row;
	long col;
}cord;

//returns the area from the shorlace fromula
long shoelace(cord *polygon, long point_count)
{
	long area = 0;
	long index = 0;

	for(index = 0; index + 1 < point_count; ++index){
		area += (polygon[index].row * polygon[index + 1].col) - (polygon[index].col * polygon[index + 1].row);
	}

	area += (polygon[index].row * polygon[0].col) - (polygon[index].col * polygon[0].row);

	area = labs(area);

	return area >> 1;
}
