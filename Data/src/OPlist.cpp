#include "./Data/include/OPlist.h"

//-----------------------------------------------------------------------------
OPint _oplNextExceedsCap(OPlist* list){
	OPint eleSize = list->_elementSize;
	if((list->_size + 1) * eleSize >= list->_capacity * eleSize)
		return 1;
	else
		return 0;
}

//-----------------------------------------------------------------------------
OPlist* OPlistCreate(OPint capacity, OPint elementSize){
	OPlist* list = (OPlist*)OPalloc(sizeof(OPlist));

	list->_capacity = capacity;
	list->_elementSize = elementSize;
	list->_indices = (ui8*)OPalloc(sizeof(ui8) * elementSize * capacity);
	list->_size = 0;

	return list;
}
//-----------------------------------------------------------------------------
OPint OPlistDestroy(OPlist* list){
	OPfree(list->_indices);
	return 1;
}
//-----------------------------------------------------------------------------
OPint OPlistPush(OPlist* list, ui8* value){
	OPint eleSize = list->_elementSize;

	if(_oplNextExceedsCap(list)){
		list->_capacity *= 2;
		list->_indices = (ui8*)OPrealloc(list->_indices, eleSize * list->_capacity);
	}

	// copy bytes into the array
	OPmemcpy((list->_indices + (list->_size * eleSize)), value, eleSize);

	list->_size++;

	return list->_size - 1;
}

//-----------------------------------------------------------------------------
ui8* OPlistPop(OPlist* list){
	list->_size--;
	return list->_indices + list->_size * list->_elementSize;
}

//-----------------------------------------------------------------------------
ui8* OPlistPeek(OPlist* list) {
	return list->_indices + (list->_size - 1) * list->_elementSize;
}

//-----------------------------------------------------------------------------
OPint OPlistInsert(OPlist* list, ui8* value, OPuint index){
	OPuint eleSize = list->_elementSize;
	OPuint i;

	// expand the array if needed
	if(_oplNextExceedsCap(list)){
		list->_capacity *= 2;
		list->_indices = (ui8*)OPrealloc(list->_indices, eleSize * list->_capacity);
	}
	list->_size++;

	// shift elements to the right one
	for(i = list->_size; i > index; i--){
		OPlistSet(list, i, OPlistGet(list, i - 1));
	}

	// set the value
	OPlistSet(list, index, value);

	return 1;
}
//-----------------------------------------------------------------------------
OPint OPlistRemoveAt(OPlist* list, OPuint index){
	OPuint i;
	list->_size--;

	// shift all the elements to the left copying over the value at index
	for(i = index; i < list->_size; i++){
		OPlistSet(list, i, OPlistGet(list, i+1));
	}
	return 1;
}
//-----------------------------------------------------------------------------
ui8* OPlistGet(OPlist* list, OPuint index){
	return list->_indices + (index * list->_elementSize);
}
//-----------------------------------------------------------------------------
ui8* OPlistSet(OPlist* list, OPuint index, ui8* value){
	OPint eleSize = list->_elementSize, i = 0;

	// copy bytes into the array
	for(i = eleSize; i--;)
		list->_indices[index * eleSize + i] = value[i];

	return list->_indices + (index * list->_elementSize);
}
//-----------------------------------------------------------------------------
OPuint OPlistSize(OPlist* list){
	return list->_size;
}