#include "mailbox.h"

extern void PUT32(unsigned int, unsigned int);
extern unsigned int GET32(unsigned int);

void mailboxWrite(unsigned int data, unsigned char channel)
{
    // Wait until the mailbox is not full
    while ((GET32(MAILBOX_STATUS_REGISTER) & MASK_MAILBOX_FULL) != 0) {}

    // Write the data to the mailbox
    // Bits [0:4] are the channel ID bits
    // Bits [4:32] are the data bits however the data is assumed to be 32 bits long with the last 4 bits as zeros

    unsigned int mailbox_payload = data & MASK_MAILBOX_DATA;
    mailbox_payload |= (channel & MASK_MAILBOX_CHANNEL);
    PUT32(MAILBOX_WRITE_REGISTER, mailbox_payload);
}

unsigned int mailboxRead(unsigned char channel)
{
    while (1) {
        // Keep reading until we get a message from our desired channel

        // Wait for a message to be ready
        while ((GET32(MAILBOX_STATUS_REGISTER) & MASK_MAILBOX_EMPTY) != 0) {}

        // Check if this message comes from the desired channel
        unsigned int mailbox_data = GET32(MAILBOX_READ_REGISTER);
        if ((mailbox_data & MASK_MAILBOX_CHANNEL) == channel) {
            return mailbox_data >> 4;
        }
    }
}
