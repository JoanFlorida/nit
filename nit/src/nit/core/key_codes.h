#pragma once

namespace nit
{
    enum InputState : i32
    {
        InputRelease = 0,
        InputPress = 1,
        InputRepeat = 2
    };
    
    enum HatState : i32
    {
        HatCentered = 0,
        HatUp = 1,
        HatRight = 2,
        HatDown = 4,
        HatLeft = 8,
        HatRightUp = (HatRight | HatUp),
        HatRightDown = (HatRight | HatDown),
        HatLeftUp = (HatLeft | HatUp),
        HatLeftDown = (HatLeft | HatDown)
    };
    
    enum Key : i32
    {
        KeyUnknown = -1,
        KeySpace = 32,
        KeyApostrophe = 39, // '
        KeyComma = 44, // ,
        KeyMinus = 45, // -
        KeyPeriod = 46, // .
        KeySlash = 47, // /
        Key0 = 48,
        Key1 = 49,
        Key2 = 50,
        Key3 = 51,
        Key4 = 52,
        Key5 = 53,
        Key6 = 54,
        Key7 = 55,
        Key8 = 56,
        Key9 = 57,
        KeySemicolon = 59, // ;
        KeyEqual = 61, // =
        KeyA = 65,
        KeyB = 66,
        KeyC = 67,
        KeyD = 68,
        KeyE = 69,
        KeyF = 70,
        KeyG = 71,
        KeyH = 72,
        KeyI = 73,
        KeyJ = 74,
        KeyK = 75,
        KeyL = 76,
        KeyM = 77,
        KeyN = 78,
        KeyO = 79,
        KeyP = 80,
        KeyQ = 81,
        KeyR = 82,
        KeyS = 83,
        KeyT = 84,
        KeyU = 85,
        KeyV = 86,
        KeyW = 87,
        KeyX = 88,
        KeyY = 89,
        KeyZ = 90,
        KeyLeftBracket = 91, // [
        KeyBackSlash = 92,
        KeyRightBracket = 93, // ]
        KeyGraveAccent = 96, // `
        KeyWorld1 = 161, // non-US #1
        KeyWorld2 = 162, // non-US #2

        // Function keys
        KeyEscape = 256,
        KeyEnter = 257,
        KeyTab = 258,
        KeyBackSpace = 259,
        KeyInsert = 260,
        KeyDelete = 261,
        KeyRight = 262,
        KeyLeft = 263,
        KeyDown = 264,
        KeyUp = 265,
        KeyPageUp = 266,
        KeyPageDown = 267,
        KeyHome = 268,
        KeyEnd = 269,
        KeyCapsLock = 280,
        KeyScrollLock = 281,
        KeyNumLock = 282,
        KeyPrintScreen = 283,
        KeyPause = 284,
        KeyF1 = 290,
        KeyF2 = 291,
        KeyF3 = 292,
        KeyF4 = 293,
        KeyF5 = 294,
        KeyF6 = 295,
        KeyF7 = 296,
        KeyF8 = 297,
        KeyF9 = 298,
        KeyF10 = 299,
        KeyF11 = 300,
        KeyF12 = 301,
        KeyF13 = 302,
        KeyF14 = 303,
        KeyF15 = 304,
        KeyF16 = 305,
        KeyF17 = 306,
        KeyF18 = 307,
        KeyF19 = 308,
        KeyF20 = 309,
        KeyF21 = 310,
        KeyF22 = 311,
        KeyF23 = 312,
        KeyF24 = 313,
        KeyF25 = 314,
        KeyKp0 = 320,
        KeyKp1 = 321,
        KeyKp2 = 322,
        KeyKp3 = 323,
        KeyKp4 = 324,
        KeyKp5 = 325,
        KeyKp6 = 326,
        KeyKp7 = 327,
        KeyKp8 = 328,
        KeyKp9 = 329,
        KeyKpDecimal = 330,
        KeyKpDivide = 331,
        KeyKpMultiply = 332,
        KeyKpSubstract = 333,
        KeyKpAdd = 334,
        KeyKpEnter = 335,
        KeyKpEqual = 336,
        KeyLeftShift = 340,
        KeyLeftControl = 341,
        KeyLeftAlt = 342,
        KeyLeftSuper = 343,
        KeyRightShift = 344,
        KeyRightControl = 345,
        KeyRightAlt = 346,
        KeyRightSuper = 347,
        KeyMenu = 348,
        KeyLast = KeyMenu
    };

    enum ModifierKey : i32
    {
        ModShift = 0x0001,
        ModControl = 0x0002,
        ModAlt = 0x0004,
        ModSuper = 0x0008,
        ModCapsLock = 0x0010,
        ModNumLock = 0x0020
    };

    enum MouseButton : i32
    {
        MouseButton1 = 0,
        MouseButton2 = 1,
        MouseButton3 = 2,
        MouseButton4 = 3,
        MouseButton5 = 4,
        MouseButton6 = 5,
        MouseButton7 = 6,
        MouseButton8 = 7,
        MouseButtonLast = MouseButton8,
        MouseButtonLeft = MouseButton1,
        MouseButtonRight = MouseButton2,
        MouseButtonMiddle = MouseButton3
    };

    enum MouseAxis : i32
    {
        MouseAxisDeltaX = 0,
        MouseAxisDeltaY = 1,
        MouseAxisScrollX = 2,
        MouseAxisScrollY = 3,
    };

    enum Joystick : i32
    {
        Joystick1 = 0,
        Joystick2 = 1,
        Joystick3 = 2,
        Joystick4 = 3,
        Joystick5 = 4,
        Joystick6 = 5,
        Joystick7 = 6,
        Joystick8 = 7,
        Joystick9 = 8,
        Joystick10 = 9,
        Joystick11 = 10,
        Joystick12 = 11,
        Joystick13 = 12,
        Joystick14 = 13,
        Joystick15 = 14,
        Joystick16 = 15,
        JoystickLast = Joystick16
    };
}