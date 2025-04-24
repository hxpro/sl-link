# Basics

## What are Sysex Messages?

MIDI (Musical Instrument Digital Interface) System Exclusive messages, commonly abbreviated as Sysex, are MIDI messages designed to transmit specific information between MIDI devices from the same manufacturer or model.

Sysex messages are a special category of MIDI messages that allow the transmission of proprietary data between devices. These messages are not standardized, so their interpretation varies depending on the manufacturer and model of the device.

## Structure of Sysex Messages

Sysex messages consist of a series of bytes that always start with a specific status byte (`0xF0`) and always end with an end-of-message byte (`0xF7`). The general structure is as follows:

1. **Start status byte**: `0xF0` (240 in decimal).

2. **Manufacturer identifier**: One or three bytes that identify the manufacturer of the device.  
For example, `0x41` for Roland, `0x7E` and `0x7F` for universal messages.  
The FATAR (Studiologic) identifier is `0x00` (byte 0), `0x20` (byte 1) and `0x1A` (byte 2)

3. **Specific data**: A sequence of bytes containing manufacturer-specific data. These bytes vary depending on the manufacturer and the type of message.

4. **End-of-message byte**: `0xF7` (247 in decimal).

It’s important to highlight that the **midi standard requires the Most Significant Bit to be zero** for all the bytes, exceptions made for all the MIDI Status bytes and the End-Of-Message byte[^1].
We will take this for granted during all this guide, so keep in mind that when we refer to a byte we always actually mean 7-bits of data.

## Example of a Sysex Message

An example of a Sysex message to modify a specific parameter on a synthesizer might look like this: `F0 [00 20 1A] [0E 01] [04 01 00] F7`

| `F0` | `00` : `20` : `1A` | `0E` : `01` | `00` | `04` : `01` : `00` | `F7` |

- `F0`: 		Start of the Sysex message;
- `00 20 1A`: 	Manufacturer identifier (FATAR);
- `0E`: 		ProductID (i.e. SL-MKII);
- `01`: 		HostID (e.g. Studiologic, Audio Modeling, ecc…);
- `00`:		DeviceID (e.g. Numa Player, Camelot, ecc…);
- `04 01 00`: 	Specific data of the message;
- `F7`: 		End of the Sysex message.

[^1]: For a detailed Sysex and MIDI reference, see [MIDI 1.0 Detailed Specification](https://midi.org/midi-1-0-detailed-specification).

[Back to index](../README.md)