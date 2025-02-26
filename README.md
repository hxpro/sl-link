# **SL Link \- Extended Controller API**

## STUDIOLOGIC ® 2025

# **Introduction**

The SL Link \- Extended Controller API provides comprehensive functionality for managing the connection, communication, and display interaction between SL-MKII and the remote device, ensuring optimal performance and user experience.

The communication protocol employed is MIDI, leveraging the transmission and reception of 'System Exclusive' messages. This approach ensures compatibility and flexibility in data exchange between devices, adhering to MIDI standards while accommodating proprietary functionalities specific to the SL-MKII and the connected remote device.  
The SL-MK2 receives and sends SL-LINK messages on the port called “SL LINK” on macOS and MIDIN3/MIDIOUT3 on Windows.

## **What are Sysex Messages?**

MIDI (Musical Instrument Digital Interface) System Exclusive messages, commonly abbreviated as Sysex, are MIDI messages designed to transmit specific information between MIDI devices from the same manufacturer or model.

Sysex messages are a special category of MIDI messages that allow the transmission of proprietary data between devices. These messages are not standardized, so their interpretation varies depending on the manufacturer and model of the device.

## **Structure of Sysex Messages**

Sysex messages consist of a series of bytes that always start with a specific status byte (**0xF0**) and always end with an end-of-message byte (**0xF7**). The general structure is as follows:

1. **Start status byte**: 0xF0 (240 in decimal).  
2. **Manufacturer identifier**: One or three bytes that identify the manufacturer of the device. For example, 0x41 for Roland, 0x7E and 0x7F for universal messages.  
   The FATAR (Studiologic) identifier is **0x00 (**byte 0), **0x20** (byte 1\) and **0x1A** (byte 2\)  
3. **Specific data**: A sequence of bytes containing manufacturer-specific data. These bytes vary depending on the manufacturer and the type of message.  
4. **End-of-message byte**: 0xF7 (247 in decimal).

It’s important to highlight that the midi standard requires the Most Significant Bit to be zero for all the bytes, exceptions made for Status bytes and the End-Of-Message byte(1).  
We will take this for granted during all this guide, so keep in mind that when we refer to a byte we always actually mean 7-bits of data.

## **Example of a Sysex Message**

An example of a Sysex message to modify a specific parameter on a synthesizer might look like this: F0 \[00 20 1A\] \[0E 01\] \[04 01 00\] F7

| F0 | 00 | 20 | 1A | 0E | 01 | 04 | 01 | 00 | F7 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |

* **F0**: 		Start of the Sysex message.  
* **00 20 1A**: 	Manufacturer identifier (FATAR).  
* **0E**: 		Product ID (i.e. SL-MKII ID).  
* **01**: 		Host ID (e.g. NumaPlayer, Camelot etc.)  
* **04 01 00**: 	Specific data of the message.  
* **F7**: 		End of the Sysex message.

# **SL Link message structure**

The SL Link provides a full range protocol to give full control of the keyboard to the developer.  
This is achieved by implementing specific Sysex messages for streaming the I/O from and to the keyboard.  
The messages are both sent and received by the SL-MKII.

## **General SL-Link Message Structure**

### **Header and Stop**

All the SL-Link messages are created appending different commands to a standard Sysex message header, with 3-byte manufacturers’ ID number, one byte device ID, and one byte to identify the software that is interfacing with the SL.  
All the messages must end with an F7 byte.

| F0 | 00 | 20 | 1A | 0E | HID | …message… |  | F7 |
| :---: | :---: | :---: | :---: | :---: | :---: | ----- | ----- | ----- |
| Sysex start | FATAR manufacturer ID |  |  | SL product ID | Host ID |  |  | Sysex stop |
| **Header** |  |  |  |  |  |  |  |  |

### **Host ID**

To avoid conflicts between different application streams of messages, an Host ID is granted to manufacturers and software companies that want their product to interface with the SL.  
The host ID can be requested directly to FATAR/Sudiologic contacting.

We can thus see that for a given application the header is fixed for all the SL-Link messages.

### **Item Type**

The byte that follows the Host ID is called ItemType, and identifies the type of message the current Sysex represents.  
The SL-Link messages are grouped in six different categories.  
Such categories and their specific messages will be addressed in the following sections.

| F0 00 20 1A 0E HID |  |  |  |  |  | ItemType | …message… |  | F7 |
| :---: | ----- | ----- | ----- | ----- | ----- | :---: | :---: | ----- | :---: |
| Header |  |  |  |  |  |  |  |  |  |

The SL-Link messages are grouped in six different categories, three of which will be groped into one macro-category.  
We now present a brief list of such categories, and will address each one in the following sections.

| ItemType | Corresponding message |
| ----: | ----: |
| 0x00 | System Messages |
| 0x01 | Button messages |
| 0x02 | LED messages |
| 0x03 | Encoder Messages |
| 0x04 | Display Messages |
| 0x05 | RGB LED messages |

## **List of message types**

There are three types of messages that will be discussed separately.

1. ### **System messages ( ItemType \= 0x00 )**

The system messages are two way acknowledged messages, used for the establishing of connection and disconnection, thus managing the login, logout, standby and restart of the SL-Link.

2. ### **Display Messages ( ItemType \= 0x04 )**

This type message controls the output displayed on the LCD.  
These are one way messages that flow from the Host to the SL-MK2, and allow the provider to populate the screen through some primitives: text, rectangles and pre-loaded bitmaps.

3. ### **Hardware I/O ( ItemType \= 0x01, 0x02, 0x03, 0x05 )**

This messages allow the user to interact with the Host Device using the controllers available on the SL-MK2: encoders, buttons and LEDs

# 

# **System Messages**

## **Structure**

Every System Message is identified by the ItemType byte set to **0x00** and each one is distinguished by the **Function** byte.

| F0 00 20 1A 0E HID |  |  |  |  |  | 0x00 | Function | …message… |  | F7 |
| :---: | ----- | ----- | ----- | ----- | ----- | :---: | :---: | ----- | ----- | :---: |
| Header |  |  |  |  |  | ItemType |  |  |  |  |

Some of the System Messages can flow in only one direction, while others can go in both direction  
The System Messages , along with their corresponding Function byte, are as follows:

| System Message | Function byte | Direction |
| ----- | :---: | :---: |
| System Login Request | 0x00 | Host → SL |
| System Login Confirmation | 0x01 | Host ← SL |
| System Logout Request | 0x02 | Host ↔ SL |
| System Logout Confirmation | 0x03 | Host ↔ SL |
| System Standby | 0x04 | Host → SL |
| System Restart | 0x05 | Host → SL |

## **Login and logout dynamics**

System Messages establishes the connection and disconnection policy between the SL-MKII and the remote device. This message serves a dual purpose and can be both sent and received.

The Host is responsible for the first communication with the keyboard, and do this via a  
System Login Request Message.  
The Host then becomes visible in the list of available apps in the proper section of the SL-MK2.  
When the user selects the Host from the list, a System Login Confirmation Message is sent from the keyboard to the Host, and the former becomes available to receive and reply to the other types of messages.

When the user exits from the SL-Link Mode on the keyboard, the latter sends a System Logout Request Message.  
The host should now suspend all the further messages and promptly send a System Logout Confirmation Message before ceasing the activities.

The same messages, but reversed are exchanged when the Host needs to permanently stop the communication with the SL-MK2.  
In this case will be the Host to send a System Logout Request Message and the keyboard to reply with a System Logout Confirmation Message.

## **System Login message**

| F0 00 20 1A 0E HID |  |  |  |  |  | 0x00 | 0x00 | S1 | …  | SN | 0x00 | F7 |
| :---: | ----- | ----- | ----- | ----- | ----- | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| Header |  |  |  |  |  | ItemType | Function | Identification String |  |  | String terminator |  |

With this message the remote device requests SL-MKII to appear in the list of  ‘Available Apps’ accessible through the 'APP' button.

Each message must contain an identification string, represented by SN ASCII bytes, along with its string terminator (‘\\0’ \= 0x00).  
This string will uniquely identify the sending device in the above mentioned list, and has a maximum length of  32 bytes.

**Note:** This message needs to be sent continuously, for example, every 1 or 2 seconds, also after a System Login Confirmation Message is received. If the message is no longer received by SL-MKII, after a certain time (5 seconds), the APP is removed from the list.

## **System Login Confirmation Message**

The following is the response to the System Login Message

| F0 00 20 1A 0E HID |  |  |  |  |  | 0x00 | 0x01 | F7 |
| :---: | ----- | ----- | ----- | ----- | ----- | :---: | :---: | :---: |
| Header |  |  |  |  |  | ItemType | Function |  |

It’s important to understand that this message is only sent when the user, once in APP mode, selects the Host device to activate the interaction with it.

Upon receiving this value, the APP should proceed to refresh the display with its contents.

## **System Logout Request Message**

The Host can request to be removed from the APP list by sending the following message:

| F0 00 20 1A 0E HID |  |  |  |  |  | 0x00 | 0x02 | F7 |
| :---: | ----- | ----- | ----- | ----- | ----- | :---: | :---: | :---: |
| Header |  |  |  |  |  | ItemType | Function |  |

In response, the SL will send

In addition, this same message is sent by the SL when the user exits from the APP mode, reverting to the normal use of  the keyboard as a controller.

When receiving this message, the APP should suspend sending messages and respond with System Logout Confirmation Message message, described in the following section:

## **System Logout Confirmation Message**

| F0 00 20 1A 0E HID |  |  |  |  |  | 0x00 | 0x02 | F7 |
| :---: | ----- | ----- | ----- | ----- | ----- | :---: | :---: | :---: |
| Header |  |  |  |  |  | ItemType | Function |  |

The SL sends this message as a response to the System Logout Request Message sent by the Host device.

In the same way the Host sends this message promptly after receiving a System Logout Request Message from the SL, and then stops to send messages other than System Login Request Message, in order to maintain its name in the list of the available apps.

## **System Standby Message**

| F0 00 20 1A 0E HID |  |  |  |  |  | 0x00 | 0x03 | F7 |
| :---: | ----- | ----- | ----- | ----- | ----- | :---: | :---: | :---: |
| Header |  |  |  |  |  | ItemType | Function |  |

Not yet implemented.

## **System Restart Message**

| F0 00 20 1A 0E HID |  |  |  |  |  | 0x00 | 0x04 | F7 |
| :---: | ----- | ----- | ----- | ----- | ----- | :---: | :---: | :---: |
| Header |  |  |  |  |  | ItemType | Function |  |

Not yet implemented.

# **Display Messages**

As above mentioned, the Display Messages are used to manage the LCD output of the SL.  
This set is the most complicated of the A.P.I. and must be treated and understood carefully.

Furthermore the LCD management is particularly resource demanding in terms of computation and I/O machine time, so particular care must be taken when constructing the user interface.

## **About the Coordinates**

Before proceeding with a detailed explanation of each Display Message, it is necessary to clarify the functioning of the X and Y coordinate mechanism.

The SL-MK2 is equipped with a 320x240 (X,Y) LCD display, with the (0,0) coordinate placed in the upper left corner, and the (319,239) in the lower right corner.

We already mentioned that the MIDI protocol requires the Most Significant Bit to be set to zero, leaving 7 bits of bandwidth for information.  
This implies that two bytes per coordinate (four in total) are needed in order to properly communicate one point on the screen.

The two bytes are organized in MSB and LSB bytes, with the 7 Least Significant Bit stored in the LSB byte and the remaining seven bit stored in the MSB byte (though only two are actually needed).  
This concept actually applies to every chunk of data that exceeds the 7-bit limit of the MIDI Data Bytes, as we will see further ahead.

To further clarify the concept some example are given:

|  | 16-bit  coordinate Value (dec and bin) |  | 8-bit MSB |  | 8-bit LSB |  |
| ----- | ----- | :---- | ----- | :---- | ----- | :---- |
| X | 84  | 0b0000 0000 0101 0100 | 0 | 0b0000 0000 | 84 | 0b0101 0100 |
| Y | 156 | 0b0000 0000 1001 1100 | 1 | 0b0000 0001 | 28 | 0b0001 1100 |
| X | 293 | 0b0000 0001 0010 0101 | 2 | 0b0000 0010 | 5 | 0b0010 0101 |
| Y | 31 | 0b0000 0000 0001 1111 | 0 | 0b0000 0000 | 31 | 0b0001 1111 |

The code below can be integrated to convert the 16-bit value for the X or Y axes (and for other data exceeding 7-bit of range).   
The return value is a structure (tuple) with two fields for the MSB and LSB values.  
This principle will become clearer when the examples are presented.

| struct MsbLsb {     MsbLsb(uint8\_t m, uint8\_t l) : msb(m), lsb(l)     {}     uint8\_t msb \= 0;     uint8\_t lsb \= 0; }; MsbLsb SysexManagerNew::get7BitMsbLsb(uint16\_t val) {     uint8\_t msb \= val \>\> 0x07;     uint8\_t lsb \= val & 0x7F;     return MsbLsb(msb, lsb); } |
| :---- |

## **About the Colors**

The most common standard for color coding is the 8-bit per channel, 32-bit RGBA.  
The SL-MK2 does not support the alpha channel, leaving us with 24-bit of data to handle, 8-bit per color.

As we pointed out above, the MIDI standard requires the use of 7-bits per byte, forcing us to manage the color coding differently.

That said, all the messages that involve colors (all the Display ones but also the RGB LED message), accept 7-bit of data per channel, each in its own Data Byte.  
This implies a loss in color depth precision, but given the color resolution of the SL-MK2 LCD display, no difference will be noticed.

The conversion between a 24-bit RGB value can be achieved by keeping the 7-th most significant bit of each channel (in other words shifting one bit right).  
As an example we show a C function that perform a step-by-step conversion from 24-bit RGB to our 7-bit per channel format:	

| struct Color21 {     uint8\_t red : 7;     uint8\_t green : 7;     uint8\_t blue : 7; }; Color21 RGB24\_to\_21(uint32\_t rgb24) { 	uint8\_t red8 \= (rgb24 \>\> 16) & 0xFF; 	uint8\_t green8 \= (rgb24 \>\> 8) & 0xFF; 	uint8\_t blue8 \= rgb24 & 0xFF; 	uint8\_t red \= red8 \>\> 3; 	uint8\_t green \= green8 \>\> 2; 	uint8\_t blue \= blue8 \>\> 3; 	return { red, green, blue }; } |
| :---- |

## **Structure**

Every Display Message is identified by the ItemType byte set to **0x04** and each one is distinguished by the **Function** byte.

| F0 00 20 1A 0E HID |  |  |  |  |  | 0x04 | Function | …message… |  | F7 |
| :---: | ----- | ----- | ----- | ----- | ----- | :---: | ----- | ----- | ----- | :---: |
| Header |  |  |  |  |  | ItemType |  |  |  |  |

All the Display Messages flow in only one direction, from the Host Device to the SL.  
Here is a list of all the display messages with their data flow.

| System Message | Function byte | Direction |
| ----- | :---: | ----- |
| Clear Screen | 0x01 | Host → SL |
| Draw Rectangle | 0x02 | Host → SL |
| Plot Bitmap | 0x03 | Host → SL |
| Write Text | 0x00 | Host → SL |

## **Clear Screen**

This message instructs the SL to perform a full monochromatic pain of the canvas.

| F0 00 20 1A 0E HID | 0x04 | 0x01 | R | G | B | F7 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| Header | ItemType | Function | Color Red | Color Green | Color Blue |  |

This is used to refresh the screen and prepare the canvas to draw a new user interface.  
Since this command results in a full screen update, it should be used with parsimony and only when  needed.  
If only a small portion of the screen must be updated the use of the Draw Rectangle Message is recommended.

## **Draw Rectangle**

The Draw Rectangle Message is structured as follow:

| F0 00 20 1A 0E HID | 0x04 | 0x02 | XMSB | XLSB | YMSB | YLSB | continue… |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| Header | ItemType | Function | X coordMSB | X coordLSB | Y coordMSB | Y coordLSB |  |

## 

| … continue | WMSB | WLSB | HMSB | HLSB | R | G | B | F7 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
|  | WidthMSB | WidthLSB | HeightMSB | HeightLSB | Color Red | Color Green | Color Blue |  |

Where:

* **XMSB**: 	MSB of the x-coordinate of the top left pixel of the rectangle;  
* **XLSB**: 	LSB of the x-coordinate of the top left pixel of the rectangle;  
* **YMSB**: 	MSB of the y-coordinate of the top left pixel of the rectangle;  
* **YLSB**: 	LSB of the y-coordinate of the top left pixel of the rectangle;  
* **WMSB**:	MSB of the width of the rectangle;  
* **WLSB**: 	LSB of the width of the rectangle;  
* **HMSB**: 	MSB of the height of the rectangle;  
* **HLSB**: 	LSB of the height of the rectangle;  
* **R:**		Red color byte  
* **G:**		Green color byte  
* **B:**		Blue color byte

This instruction draws a rectangle on the screen filled with the given color.  
The (X,Y) coordinate represents the top left corner of the rectangle, while the bottom right coordinate is given by the formula ( X \+ W \- 1, Y \+ H \- 1).

## **Plot Bitmap**

The Plot Bitmap Message allows the Host Device to access the internal SL bitmap library.  
The library is organized in Groups, each one containing a list of Icons.  
Each Group is indexed by a 7-bit value, in which the Host can access the proper Icon via another 7-bit index.  
For each Bitmap, the Host can choose the background and foreground color.

| F0 00 20 1A 0E HID | 0x04 | 0x03 | XMSB | XLSB | YMSB | YLSB |  GIDX | IIDX | continue… |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| Header | ItemType | Function | X coordMSB | X coordLSB | Y coordMSB | Y coordLSB | GroupIndex | IconIndex |  |

## 

| … continue | FGR | FGG | FGB | BGR | BGG | BGB | F7 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
|  | Foregroundred | Foregroundgreen | Foregroundblue | Backgroundred | Backgroundgreen | Backgroundblue |  |

Where:

* **XMSB**: 	MSB of the x-coordinate of the top left pixel of the bitmap;  
* **XLSB**: 	LSB of the x-coordinate of the top left pixel of the bitmap;  
* **YMSB**: 	MSB of the y-coordinate of the top left pixel of the bitmap;  
* **YLSB**: 	LSB of the y-coordinate of the top left pixel of the bitmap;  
* **GIDX**:		Bitmap Group index, see Appendix A;  
* **IIDX**: 		Bitmap Icon index, see Appendix A;  
* **FGR**: 		Foreground color Red value;  
* **FGG**: 		Foreground color Green value;  
* **FGB**: 		Foreground color Blue value;  
* **BGR:**		Background color Red value;  
* **BGG:**		Background color Green value;  
* **BGB:**		Background color Blue value.

Once requesting the selected Icon, the SL proceeds to compute the color gradient and apply it to the black and white image.  
It’s important to know that the Icon will completely replace the content beneath it.   
This includes both the visible part of the Icon and its background, thus no alpha channel is considered.

A detailed list of Groups and Bitmaps, along with a preview and pixel dimension can be found in Appendix A.

### **Plot Logo**

Every HostID can require a custom logo to be uploaded into the SL-MK2 permanently.  
The logo must be sent directly to FATAR/Studiologic and must be in 24-bit BMP format with a maximum size of 50x50 pixels.  
To retrieve the logo, a special kind of Plot Image message is required:

| F0 00 20 1A 0E HID | 0x04 | 0x03 | XMSB | XLSB | YMSB | YLSB |  0x7F | F7 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| Header | ItemType | Function | X coordMSB | X coordLSB | Y coordMSB | Y coordLSB | Logo request |  |

As we can see, the message is in the same format, with a Group Id fixed to 0x7F and with no color information because of the custom colors stored in the image.  
For simplifying the programmer job, a Plot Image message with a Group Id of 0x7F will ignore all the color bytes and the Icon Index byte.

## **Write Text**

The write text message allows the Host Device to print a string on the LCD screen.

In addition to the (X,Y) coordinates, the routine accepts a Maximum Width argument.  
This argument refers to the pixel width of the string, and as every screen positional argument is composed of an MSB and LSB part.  
The SL guarantees that the given string will not exceed the Maximum Width given in pixels, truncating it and printing a three dot character if needed (‘...’).  
If the Maximum Width is set to zero, all the characters of the string will be printed, if it fit in the screen (no word wrap is implemented).

In the selected area (the area between (X, Y) and (X \+ Width, Y) the string can be justified to the left, right or center, according to the proper alignment byte in the message.

Moreover, the size of the text can be selected between small (21px), medium (27px), big (33px), via the size byte.

Also in this case there is a background and foreground color parameter, and like the precedent command the text will completely overwrite any existing content on the screen pixels within the area where the text is printed.   
This means that the text rendering will replace the underlying pixel data entirely, without preserving or blending with the previous content.

Finally the string is sent in 7-bit ASCII format, along with its string terminator.  
The message is structured as follows:

| F0 00 20 1A 0E HID | 0x04 | 0x00 | XMSB | XLSB | YMSB | YLSB | WMSB | WLSB | continue… |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| Header | ItemType | Function | X coordMSB | X coordLSB | Y coordMSB | Y coordLSB | Max widthMSB | Max widthLSB |  |

## 

| … continue |  ALIG | SIZE | FGR | FGG | FGB | BGR | BGB | BGB | S1 | … | SN | 0x00 | F7 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
|  | Alignment | Size | FgRed | FgGreen | FgBlue | BgRed | BgGreen | BgBlue | String to write |  |  | Stringterminator |  |

where:

* **XMSB**: 	MSB of the x-coordinate of the first character’s top left pixel;  
* **XLSB**: 	LSB of the x-coordinate of the first character’s top left pixel;  
* **YMSB**: 	MSB of the y-coordinate of the first character’s top left pixel;  
* **YLSB**: 	LSB of the y-coordinate of the first character’s top left pixel;  
* **WMSB**:	MSB of the width at which the string will be truncated (in pixels);  
* **WLSB**: 	LSB of the width at which the string will be truncated (in pixels);  
* **ALIG:**	Justification of the text in the given area, see the table below.  
* **SIZE:**	Size of the text, see the table below.  
* **FGR:**	Foreground color Red value;  
* **FGG:**	Foreground color Green value;  
* **FGB:**	Foreground color Blue value;  
* **BGR:**	Background color Red value;  
* **BGG:**	Background color Green value;  
* **BGB:**	Background color Blue value.  
* **S1 … Sn:**	The content of the string, in ASCII format;  
* **0x00:**	The string terminator

The **ALIG** and **SIZE** bytes take values according to the following tables:

| ALIG | Resulting justification |  | SIZE | Resulting size |
| ----: | :---: | ----- | ----: | ----: |
| 0x00 | Left |  | 0x00 | Small (21px) |
| 0x01 | Center |  | 0x01 | Medium (22px) |
| 0x02 | Right |  | 0x02 | Big (33px) |

# **Hardware I/O**

In this section, we focus on input/output messages to and from the machine's hardware.  
Specifically, we refer to the control of LEDs, Encoders, and buttons.

This allows the Host to receive user interactions with the keyboard’s controllers and provide instant feedback through the activation and coloring of the LEDs on the SL-MK2 panel.

In this chapter, unlike the previous ones, each message is assigned with its own **ItemType**.

We present a brief list of the messages presented in this chapter with the direction of each:

| System Message | ItemType | Direction |
| ----- | :---: | ----- |
| LED message | 0x02 | Host → SL |
| RGB LED message | 0x05 | Host → SL |
| Encoder message | 0x03 | Host ← SL |
| Button message | 0x01 | Host ← SL |

| WLID | Corresponding LED |
| ----: | ----: |
| 0x00 | Zone 1 button LED |
| 0x01 | Zone 2 button LED |
| 0x02 | Zone 3 button LED |
| 0x03 | Zone 4 button LED |
| 0x04**?** | APP button LED**?** |
| ~~0x05~~ | ~~Apply button LED~~ |
| ~~0x06~~ | ~~Cancel button LED~~ |
| ~~0x07~~ | ~~Home button LED~~ |
| 0x08 | Global button LED |
| 0x09 | DAW button LED |
| 0x0A | A encoder LED |
| 0x0B | B encoder LED |

## **White LEDs**

The SL-MK2 has 17 LEDs, 4 of which are RGB LEDs (those corresponding to the four zone encoders), 12 are white LEDs (corresponding to buttons and the A and B encoders), and the last one is a red LED that is not controllable via SL-Link.

While the white LEDs can only be turned on or off, the RGB ones can be colored as desired and their brightness can vary.

### **LED White**

The white LEDs are the ones associated with the buttons and the A and B encoders.  
The message to control them has the **ItemType** set to **0x02**, contains one byte to identify the LED to be controlled, and  another byte to set its state (on or off).

The message it’s structured as follows:

| F0 00 20 1A 0E HID | 0x02 | WLID | LST | F7 |
| :---: | :---: | :---: | :---: | :---: |
| Header | ItemType | LED ID | LED state |  |

where:

* **WLID**:  LED identification number, see the table on the right;  
* **LST**: Led state, 0 \= off, 1 \= on.

### 

### **LED RGB**

The RGB LEDs are associated with the four zone encoders ( the ones labeled from 1 to 4).

Each LED is made up of three sub-LEDs, one for each primary color (Red, Green and Blue), that can be controlled with one single message, using the color coding discussed in the Display chapter, section [About the Colors](#heading=h.9i17aa4js3m9).

Moreover each RGB LED has an adjustable brightness that affects all of the three sub-LEDs.

The associated **ItemType** for the RGB LED message is **0x05**.  
The RGB LED is selected via **LID** byte ( ranging from 0 to 3 ), the color is set via the **R**, **G**, and **B** bytes while the brightness is controlled with the **BR** byte (ranging from 0 to 127):

| F0 00 20 1A 0E HID | 0x05 | LID | R | G | B | BR | F7 |
| :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| Header | ItemType | RGB LED ID | Color Red | Color Green | Color Blue | LEDbrightness |  |

where:

* **LID**:	Identify the RGB LED to act on, goes from 0 to 3;  
* **R**: 	Red color intensity;  
* **G:** 	Green color intensity;  
* **B:** 	Blue color intensity;  
* **BR:**  	The brightness of the whole RGB LED.

## **Encoder**

| EID | Encoder label |
| ----: | ----: |
| 0x00 | Zone 1 Encoder |
| 0x01 | Zone 2 Encoder |
| 0x02 | Zone 3 Encoder |
| 0x03 | Zone 4 Encoder |
| 0x04 | Joystick Encoder |
| 0x06 | B Encoder |

There are a total of 7 encoders on the SL-MK2.The Host Device can receive messages from all of them, except from the encoder labeled A on the SL, which controls the overall volume of the USB audio.

The Encoder Messages flow only from the Sl-MK2 to the Host Device, and must be handled by the recipient.  
It has the **ItemType** bit set to **0x03**, the encoders are indexed by the **EID** byte, and the encoder rotation is stored in the **TK** byte:

| F0 00 20 1A 0E HID | 0x03 | EID | TK | F7 |
| :---: | :---: | :---: | :---: | :---: |
| Header | ItemType | Encoder ID | Tick value |  |

where:

* **EID**: 	Encoder ID, identify the encoder that sent the message, see the table above;  
* **TK:** 	Tick value, stores the rotation value of the encoder.

It is important to note that encoders do not send absolute values but rather relative ones.  
When turned clockwise, they send “positive ticks”; conversely, when turned counterclockwise, they send “negative ticks”.

The speed at which the encoder is rotated will produce a larger gap in values, for example, \+2, \+3, or \-2, \-3.

Moreover, the tick value is 64-centered, meaning that the negative values are all the ones inferior to 0x40, while the positive ones are above this value.

## **Button**

Like the Encoder Messages, Button Messages can flow only from the SL-MK2 to the Host Device.

There are a total of 21 buttons, including one push-buttons integrated in each encoder (7 in total), plus four directions of the Joystick Encoder.

The APP button is reserved in order to allow the user to go back to the Control Mode of the SL-MK2, and the A Encoder Button is reserved for the USB audio mute/unmute function, leaving the Host Device with the chance to control a total of 19 buttons.

The Button Message has the **ItemType** bit set to **0x01**, the buttons are indexed by the **BID** byte, and the encoder rotation is stored in the **EVT** byte.

The Buttons can handle two types of events, called SHORT\_PRESSION (**EVT** \= **0x01**), and LONG\_PRESSION (**EVT** \= **0x02**).  
If one Button is pressed for more than one second, a LONG\_PRESSION event is sent, while if the pression lasts for less than a second it will send a SHORT\_PRESSION event on its release.The Button Message is structured as follows:

| BID | Corresponding Button |
| ----: | ----: |
| 0x00 | Zone 1 Encoder Button |
| 0x01 | Zone 2 Encoder Button |
| 0x02 | Zone 3 Encoder Button |
| 0x03 | Zone 4 Encoder Button |
| 0x04 | Zone 1 select Button |
| 0x05 | Zone 2 select Button |
| 0x06 | Zone 3 select Button |
| 0x07 | Zone 4 select Button |
| 0x09 | Global Button |
| 0x0A | DAW Button |
| 0x0C | B Encoder Button |
| 0x0E | Apply Button |
| 0x0F | Cancel Button |
| 0x10 | Home Button |
| 0x11 | Joystick Up Button |
| 0x12 | Joystick Left Button |
| 0x13 | Joystick Down Button |
| 0x14 | Joystick Right Button |
| 0x15 | Joystick Main Button |

| F0 00 20 1A 0E HID | 0x01 | BID | EVT | F7 |
| :---: | :---: | :---: | :---: | :---: |
| Header | ItemType | Button ID | Event type |  |

where:

* **BID**: 	Button ID, identify the Button that sent the  
  		message, see the table on the right;  
* **EVT:** 	Event value.

# **Appendix A**

## *Bitmap Groups and Icons reference*

| Knob Group GIDX \= 0x00		width \= 61px		height \= 54px |  |  |  |  |  |  |  |  |
| :---- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **IIDX \=** | 0x00 | 0x01 | 0x02 | 0x03 | 0x04 | 0x05 | 0x06 | 0x07 |
|  | ![][image1] | ![][image2] | ![][image3] | ![][image4] | ![][image5] | ![][image6] | ![][image7] | ![][image8] |
| **IIDX \=** | 0x08 | 0x09 | 0x0A | 0x0B | 0x0C |  |  |  |
|  | ![][image9] | ![][image10] | ![][image11] | ![][image12] | ![][image13] |  |  |  |

## 

| Knob Center Group GIDX \= 0x01		 width \= 61px		height \= 54px |  |  |  |  |  |  |  |  |
| :---- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **IIDX \=** | 0x00 | 0x01 | 0x02 | 0x03 | 0x04 | 0x05 | 0x06 | 0x07 |
|  | ![][image14] | ![][image15] | ![][image16] | ![][image17] | ![][image18] | ![][image19] | ![][image20] | ![][image21] |
| **IIDX \=** | 0x08 | 0x09 | 0x0A | 0x0B | 0x0C |  |  |  |
|  | ![][image22] | ![][image23] | ![][image24] | ![][image25] | ![][image26] |  |  |  |

## 

| Toggle Group GIDX \= 0x02		 width \= 35		height \= 35 |  |  |  |  |  |  |  |  |
| :---- | :---: | :---: | :---: | :---: | :---: | :---: | :---: | :---: |
| **IIDX \=** | 0x00 | 0x01 |  |  |  |  |  |  |
|  | ![][image27] | ![][image28] |  |  |  |  |  |  |

## 

| Navigation Group GIDX \= 0x03		width \= 20px		height \= 20px |  |  |  |  |  |  |  |  |
| :---- | :---: | :---: | :---: | :---: | :---: | :---: | ----- | ----- |
| **IIDX \=** | 0x00 | 0x01 | 0x02 | 0x03 | 0x04 | 0x05 | 0x06 | 0x07 |
|  | ![][image29] | ![][image30] | ![][image31] | ![][image32] | ![][image33] | ![][image34] | ![][image35] | ![][image36] |

## 

| Arrow Group GIDX \= 0x04	 |  |  |  |  |  |
| :---- | :---: | :---: | :---: | :---: | ----- |
| **IIDX \=** | 0x00 | 0x01 | 0x02 | 0x03 | 0x04 |
|  | ![][image37] | ![][image38] | ![][image39] | ![][image40] | ![][image41] |
|  | **width \= 20pxheight \= 10px** | **width \= 20pxheight \= 10px** | **width \= 10pxheight \= 20px** | **width \= 10pxheight \= 20px** | **width \= 14pxheight \= 10px** |

| General Group GIDX \= 0x05	 |  |  |  |  |
| :---- | :---: | :---: | :---: | ----- |
| **IIDX \=** | 0x00 | 0x01 | 0x03 | 0x04 |
|  | ![][image42] | ![][image43] | ![][image44] | ![][image45] |
|  | **width \= 20pxheight \= 20px** | **width \= 20pxheight \= 20px** | **width \= 20pxheight \= 20px** | **width \= 16pxheight \= 15px** |
| **IIDX \=** | 0x05 | 0x06 |  |  |
|  | ![][image46] | ![][image47] |  |  |
|  | **width \= 16pxheight \= 12px** | **width \= 16pxheight \= 12px** |  |  |

# **Appendix B**

## *JUCE MIDI example*

In this section we will show some snippets of code taken from the official SL-Link example, downloadable from [https://www.studiologic-music.com/dammi\_il\_pollo](https://www.studiologic-music.com/dammi_il_pollo).

The example uses the JUCE framework for handling the MIDI connection and traffic.

## **Smooth and efficient MIDI communication**

The connection between the SL-MKII and the device (App) is established via MIDI, making it essential to enable both reception and transmission of MIDI messages. To prevent congestion from excessive messages, the 'pulling' technique is used for both reading and writing.

The following implementation ensures a smooth and efficient MIDI communication between the SL-MKII and the device, leveraging the 'pulling' technique to manage message flow effectively.

In essence, there are two timers, one for input and the other for output, which control the data flow at millisecond intervals.

| std::vector\<MidiMessage\> queueIn; std::vector\<MidiMessage\> queueOut; void handleIncomingMidiMessage(MidiInput\* source,                                        const MidiMessage& message) {     queueIn.push\_back(message); } void sendMidiMessage(const MidiMessage& sysMsg) {    queueOut.push\_back(sysMsg); } void timerCallback(int timerID) {     switch (timerID)     {         case TIMER\_MIDI\_OUT:             if (queueOut.size() \> 0)             {                 MidiMessage sysMsg \= queueOut.front();                 queueOut.erase(queueOut.begin());                 mOut\-\>sendMessageNow(sysMsg);             }         break;         case TIMER\_MIDI\_IN:             if (queueIn.size() \> 0)             {                 MidiMessage sysMsg \= queueIn.front();                 queueIn.erase(queueIn.begin());                 parseMidiInput(sysMsg);             }         break;     } } |
| :---- |

### **Code Explanation**

1. **Queues for MIDI Messages:**

   * std::vector\<MidiMessage\> queueIn; \- This queue stores incoming MIDI messages.  
   * std::vector\<MidiMessage\> queueOut; \- This queue stores outgoing MIDI messages.

2. **Handling Incoming MIDI Messages:**

   * void handleIncomingMidiMessage(MidiInput\* source, const MidiMessage& message) \- This callback function is triggered by the system when a MIDI message is received. The message is added to queueIn.

3. **Sending MIDI Messages:**

   * void sendMidiMessage(const MidiMessage& sysMsg) \- This function adds outgoing MIDI messages to queueOut.

4. **Timer Callback Function:**

   * void timerCallback(int timerID) \- This function is called by a timer to process MIDI messages. There are two cases:  
     * TIMER\_MIDI\_OUT: Checks if there are any messages in queueOut. If there are, it sends the first message and removes it from the queue.  
     * TIMER\_MIDI\_IN: Checks if there are any messages in queueIn. If there are, it processes the first message and removes it from the queue.

**Additional Notes:**

* **Timer Intervals:** The timers operate at millisecond intervals, ensuring efficient data processing without overwhelming the system.  
* **Callback Function:** handleIncomingMidiMessage is a callback function triggered by the system. **MidiInput** is  the input source, and **MidiMessage** is the MIDI message received.  
* **JUCE Library:** Refer to the JUCE documentation for more details on handling MIDI messages and using the JUCE framework for audio and MIDI development.

## **MIDI Send**

The sendMessageNow() function is a method of the MidiOutput class, and the mOut pointer is obtained from the AudioDeviceManager.

| … MidiOutput\* mOut \= audioDeviceManager\-\>getDefaultMidiOutput();     if (mOut) mOut\-\>sendMessageNow(sysMsg); … |
| :---- |

## **MIDI Receive**

The following section demonstrates how received MIDI packets are interpreted. It's essential to analyze the MIDI messages contained in queueIn. Each MIDI message can represent different actions, such as button pressed, encoder changes, etc.	

| constexpr uint8\_t FATAR\_SYSEX\_ID\[3\] \= { 0x00, 0x20, 0x1A }; constexpr uint8\_t PRODUCT\_ID \= 0x0E; constexpr uint8\_t HOST\_ID \= 0x01; void parseMidiInput(const MidiMessage& message) {     if (message.isSysEx()) {         if (message.getSysExDataSize() \< 7) return;          uint8\_t fatar1 \= message.getSysExData()\[0\];         uint8\_t fatar2 \= message.getSysExData()\[1\];         uint8\_t fatar3 \= message.getSysExData()\[2\];         uint8\_t prodId \= message.getSysExData()\[3\];         uint8\_t hostId \= message.getSysExData()\[4\];         if (                fatar1 \!= FATAR\_SYSEX\_ID\[0\] ||                 fatar2 \!= FATAR\_SYSEX\_ID\[1\] ||                 fatar3 \!= FATAR\_SYSEX\_ID\[2\] ||                 prodId \!= PRODUCT\_ID        ||                 hostId \!= HOST\_ID            ) return;         uint8\_t item\_id \= message.getSysExData()\[5\];         uint8\_t item\_num \= message.getSysExData()\[6\];         uint8\_t item\_val \= message.getSysExData()\[7\];         switch (item\_id)         {             case ITEM\_SYSTEM\_MESSAGE:                  // Handle system messages                 handleSystemMessage(item\_num, item\_val);             break;             case ITEM\_ENCODER\_MESSAGE:                 // Handle display messages                 handleEncoderMessage(item\_num, item\_val);             break;             case ITEM\_BUTTON\_MESSAGE:                 // Handle button messages                 handleButtonMessage(item\_num, item\_val);             break;         }     } } |
| :---- |

## 

## **MIDI Parse**

This example illustrates how to read the values from various controls on the SL-MKII. This logic is applicable to all other control elements as well.

| void handleSystemMessage(uint8\_t item\_num, uint8\_t item\_val) {     switch (item\_num)     {         case SYSTEM\_LOGIN\_REQUEST:             // Handle login request message             break;         case SYSTEM\_LOGIN\_CONFIRM:             // Handle login confirmation message             break;         case SYSTEM\_LOGOUT\_REQUEST:             // Handle logout request message             break;         case SYSTEM\_LOGOUT\_CONFIRM:             // Handle logout confirm message             break;     } } void handleButtonMessage(uint8\_t item\_num, uint8\_t item\_val) {     switch (item\_num)     {         case JOYSTICK\_BUTTON\_MAIN:             // Handle Joystick button main press             break;                     case APPLY\_BUTTON:             // Handle Apply button             break;                     case ENCODER\_BUTTON\_1:             // Handle Zone 1 Encoder             break;                     case SELECT\_ZOOM\_BUTTON\_1:             // Handle Zoom select button 1             break;     } } |
| :---- |

# **Reference Section**

**STUDIOLOGIC Official Website**

[Studiologic](https://www.studiologic-music.com/): Official website for Studiologic products including manuals, software updates, and support.

**JUCE Official Website**

[JUCE](https://juce.com/): Official website for JUCE, a widely used C++ framework for developing cross-platform audio applications.

**MIDI Manufacturers Association (MMA)**

[MIDI.org](https://www.midi.org/): The official site of the MIDI Manufacturers Association with resources, specifications, and news about MIDI technology.

**Additional Resources**

**Audio Developer Conference (ADC)**

Audio Developer Conference: Annual conference focused on audio development, often featuring JUCE-related content and talks.

**GitHub Repositories**

[JUCE on GitHub](https://github.com/juce-framework/JUCE): Official JUCE repository with source code and contributions from the community.  
[MIDI Projects on GitHub](https://github.com/topics/midi): Explore various MIDI-related projects on GitHub.

**Books and Guides**

[Designing Audio Effect Plug-Ins in C++](https://www.amazon.com/Designing-Audio-Effect-Plug-Ins-Software/dp/1138499710): A book by Will Pirkle that provides an in-depth look at audio effect plugin design using C++ and frameworks like JUCE.  
[MIDI Power\!: The Comprehensive Guide](https://www.amazon.com/MIDI-Power-Comprehensive-Robert-Gu%C3%A9rin/dp/1598630842): A comprehensive guide to understanding and using MIDI technology.

This reference section includes essential links for getting started and diving deeper into Studiologic products, JUCE framework, and MIDI technology.

[image1]: <data:image/png;base64,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>

[image2]: <data:image/png;base64,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>

[image3]: <data:image/png;base64,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>

[image4]: <data:image/png;base64,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>

[image5]: <data:image/png;base64,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>

[image6]: <data:image/png;base64,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>

[image7]: <data:image/png;base64,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>

[image8]: <data:image/png;base64,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>

[image9]: <data:image/png;base64,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>

[image10]: <data:image/png;base64,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>

[image11]: <data:image/png;base64,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>

[image12]: <data:image/png;base64,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>

[image13]: <data:image/png;base64,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>

[image14]: <data:image/png;base64,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>

[image15]: <data:image/png;base64,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>

[image16]: <data:image/png;base64,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>

[image17]: <data:image/png;base64,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>

[image18]: <data:image/png;base64,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>

[image19]: <data:image/png;base64,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>

[image20]: <data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAADoAAAA0CAAAAAD+2nSEAAAF2ElEQVR4Xo1WDVfaTBa+ySSEAAVf62ot1r671K8qrVrbuioSQjLfMwmgtD3dff//z9g7iD0W5XWfw4GTYZ7cO/fjuQMwRxP8CoAPDYAYgHhxvPF297i9ioseQBXq4JGAACH3hAdwa0FUiaFaWd07ZCpTUgijtRx+2m6RCEnEA+LjLs9boHrgVZDu+529xNJiLAuDXKHLccGSzhGEuCWoEsdcoNYgco42Xm8f6OmEpjThQsixYRlVttQnrbdNEkLF99G2M/0AHuCpGh8S/rlhNBPSMqOltOixljwvXn7RV5013FepeBD8TnWven0uzHQQJpyW0lIlWJ6kuVDI7sX8W5mcbQVAQs+r/M4kEO3mhRhoubOnpUWrzuK3m1Lhee32eyaFUvnBKrhg/U4FOJGSaVlMBgHllOtMCsZyJhVyBZRM8ELr8j34D4MUogNRHFjFuLn+0WOdA2tvKC3o4U5PGm6n2W4nubEZE1SqhouoTzBcmMvw7gXFSDOZ20Kpq+bFVeePEIPRxGS2usOvbTZIZWmULjTH3N6fdUYl4ZvWYKwGhquyvISX8d37Znu80AOqNBdW571p8LYazosiwg+B1glt28QYmY0uLpozXhhVIwiCYJbEV8fcsiG3k/47dQatgASB56hBEO2PrYry/lTpwasX4MXBPBrObBxX0PfNa8P47WXz1oxOMZGeO0/Fh5eXludDefwiP8CK8l/MnIUQHQUvcpkI/aYP8Zeva4fTKZv0/rvqYuTeWzsSBRtpZc4xAJ4XYQgh8lyTBKEjog/40/Ch/pWKbCxVtrOC9ewcjgXWizL25+d5sB8D6xYdXDkYJ+N+30qJLViDWtToCptSMeRH9UfVeY9w1pTB6kDKmx821bjVYUWqW6YnTK+5pn4aeCziXruSXYpSTuxwBR/i2juV5ELz4Q5BJ5YATYZVxz0tNEu5Hm7WIicoa2f0u7j5i9xpwJNAHcAM43kb2Bq3+lW76Va3yi8b8I/OF0vI0ihhxog7DFbCz/SfTXj3736zCfEHxgp+vFarYWn4S8l3pYdf9db6p/LSjg/Q/WujBpOMjdZdKBYZD+GHgUei17nsW2VU6kO1JymVI1ps4J+LgvUIIWyUBlvfMo21KSaoQWXOW/7fG52jQUU+zHlu6tCmmlFq80EYzDL3HCppWQhp5KgN+9pIJUaq54Wzgn0GBC6LEUOq2YETIzkfivzCC1Ezlib2HiGc5CLJOGXH8ElSYZhRxx6WxPPUAM74qDSlFtdwrnJpcyneIjF43uMAPlxSLTRL+vD+vNs9bG+uo8CCU4O/B5bixvrWu+7px8MzqMd/7u0cHp2cOOqz/iKOjs8O93e3mzF86HOthLFXXvh/5JWQ6jWlQ82zZA86I2XKzKq8trR+H4CEMRspbDw+fgONEWVFvxRsxfddFJ4BqdMi1Tyl31BjDJOj4USzLZeb5yz7QZuXA4MjZoLlfsWkHiL/Iyb12TB53qkY0VII2kcPumZstFU68R/P3CfAKQ4wMxZdNLORm0xypgTKS3Vx4yIIfM+l1HjeTXyq7g/ycboFH3dRzO808jE8JzHolre/tbpxcvPjtgNuXL0wvS3/TUp7m8uPSuZiuiHU2b9W6x+vKmgkwItSdevzXz9708vaUo8dFX9q15KV44vtl+tQd8+k2R+N0376PTtoLVLu4c10bWVX04zaSXkeuxJo4/rnpKQ3mmnbXaT8Arak90c3Z+yHTGl+6uQb3NWg1v/PtRpKrsTVAuMXUIYrnUGpBjzDi0oFUHvdQKzB9rBUirNiuLNIuQfGqHpEdSlZWuSvMY9u6ADOZ7Jf0jJR5mS5mGKFV/ax6AZWHM2XyCxpa10jp+np0nEV3O06Gqvv9Kw+N4CjGVdJLbmQw+X96m7FOJ7r2TUbuHvCDNgrvhOWxpWII3eLfhIur0jxm4PUr95Lgu8Wcf7BWi12MVsGN3Hw/tDGyMyvKeD5s6mKF5T6r7Un4M/UkkA9gtloRfwPnz4B+TS4Z1gAAAAASUVORK5CYII=>

[image21]: <data:image/png;base64,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>

[image22]: <data:image/png;base64,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>

[image23]: <data:image/png;base64,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>

[image24]: <data:image/png;base64,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>

[image25]: <data:image/png;base64,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>

[image26]: <data:image/png;base64,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>

[image27]: <data:image/png;base64,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>

[image28]: <data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAACMAAAAjCAIAAACRuyQOAAAFV0lEQVR4Xo2X51IjSwyFZzzYwAIFFDmbnKHIFDnnTEFd3v9F7tc6M6LXu95d/XD19Kh1pKPQ4yQppGSSpinr1IRFlmX8NjY2Njc3u2YslUqlXC5rXTb5dZ2LjLr4JqZRbWpq4rGhoQGLLNicnZ1dWFiYnJzs7OxkPz6FW/iqNQt5+S3oKZoa0XlsSY1j7AwODt7c3Dw+Pp6dnU1MTLjXMqqD2onX31IyfL3DdOyLQkkKh6rVKkivr69XV1eEJT/YdzXOVkxqA5IISWtnQLS0tLRwzCMjptPT07u7u5OTk7GxMe1nJh6BHrWuDYsDvkVi2traANBjyWhUPhJDuri4eHp6Oj8/Jz7Ycy/dmx8mWvtmEExwQIZwf2RkZGVlZX19vb29/Vup8I638Pb29nZ9fQ2SMPQr5vv6+lZMWMQBBIldxjoYuPz19bWxsTEwMKDiVpKSgr2XlxfwQJUFHFWeIGNtbe0/ExY8fsMgra2tiUWDub29vfv7e2wBtr+/PzQ0JBNWK4FG4oC95+dn2BsdHRW8YqLnenp6Dg4OOHt5eUnhbG5uCoKDoVsUEL/4qAom4cfHxxzLrLLdFjI8PFwPSRZRwF3VJ3h0Ah6wn+tAEZhHR0e8JiY08F2HlXBP7Pj4eD0kj56zYhiPDw8Pu7u75W4QkEggMA8PDyRgcXFRdCWWA/EmK9PT0/WQqDc5xGJ1dZWSub29JTiO5A2ONqvl5eX393c6H3c6OjpiH3N3TLBeDymxVGlB4cAQiSAyspWXBnoUBZ3ILunZ2tqSax5HEo3L/v7+PyApC4LEddgDDNchLLwWs+xSM1ghpVnU8GKP+DjMPtVYD0lDHTBtYgedWxPVRfCdAMkQSNBaM7LiJkfzD+wpGnREe29vL5rknrDm5uZCL/GOJlVxU6Ac0xmZCIVhokc8JZfwTH0KKbORmkRzll/1KJoEhCY5y82BpKLc2dlxH9VkglTOMMFUFVI8I7Ki7aTvkABcmzAEcisUJWGCtL29rcM6UINErCCh9vn5SdtxayhW14xLCWFekA40MSvNZH5+XkiwJ3c0cx1JemzCPgTgI6llKjqr0tRI0yZdRUzAkFQiCWZRmpqaghBNPO5sHU7Mx5iWxMJi3WzivpeK1pZ/QqKy6RkhYT/cIxyG8XsTEujsJxGSXys1It78lSeYX6zTo3cmMBGQUGUosEvquHiWlpa8yh2AhW8STcWk0SRuA09bavVMlQNDPzSZBA2skDS2uFcoPzGT2g0pE+XiBopbzUnTvteF/CBJMETzMGRlIdebmZnRlUFkhJVEJZSZaK0DqbWn+5EUn2nShCEs0Eaa111dXXwrBAvqMoYgFcWYIFssYNlN64LRIyQoXMdwkZ3EpjDu0gn8UqI+4wOGNLh7yBblDh6xUxr+3QIYB0Sgk5ZaG+CKFwW0A7O7u0u9kXL6yXsul8waAnO8IKaPjw9yBiqTUSbQjpFKxfXY8PM3CZ+30EUVkCEWcMipnwjAd9WGLjESRlhok0yGo64riUcTnweM43wP0UBkmoCwQIlR3HorCaXiI5VnyOTO5VaERs036rVarSqxqiuVQ2azlX1CoWIBULFxikd9xHleg2dxFSl1gGEdEh5MOE9uGR98WMHntAm1ygyDAJIB4eQfTRZ4xl0KvAzGfATx68tbFUgigw0c1PcFVgT8aAK97JN2FNiELsoan/BGdzw2vaD+IkDiGuzr8xhyNK4wqhmjmQYSO9QqDPPhppYoWRfHxfJ34QBk8gXCfyYKBADi0OCnOMkHrtAeJB8177zUPvD+FYk61OTWo4oFr+nQDhO15G/NeRUIWPI/nzAt7c8AhikAAAAASUVORK5CYII=>

[image29]: <data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABQAAAAUCAIAAAAC64paAAACv0lEQVR4Xn2U2UtVURTGj1fvYHqHzKSHCv+S6qESy+Fm2IBl0h9QiRRhplEYRlBYFNEoUZhm9Fz0XEbklFRSas569c7zePptl0RwxcO6m3X2Xt/61vr2OlfLydW0HM2yycyK8bqvbG9vX8/07NT84tyy2zX26+fTridVzsqi4s0qxqCZ800SrH55plxZnTXVM3PTGT0dCPn9QR8OFo6GxHn/4d2J+jqjOY/gkm1bFRhOIb//4B5IwbDGkzGcVCaJ/w8fjUeutV9dK0GYsdudt3wBL8crnmUhx9jx+NzJdCKRirMvefGvd7QDKbQVKGTD6VPkxqhWYETEElFgMGOyyc6iayESC7u9K/UNJ1XnVnvh+ORvtJEiAUB4tvGMFCItgAlFgmk9xanwDwx9VUpVVB2kEwliJfGFi+fJmlmVCoykAMmRNE+B+GXl+7Wu588AY8FwgIPGpnMmixEJEYziocI4JYtISPFC86q3WxsZHcbjjHwoKVdtyMshdF2TMmGe+DOuISNItugzv8Ai14Zlw/4Hw6/ArpUlkepyW4vMGWMgZa9rAqYXNNd+jH2nN7bo5MbNDsFTQjZMDBoRaGFpXnvztk8qJ9/k1AR4Gt6gbMLgpOyX3S+0yuoKuQwZQ2Rrab20AViGh/gDFeVK2MHhAdGAy8Tx+j1cdTZMTGr8/KXf5rAq8OHaGnpAfRkvmcdsmBgxVHe87tjah1Fos7ZeaQuESKlH43SlJ9MpcTw+b1rP4ATDoUgsGkvEdV1vamqy2+1AteIS9WUWFW/pvHtndn4OWDyZ8Pp9bq9HspB0adnFJq+hSLi5ubm0tFQh1aP+Boyrg2WoPXpkZPSb8LCGoxGYEym60LGP/Z+cNYccDofgLBYLX5VNRspoNkmWXXt297zuHRwempqZhpb14eNHVc7q7Tt3rPapHoPBYLVa/wI52EgkIL0hDgAAAABJRU5ErkJggg==>

[image30]: <data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABQAAAAUCAIAAAAC64paAAADFElEQVR4XkWUaW9MYRTHn7mdmU7DtNPFEkH6SWyRIKEtCdVIaH0AVRWiVJsWJSGiIkERxAtLQojlE2i9Ml0sgzfa0WX2fZ+51+/e0zA5PTnPvfd//v+zPFW2KqVsylFtx4t1HDr44OH9b76vsUQ0FAl6pz6/ev1y2/atvKqucSrNVl3jWv6Yv4amejkc6Tr845cvEgvrRgUDLMFScBH/5dtM694WR7WTL1evXbMMrllhZrp5azRfzKWzqVKliBVKeQmy+YykIEimE8MXLzQ0Nf5nxu49GJv1/+aLilFeDCzI17lCFh8ILRXLhYWleV5xLJZLly6PAFlZ6zaR7R0HYMjk0uFoSGBlvcQRIQQco/GIPCfI5nORWPRw5xGzcnowOe0lsbz+s+DH0yeMIBgO4IcvDvnn56iIOJFK6obxedJrdzpUS9seVIm8fx36Z9QZT8ZQd+36VWJLgpFMp+DfsWunevzkEWBkpzJJ4cSjP5GKS800ye6sUpo6cbKHY75YEPKnz58p+gSY9LrVIWIYqJYjZUvN0tQqh3b6zKlSpYyRAuXK6oE5DJBSlb7cmIz0DC21HjfgtevM2Xb3HId2fpGJGAq1QotOZiv184RACiFGttPlEP6z/eeYFshQJKx8P7+THoNHt7aKeaJWGqFbkwej2W34kSuXUpk0BvjjxLi6M3ZbuiJ1AoafQEpAPIqAQd4/cM7SaMC8FAy8+/Be7WndrVtbwXeykrIhsjbwk66+0TM4NCCbl86ypKxwufNol2pa3fjm7WshkSIFLF3Ak6W75xhZkGZtqBGORr7/8Gl26z5ymUDKHYBcmi8LIxg8s8Cs7tBRo73j4PLFYMXPDw4ghqe5AgUbqJIgGo9VDJ2AJrFVIA3D6O3traurA4rsVcq80o03bo7+WZgHVigVY4k42y9ZSBoIBXnIkYL7+vqam5tNpPkzO2nO0Fal7W8/MP1lRnjwmVwWZpkqNv5pom3fXo/HIziXy6XcdbUyff5FSJZNWzY/e/HcOzU565+DFn/33lhLW+v6jRusOs2fpmlut/svwhdHvvgJdBwAAAAASUVORK5CYII=>

[image31]: <data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABQAAAAUCAIAAAAC64paAAADHklEQVR4XkWUV09UYRCGvz1sQ1lYBdEYNfwSWzRoRJrBciPoD1BXFBVpAWmxEDQaoyYarwRR4oX6CyxXIoqoiSWAlN1lC9srx+ecifFkdvJ9c+adeaecVZYCpSzKucaBRrjurdwzOjYy/e1LaCXoD/gmJj+Mv3i+e88u3joK7UqzOAqd4mz8rPYC0bX1NXPzs6t6PhJbQQNGI0u+RfTU9Oeaumqbw45z+aaNBpickvzO3dsgV6Jh/NDpbCqbzyCJVFxCcCBoT++V9WWl/zMjQ8M3wpEQHstBvyRHkukE2utfyuTSC0vzeT3HNZPL9g30AykqdhnIppON8WQMEbamR5prKpPMrWa5BsMBsXNIpJKBUPB4U6NRuauk6OfvHwTmHa7whMJpzylahcW37EX39HZDJ5aImhVFVnX9w8cJq92mqqoPCD2plsJaLp6nq5KKK7Fgd33oGmeTgh6JRclfuX+fevT4IWAkGidk3tN8xu600UJiSVBqMcahqbPnPFxTmbQkfzI6oj5NTWKCktnJbhm1ZrVgpGypWZpaYNMuXGrJ5nMIIWCuaC9IklBn4VqnjA2BpPSM4ovdLiybNhuzPe05Q9r5xQW0kpaQob2zTfbM5rASgoZLIdQMbWqRoJfb25gWSH9gWX39Po0fmSly8OqA4KGAkc4DZrxYKATdP9gXjccQwG/evVXPxseEOX6/Z36BFz+xQJ6gylxeqJmLpJN5yed9+fqVOlhTJVPBSfakreMy3qTlSn42fF2pu6u7c9G7YLaWJY3Ss6aTJww+fDfmDJKygHgzagmHJgq9JMo/B305GPj6/ZtmNadyqKGe3sBQ1kvGI5+UYKQpCGeGBP7IsaNG/5S54h1dnZDBmkyn0LCSQzAcyuurHGgSWwVS1/Xm5uaSkhKgqqx8A3i+suFbN/8szANLZzOhlTDbL1EI6vX7MHKl4NbW1oqKCgNpPEYnjRlaCrSGI4c/TX2WPOh4MkFmmSry9v272vo6t9stOKfT+KqKZfr8RUiU7Tt3jDwdnZj8ODM3S1r0vQf3q2trtmzbatZpPJqmuVyuvxyd/r1LVc1eAAAAAElFTkSuQmCC>

[image32]: <data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABQAAAAUCAIAAAAC64paAAADBklEQVR4Xk2UaU9TURCGTwuFChTKIvEjxv+hJrLI4hIMLjEC8YvGKEvYGwWCsmmiQaMxGqOiBgEx/gyNggiaiKxhX1qW0pUu1+cyWm1OJ3PPmXfmnXfOvcoQpZRBmeNisSwes3Iy+971Do98dXm2Z2anx8Z/vv8wcKqokNPYPTHKuGt3g/V/dEyU2JOFJ+YWZsNayOnawjo27Fj7+trW9ub6pmP0x8i582dNsdEEp+/bq4OpKcUfPX4IkjgA2E3nhtfvCYYDPPoDPiyLFDdbW1LSkv9VZt3ruku01JHi226n7XpDva2OFPDf2FqXvDtBf1tHK5CExHgdWXqxxO11sYStRHTe6ZC8NXXVkcrLq0sen5t2iksv6J1bkhImpycWlxc4g2QgtAOF8soyYPGWOMG3tt9aWlkMaUHpCDs0PKgrVXA8H2KRXRLX1teInuRFniiTEb/jdjtlpT4ECcvJzVYvup8DZtEkB5VVFTFmExIK3hhtkOI4jc034O/b8UqZt309inmKPJH60CMx/KUO0ZJXeEkYztTMpIIDYgKgVSwY6Q3NwKw5VnGEKlPAFzAZdbBMKPx3mEKJ9kAKDBWJiQguYGqgufo1MUZWjoUnlplJHImgB0wcckGEXCIQ+qv+gT62JBomT589+XN7DCo51SqCYVH+WvlVCaMmtN/0vFb5x/KEjPTDm8AFisCYE4sr3NzSRDXuHzDhmFeQq3hLvo0O/3+N8S9fuQRy/4EMoVDXULtqX5GyqIv99PljotWimGr20SyQ7LLIKjpxsQXZ3tkmo6JtCUAjXi/9VOlX3NLY3OR0kVLz+lFVC4SCONW1NWUV5S6Pm501hz0YpjufpmlVVVVJSUlAVVq6/mampKV2Pbg/v7gAzB/Y2dhiYFpkzc7Pub1MSFtZW7XZbBkZGTpS/+mfAZPSvyHGojOnR76PErTtdtnXHSEtTCJ8arL5ZWiwuLTEarUKzmw2c/sTpTdTbIxkOXj4UG9/3/jkBHgoTM1Mv3zVfSQrM8qkf0PkZzQaLRbLb8mY/tuk6lvWAAAAAElFTkSuQmCC>

[image33]: <data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABQAAAAUCAIAAAAC64paAAADaklEQVR4Xk2U509TURjGD2W0BUqrqIgD6/ioUWOi8ZsjAlEZbo3GL4IYjVEorkaNEXCAe0QZQQEH4CeNifoHaDQmGqBAlCWyh4wOoAV6/d17iLE5vXnved/nHc95zhUiQIQYgnmygvVBPLcmbnla+qSxucHlcXpG3bX1jiclxWwaQvUyLHLmDGkIY5hBWmGm0PUb1/363eIdH/Mrk6PekY6udowx3+jA0B+MmtrqhKStoeFhBPPUGw0qLCBQLX73/p0h56Bvwjsy5vnd3lpe+fLZizL7+XNv3r5m589gP+lIkZWTLfEhBr0ICgnUG0NOncnE4R5xUfnY8aP/2gmPUONYh4+ktne2kWVS8V+9fk0rqVMdu/fuAklv1Y4q66IFMnrGrMipwTSbdOZpEQ1NP/2K0t3bcyg1RXWx66irgZvWtl8x1vmh4UamoJ2Fi61HjqZRcOWqFTJFhMU0e05UtaMGvKOuVt3csWs781CZzqdK6UTpsxJJldM9jPHw0QNGi4qehfdkRrrL4wa/bsN6UVCUj7u7twuHyRyuCwp4Uf4c2ugFhsYnfTJ12fNSoZJsFLqAtg5OQal4VSmaWhoJpRRd4V60ZCEwMCx5YDzBDw4PrFm7Wmgcvfvwfqrz3v4eoouKC4XGbUZmOnGTygSY/oE+Fgxjs7bv3EbMtMjp2VdyfjY2eMd9At+waygr5zIiwXfg4H6QTEtlqRYMClzLvRoYrGNyyUt+YUF7Z4f4XvWNCJQAyewuW76UVzIiGMDIk8UO+IuXLsAIM6fbMpxuV1NLsygpe0oQAmAeTtIy3Xzn3m2iqc8+LWB39XTKzs/az5xIP8nA7hFPeWWF2LNvN3Ekzjxtky1Fz52dX/gYJAs6wEz4xyFCdgSSaYddzsTkJDW6/kcdDtSDSHhFNkgdqUEe58wpysnlmUmF9fb3qZX4b0nY7Nck0dLaLJUAGHowZNtoXhpUHvN5wW+Kiw0MDprSXfaVLNy0x8VITUuRNxwKoE1KBRcG5CmKYrPZzGYzdcXMKPVmE513M5fKpGfOnr5uvgc3buXJ0/Jr10YadrvdarWqSPWnDckBIr3E5AQuiZxQhtInOpH2p88f+Z5YLBaJMxi0j8H/iyyx8ZsqXpV/+fqZninI9wjxxm+OmxczV5Wn9tPpdCaT6S8AqoE4CN4iNwAAAABJRU5ErkJggg==>

[image34]: <data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABMAAAATCAIAAAD9MqGbAAACe0lEQVR4XlWTSVMTURDHXyYJCYQhISB6UItPIngSPCha5b5+A7U4KMFSq2TxoJbgTb0IlgJ+D/UkAdGTIvuWhUAgIev4e2ny1Kmunp5+r/vf/e8epVxKWS6tXcrt9aCvXLv6dnQEo7EpjH4/9qHr7BmMGr+Pm75av1xWdfUBXpbH7XJbJzo71mMbu9nMTmZX/Eg2t5cvFqLTUxcvX/L6avC0HDqoj4zVPzhQKBX38jnuYXhqvDjRZcdJbW+ht9Lbj/v7ws1N+5hS4dPnz8jNMYBo4nHWBurQJacsHjRJB54M4qxvsHX1bcfb8XJjdu43BqVuxGPUT1ecLq+ugLa9k07v7mT2sonN5PWbN3S3JFhZW40nE/9iIlJSQyiITYBgkgL9NTqpe+k42SnFUAloydQmn2sb60rIdCnQ8NB5LBHH4JNEcKlG3o2CxoFg3rpzO2DXw7OEYQh45H6vAAjs2MS4mvnxHSiaREuTEGjoEYZ7eiPE0AgAxXIJqn/O/tovJlfI0zoFY5PrxfAQMTQJ830D/XKHGOkWA4+aW5g3MbhIgZCbqu723IPPcpU2ucYpR4vLS7oHmtHLUd1BET1ny4XmlAlhGKnecVsUpqdXDTC2tCoaCYUbDYzmz4DgEpz/YCsGO2zWWFWWXDMn7CNkMmjcM7YZjMT8Ta0qS/jg0UMhUKZqxitLK3sHsY7jdHd3B4NB4lRzywGC+QOGXg4vrSwTA3ubWymYlI3hv8Mjo45EIq2trTpMP9VxU9W5C+enZ74JiAxDpoV8+vKZ/zsUCkmQ3+9XdrDB9CkpjrW3jX+cmJyKzi8uIK/evD7Vdfrw0SOV3vRjWZZt238AhePrD5sF9UcAAAAASUVORK5CYII=>

[image35]: <data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABMAAAATCAIAAAD9MqGbAAACKElEQVR4XnVTuaoqURAc9bmvCCKCGBgIJoI/oImJub8gIppoaiQYaKRmgpHgB4hgYGIimPgLgoiIGCguuOF2a07Nnfe4l1dB03P6VHdX9xlJEjAajbB6vd5sNsPRaDSwfwTgWCwWWJ1ONxqNCoUCL8unIJCvwuVywWq1Wlir1SqJ1Nls9iMwnU6LxaJ8z+l0SiKFwWAwmUzIjU/4zMLKdrt9uVyCdj6fYavVqhxjVgJMOiyItulkMhkQrtfr4/GAEwgE5BuASQD3bDabmot6zALb7fZ+v7Pber2u1MENh8NBHz1rBNgzgd5AeD6fsLPZTBKzlAPqhLxeLyzLqofhcBgE1ITd7XbNZhNJ5RFyDUClUun1eqimSmWKdrvNJm+322q1YkhBPp9HMgQQTiaTXAk3GQwG9/s9mdCZSqV8Pp9Cg8JOp3O5XDA3hOfzuSSWTvJgMCDn9XotFgt1HDKop9vtclEYOvaGaWOBOGc1zGYymXBDUKfIoc5QKIQbh8OBV3O5HA4TiQR8dAsh6BPM369NXkar1eLcgdPp5PF4xuPx8XhEF8Ph8N8lSXxhEAMaAxAD2nq9hoXClwD8eDzO7Fw1KsuPRG0AAbxj1uScP0Jhv9+Xvv8BDlwBPyCd29tsNuR8hELYaDQqfb/En+DvR6TT6ZsA+ny/341Gg1G2+pdDsBoCbrfb7/fjZaplI5GI9L+CWA6ZKmKxWLlcLpVKtVqNJzoBbljFF815RZONyPOVAAAAAElFTkSuQmCC>

[image36]: <data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABMAAAATCAIAAAD9MqGbAAACW0lEQVR4Xk1Uu2pqURDdvqIxvgixUMTeNzYWFjaCSRNiERARFET8g3yAIDYWNlaSL4qCnyDBiA98a4xv7zpn6eFOsZ2z96w9a9bMVgghbDYbVrfbLWQzmUx0YDqdTq/Xq1QqflqtVjoPDw8CB/Ci0ehqtcrn8zy4u7szm830YWq12m63l8vl6XQaCASuuwaDIZFIXGT7/f3NZrPilhZ4h8PBsM/Pz263i5hOp4M0Wq1W4LJer0fkfD4/n88E4wxIOCBVKpX2+z1jYF9fX9IRqGcyGXwvl0usPz8/WNPpNGrTaDRAAoadw+Gw2Wzg9Pt9j8dzJQwrFAp/f3+73Q5njEgmkyivWq0yD3MOh0O/3y/JA4MS9/f3cF5eXhix3W7X6zXw7+/vuI5ISLBYLHw+H+WVICwGeHBDKGAMVfIAwGzhcJgECbn9CAEkdH59fVWQyEwYGuZyuZDn/1ZLhnZzC0isb29vx+NxNpsRD20oCQUTcjJGSqYMR71ep0KkCs2AhIOeE6lECovFQsfpdH58fDAOJE+nE5FwuPn8/MxITKt0C4WF1Wo1kCQABlWDwSAcajaZTLAWi0UGG41G6QdFNhoNXg8AeI7HYzwADEM8Hr/IUl3kURmNRpiwq6hod6VSoR4Eo3WhUIhVgBFIkgUGk1xSqZSEfHx8pPQsZjAYeL1eqI0bxU1tDCNlO8rWbrcRIIEjkQheAIuBz/cFDTi6uAL4XC6HYQCj7+9vnD49Pcn1ChGLxVqtFt4eYXzKTHidUnm2m82mUBpDNJ6SMh/gT4eGnPzTEHLZSj//AWXvu2VIQnG0AAAAAElFTkSuQmCC>

[image37]: <data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABQAAAAKCAAAAACRPiE6AAAAoElEQVR4Xi1PywrCMBCcpGliWin2UPVHVfCB2It4EO9+pWKl1T7GTXFg2ffMLoAEmMCIJSZEOhVXQBcQrO7APDZJhHEuBqw6s7qEnrLOS25lR5Uk6yvgobW0ljAu2g18kN3NwzqpTaE0juxafthzLYxpKmUbb8g3+++LLcsMmAEm3g+shJJ9wye3eRD2p5qNbP4xHOSeDG4hVwXJEbk88AMcMkFPYCEFHQAAAABJRU5ErkJggg==>

[image38]: <data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABQAAAAKCAAAAACRPiE6AAAAn0lEQVR4Xj2OTQuCYBCEx1VfPzKJIOjW/+vaTcKSCLrWT+wWhBhKlKbTvgbNYZedfXZYIHEAGEBcbT60AhHyF/kkB6r69lMsrR2fy5qPhm/d1MxWagU+pLCQkhXvJ8CbqZ1ierwOFduS3R5iJmJDE8hF0YbdIbZ5BiE8BEDOG7l2RO/g/75RPOu5mWugTsbio8LddgHjyt8YlTqI7InqCy10QFVgnQr+AAAAAElFTkSuQmCC>

[image39]: <data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAoAAAAUCAAAAACRhfOKAAAAoElEQVR4Xh2PwQrCMBBEZ5M0sbbqqeCfVrCgFlH8VcGDYAu1NeMmt9nZt8wsgDWwgkMFNDANgDJ7BeDlqqP3upc+qr2HC/YYCdQQgxOXHxJRtOSgLlzRRX6orrH9zCUyWoQzp4HvqBKPidPIUaXY24t8JkDTL1zmJK2YTftlZoP2OGTWp0B3z2yA2WLXRU2rBLWgSSW1sNTpkXSluqyw/QOTi0DwXxiwdwAAAABJRU5ErkJggg==>

[image40]: <data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAAoAAAAUCAAAAACRhfOKAAAAoklEQVR4XiWOwQrCQAxEJ9ttt6BUUfFDPVkVRNGD4M2PFAQpUrFbrR2zaw6ZR0gyA0yARBJoabPaCkU5ONUsTNP6DEzTgGB3SjD844f7CBhU5P2CuMGmZ1taJftteGu5HQGObU1e+81YFzz5IBcBO1YdS/W3nk/2y/Ahb8jXroAzwJv1SnIDATyPEuKkMFyrSnTWhy6L0TFHbiFxbDEDjN7/APnCQEzs3NnoAAAAAElFTkSuQmCC>

[image41]: <data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAAA4AAAAKCAAAAAChsjAbAAAAgUlEQVR4Xg2OwQqCUBREz/OpZQ+ioEV9Y2upRVYGEfYB9ZNBhBk8LKc7qzmbMwPesQnknGHiWMDtjaW7G8P00A5pyEZ6XB3441dtSClaqRyzVydBgvRURS29DJeoV38iL6W4woePtJ6ZstJAlvCLtc1Z28ql5HE3x0PAXSigSezGH8ukMnmXAdO/AAAAAElFTkSuQmCC>

[image42]: <data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABQAAAAUCAIAAAAC64paAAADAElEQVR4Xk2UOUtkURCFq7XbfUNFEdRAQQQFQUVBA8XAH2AgYiAiggsIJgZi0uOkgpiIu4lLZiAm4p/oGQ1U3HHrbbS7XXqz33z9anzMpbnvLnXqVJ2q2xIMBg3DiEajhjkuLy+3t7f7+vpqamrS0tKKioqqqqqGhoY2NzfPz8/V5vPzkzmRSAgfv9/PHIvF5ubmmpqaRMRut6ekpGRlZTGzxQtzQ0PDxsaGIj0eD7OA4fPw8DA5OelwOIqLi5nT09OxdpgDR7ouKChITU2dmpp6fHzUECQcDsfj8ZGRkezsbIyUymazyffIy8vThfJzNTY2BhKgkO3Ozo5a5Obmql1OTk5mZubMzMzo6ChXECo/h7iAZmVlJRQKCQq1tbVZVOCxYIFU+IahsrJSt5qLWpL/3d2dQKswK7zCwkJmMry4uACM2rCpx4yMDGaYCWRra0v6+/u5UH8kXFFRsbe3d3h4yOL+/j4QCNTX1+/v7x8cHJSVlYkZueoyMDCQDEBMMZW2paXl+fkZwqOjo9fXV6rocrm+vr7e3t46OzuVQC1xmvxoMIz8/HxC6O3tPT09BR+JRCgkLnw+X0dHh5i5IJ6YjZAUnzDQVvcqCaOnp+fp6Qk2w2y+1tZWnKKZfAumEgi5KUDMSkKu6/b2djr37Oysurrabo7/zQiec6GSlEfJNaTS0tK6urry8vLBwcGJiQmMEIITrmhYmNWYW1laWlJ//yIRWV9f1+5zu91er5fFy8sLh1xpXtr59Llg0dzcLKYYCl5YWLi9vb26ujo+PqaNT05OeAZOp7OkpEQNGN3d3Tyy5MNYXl7WUoHXSmjD8EjErA1doRUhL7bM8/PzyYdhmE9qenra6jCuVVKCtOovps7J8tjEOfvD98efAPzx8QH++vp6fHwcKrVgWDDWKGo1/4+fs67fv0AmwaqNYf4lLC4uNjY2WlRiklvF7+rqWltbc3s9igxHI8Jjfn9/Vzzrm5ub3d3d4eFhnprqX1tby3Z1dRXlDBPGLxqPBULBv2vIkO3hK5dSAAAAAElFTkSuQmCC>

[image43]: <data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABQAAAAUCAIAAAAC64paAAAC60lEQVR4Xl2UyUuqYRTGv6uVaYPNhWSDYEjUxtr3FxRBi1ZBkRhp464BqZUILYL+kCAiaCtEi6iFaRPNk1opLbI58/76jtd7vWfx8nq+85zhec6roihKeXk5p8Fg+KWaRqNR/lhZWVnmLmF4pqenW1pa0l4AOTk5/2IKCgrwaLVa+Wk0GuXS3t5+fHwcjUZTqZTdblf0ej3e/Pz83NzcwsJCiQNGI5IFvyCbm5u3trZSqr2/v4+MjCh5eXlUJoJSEs1ZXFycOcmu0+na2tp2dnaAPT09cZ6dndXX1/+knJiYWFlZCYfDJycnl5eXh4eH5+fn8Xjc5/OBJG9FRUUoFPr6+gL2/Px8e3tbW1tLs0pXV9fDw4M08/j4+Pb29vn5+fHxsbq6ygjUbGxszHSbTCZBNjU1ySCKw+F4fX1lhrm5ufn5ea/X6/F4XC4X1UpKSmBxd3cXDAGADw4OpFtm/CF/amqKahSXXMwPWxrVSktL6fZbNZAbGxt1dXXCX1qI4eFh+mRai8Ui+JqaGk6z2by+vi7dJhKJvb09k8mEn0H+ijo2Nsbn6+vrjCRYdXX15uamwCDi6OiILgyqZYEHBweZB25hjyFFnoWFhZeXF8Aw7Pf7bTab6L+4uJgFHh8fZ+b7+3sURnNxMqqQj2zCUGVlJVNQIwvMolAE0Vla8FTo7OxEqu3t7aWlJdkZ6EEC0uHMAqMK3ouLC4lTVKq6u7uLioq4V1VV0Q7RMzMzsVjs6uoqC9zX14cSkUgE3VCPsWEu85mLPBvA1Njf388Cj46OsgM3NzfyEoRzyAMjFEhHrANh0P4/YaQMBAINDQ28KrqlBdaLOyxwJyn42dlZeGU3yUgjpP4B9/T0kBJ8MBjkG8ydnp5yMsjd3R0nT4VuoZCYtbU1eX9pcGtr6/LyMutN4pS6+rKMSdUEI8bfAM3Ln0QajFmt1oGBAbfbPTk52dvbyx0inE7n0NBQf3+//ETRjo4ORZVNWOT+G2SGthNvbDx9AAAAAElFTkSuQmCC>

[image44]: <data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABQAAAAUCAIAAAAC64paAAAChklEQVR4Xm1UO2syURC9vmJ8JNFgfFRiK8QuWCaYToPa2IlaWMQHQRTBPn1+g4UW1sFCTEALK0EwnaKIraBoEXxrzu5sbuTjG9i7c+fOmce5s8sYYxcXF1iVSiUTRavVyuVy0mE8OzuDolAoVCoVKXSk1+uljVqtFjaM2e12OrNarRSCB4XIZDIY4SxFp7OrqyusXq+30WiEw+FTAPyw1el0Go2GLNjy0iSPYDA4n8+Px2M+n4cfFYxyqFouPC6qEB6LxfL09DQcDheLxXK5BPj5+TmTyWDNZrOFQiESidzd3RmNRmDOz8+Z2OZfvHa7ffyfbLfb9XoNZbPZFIvF29tb8kcXwosICwQC0+kUySeTCbIlk8l0Op3L5bCmUqlyuUyxer3e4+MjE2sW6sdDTPh8vu/v78Ph8Pr6yinhDbtcrlKpBHyn03E4HLAIV0g9EB5sf3x8RKNRAhBhpDORVHS33+8R/dTOrq+viUayohd+GRQXDKHP+/t7JP/6+sIgCWeUmQth1KLQbCEcFKIGOUAN8E6nU/JGSH57yCNF/R1bdlLIzc1Nv98H2O/305F43SLsn3lgYtuU02AwYDWbzbPZDOPgdruFYw6AghKQAQCeH1uUTdEhLy8vSNvtdhmfE7jSbHMhb+qFM/fw8DAYDJAZIST7aakgD5SgTsCAv7y8xCn6xFEoFBqPx0hbrVbZbxd/8vb21mq1KpVKrVb7/PyEU7PZhIKbH41GgOGzqdfrKJMTIdSGhDab7f39HR6YMJpEyG63W61WZMFgYmzxbaBVlMN/CZJ4PJ5EIoGpjsfjsVgMc0bfFrbo1mQykRshJSLQGM0DBH3yHwuPzQmDgv6xykSB5QdBlwJ86adIIwAAAABJRU5ErkJggg==>

[image45]: <data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAPCAAAAADIGBDzAAAApUlEQVR4XiWOWQ6DMBBDTQgEKFCJ+x+x0EVsJYs7k+bDsp4cj2FchRb5taicgV1IYIpxAsjFin6JCjGKqFV89KpAf6jaFCRhQjCSCEm+ZOxIl8NAt6Z1sON5jnYQ2wkO/yaVbJP6JqZWqdqNp1y8PGp8uSt+8rIruVWeL0mYyJm1J4PjwlSilHnv5uHD3H54UXZcG2NBqTaJu4f0jbjJsUKm4476B+nnd28vmONPAAAAAElFTkSuQmCC>

[image46]: <data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAMCAIAAADkharWAAABBUlEQVR4Xn2QPc5EUBiFhYjfREwoFFYxNVPagFqisoHRaRUK+xHWIPrpqEQzBaLw952MRMxNvnmKkyt57j3el6IoSpIkpCzLSIZh7vf76/Wi/oPjOJqmWZZVFAWfjuPs+951HemdaJqGFEURDc/nc9u2pmmWZSG9E9d1b7fbcS6KYv8wz/O3dQHtbds+Hg/DMARBiOMYz+MO6Z0cT1ZVhSpMgnnCMPx1Ae3jOE7TBCmKIp7nsSjP80jvBN66rsj3+41M01TXdVK6EgSBbdvIsiz7vseW8jxHCeldUVUViZ9JkuRoy7KMlE6wGaT+AQff94dh+DU0ME3zOGBLSMuy6rr+Mi78AeVJfvTmOnRBAAAAAElFTkSuQmCC>

[image47]: <data:image/png;base64,iVBORw0KGgoAAAANSUhEUgAAABAAAAAMCAAAAABOjGJdAAAAyUlEQVR4XhWNMW8BYRyHn/e8xx0uuTNIGiZfol2lhoqhQ8NKt+oH8B0MvolIJB2aMNhbiVU0kQ6UaCQ4FOHf82zPk1/yIwJRjwdfRda9Jw+FMr0oZV/Q3PfbLuBgNGRzjqE0tU8oPRebEuBaNi6dCjISma0WfzB5JHE7YCbf0+si7ciXxvbZba4634aRMWR+uMj+EJRTWP/mQtx98JqvvstWxNCh4LNThZShGkvxnaSJqrfA4ibBy/ECduGtm9TEHTAp7LFkmA38H2/LUQOg9LwKAAAAAElFTkSuQmCC>
