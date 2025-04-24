# SL Link message structure
The SL Link provides a full range protocol to give full control of the keyboard to the developer.  
This is achieved by implementing specific Sysex messages for streaming the I/O from and to the keyboard.  
The messages are both sent and received by the SL-MKII.

## General SL-Link Message Structure

### Header and Stop

All the SL-Link messages are created appending different commands to a standard Sysex message header, with 3-byte manufacturers' ID number, one byte device ID, and one byte to identify the software that is interfacing with the SL.

All the messages must end with an `F7` byte.

| `F0` |`00 20 1A` | `0E` | HID | DID | *...message...* | `F7` | 
|:---:|:---:|:---:|:---:|:---:|:---:|:---:|
| Sysex start | FATAR manufacturer ID | SL product ID | HostID | DeviceID | | Sysex stop | 
| <td colspan=5>Header |||

### HostID and DeviceID

To avoid conflicts between different application streams of messages, a HostID is granted to manufacturers and software companies that want their products to interface with the SL.  
The HostID can be requested directly from FATAR/Sudiologic.

For each manufacturer, different products can be developed in order to interact with the SL mk2. These products are identified by the DeviceID.

The SL mk2 identifies each product by the couple (HostID, DeviceID), therefore such a couple must be unique for each application/device.

Further on, we can see that for a given application the couple HostID and DeviceID is fixed for all the SL-Link messages, and are therefore included in the header in the course of this document.

### Item Type

The byte that follows the Host ID is called ItemType, and identifies the type of message the current Sysex represents.  
The SL-Link messages are grouped in six different categories.  
Such categories and their specific messages will be addressed in the following sections.

| `F0 00 20 1A 0E HID DID` | ItemType | *...message...* | `F7` |
|:---:|:---:|:---:|:---:|
| Header | | | |			

### List of message types

The SL-Link messages are grouped in six different categories, four of which will be further grouped into one macro-category.  
We now present a brief list of such categories, and will address each one in the following sections.



| ItemType | Corresponding message |
|:---:|:---|
| `0x00` | System Messages |
| `0x04` | Display Messages |
| `0x01` | **Button messages**[^1] |
| `0x02` | **LED messages**[^1] |
| `0x03` | **Encoder Messages**[^1] |
| `0x05` | **RGB LED messages**[^1] |
| `0x06` | Pedal message |

[^1]: Hardware I/O messages

The three resulting types of messages that will be discussed separately.

1. **System messages ( ItemType = `0x00` )**  
The system messages are two way acknowledged messages, used for the establishing of connection and disconnection, thus managing the **login**, **logout**, **standby** and **restart** of the SL-Link.

2. **Display Messages ( ItemType = `0x04` )**  
This type of message controls the output displayed on the LCD.
These are one way messages that flow from the Device to the SL mk2, and allow the provider to populate the screen through some primitives: **text**, **rectangles** and pre-loaded **bitmaps**.

3. **Hardware I/O ( ItemType = `0x01`, `0x02`, `0x03`, `0x05` )**  
These messages allow the user to interact with the Device using the controllers available on the SL mk2: **encoders**, **buttons** and **LEDs**.

[Back to index](../README.md)
