#include "include/OPstream.h"
#include <stdio.h>

int main(){
	OPstream* stream = (OPstream*)OPalloc(sizeof(OPstream));
	stream->Data = (ui8*)OPalloc(sizeof(OPint) * 10);
	stream->Length = 10 * sizeof(OPint);
	stream->_pointer = 0;

	OPwrite(stream, 10, sizeof(OPint));

	return 0;
}
