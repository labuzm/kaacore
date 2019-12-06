#pragma once

#include <initializer_list>
#include <string>
#include <unordered_map>
#include <vector>

#include <SDL.h>
#include <glm/glm.hpp>

namespace kaacore {

typedef SDL_JoystickID ControllerID;

enum class Keycode {
    unknown = SDLK_UNKNOWN,
    return_ = SDLK_RETURN,
    escape = SDLK_ESCAPE,
    backspace = SDLK_BACKSPACE,
    tab = SDLK_TAB,
    space = SDLK_SPACE,
    exclaim = SDLK_EXCLAIM,
    quotedbl = SDLK_QUOTEDBL,
    hash = SDLK_HASH,
    percent = SDLK_PERCENT,
    dollar = SDLK_DOLLAR,
    ampersand = SDLK_AMPERSAND,
    quote = SDLK_QUOTE,
    leftparen = SDLK_LEFTPAREN,
    rightparen = SDLK_RIGHTPAREN,
    asterisk = SDLK_ASTERISK,
    plus = SDLK_PLUS,
    comma = SDLK_COMMA,
    minus = SDLK_MINUS,
    period = SDLK_PERIOD,
    slash = SDLK_SLASH,
    num_0 = SDLK_0,
    num_1 = SDLK_1,
    num_2 = SDLK_2,
    num_3 = SDLK_3,
    num_4 = SDLK_4,
    num_5 = SDLK_5,
    num_6 = SDLK_6,
    num_7 = SDLK_7,
    num_8 = SDLK_8,
    num_9 = SDLK_9,
    colon = SDLK_COLON,
    semicolon = SDLK_SEMICOLON,
    less = SDLK_LESS,
    equals = SDLK_EQUALS,
    greater = SDLK_GREATER,
    question = SDLK_QUESTION,
    at = SDLK_AT,
    leftbracket = SDLK_LEFTBRACKET,
    backslash = SDLK_BACKSLASH,
    rightbracket = SDLK_RIGHTBRACKET,
    caret = SDLK_CARET,
    underscore = SDLK_UNDERSCORE,
    backquote = SDLK_BACKQUOTE,
    a = SDLK_a,
    b = SDLK_b,
    c = SDLK_c,
    d = SDLK_d,
    e = SDLK_e,
    f = SDLK_f,
    g = SDLK_g,
    h = SDLK_h,
    i = SDLK_i,
    j = SDLK_j,
    k = SDLK_k,
    l = SDLK_l,
    m = SDLK_m,
    n = SDLK_n,
    o = SDLK_o,
    p = SDLK_p,
    q = SDLK_q,
    r = SDLK_r,
    s = SDLK_s,
    t = SDLK_t,
    u = SDLK_u,
    v = SDLK_v,
    w = SDLK_w,
    x = SDLK_x,
    y = SDLK_y,
    z = SDLK_z,
    A = SDLK_a,
    B = SDLK_b,
    C = SDLK_c,
    D = SDLK_d,
    E = SDLK_e,
    F = SDLK_f,
    G = SDLK_g,
    H = SDLK_h,
    I = SDLK_i,
    J = SDLK_j,
    K = SDLK_k,
    L = SDLK_l,
    M = SDLK_m,
    N = SDLK_n,
    O = SDLK_o,
    P = SDLK_p,
    Q = SDLK_q,
    R = SDLK_r,
    S = SDLK_s,
    T = SDLK_t,
    U = SDLK_u,
    V = SDLK_v,
    W = SDLK_w,
    X = SDLK_x,
    Y = SDLK_y,
    Z = SDLK_z,
    capslock = SDLK_CAPSLOCK,
    F1 = SDLK_F1,
    F2 = SDLK_F2,
    F3 = SDLK_F3,
    F4 = SDLK_F4,
    F5 = SDLK_F5,
    F6 = SDLK_F6,
    F7 = SDLK_F7,
    F8 = SDLK_F8,
    F9 = SDLK_F9,
    F10 = SDLK_F10,
    F11 = SDLK_F11,
    F12 = SDLK_F12,
    printscreen = SDLK_PRINTSCREEN,
    scrolllock = SDLK_SCROLLLOCK,
    pause = SDLK_PAUSE,
    insert = SDLK_INSERT,
    home = SDLK_HOME,
    pageup = SDLK_PAGEUP,
    delete_ = SDLK_DELETE,
    end = SDLK_END,
    pagedown = SDLK_PAGEDOWN,
    right = SDLK_RIGHT,
    left = SDLK_LEFT,
    down = SDLK_DOWN,
    up = SDLK_UP,
    numlockclear = SDLK_NUMLOCKCLEAR,
    kp_divide = SDLK_KP_DIVIDE,
    kp_multiply = SDLK_KP_MULTIPLY,
    kp_minus = SDLK_KP_MINUS,
    kp_plus = SDLK_KP_PLUS,
    kp_enter = SDLK_KP_ENTER,
    kp_1 = SDLK_KP_1,
    kp_2 = SDLK_KP_2,
    kp_3 = SDLK_KP_3,
    kp_4 = SDLK_KP_4,
    kp_5 = SDLK_KP_5,
    kp_6 = SDLK_KP_6,
    kp_7 = SDLK_KP_7,
    kp_8 = SDLK_KP_8,
    kp_9 = SDLK_KP_9,
    kp_0 = SDLK_KP_0,
    kp_period = SDLK_KP_PERIOD,
    application = SDLK_APPLICATION,
    power = SDLK_POWER,
    kp_equals = SDLK_KP_EQUALS,
    F13 = SDLK_F13,
    F14 = SDLK_F14,
    F15 = SDLK_F15,
    F16 = SDLK_F16,
    F17 = SDLK_F17,
    F18 = SDLK_F18,
    F19 = SDLK_F19,
    F20 = SDLK_F20,
    F21 = SDLK_F21,
    F22 = SDLK_F22,
    F23 = SDLK_F23,
    F24 = SDLK_F24,
    execute = SDLK_EXECUTE,
    help = SDLK_HELP,
    menu = SDLK_MENU,
    select = SDLK_SELECT,
    stop = SDLK_STOP,
    again = SDLK_AGAIN,
    undo = SDLK_UNDO,
    cut = SDLK_CUT,
    copy = SDLK_COPY,
    paste = SDLK_PASTE,
    find = SDLK_FIND,
    mute = SDLK_MUTE,
    volumeup = SDLK_VOLUMEUP,
    volumedown = SDLK_VOLUMEDOWN,
    kp_comma = SDLK_KP_COMMA,
    kp_equalsas400 = SDLK_KP_EQUALSAS400,
    alterase = SDLK_ALTERASE,
    sysreq = SDLK_SYSREQ,
    cancel = SDLK_CANCEL,
    clear = SDLK_CLEAR,
    prior = SDLK_PRIOR,
    return2 = SDLK_RETURN2,
    separator = SDLK_SEPARATOR,
    out = SDLK_OUT,
    oper = SDLK_OPER,
    clearagain = SDLK_CLEARAGAIN,
    crsel = SDLK_CRSEL,
    exsel = SDLK_EXSEL,
    kp_00 = SDLK_KP_00,
    kp_000 = SDLK_KP_000,
    thousandsseparator = SDLK_THOUSANDSSEPARATOR,
    decimalseparator = SDLK_DECIMALSEPARATOR,
    currencyunit = SDLK_CURRENCYUNIT,
    currencysubunit = SDLK_CURRENCYSUBUNIT,
    kp_leftparen = SDLK_KP_LEFTPAREN,
    kp_rightparen = SDLK_KP_RIGHTPAREN,
    kp_leftbrace = SDLK_KP_LEFTBRACE,
    kp_rightbrace = SDLK_KP_RIGHTBRACE,
    kp_tab = SDLK_KP_TAB,
    kp_backspace = SDLK_KP_BACKSPACE,
    kp_a = SDLK_KP_A,
    kp_b = SDLK_KP_B,
    kp_c = SDLK_KP_C,
    kp_d = SDLK_KP_D,
    kp_e = SDLK_KP_E,
    kp_f = SDLK_KP_F,
    kp_xor = SDLK_KP_XOR,
    kp_power = SDLK_KP_POWER,
    kp_percent = SDLK_KP_PERCENT,
    kp_less = SDLK_KP_LESS,
    kp_greater = SDLK_KP_GREATER,
    kp_ampersand = SDLK_KP_AMPERSAND,
    kp_dblampersand = SDLK_KP_DBLAMPERSAND,
    kp_verticalbar = SDLK_KP_VERTICALBAR,
    kp_dblverticalbar = SDLK_KP_DBLVERTICALBAR,
    kp_colon = SDLK_KP_COLON,
    kp_hash = SDLK_KP_HASH,
    kp_space = SDLK_KP_SPACE,
    kp_at = SDLK_KP_AT,
    kp_exclam = SDLK_KP_EXCLAM,
    kp_memstore = SDLK_KP_MEMSTORE,
    kp_memrecall = SDLK_KP_MEMRECALL,
    kp_memclear = SDLK_KP_MEMCLEAR,
    kp_memadd = SDLK_KP_MEMADD,
    kp_memsubtract = SDLK_KP_MEMSUBTRACT,
    kp_memmultiply = SDLK_KP_MEMMULTIPLY,
    kp_memdivide = SDLK_KP_MEMDIVIDE,
    kp_plusminus = SDLK_KP_PLUSMINUS,
    kp_clear = SDLK_KP_CLEAR,
    kp_clearentry = SDLK_KP_CLEARENTRY,
    kp_binary = SDLK_KP_BINARY,
    kp_octal = SDLK_KP_OCTAL,
    kp_decimal = SDLK_KP_DECIMAL,
    kp_hexadecimal = SDLK_KP_HEXADECIMAL,
    lctrl = SDLK_LCTRL,
    lshift = SDLK_LSHIFT,
    lalt = SDLK_LALT,
    lgui = SDLK_LGUI,
    rctrl = SDLK_RCTRL,
    rshift = SDLK_RSHIFT,
    ralt = SDLK_RALT,
    rgui = SDLK_RGUI,
    mode = SDLK_MODE,
    audionext = SDLK_AUDIONEXT,
    audioprev = SDLK_AUDIOPREV,
    audiostop = SDLK_AUDIOSTOP,
    audioplay = SDLK_AUDIOPLAY,
    audiomute = SDLK_AUDIOMUTE,
    mediaselect = SDLK_MEDIASELECT,
    www = SDLK_WWW,
    mail = SDLK_MAIL,
    calculator = SDLK_CALCULATOR,
    computer = SDLK_COMPUTER,
    ac_search = SDLK_AC_SEARCH,
    ac_home = SDLK_AC_HOME,
    ac_back = SDLK_AC_BACK,
    ac_forward = SDLK_AC_FORWARD,
    ac_stop = SDLK_AC_STOP,
    ac_refresh = SDLK_AC_REFRESH,
    ac_bookmarks = SDLK_AC_BOOKMARKS,
    brightnessdown = SDLK_BRIGHTNESSDOWN,
    brightnessup = SDLK_BRIGHTNESSUP,
    displayswitch = SDLK_DISPLAYSWITCH,
    kbdillumtoggle = SDLK_KBDILLUMTOGGLE,
    kbdillumdown = SDLK_KBDILLUMDOWN,
    kbdillumup = SDLK_KBDILLUMUP,
    eject = SDLK_EJECT,
    sleep = SDLK_SLEEP,
};

enum class MouseButton {
    left = SDL_BUTTON_LEFT,
    middle = SDL_BUTTON_MIDDLE,
    right = SDL_BUTTON_RIGHT,
    x1 = SDL_BUTTON_X1,
    x2 = SDL_BUTTON_X2
};

enum class ControllerButton {
    a = SDL_CONTROLLER_BUTTON_A,
    b = SDL_CONTROLLER_BUTTON_B,
    x = SDL_CONTROLLER_BUTTON_X,
    y = SDL_CONTROLLER_BUTTON_Y,
    back = SDL_CONTROLLER_BUTTON_BACK,
    guide = SDL_CONTROLLER_BUTTON_GUIDE,
    start = SDL_CONTROLLER_BUTTON_START,
    left_stick = SDL_CONTROLLER_BUTTON_LEFTSTICK,
    right_stick = SDL_CONTROLLER_BUTTON_RIGHTSTICK,
    left_shoulder = SDL_CONTROLLER_BUTTON_LEFTSHOULDER,
    right_shoulder = SDL_CONTROLLER_BUTTON_RIGHTSHOULDER,
    dpad_up = SDL_CONTROLLER_BUTTON_DPAD_UP,
    dpad_down = SDL_CONTROLLER_BUTTON_DPAD_DOWN,
    dpad_left = SDL_CONTROLLER_BUTTON_DPAD_LEFT,
    dpad_right = SDL_CONTROLLER_BUTTON_DPAD_RIGHT,
};

enum class ControllerAxis {
    left_x = SDL_CONTROLLER_AXIS_LEFTX,
    left_y = SDL_CONTROLLER_AXIS_LEFTY,
    right_x = SDL_CONTROLLER_AXIS_RIGHTX,
    right_y = SDL_CONTROLLER_AXIS_RIGHTY,
    trigger_left = SDL_CONTROLLER_AXIS_TRIGGERLEFT,
    trigger_right = SDL_CONTROLLER_AXIS_TRIGGERRIGHT,
};

enum class EventType {
    quit = SDL_QUIT,
    clipboard_updated = SDL_CLIPBOARDUPDATE,

    key_down = SDL_KEYDOWN,
    key_up = SDL_KEYUP,

    mouse_motion = SDL_MOUSEMOTION,
    mouse_button_down = SDL_MOUSEBUTTONDOWN,
    mouse_button_up = SDL_MOUSEBUTTONUP,
    mouse_wheel = SDL_MOUSEWHEEL,

    controller_axis_motion = SDL_CONTROLLERAXISMOTION,
    controller_button_down = SDL_CONTROLLERBUTTONDOWN,
    controller_button_up = SDL_CONTROLLERBUTTONUP,
    controller_added = SDL_CONTROLLERDEVICEADDED,
    controller_removed = SDL_CONTROLLERDEVICEREMOVED,
    controller_remapped = SDL_CONTROLLERDEVICEREMAPPED,
};

enum class WindowEventType {
    shown = SDL_WINDOWEVENT_SHOWN,
    hidden = SDL_WINDOWEVENT_HIDDEN,
    exposed = SDL_WINDOWEVENT_EXPOSED,
    moved = SDL_WINDOWEVENT_MOVED,
    resized = SDL_WINDOWEVENT_RESIZED,
    minimized = SDL_WINDOWEVENT_MINIMIZED,
    maximized = SDL_WINDOWEVENT_MAXIMIZED,
    restored = SDL_WINDOWEVENT_RESTORED,
    enter = SDL_WINDOWEVENT_ENTER,
    leave = SDL_WINDOWEVENT_LEAVE,
    focus_gained = SDL_WINDOWEVENT_FOCUS_GAINED,
    focus_lost = SDL_WINDOWEVENT_FOCUS_LOST,
    close = SDL_WINDOWEVENT_CLOSE
};

enum class CompoundEventType {
    window = SDL_WINDOWEVENT,
    system = 0,
    keyboard,
    mouse,
    controller
};

enum class CompoundControllerAxis { left_stick, right_stick };

static inline bool
_is_event_supported(uint32_t type)
{
    if (type == static_cast<uint32_t>(EventType::quit) or
        type == static_cast<uint32_t>(EventType::clipboard_updated) or
        type == static_cast<uint32_t>(EventType::key_down) or
        type == static_cast<uint32_t>(EventType::key_up) or
        type == static_cast<uint32_t>(EventType::mouse_motion) or
        type == static_cast<uint32_t>(EventType::mouse_button_down) or
        type == static_cast<uint32_t>(EventType::mouse_button_up) or
        type == static_cast<uint32_t>(EventType::mouse_wheel) or
        type == static_cast<uint32_t>(EventType::controller_axis_motion) or
        type == static_cast<uint32_t>(EventType::controller_button_down) or
        type == static_cast<uint32_t>(EventType::controller_button_up) or
        type == static_cast<uint32_t>(EventType::controller_added) or
        type == static_cast<uint32_t>(EventType::controller_removed) or
        type == static_cast<uint32_t>(EventType::controller_remapped)) {
        return true;
    }
    return false;
}

struct BaseEvent {
    SDL_Event sdl_event;

    uint32_t type() const;
    uint32_t timestamp() const;
};

struct SystemEvent : public BaseEvent {
    bool quit() const;
    bool clipboard_updated() const;
};

struct WindowEvent : public BaseEvent {
    bool shown() const;
    bool exposed() const;
    bool moved() const;
    bool resized() const;
    bool minimized() const;
    bool maximized() const;
    bool restored() const;
    bool enter() const;
    bool leave() const;
    bool focus_gained() const;
    bool focus_lost() const;
    bool close() const;

    glm::dvec2 size() const;
    glm::dvec2 position() const;
};

struct KeyboardEvent : public BaseEvent {
    bool is_pressing(const Keycode kc) const;
    bool is_releasing(const Keycode kc) const;
};

struct MouseEvent : public BaseEvent {
    bool button() const;
    bool motion() const;
    bool wheel() const;

    bool is_pressing(const MouseButton mb) const;
    bool is_releasing(const MouseButton mb) const;
    glm::dvec2 position() const;
    glm::dvec2 scroll() const;
};

struct ControllerEvent : public BaseEvent {
    bool button() const;
    bool axis() const;
    bool added() const;
    bool removed() const;
    bool remapped() const;

    ControllerID id() const;
    bool is_pressing(const ControllerButton cb) const;
    bool is_releasing(const ControllerButton cb) const;
    double axis_motion(const ControllerAxis ca) const;
};

struct Event {
    union {
        BaseEvent common;

        SystemEvent _system;
        WindowEvent _window;
        KeyboardEvent _keyboard;
        MouseEvent _mouse;
        ControllerEvent _controller;
    };

    Event();
    Event(const SDL_Event sdl_event);

    uint32_t type() const;
    uint32_t timestamp() const;

    const SystemEvent* const system() const;
    const WindowEvent* const window() const;
    const KeyboardEvent* const keyboard() const;
    const MouseEvent* const mouse() const;
    const ControllerEvent* const controller() const;
};

struct InputManager {
    std::vector<Event> events_queue;

    struct SystemManager {
        std::string get_clipboard_text() const;
        void set_clipboard_text(const std::string& text) const;
    } system;

    struct KeyboardManager {
        bool is_pressed(const Keycode kc) const;
        bool is_released(const Keycode kc) const;
    } keyboard;

    struct MouseManager {
        bool is_pressed(const MouseButton mb) const;
        bool is_released(const MouseButton mb) const;
        glm::dvec2 get_position() const;
    } mouse;

    struct ControllerManager {
        ~ControllerManager();
        bool is_connected(const ControllerID id) const;
        bool is_pressed(const ControllerButton cb, const ControllerID id) const;
        bool is_released(
            const ControllerButton cb, const ControllerID id) const;
        bool is_pressed(const ControllerAxis ca, const ControllerID id) const;
        bool is_released(const ControllerAxis ca, const ControllerID id) const;
        double get_axis_motion(
            const ControllerAxis axis, const ControllerID id) const;
        std::string get_name(const ControllerID id) const;
        glm::dvec2 get_triggers(const ControllerID id) const;
        glm::dvec2 get_sticks(
            const CompoundControllerAxis axis, const ControllerID id) const;
        std::vector<ControllerID> get_connected_controllers() const;

        ControllerID connect(int device_index);
        void disconnect(ControllerID id);

      private:
        std::unordered_map<ControllerID, SDL_GameController*> _connected_map;
    } controller;

    void push_event(SDL_Event sdl_event);
    void clear_events();
};

} // namespace kaacore
