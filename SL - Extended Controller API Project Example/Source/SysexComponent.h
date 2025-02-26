/*
 ==============================================================================

 SysexComponent.h
 Created: June 2024
 Author: Alessandro Petrolati
 SL - Extended Controller API
 STUDIOLOGIC/FATAR Srl
 alessandro.petrolati@studiologic-music.com
 www.studiologic-music.com

 ==============================================================================
 */

#include <JuceHeader.h>

using namespace juce;

const int LCD_WIDTH = 320;
const int LCD_HEIGHT = 240;

#if JUCE_MAC || JUCE_IOS
const String LINK_PORT_IN = "SL LINK";
const String LINK_PORT_OUT = "SL LINK";
#elif JUCE_WINDOWS
const String LINK_PORT_IN = "MIDIIN3 (SL)";
const String LINK_PORT_OUT = "MIDIOUT3 (SL)";
#endif

//------------------------------------------
typedef enum
{
    TIMER_IDENTITY = 0x00,
    
    TIMER_MIDI_PORT,
    TIMER_MIDI_IN,
    TIMER_MIDI_OUT,
    
} TimerId;

//------------------------------------------
typedef enum
{
    SYSTEM_LOGIN_REQUEST = 0x00,
    SYSTEM_LOGIN_CONFIRM,
    SYSTEM_LOGOUT_REQUEST,
    SYSTEM_LOGOUT_CONFIRM,
    SYSTEM_STANDBY,
    SYSTEM_RESTART
} system_type;

//------------------------------------------
typedef enum
{
    ITEM_SYSTEM_SYSTEM = 0x00,
    ITEM_BUTTON,
    ITEM_LED,
    ITEM_ENCODER,
    ITEM_LCD,
    ITEM_LED_RGB
} item_type;

//------------------------------------------
typedef enum
{
    ENCODER_BUTTON_1 = 0x00,
    ENCODER_BUTTON_2,
    ENCODER_BUTTON_3,
    ENCODER_BUTTON_4,
    SELECT_ZOOM_BUTTON_1,
    SELECT_ZOOM_BUTTON_2,
    SELECT_ZOOM_BUTTON_3,
    SELECT_ZOOM_BUTTON_4,
    MODE_BUTTON,
    GLOBAL_BUTTON,
    DAW_BUTTON,
    NOT_USED_BUTTON_1,
    NOT_USED_BUTTON_2,
    NOT_USED_BUTTON_3,
    APPLY_BUTTON,
    CANCEL_BUTTON,
    HOME_BUTTON,
    JOYSTICK_BUTTON_UP,
    JOYSTICK_BUTTON_LEFT,
    JOYSTICK_BUTTON_DOWN,
    JOYSTICK_BUTTON_RIGHT,
    JOYSTICK_BUTTON_MAIN,
    BUTTON_NUM
} list_button;

//------------------------------------------
typedef enum
{
    ENCODER_1 = 0x00,
    ENCODER_2,
    ENCODER_3,
    ENCODER_4,
    JOYSTICK_ENCODER,
    ENCODER_NUM
} list_encoder;

//------------------------------------------
typedef enum
{
    SELECT_ZOOM_1_LED,
    SELECT_ZOOM_2_LED,
    SELECT_ZOOM_3_LED,
    SELECT_ZOOM_4_LED,
    MODE_LED,
    GLOBAL_LED,
    DAW_LED,
    NOT_USED_EX_SPLIT_LED,
    NOT_USED_EX_EDIT_LED,
    NOT_USED_EX_GLOBAL_LED,
    APPLY_LED,
    CANCEL_LED,
    HOME_LED,
    LED_NUM
} list_led;

//------------------------------------------
typedef enum
{
    LED_RGB_1 = 0x00,
    LED_RGB_2,
    LED_RGB_3,
    LED_RGB_4,
    LED_RGB_NUM
} list_led_rgb;

//------------------------------------------
typedef enum
{
    IMG_KNOB_NORMAL = 0x00,
    IMG_KNOB_CENTERED,
    IMG_TOGGLE,
    IMG_NAVIGATION,
    IMG_ARROWS,
    IMG_GENERAL,

    // STUDIOLOGIC ICON
    IMG_SL_ICON
} group_icon;

//------------------------------------------
typedef enum
{
    TOGGLE_ON,
    TOGGLE_OFF
} TOGGLE_iidx;

typedef enum
{
    NAV_LEFT = 0x00,
    NAV_RIGHT,
    NAV_LEFT_RIGHT,
    NAV_UP_DOWN,
    NAV_ROTATE,
    NAV_PUSH,
    NAV_APPLY,
    NAV_CANCEL,
} nav_iidx;

//------------------------------------------
typedef enum
{
    ARROW_UP = 0x00,
    ARROW_DOWN,
    ARROW_LEFT,
    ARROW_RIGHT,
    ARROW_LEFT_RIGHT,
} arrow_iidx;

//------------------------------------------
typedef enum
{
    TEXT_SIZE_SMALL = 0x00, // size 21
    TEXT_SIZE_MIDDLE,    // size 27
    TEXT_SIZE_BIG        // size 33
} text_size;

//------------------------------------------
typedef enum
{
    TEXT_ALIGN_LEFT = 0x00, // alignement: left
    TEXT_ALIGN_CENTER,   // alignement: center
    TEXT_ALIGN_RIGHT     // alignement: right
} text_align;

//=========================================================================
/**
 */
class SysexComponent : public Component, private MidiInputCallback, private MultiTimer
{
public:
    SysexComponent();
    ~SysexComponent() override;

    void paint(Graphics&) override;
    void resized() override;
    
    // Send MIDI Message
    void sendMidiMessage(const MidiMessage& sysMsg);
    void sendIdentity();
    void logoutRequest();
    
    // Send Sysex to SL
    //------------------------------------------
    void CLEAR_LCD(uint8_t r = 0, uint8_t g = 0, uint8_t b = 0);
    void LED_RGB(uint8_t num, uint8_t r, uint8_t g, uint8_t b, uint8_t brigh = 0x7F);
    void LED_WHITE(uint8_t num, uint8_t state);
    
    void PLOT_TEXT(String text,
                   uint16_t x,
                   uint16_t y,
                   uint16_t width,
                   uint8_t txIndent,
                   uint8_t fontSize,
                   uint8_t r_fore, uint8_t g_fore, uint8_t b_fore,
                   uint8_t r_back, uint8_t g_back, uint8_t b_back);
    
    void PLOT_RECT(uint16_t x,
                   uint16_t y,
                   uint16_t w,
                   uint16_t h,
                   uint8_t r, uint8_t g, uint8_t b
                   );
    
    void PLOT_BITMAP(uint16_t x,
                     uint16_t y,
                     uint8_t bmp_gdx,
                     uint8_t bmp_idx,
                     uint8_t r_fore, uint8_t g_fore, uint8_t b_fore,
                     uint8_t r_back, uint8_t g_back, uint8_t b_back);

    void PLOT_VIEW_MESSAGE(String text);
    void PLOT_VIEW_INFO();
    
private:
    AudioDeviceManager audioDeviceManager;
    AudioDeviceSelectorComponent audioSetupComp;
    String deviceName;

    std::vector<MidiMessage> queueIn;
    std::vector<MidiMessage> queueOut;

    int myEncoderValue = 0;
    
    // Callbacks
    //------------------------------------------
    void handleIncomingMidiMessage(MidiInput* source, const MidiMessage& message) override;
    void timerCallback(int timerID) override;

    void parseMidiInput(const MidiMessage& message);
    void handleSystemMessage(uint8_t item_num, uint8_t item_val);
    void handleButtonMessage(uint8_t item_num, uint8_t item_val);
    void handleEncoderMessage(uint8_t item_num, uint8_t item_val);
    
    //------------------------------------------
    struct MsbLsb
    {
        MsbLsb(uint8_t m, uint8_t l) : msb(m), lsb(l)
        {
        }

        uint8_t msb = 0x00;
        uint8_t lsb = 0x00;
    };
    
    MsbLsb get7BitMsbLsb(uint16_t val)
    {
        uint8_t msb = val >> 0x07;
        uint8_t lsb = val & 0x7F;
        return MsbLsb(msb, lsb);
    }
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(SysexComponent)
};
