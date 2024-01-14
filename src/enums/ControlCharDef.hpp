#pragma once

/**
 * @enum ControlChars
 * @brief An enumeration to represent control characters.
 *
 * This enumeration defines several control characters used in the application.
 * Each control character is represented by a hexadecimal value.
 */
enum ControlChars {
    PCK_START = 0x05, /**< Start of packet control character. */
    PCK_END = 0x07, /**< End of packet control character. */
    OK = 0x09, /**< OK control character. */
    RESEND = 0x0E, /**< Resend control character. */
    ESC1 = 0x0A, /**< First escape control character, used for data. */
    ESC2 = 0x0D, /**< Second escape control character, used for data. */
    ESC3 = 0x0B, /**< Third escape control character, used for 200Beathoven. */
};