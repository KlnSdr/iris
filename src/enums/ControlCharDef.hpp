#pragma once

/**
 * @brief Enum representing control characters.
 *
 * This enum defines several control characters used in the communication protocol.
 * Each control character is represented by a hexadecimal value.
 *
 * @enum ControlChars
 *
 * @var PCK_START
 * Start of packet character. Represented by the hexadecimal value 0x05.
 *
 * @var PCK_END
 * End of packet character. Represented by the hexadecimal value 0x07.
 *
 * @var OK
 * Acknowledgement character. Represented by the hexadecimal value 0x09.
 *
 * @var RESEND
 * Resend request character. Represented by the hexadecimal value 0x0E.
 *
 * @var ESC1
 * First escape character. Represented by the hexadecimal value 0x0A.
 *
 * @var ESC2
 * Second escape character. Represented by the hexadecimal value 0x0D.
 */
enum ControlChars {
    PCK_START = 0x05,
    PCK_END = 0x07,
    OK = 0x09,
    RESEND = 0x0E,
    ESC1 = 0x0A, // Data
    ESC2 = 0x0D, // Data
    ESC3 = 0x0B, // 200Beathoven
};