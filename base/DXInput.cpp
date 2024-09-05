#include "DXInput.h"

DXInput::DXInput()
{
}

DXInput::~DXInput()
{
}

void DXInput::InputProcess() {

    XInputGetState(0, &GamePad.state);

    int pad_A = 0, pad_B = 0, pad_X = 0, pad_Y = 0;
    int pad_leftShoulder = 0, pad_rightShoulder = 0;
    int pad_left = 0, pad_right = 0, pad_up = 0, pad_down = 0;

    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_LEFT) pad_left = 1;               //ゲームパッド十字キー左
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_RIGHT) pad_right = 1;             //ゲームパッド十字キー右
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_UP) pad_up = 1;                   //ゲームパッド十字キー上
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_DPAD_DOWN) pad_down = 1;               //ゲームパッド十字キー下
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_A) pad_A = 1;                          //ゲームパッドA
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_B) pad_B = 1;                          //ゲームパッドB
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_X) pad_X = 1;                          //ゲームパッドX
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_Y) pad_Y = 1;                          //ゲームパッドY
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_LEFT_SHOULDER) pad_leftShoulder = 1;   //ゲームパッドL
    if (GamePad.state.Gamepad.wButtons & XINPUT_GAMEPAD_RIGHT_SHOULDER) pad_rightShoulder = 1; //ゲームパッドR

    // ゲームパッドデッドゾーン処理
    if ((GamePad.state.Gamepad.sThumbLX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
        GamePad.state.Gamepad.sThumbLX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
        (GamePad.state.Gamepad.sThumbLY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
            GamePad.state.Gamepad.sThumbLY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
    {
        GamePad.state.Gamepad.sThumbLX = 0;
        GamePad.state.Gamepad.sThumbLY = 0;
    }
    if ((GamePad.state.Gamepad.sThumbRX < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
        GamePad.state.Gamepad.sThumbRX > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE) &&
        (GamePad.state.Gamepad.sThumbRY < XINPUT_GAMEPAD_RIGHT_THUMB_DEADZONE &&
            GamePad.state.Gamepad.sThumbRY > -XINPUT_GAMEPAD_LEFT_THUMB_DEADZONE))
    {
        GamePad.state.Gamepad.sThumbRX = 0;
        GamePad.state.Gamepad.sThumbRY = 0;
    }

    // ゲームパッドの振動
    /*XInputSetState(0, &GamePad.vibration);*/

    if (pad_A)
    {
        GamePad.vibration.wLeftMotorSpeed = MaxVibration;
        /*XInputSetState(0, &GamePad.vibration);*/
    }
    if (pad_B)
    {
        GamePad.vibration.wRightMotorSpeed = MaxVibration;
        /*XInputSetState(0, &GamePad.vibration);*/
    }
    if (pad_X)
    {
        GamePad.vibration.wLeftMotorSpeed = MaxVibration;
        /* XInputSetState(0, &GamePad.vibration);*/
    }
    if (pad_Y)
    {
        GamePad.vibration.wRightMotorSpeed = MaxVibration;
        /*XInputSetState(0, &GamePad.vibration);*/
    }

    /*WCHAR wcText[256] = { 0 };
    swprintf(wcText, 256, L"PAD_A=%d PAD_B=%d PAD_X=%d PAD_Y=%d", pad_A, pad_B, pad_X, pad_Y);*/

}

void DXInput::XInputInvaid() {

    //XInputEnable(false); // XInputを無効

}