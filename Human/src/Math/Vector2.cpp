#include "./Human/include/Math/Vector2.h"

OPfloat& Vector2::operator[](int idx){		
	switch(idx){
	case 0:
		return x;
		break;
	case 1:
		return y;
		break;
	}
}
