// prevent name mangling if compiling with c++
#ifdef __cplusplus
using namespace std;

extern "C" {
#endif
#include <stdio.h>
#include "./../include/Core.h"

OPint* i;

void initialize(){
	i = (OPint*)OPalloc(sizeof(OPint));
	*i = 1337;
	printf("Int: %d Size: %d\n", *i, sizeof(OPint));
	printf("PI = %f\n", OPpi); 


	printf("i = %d\n", (OPint)i);

	OPfloat pi = -21;//OPpi;
	printf("-pi = %f\nabs(-pi) = %f\nsin(-pi) = %f\nceil(-pi) = %d\n", pi, (OPfloat)OPabs(pi), (OPfloat)OPsin(pi), (OPint)OPceil(pi));
	
	printf("Rand 0 - 1000: %f\n", OPrandRange(0, 1000));
	printf("Random: %f\n", OPrandom());
	*i = 0;
}
//----------------------------------------------------------------------------
void update(OPtimer* timer){
	if(*i){
		printf("Seconds: %f\n", OPtimerDelta(timer));
		OPend();
	}

	*i = (OPint)getchar();
}
//----------------------------------------------------------------------------
void destroy(){
	OPfree(i);
}
//----------------------------------------------------------------------------
OPint main(){
	// use custom functions for
	// the game's duration
	OPinitialize = initialize;
	OPupdate = update;
	OPdestroy = destroy;
	
	// start the game cycle
	OPstart();

	return 0;
}

// prevent name mangling if compiling with c++
#ifdef __cplusplus
}
#endif
