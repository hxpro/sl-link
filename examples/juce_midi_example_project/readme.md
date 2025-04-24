# JUCE MIDI example project

The example uses the JUCE framework for handling the MIDI connection and traffic.

## Smooth and efficient MIDI communication

The connection between the SL-MKII and the device (App) is established via MIDI, making it essential to enable both reception and transmission of MIDI messages. To prevent congestion from excessive messages, the 'pulling' technique is used for both reading and writing.

The following implementation ensures a smooth and efficient MIDI communication between the SL-MKII and the device, leveraging the 'pulling' technique to manage message flow effectively.

In essence, there are two timers, one for input and the other for output, which control the data flow at millisecond intervals.

```cpp
std::vector<MidiMessage> queueIn;
std::vector<MidiMessage> queueOut;

void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message)
{
    queueIn.push_back(message);
}

void sendMidiMessage(const MidiMessage& sysMsg)
{
   queueOut.push_back(sysMsg);
}

void timerCallback(int timerID)
{
    switch (timerID)
    {
        case TIMER_MIDI_OUT:
            if (queueOut.size() > 0)
            {
                MidiMessage sysMsg = queueOut.front();
                queueOut.erase(queueOut.begin());
                mOut->sendMessageNow(sysMsg);
            }
        break;

        case TIMER_MIDI_IN:
            if (queueIn.size() > 0)
            {
                MidiMessage sysMsg = queueIn.front();
                queueIn.erase(queueIn.begin());
                parseMidiInput(sysMsg);
            }
        break;
    }
}
```

### Code Explanation

1. **Queues for MIDI Messages**

   - `std::vector\<MidiMessage\> queueIn;`  
   This queue stores incoming MIDI messages.  
   
   - `std::vector\<MidiMessage\> queueOut;`  
   This queue stores outgoing MIDI messages.

2. **Handling Incoming MIDI Messages**

   - `void handleIncomingMidiMessage(MidiInput\* source, const MidiMessage& message)`  
   This callback function is triggered by the system when a MIDI message is received. The message is added to *queueIn*.

3. **Sending MIDI Messages**

   - `void sendMidiMessage(const MidiMessage& sysMsg)`  
   This function adds outgoing MIDI messages to *queueOut*.

4. **Timer Callback Function**

   - `void timerCallback(int timerID)`  
   This function is called by a timer to process MIDI messages. There are two cases:  
     - `TIMER\_MIDI\_OUT`  
     Checks if there are any messages in *queueOut*. If there are, it sends the first message and removes it from the queue.  
     
     - `TIMER\_MIDI\_IN`  
     Checks if there are any messages in *queueIn*. If there are, it processes the first message and removes it from the queue.

**Additional Notes:**

- **Timer Intervals**  
The timers operate at millisecond intervals, ensuring efficient data processing without overwhelming the system.  

- **Callback Function**  
`handleIncomingMidiMessage` is a callback function triggered by the system. `MidiInput` is  the input source, and `MidiMessage` is the MIDI message received.  

- **JUCE Library**  
Refer to the JUCE documentation for more details on handling MIDI messages and using the JUCE framework for audio and MIDI development.

## MIDI Send

The `sendMessageNow()` function is a method of the *MidiOutput* class, and the *mOut* pointer is obtained from the *AudioDeviceManager*.

```
...
MidiOutput* mOut = audioDeviceManager->getDefaultMidiOutput();
    if (mOut) mOut->sendMessageNow(sysMsg);
...
```

## MIDI Receive

The following section demonstrates how received MIDI packets are interpreted. It's essential to analyze the MIDI messages contained in queueIn. Each MIDI message can represent different actions, such as button pressed, encoder changes, etc.	

```cpp
constexpr uint8_t FATAR_SYSEX_ID[3] = { 0x00, 0x20, 0x1A };
constexpr uint8_t PRODUCT_ID = 0x0E;
constexpr uint8_t HOST_ID = 0x01;
constexpr uint8_t DEVICE_ID = 0x00;

void parseMidiInput(const MidiMessage& message)
{
    if (message.isSysEx()) {
        if (message.getSysExDataSize() < 7) return;

        uint8_t fatar1 = message.getSysExData()[0];
        uint8_t fatar2 = message.getSysExData()[1];
        uint8_t fatar3 = message.getSysExData()[2];
        uint8_t prodId = message.getSysExData()[3];
        uint8_t hostId = message.getSysExData()[4];
        uint8_t deviceId = message.getSysExData()[5];

        if (
                fatar1 != FATAR_SYSEX_ID[0] ||
                fatar2 != FATAR_SYSEX_ID[1] ||
                fatar3 != FATAR_SYSEX_ID[2] ||
                prodId != PRODUCT_ID        ||
                hostId != HOST_ID           ||
                deviceId != DEVICE_ID
           ) return;

        uint8_t item_id = message.getSysExData()[6];
        uint8_t item_num = message.getSysExData()[7];
        uint8_t item_val = message.getSysExData()[8];

        switch (item_id)
        {
            case ITEM_SYSTEM_MESSAGE: 
                // Handle system messages
                handleSystemMessage(item_num, item_val);
            break;
            case ITEM_ENCODER_MESSAGE:
                // Handle display messages
                handleEncoderMessage(item_num, item_val);
            break;
            case ITEM_BUTTON_MESSAGE:
                // Handle button messages
                handleButtonMessage(item_num, item_val);
            break;
        }
    }
}
```

## MIDI Parse

This example illustrates how to read the values from various controls on the SL-MKII. This logic is applicable to all other control elements as well.

```cpp
void handleSystemMessage(uint8_t item_num, uint8_t item_val)
{
    switch (item_num)
    {
        case SYSTEM_LOGIN_REQUEST:
            // Handle login request message
            break;

        case SYSTEM_LOGIN_CONFIRM:
            // Handle login confirmation message
            break;

        case SYSTEM_LOGOUT_REQUEST:
            // Handle logout request message
            break;

        case SYSTEM_LOGOUT_CONFIRM:
            // Handle logout confirm message
            break;
    }
}

void handleButtonMessage(uint8_t item_num, uint8_t item_val)
{
    switch (item_num)
    {
        case JOYSTICK_BUTTON_MAIN:
            // Handle Joystick button main press
            break;
           
        case APPLY_BUTTON:
            // Handle Apply button
            break;
           
        case ENCODER_BUTTON_1:
            // Handle Zone 1 Encoder
            break;
           
        case SELECT_ZOOM_BUTTON_1:
            // Handle Zoom select button 1
            break;
    }
}
```
