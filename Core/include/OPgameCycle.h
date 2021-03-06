#pragma once

#include "./Core/include/OPtimer.h"

/**
 * Game engine initialization.
 *	This function pointer points to a user function which is responsible
 *	for performing any essential initialization, data allocation and setup
 *	which is needed to begin the primary game loop.
 */
extern void (*OPinitialize)();

//----------------------------------------------------------------------------
/**
 * Game engine update.
 *	This function pointer points to a user function which accepts an
 *	OPtimer as an argument. This function performs whatever action needs
 *	to be taken to actually run the game. For example, this function would
 *	be responsible for invoking game-logic updates, input device polling
 *	and scene rendering.
 * @param timer OPtimer object representing current time measurments
 */
extern OPint(*OPupdate)(struct OPtimer*);

//----------------------------------------------------------------------------
/**
* Game engine render.
*	This function pointer points to a user function which accepts an
*	OPfloat as an argument. This function performs whatever actions are
*   needed to do scene rendering.
* @param delta OPfloat object representing time into the frame
*/
extern void(*OPrender)(OPfloat);

//----------------------------------------------------------------------------
/**
 * Game engine termination.
 *	This function pointer points to a user function which is responsible
 *	for gracefully terminating the game engine. This may include saving
 *	data, deallocating memory, releasing OS resources and closing
 *	network connections.
 */
extern void (*OPdestroy)();