#pragma once

/**
 * @enum ControlChars
 * @brief An enumeration to represent control characters.
 *
 * This enumeration defines several control characters used in the application.
 * Each control character is represented by a hexadecimal value.
 */
enum ControlChars {
    PKG_START = 0x05,
    PKG_END = 0x07,
    OK = 0x09,
    RESEND = 0x0E,
    ESC1 = 0x0A, // this is Data
    ESC2 = 0x0D, // this is Data t(o/w)o
    ESC3 = 0x0B, // 200Beathoven
};