#pragma once

struct OPgamePad;
typedef struct OPgamePad OPgamePad;

#include "./Core/include/OPtypes.h"
#include "./Math/include/OPvec2.h"
#include "Enums/OPgamePadAxes.h"
#include "Enums/OPgamePadButtons.h"
#include "Enums/OPgamePadIndices.h"
#include "./Core/include/Assert.h"

struct OPgamePad {
	OPgamePad(i8 index) {
		controllerIndex = index;
		connected = false;
		deadzone = 0.1f;
	}

	i8 controllerIndex;
	OPfloat axes[(ui32)OPgamePadAxis::_MAX];
	OPfloat prevAxes[(ui32)OPgamePadAxis::_MAX];
	OPfloat deadzone;
	i64 rumbleTime;
	OPfloat rumble[2];
	bool connected;
	bool buttons[(ui32)OPgamePadButton::_MAX];
	bool prevButtons[(ui32)OPgamePadButton::_MAX];

	void Update(OPtimer* timer);
	void Reset();

	OPvec2 LeftThumb();
	OPvec2 RightThumb();

	bool IsConnected() {
		return connected;
	}

	void SetDeadzone(OPfloat deadzone) {
		this->deadzone = deadzone;
	}

	void SetRumble(OPfloat left, OPfloat right, i64 time);
	inline void SetRumble(OPfloat left, OPfloat right) {
		SetRumble(left, right, 2147483647);
	}
	inline void SetRumbleShort() {
		SetRumble(1.0, 1.0, 200);
	}
	inline void SetRumbleLong() {
		SetRumble(1.0, 1.0, 500);
	}

	// Buttons

	bool IsDown(OPgamePadButton button) {
		return buttons[(ui32)button];
	}
	bool IsUp(OPgamePadButton button) {
		return !buttons[(ui32)button];
	}
	bool WasPressed(OPgamePadButton button) {
		return buttons[(ui32)button] && !prevButtons[(ui32)button];
	}
	bool WasReleased(OPgamePadButton button) {
		return !buttons[(ui32)button] && prevButtons[(ui32)button];
	}

	// Left Thumbstick

	bool LeftThumbIsRight() {
		return axes[(ui32)OPgamePadAxis::LS_X] > deadzone;
	}
	bool LeftThumbNowRight() {
		return prevAxes[(ui32)OPgamePadAxis::LS_X] <= deadzone && axes[(ui32)OPgamePadAxis::LS_X] > deadzone;
	}
	bool LeftThumbWasRight() {
		return prevAxes[(ui32)OPgamePadAxis::LS_X] > deadzone && axes[(ui32)OPgamePadAxis::LS_X] <= deadzone;
	}
	bool LeftThumbIsLeft() {
		return axes[(ui32)OPgamePadAxis::LS_X] < -deadzone;
	}
	bool LeftThumbNowLeft() {
		return prevAxes[(ui32)OPgamePadAxis::LS_X] >= -deadzone && axes[(ui32)OPgamePadAxis::LS_X] < -deadzone;
	}
	bool LeftThumbWasLeft() {
		return prevAxes[(ui32)OPgamePadAxis::LS_X] < -deadzone && axes[(ui32)OPgamePadAxis::LS_X] >= -deadzone;
	}
	bool LeftThumbIsUp() {
		return axes[(ui32)OPgamePadAxis::LS_Y] > deadzone;
	}
	bool LeftThumbNowUp() {
		return prevAxes[(ui32)OPgamePadAxis::LS_Y] <= deadzone && axes[(ui32)OPgamePadAxis::LS_Y] > deadzone;
	}
	bool LeftThumbWasUp() {
		return prevAxes[(ui32)OPgamePadAxis::LS_Y] > deadzone && axes[(ui32)OPgamePadAxis::LS_Y] <= deadzone;
	}
	bool LeftThumbIsDown() {
		return axes[(ui32)OPgamePadAxis::LS_Y] < -deadzone;
	}
	bool LeftThumbNowDown() {
		return prevAxes[(ui32)OPgamePadAxis::LS_Y] >= -deadzone && axes[(ui32)OPgamePadAxis::LS_Y] < -deadzone;
	}
	bool LeftThumbWasDown() {
		return prevAxes[(ui32)OPgamePadAxis::LS_Y] < -deadzone && axes[(ui32)OPgamePadAxis::LS_Y] >= -deadzone;
	}
	OPfloat LeftThumbX() {
		return axes[(ui32)OPgamePadAxis::LS_X];
	}
	OPfloat LeftThumbY() {
		return axes[(ui32)OPgamePadAxis::LS_Y];
	}

	// Right Thumbstick

	bool RightThumbIsRight() {
		return axes[(ui32)OPgamePadAxis::RS_X] > deadzone;
	}
	bool RightThumbNowRight() {
		return prevAxes[(ui32)OPgamePadAxis::RS_X] <= deadzone && axes[(ui32)OPgamePadAxis::RS_X] > deadzone;
	}
	bool RightThumbWasRight() {
		return prevAxes[(ui32)OPgamePadAxis::RS_X] > deadzone && axes[(ui32)OPgamePadAxis::RS_X] <= deadzone;
	}
	bool RightThumbIsLeft() {
		return axes[(ui32)OPgamePadAxis::RS_X] < -deadzone;
	}
	bool RightThumbNowLeft() {
		return prevAxes[(ui32)OPgamePadAxis::RS_X] >= -deadzone && axes[(ui32)OPgamePadAxis::RS_X] < -deadzone;
	}
	bool RightThumbWasLeft() {
		return prevAxes[(ui32)OPgamePadAxis::RS_X] < -deadzone && axes[(ui32)OPgamePadAxis::RS_X] >= -deadzone;
	}
	bool RightThumbIsUp() {
		return axes[(ui32)OPgamePadAxis::RS_Y] > deadzone;
	}
	bool RightThumbNowUp() {
		return prevAxes[(ui32)OPgamePadAxis::RS_Y] <= deadzone && axes[(ui32)OPgamePadAxis::RS_Y] > deadzone;
	}
	bool RightThumbWasUp() {
		return prevAxes[(ui32)OPgamePadAxis::RS_Y] > deadzone && axes[(ui32)OPgamePadAxis::RS_Y] <= deadzone;
	}
	bool RightThumbIsDown() {
		return axes[(ui32)OPgamePadAxis::RS_Y] < -deadzone;
	}
	bool RightThumbNowDown() {
		return prevAxes[(ui32)OPgamePadAxis::RS_Y] >= -deadzone && axes[(ui32)OPgamePadAxis::RS_Y] < -deadzone;
	}
	bool RightThumbWasDown() {
		return prevAxes[(ui32)OPgamePadAxis::RS_Y] < -deadzone && axes[(ui32)OPgamePadAxis::RS_Y] >= -deadzone;
	}
	OPfloat RightThumbX() {
		return axes[(ui32)OPgamePadAxis::RS_X];
	}
	OPfloat RightThumbY() {
		return axes[(ui32)OPgamePadAxis::RS_Y];
	}

	// Left Trigger

	OPfloat LeftTrigger() {
		return axes[(ui32)OPgamePadAxis::L2];
	}
	bool LeftTriggerWasPressed() {
		return axes[(ui32)OPgamePadAxis::L2] > 0 && prevAxes[(ui32)OPgamePadAxis::L2] <= 0;
	}
	bool LeftTriggerWasReleased() {
		return axes[(ui32)OPgamePadAxis::L2] <= 0 && prevAxes[(ui32)OPgamePadAxis::L2] > 0;
	}
	bool LeftTriggerIsDown() {
		return axes[(ui32)OPgamePadAxis::L2] > 0;
	}
	bool LeftTriggerIsUp() {
		return axes[(ui32)OPgamePadAxis::L2] <= 0;
	}

	// Right Trigger

	OPfloat RightTrigger() {
		return axes[(ui32)OPgamePadAxis::R2];
	}
	bool RightTriggerWasPressed() {
		return axes[(ui32)OPgamePadAxis::R2] > 0 && prevAxes[(ui32)OPgamePadAxis::R2] <= 0;
	}
	bool RightTriggerWasReleased() {
		return axes[(ui32)OPgamePadAxis::R2] <= 0 && prevAxes[(ui32)OPgamePadAxis::R2] > 0;
	}
	bool RightTriggerIsDown() {
		return axes[(ui32)OPgamePadAxis::R2] > 0;
	}
	bool RightTriggerIsUp() {
		return axes[(ui32)OPgamePadAxis::R2] <= 0;
	}

	bool AnyPrevInputIsDown();
	bool AnyPrevInputIsUp() {
		return !AnyPrevInputIsDown();
	}
	bool AnyInputIsDown();
	bool AnyInputIsUp() {
		return !AnyInputIsDown();
	}
	bool AnyInputWasPressed() {
		return AnyInputIsDown() && AnyPrevInputIsUp();
	}
	bool AnyInputWasReleased() {
		return AnyInputIsUp() && AnyPrevInputIsDown();
	}
};
