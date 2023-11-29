enum control {
    packageStart = 0x09,
    packageEnd = 0x0A,
    transmissionEnd = 0x0B,
    acknowledge = 0x0D,
    resend = 0x0E,
    /**
     * @brief escape character used to indicate that the next nibble is inverted
     */
    esc1 = 0x07,
    /**
     * @brief escape character used to indicate that the next nibble is data not a command
     */
    esc2 = 0x05
};
