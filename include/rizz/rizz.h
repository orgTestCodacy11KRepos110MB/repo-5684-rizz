//
// Copyright 2019 Sepehr Taghdisian (septag@github). All rights reserved.
// License: https://github.com/septag/rizz#license-bsd-2-clause
//
#pragma once

#include "config.h"

#include "sx/allocator.h"
#include "sx/fiber.h"
#include "sx/io.h"
#include "sx/jobs.h"
#include "sx/math-types.h"
#include "sx/platform.h"
#include "sx/timer.h"

#define RIZZ_INCLUDE_SG_TYPES
#   include "sg-types.h"
#undef RIZZ_INCLUDE_SG_TYPES


////////////////////////////////////////////////////////////////////////////////////////////////////
// @types
#ifdef __cplusplus
#    define RIZZ_API extern "C"
#else
#    define RIZZ_API extern
#endif

// Global Ids
typedef union  { uintptr_t id;  void* ptr; }    rizz_asset_obj;
typedef struct { uint32_t id; } rizz_asset;
typedef struct { uint32_t id; } rizz_asset_group;
typedef struct { uint32_t id; } rizz_http;
typedef struct { uint32_t id; } rizz_gfx_stage;
typedef struct { uint32_t id; } rizz_profile_capture;

typedef struct sx_thread rizz_thread;

// Id conversion macros
#define rizz_to_id(_index) ((uint32_t)(_index) + 1)
#define rizz_to_index(_id) ((int)(_id)-1)

// versions are generally 4 digits: MNNF
// M: major
// NN: minor
// F: fix
#define rizz_version_major(_v) ((_v) / 1000)
#define rizz_version_minor(_v) (((_v) % 1000) / 10)
#define rizz_version_bugfix(_v) ((_v) % 10)

////////////////////////////////////////////////////////////////////////////////////////////////////
// @app

// Set this flag for rizz_config.swap_interval to disable vsync
#define RIZZ_APP_SWAP_INTERVAL_NOSYNC 0x7fffffff

enum {
    RIZZ_APP_MAX_TOUCHPOINTS = 8,
    RIZZ_APP_MAX_MOUSEBUTTONS = 3,
    RIZZ_APP_MAX_KEYCODES = 512,
};

typedef enum rizz_app_event_type {
    RIZZ_APP_EVENTTYPE_INVALID,
    RIZZ_APP_EVENTTYPE_KEY_DOWN,
    RIZZ_APP_EVENTTYPE_KEY_UP,
    RIZZ_APP_EVENTTYPE_CHAR,
    RIZZ_APP_EVENTTYPE_MOUSE_DOWN,
    RIZZ_APP_EVENTTYPE_MOUSE_UP,
    RIZZ_APP_EVENTTYPE_MOUSE_SCROLL,
    RIZZ_APP_EVENTTYPE_MOUSE_MOVE,
    RIZZ_APP_EVENTTYPE_MOUSE_ENTER,
    RIZZ_APP_EVENTTYPE_MOUSE_LEAVE,
    RIZZ_APP_EVENTTYPE_TOUCHES_BEGAN,
    RIZZ_APP_EVENTTYPE_TOUCHES_MOVED,
    RIZZ_APP_EVENTTYPE_TOUCHES_ENDED,
    RIZZ_APP_EVENTTYPE_TOUCHES_CANCELLED,
    RIZZ_APP_EVENTTYPE_RESIZED,
    RIZZ_APP_EVENTTYPE_ICONIFIED,
    RIZZ_APP_EVENTTYPE_RESTORED,
    RIZZ_APP_EVENTTYPE_SUSPENDED,
    RIZZ_APP_EVENTTYPE_RESUMED,
    RIZZ_APP_EVENTTYPE_UPDATE_CURSOR,
    RIZZ_APP_EVENTTYPE_QUIT_REQUESTED,
    RIZZ_APP_EVENTTYPE_CLIPBOARD_PASTED,
    RIZZ_APP_EVENTTYPE_RESIZING,
    RIZZ_APP_EVENTTYPE_MOVING,
    RIZZ_APP_EVENTTYPE_MOVED,
    _RIZZ_APP_EVENTTYPE_NUM,
    RIZZ_APP_EVENTTYPE_UPDATE_APIS,    // happens when a plugin updates it's API
    _RIZZ_APP_EVENTTYPE_FORCE_U32 = 0x7FFFFFF
} rizz_app_event_type;

/* key codes are the same names and values as GLFW */
typedef enum rizz_keycode {
    RIZZ_APP_KEYCODE_INVALID = 0,
    RIZZ_APP_KEYCODE_SPACE = 32,
    RIZZ_APP_KEYCODE_APOSTROPHE = 39, /* ' */
    RIZZ_APP_KEYCODE_COMMA = 44,      /* , */
    RIZZ_APP_KEYCODE_MINUS = 45,      /* - */
    RIZZ_APP_KEYCODE_PERIOD = 46,     /* . */
    RIZZ_APP_KEYCODE_SLASH = 47,      /* / */
    RIZZ_APP_KEYCODE_0 = 48,
    RIZZ_APP_KEYCODE_1 = 49,
    RIZZ_APP_KEYCODE_2 = 50,
    RIZZ_APP_KEYCODE_3 = 51,
    RIZZ_APP_KEYCODE_4 = 52,
    RIZZ_APP_KEYCODE_5 = 53,
    RIZZ_APP_KEYCODE_6 = 54,
    RIZZ_APP_KEYCODE_7 = 55,
    RIZZ_APP_KEYCODE_8 = 56,
    RIZZ_APP_KEYCODE_9 = 57,
    RIZZ_APP_KEYCODE_SEMICOLON = 59, /* ; */
    RIZZ_APP_KEYCODE_EQUAL = 61,     /* = */
    RIZZ_APP_KEYCODE_A = 65,
    RIZZ_APP_KEYCODE_B = 66,
    RIZZ_APP_KEYCODE_C = 67,
    RIZZ_APP_KEYCODE_D = 68,
    RIZZ_APP_KEYCODE_E = 69,
    RIZZ_APP_KEYCODE_F = 70,
    RIZZ_APP_KEYCODE_G = 71,
    RIZZ_APP_KEYCODE_H = 72,
    RIZZ_APP_KEYCODE_I = 73,
    RIZZ_APP_KEYCODE_J = 74,
    RIZZ_APP_KEYCODE_K = 75,
    RIZZ_APP_KEYCODE_L = 76,
    RIZZ_APP_KEYCODE_M = 77,
    RIZZ_APP_KEYCODE_N = 78,
    RIZZ_APP_KEYCODE_O = 79,
    RIZZ_APP_KEYCODE_P = 80,
    RIZZ_APP_KEYCODE_Q = 81,
    RIZZ_APP_KEYCODE_R = 82,
    RIZZ_APP_KEYCODE_S = 83,
    RIZZ_APP_KEYCODE_T = 84,
    RIZZ_APP_KEYCODE_U = 85,
    RIZZ_APP_KEYCODE_V = 86,
    RIZZ_APP_KEYCODE_W = 87,
    RIZZ_APP_KEYCODE_X = 88,
    RIZZ_APP_KEYCODE_Y = 89,
    RIZZ_APP_KEYCODE_Z = 90,
    RIZZ_APP_KEYCODE_LEFT_BRACKET = 91,  /* [ */
    RIZZ_APP_KEYCODE_BACKSLASH = 92,     /* \ */
    RIZZ_APP_KEYCODE_RIGHT_BRACKET = 93, /* ] */
    RIZZ_APP_KEYCODE_GRAVE_ACCENT = 96,  /* ` */
    RIZZ_APP_KEYCODE_WORLD_1 = 161,      /* non-US #1 */
    RIZZ_APP_KEYCODE_WORLD_2 = 162,      /* non-US #2 */
    RIZZ_APP_KEYCODE_ESCAPE = 256,
    RIZZ_APP_KEYCODE_ENTER = 257,
    RIZZ_APP_KEYCODE_TAB = 258,
    RIZZ_APP_KEYCODE_BACKSPACE = 259,
    RIZZ_APP_KEYCODE_INSERT = 260,
    RIZZ_APP_KEYCODE_DELETE = 261,
    RIZZ_APP_KEYCODE_RIGHT = 262,
    RIZZ_APP_KEYCODE_LEFT = 263,
    RIZZ_APP_KEYCODE_DOWN = 264,
    RIZZ_APP_KEYCODE_UP = 265,
    RIZZ_APP_KEYCODE_PAGE_UP = 266,
    RIZZ_APP_KEYCODE_PAGE_DOWN = 267,
    RIZZ_APP_KEYCODE_HOME = 268,
    RIZZ_APP_KEYCODE_END = 269,
    RIZZ_APP_KEYCODE_CAPS_LOCK = 280,
    RIZZ_APP_KEYCODE_SCROLL_LOCK = 281,
    RIZZ_APP_KEYCODE_NUM_LOCK = 282,
    RIZZ_APP_KEYCODE_PRINT_SCREEN = 283,
    RIZZ_APP_KEYCODE_PAUSE = 284,
    RIZZ_APP_KEYCODE_F1 = 290,
    RIZZ_APP_KEYCODE_F2 = 291,
    RIZZ_APP_KEYCODE_F3 = 292,
    RIZZ_APP_KEYCODE_F4 = 293,
    RIZZ_APP_KEYCODE_F5 = 294,
    RIZZ_APP_KEYCODE_F6 = 295,
    RIZZ_APP_KEYCODE_F7 = 296,
    RIZZ_APP_KEYCODE_F8 = 297,
    RIZZ_APP_KEYCODE_F9 = 298,
    RIZZ_APP_KEYCODE_F10 = 299,
    RIZZ_APP_KEYCODE_F11 = 300,
    RIZZ_APP_KEYCODE_F12 = 301,
    RIZZ_APP_KEYCODE_F13 = 302,
    RIZZ_APP_KEYCODE_F14 = 303,
    RIZZ_APP_KEYCODE_F15 = 304,
    RIZZ_APP_KEYCODE_F16 = 305,
    RIZZ_APP_KEYCODE_F17 = 306,
    RIZZ_APP_KEYCODE_F18 = 307,
    RIZZ_APP_KEYCODE_F19 = 308,
    RIZZ_APP_KEYCODE_F20 = 309,
    RIZZ_APP_KEYCODE_F21 = 310,
    RIZZ_APP_KEYCODE_F22 = 311,
    RIZZ_APP_KEYCODE_F23 = 312,
    RIZZ_APP_KEYCODE_F24 = 313,
    RIZZ_APP_KEYCODE_F25 = 314,
    RIZZ_APP_KEYCODE_KP_0 = 320,
    RIZZ_APP_KEYCODE_KP_1 = 321,
    RIZZ_APP_KEYCODE_KP_2 = 322,
    RIZZ_APP_KEYCODE_KP_3 = 323,
    RIZZ_APP_KEYCODE_KP_4 = 324,
    RIZZ_APP_KEYCODE_KP_5 = 325,
    RIZZ_APP_KEYCODE_KP_6 = 326,
    RIZZ_APP_KEYCODE_KP_7 = 327,
    RIZZ_APP_KEYCODE_KP_8 = 328,
    RIZZ_APP_KEYCODE_KP_9 = 329,
    RIZZ_APP_KEYCODE_KP_DECIMAL = 330,
    RIZZ_APP_KEYCODE_KP_DIVIDE = 331,
    RIZZ_APP_KEYCODE_KP_MULTIPLY = 332,
    RIZZ_APP_KEYCODE_KP_SUBTRACT = 333,
    RIZZ_APP_KEYCODE_KP_ADD = 334,
    RIZZ_APP_KEYCODE_KP_ENTER = 335,
    RIZZ_APP_KEYCODE_KP_EQUAL = 336,
    RIZZ_APP_KEYCODE_LEFT_SHIFT = 340,
    RIZZ_APP_KEYCODE_LEFT_CONTROL = 341,
    RIZZ_APP_KEYCODE_LEFT_ALT = 342,
    RIZZ_APP_KEYCODE_LEFT_SUPER = 343,
    RIZZ_APP_KEYCODE_RIGHT_SHIFT = 344,
    RIZZ_APP_KEYCODE_RIGHT_CONTROL = 345,
    RIZZ_APP_KEYCODE_RIGHT_ALT = 346,
    RIZZ_APP_KEYCODE_RIGHT_SUPER = 347,
    RIZZ_APP_KEYCODE_MENU = 348,
} rizz_keycode;

typedef struct rizz_touch_point {
    uintptr_t identifier;
    float pos_x;
    float pos_y;
    bool changed;
} rizz_touch_point;

typedef enum rizz_mouse_btn {
    RIZZ_APP_MOUSEBUTTON_INVALID = -1,
    RIZZ_APP_MOUSEBUTTON_LEFT = 0,
    RIZZ_APP_MOUSEBUTTON_RIGHT = 1,
    RIZZ_APP_MOUSEBUTTON_MIDDLE = 2,
} rizz_mouse_btn;

enum rizz_modifier_keys_ {
    RIZZ_APP_MODIFIERKEY_SHIFT = (1 << 0),
    RIZZ_APP_MODIFIERKEY_CTRL = (1 << 1),
    RIZZ_APP_MODIFIERKEY_ALT = (1 << 2),
    RIZZ_APP_MODIFIERKEY_SUPER = (1 << 3)
};
typedef uint32_t rizz_modifier_keys;

typedef struct rizz_app_event {
    uint64_t frame_count;
    rizz_app_event_type type;
    rizz_keycode key_code;
    uint32_t char_code;
    bool key_repeat;
    rizz_modifier_keys modkeys;
    rizz_mouse_btn mouse_button;
    float mouse_x;
    float mouse_y;
    float scroll_x;
    float scroll_y;
    int num_touches;
    rizz_touch_point touches[RIZZ_APP_MAX_TOUCHPOINTS];
    int window_width;
    int window_height;
    int framebuffer_width;
    int framebuffer_height;
    void* native_event;
} rizz_app_event;

typedef void(rizz_app_event_cb)(const rizz_app_event*);
typedef void(rizz_app_shortcut_cb)(void* user);

typedef struct rizz_config rizz_config;

typedef enum rizz_cmdline_argtype {
    RIZZ_CMDLINE_ARGTYPE_NONE,
    RIZZ_CMDLINE_ARGTYPE_REQUIRED,
    RIZZ_CMDLINE_ARGTYPE_OPTIONAL
} rizz_cmdline_argtype;

typedef struct rizz_api_app {
    int (*width)(void);
    int (*height)(void);
    void (*window_size)(sx_vec2* size);
    bool (*highdpi)(void);
    float (*dpiscale)(void);
    const rizz_config* (*config)(void);
    const char* (*config_meta_value)(const char* section, const char* name);
    void (*show_keyboard)(bool show);
    bool (*keyboard_shown)(void);
    bool (*key_pressed)(rizz_keycode key);
    void (*quit)(void);
    void (*request_quit)(void);
    void (*cancel_quit)(void);
    const char* (*name)(void);
    void (*show_mouse)(bool visible);
    bool (*mouse_shown)(void);
    void (*mouse_capture)(void);
    void (*mouse_release)(void);
    const char* (*cmdline_arg_value)(const char* name);
    bool (*cmdline_arg_exists)(const char* name);
    void (*set_clipboard_string)(const char* str);
    const char* (*clipboard_string)(void);
    void (*register_shortcut)(const char* shortcut, rizz_app_shortcut_cb* shortcut_cb, void* user);
    void (*set_crash_callback)(void (*crash_cb)(void* crash_data, void* user), void* user);
} rizz_api_app;

////////////////////////////////////////////////////////////////////////////////////////////////////
// @vfs

enum rizz_vfs_flags_ {
    RIZZ_VFS_FLAG_NONE = 0x01,
    RIZZ_VFS_FLAG_ABSOLUTE_PATH = 0x02,
    RIZZ_VFS_FLAG_TEXT_FILE = 0x04,
    RIZZ_VFS_FLAG_APPEND = 0x08
};
typedef uint32_t rizz_vfs_flags;

// if mem == NULL, then there was an error opening the file
typedef void(rizz_vfs_async_read_cb)(const char* path, sx_mem_block* mem, void* user);
// if bytes_written == -1, then there was an error writing to file
typedef void(rizz_vfs_async_write_cb)(const char* path, int64_t bytes_written, sx_mem_block* mem, void* user);

typedef void(rizz_vfs_async_modify_cb)(const char* path);

typedef struct rizz_api_vfs {
    void (*register_modify)(rizz_vfs_async_modify_cb* modify_fn);

    const sx_alloc* (*alloc)(void);

    bool (*mount)(const char* path, const char* alias, bool watch);
    void (*mount_mobile_assets)(const char* alias);

    void (*read_async)(const char* path, rizz_vfs_flags flags, const sx_alloc* alloc,
                       rizz_vfs_async_read_cb* read_fn, void* user);
    void (*write_async)(const char* path, sx_mem_block* mem, rizz_vfs_flags flags,
                        rizz_vfs_async_write_cb* write_fn, void* user);

    sx_mem_block* (*read)(const char* path, rizz_vfs_flags flags, const sx_alloc* alloc);
    int64_t (*write)(const char* path, const sx_mem_block* mem, rizz_vfs_flags flags);

    bool (*mkdir)(const char* path);
    bool (*is_dir)(const char* path);
    bool (*is_file)(const char* path);
    uint64_t (*last_modified)(const char* path);
} rizz_api_vfs;

////////////////////////////////////////////////////////////////////////////////////////////////////
// @asset
enum rizz_asset_load_flags_ {
    RIZZ_ASSET_LOAD_FLAG_NONE = 0x0,
    RIZZ_ASSET_LOAD_FLAG_ABSOLUTE_PATH = 0x1,
    RIZZ_ASSET_LOAD_FLAG_WAIT_ON_LOAD = 0x2,
    RIZZ_ASSET_LOAD_FLAG_RELOAD = 0x4
};
typedef uint32_t rizz_asset_load_flags;

typedef struct rizz_asset_meta_keyval {
    char key[32];
    char value[32];
} rizz_asset_meta_keyval;

typedef struct rizz_asset_load_params {
    const char* path;               // path to asset file
    const void* params;             // must cast to asset-specific implementation type
    const sx_alloc* alloc;          // allocator that is user sends for loading asset data
    uint32_t tags;                  // user-defined tag bits
    rizz_asset_load_flags flags;    // flags that are used for loading
    uint32_t num_meta;              // meta key-value pairs, embedded in custom _rizz_ assets
    const rizz_asset_meta_keyval* metas;
} rizz_asset_load_params;

typedef struct rizz_asset_load_data {
    rizz_asset_obj obj;    // valid internal object
    void* user1;    // user-data can be allocated and filled with anything specific to asset loader
    void* user2;    // same as user1
} rizz_asset_load_data;

typedef enum rizz_asset_state {
    RIZZ_ASSET_STATE_ZOMBIE = 0,
    RIZZ_ASSET_STATE_OK,
    RIZZ_ASSET_STATE_FAILED,
    RIZZ_ASSET_STATE_LOADING
} rizz_asset_state;

typedef struct rizz_asset_callbacks {
    // Runs on main-thread
    // Should create a valid device/subsystem object and any optional user-data.
    // The reason that it is decoupled from on_load and runs on main thread is because we cannot
    // garranty that object creation and custom memory allocation is thread-safe
    // if rizz_asset_load_data.obj.id == 0, then the asset manager assumes that an error has occured
    rizz_asset_load_data (*on_prepare)(const rizz_asset_load_params* params, const sx_mem_block* mem);

    // Runs on worker-thread
    // File data is loaded and passed as 'mem'. Should fill the allocated object and user-data.
    // It is recommended that you don't create/allocate any permanent objects/memory stuff here
    // instead, do them in `on_prepare` and pass them as `data`
    bool (*on_load)(rizz_asset_load_data* data, const rizz_asset_load_params* params, const sx_mem_block* mem);

    // Runs on main-thread
    // Any optional finalization should happen in this function.
    // This function should free any user-data allocated in 'on_prepare'
    void (*on_finalize)(rizz_asset_load_data* data, const rizz_asset_load_params* params, const sx_mem_block* mem);

    // Runs on main-thread
    // Reloading of object happens automatically within asset-lib.
    // but this function can be used to sync any dependencies to entities or other assets
    // `prev_obj` is the previous object that is about to be replaced by the new one (the one in
    // `handle`)
    //            `prev_obj` is automatically released after this call by the asset manager
    void (*on_reload)(rizz_asset handle, rizz_asset_obj prev_obj, const sx_alloc* alloc);

    // Runs on main-thread
    // asset-lib calls this if asset's refcount reaches zero.
    void (*on_release)(rizz_asset_obj obj, const sx_alloc* alloc);
} rizz_asset_callbacks;

// clang-format off
// Threading rules:
//      Internally, the asset system is offloading some work to worker threads, but the API is not
//      thread-safe 
//          1) Have to load all your assets on the main thread 
//          2) You can only use `rizz_api_asset.obj()` in worker threads.
//              So basically, you have to load the assets, and pass handles to threads, 
//              and they can only fetch the object pointer
//          3) Loading can be performed in the main thread while working threads are using the API 
//             (rule #2) but without RIZZ_ASSET_LOAD_FLAG_RELOAD flag 
//          4) Unloading can NOT be performed while working threads are using the API 
//          5) Never use asset objects across multiple frames inside worker-threads, 
//             because they may be invalidated
//
// So basically the multi-threading usage pattern is:
//      - Always Load your stuff in main update (main-thread) before running tasks that use those
//        assets
//      - in game-update function: spawn jobs and use `rizz_api_asset.obj()` to access
//        asset objects
//      - always wait/end these tasks before game-update functions ends
//      - unload assets only when the scene is not updated or no game-update tasks is running
// clang-format on
typedef struct rizz_api_asset {
    void (*register_asset_type)(const char* name, rizz_asset_callbacks callbacks,
                                const char* params_type_name, int params_size,
                                rizz_asset_obj failed_obj, rizz_asset_obj async_obj,
                                rizz_asset_load_flags forced_flags);
    void (*unregister_asset_type)(const char* name);
    void (*update_asset_callbacks)(const char* name, rizz_asset_callbacks callbacks);

    rizz_asset (*load)(const char* name, const char* path, const void* params,
                       rizz_asset_load_flags flags, const sx_alloc* alloc, uint32_t tags);
    rizz_asset (*load_from_mem)(const char* name, const char* path_alias, sx_mem_block* mem,
                                const void* params, rizz_asset_load_flags flags,
                                const sx_alloc* alloc, uint32_t tags);
    void (*unload)(rizz_asset handle);

    rizz_asset_state (*state)(rizz_asset handle);
    const char* (*path)(rizz_asset handle);
    const char* (*type_name)(rizz_asset handle);
    const void* (*params)(rizz_asset handle);
    uint32_t (*tags)(rizz_asset handle);
    rizz_asset_obj (*obj)(rizz_asset handle);
    rizz_asset_obj (*obj_unsafe)(rizz_asset handle);

    int (*ref_add)(rizz_asset handle);
    int (*ref_count)(rizz_asset handle);

    void (*reload_by_type)(const char* name);
    int (*gather_by_type)(const char* name, rizz_asset* out_handles, int max_handles);
    void (*unload_by_type)(const char* name);
    void (*reload_by_tags)(uint32_t tags);
    int (*gather_by_tags)(uint32_t tags, rizz_asset* out_handles, int max_handles);
    void (*unload_by_tags)(uint32_t tags);

    rizz_asset_group (*group_begin)(rizz_asset_group group);
    void (*group_end)(rizz_asset_group group);
    void (*group_wait)(rizz_asset_group group);
    bool (*group_loaded)(rizz_asset_group group);
    void (*group_delete)(rizz_asset_group group);
    void (*group_unload)(rizz_asset_group group);
    int (*group_gather)(rizz_asset_group group, rizz_asset* out_handles, int max_handles);
} rizz_api_asset;

////////////////////////////////////////////////////////////////////////////////////////////////////
// @camera
typedef struct rizz_camera {
    sx_vec3 forward;
    sx_vec3 right;
    sx_vec3 up;
    sx_vec3 pos;

    sx_quat quat;
    float ffar;
    float fnear;
    float fov;
    sx_rect viewport;
} rizz_camera;

typedef struct rizz_camera_fps {
    rizz_camera cam;
    float pitch;
    float yaw;
} rizz_camera_fps;

typedef enum rizz_camera_view_plane {
    RIZZ_CAMERA_VIEWPLANE_LEFT = 0,
    RIZZ_CAMERA_VIEWPLANE_RIGHT, 
    RIZZ_CAMERA_VIEWPLANE_TOP,
    RIZZ_CAMERA_VIEWPLANE_BOTTOM,
    RIZZ_CAMERA_VIEWPLANE_NEAR,
    RIZZ_CAMERA_VIEWPLANE_FAR,
    _RIZZ_CAMERA_VIEWPLANE_COUNT
} rizz_camera_view_plane;

typedef struct rizz_api_camera {
    void (*init)(rizz_camera* cam, float fov_deg, sx_rect viewport, float fnear, float ffar);
    void (*lookat)(rizz_camera* cam, sx_vec3 pos, sx_vec3 target, sx_vec3 up);
    void (*location)(rizz_camera* cam, sx_vec3 pos, sx_quat rot);
    void (*ortho_mat)(const rizz_camera* cam, sx_mat4* proj);
    void (*perspective_mat)(const rizz_camera* cam, sx_mat4* proj);
    void (*view_mat)(const rizz_camera* cam, sx_mat4* view);
    void (*calc_frustum_points)(const rizz_camera* cam, sx_vec3 frustum[8]);
    void (*calc_frustum_points_range)(const rizz_camera* cam, sx_vec3 frustum[8], float fnear, float ffar);
    void (*calc_frustum_planes)(sx_plane frustum[_RIZZ_CAMERA_VIEWPLANE_COUNT], const sx_mat4* viewproj_mat);
    void (*fps_init)(rizz_camera_fps* cam, float fov_deg, sx_rect viewport, float fnear, float ffar);
    void (*fps_lookat)(rizz_camera_fps* cam, sx_vec3 pos, sx_vec3 target, sx_vec3 up);
    void (*fps_pitch)(rizz_camera_fps* cam, float pitch);
    void (*fps_pitch_range)(rizz_camera_fps* cam, float pitch, float _min, float _max);
    void (*fps_yaw)(rizz_camera_fps* cam, float yaw);
    void (*fps_forward)(rizz_camera_fps* cam, float forward);
    void (*fps_strafe)(rizz_camera_fps* cam, float strafe);
} rizz_api_camera;

////////////////////////////////////////////////////////////////////////////////////////////////////
// @core
#define RIZZ_MAX_TEMP_ALLOCS 64

enum rizz_app_flags_ {
    RIZZ_APP_FLAG_HIGHDPI = 0x01,
    RIZZ_APP_FLAG_FULLSCREEN = 0x02,
    RIZZ_APP_FLAG_ALPHA = 0x04,
    RIZZ_APP_FLAG_PREMULTIPLIED_ALPHA = 0x08,
    RIZZ_APP_FLAG_PRESERVE_DRAWING_BUFFER = 0x10,
    RIZZ_APP_FLAG_HTML5_CANVAS_RESIZE = 0x20,
    RIZZ_APP_FLAG_IOS_KEYBOARD_RESIZES_CANVAS = 0x40,
    RIZZ_APP_FLAG_USER_CURSOR = 0x80,           // manage cursor image in RIZZ_APP_EVENTTYPE_UPDATE_CURSOR event
    RIZZ_APP_FLAG_FORCE_GLES2 = 0x100,
    RIZZ_APP_FLAG_CRASH_DUMP = 0x200,           // creates crash dump on program exceptions
    RIZZ_APP_FLAG_RESUME_ICONIFIED = 0x400      // do not pause the engine when it's iconified
};
typedef uint32_t rizz_app_flags;

enum rizz_core_flags_ {
    RIZZ_CORE_FLAG_LOG_TO_FILE = 0x01,          // log to file defined by `app_name.log`
    RIZZ_CORE_FLAG_LOG_TO_PROFILER = 0x02,      // log to remote profiler
    RIZZ_CORE_FLAG_PROFILE_GPU = 0x04,          // enable GPU profiling
    RIZZ_CORE_FLAG_DUMP_UNUSED_ASSETS = 0x08,   // write `unused-assets.json` on exit
    RIZZ_CORE_FLAG_DETECT_LEAKS = 0x10,         // Detect memory leaks (default on in _DEBUG builds)
    RIZZ_CORE_FLAG_HEAP_TEMP_ALLOCATOR = 0x20,  // Replace temp allocator backends with heap, so we can better trace out-of-bounds and corruption
    RIZZ_CORE_FLAG_HOT_RELOAD_PLUGINS = 0x40,   // Enables hot reloading for all modules and plugins including the game itself
    RIZZ_CORE_FLAG_TRACE_TEMP_ALLOCATOR = 0x80  // Enable memory tracing on temp allocators, slows them down, but provides more insight on temp allocations
};
typedef uint32_t rizz_core_flags;

// logging backend entry type
typedef enum rizz_log_level {
    RIZZ_LOG_LEVEL_ERROR = 0,
    RIZZ_LOG_LEVEL_WARNING,
    RIZZ_LOG_LEVEL_INFO,
    RIZZ_LOG_LEVEL_VERBOSE,
    RIZZ_LOG_LEVEL_DEBUG,
    _RIZZ_LOG_LEVEL_COUNT
} rizz_log_level;

enum rizz_mem_options_ {
    RIZZ_MEMOPTION_TRACE_CALLSTACK  = 0x1,  // Stores callstacks per allocation call
    RIZZ_MEMOPTION_INSERT_CANARIES  = 0x4,  // inserts canaries for out of boundary detection
    RIZZ_MEMOPTION_MULTITHREAD      = 0x8,  // allocation calls can be called from multiple threads
    RIZZ_MEMOPTION_ALL              = 0xf   // all options above
};
typedef uint32_t rizz_mem_options;

#define RIZZ_MEMOPTION_INHERIT 0xffffffff

// main app/game config
typedef struct rizz_config {
    const char* app_name;
    const char* app_title;
    const char* plugin_path;
    const char* cache_path;
    const char* cwd;
    uint32_t app_version;
    rizz_app_flags app_flags;
    rizz_core_flags core_flags;
    rizz_log_level log_level;   // default = RIZZ_LOG_LEVEL_INFO 

    const char* plugins[RIZZ_CONFIG_MAX_PLUGINS];
    const char* _dummy;     // this is always initialized to 0, so we can count plugins array

    int window_width;
    int window_height;
    int multisample_count;          // MSAA: (0, 2, 4, 8, 16)
    int swap_interval;              // default = 1, set RIZZ_APP_SWAP_INTERVAL_NOSYNC to disable vsync
    int texture_first_mip;          // default = 0, first texture mip to load from textures, higher values lowers overall texture quality and improves performance
    sg_filter texture_filter_min;   // default = SG_FILTER_LINEAR_MIP_LINEAR
    sg_filter texture_filter_mag;   // default = SG_FILTER_LINEAR
    int texture_aniso;              // default = 0, texture anisotropy quality              

    rizz_app_event_cb* event_cb;

    int job_num_threads;    // number of worker threads (default:-1, then it will be num_cores-1)
    int job_max_fibers;     // maximum active jobs at a time (default = 64)
    int job_stack_size;     // jobs stack size, in kbytes (default = 1mb)

    int coro_num_init_fibers;  // number of fibers initialized for coroutines. (default = 64)
    int coro_stack_size;       // coroutine stack size (default = 2mb). in kbytes

    int tmp_mem_max;        // per-frame temp memory size. in kbytes (default: 10mb per-thread)

    int profiler_listen_port;           // default: 17815
    int profiler_update_interval_ms;    // default: 10ms

    bool imgui_docking;     // Enable imgui docking. (also see rizz_api_imgui_extra.dock_space_id)
} rizz_config;

typedef void(rizz_register_cmdline_arg_cb)(const char* name, char short_name,
                                           rizz_cmdline_argtype type, const char* desc,
                                           const char* value_desc);

// Game plugins should implement this function (name should be "rizz_game_config")
// It is called by engine to fetch configuration before initializing the app
// The contents of conf is also set to defaults before submitting to this callback
// Note: use register_cmdline_arg (see above decleration) to add command line arguements to your app
typedef void(rizz_game_config_cb)(rizz_config* conf, rizz_register_cmdline_arg_cb* register_cmdline_arg);

#if SX_PLATFORM_ANDROID
typedef struct ANativeActivity ANativeActivity;
RIZZ_API void ANativeActivity_onCreate_(ANativeActivity*, void*, size_t);

#    define rizz__app_android_decl()                                               \
        __attribute__((visibility("default"))) void ANativeActivity_onCreate(      \
            ANativeActivity* activity, void* saved_state, size_t saved_state_size) \
        {                                                                          \
            ANativeActivity_onCreate_(activity, saved_state, saved_state_size);    \
        }
#else
#    define rizz__app_android_decl()
#endif

#define rizz_game_decl_config(_conf_param_name)                        \
    rizz__app_android_decl() RIZZ_PLUGIN_EXPORT void rizz_game_config( \
        rizz_config* _conf_param_name, rizz_register_cmdline_arg_cb* register_cmdline_arg)

// use this macro instead of rizz_game_decl_config, to declare config with custom command-line params function argument
#define rizz_game_decl_config_cmdline(_conf_param_name, _register_cmdline_arg_fn)   \
    rizz__app_android_decl() RIZZ_PLUGIN_EXPORT void rizz_game_config(              \
        rizz_config* _conf_param_name, rizz_register_cmdline_arg_cb* _register_cmdline_arg_fn)


// custom console commands that can be registered (sent via profiler)
// return >= 0 for success and -1 for failure
typedef int(rizz_core_cmd_cb)(int argc, char* argv[], void* user);

typedef struct rizz_log_entry {
    rizz_log_level type;
    uint32_t channels;
    int text_len;
    int source_file_len;
    const char* text;
    const char* source_file;
    int line;
} rizz_log_entry;

typedef struct rizz_version {
    int major;
    int minor;
    char git[32];
} rizz_version;

typedef enum rizz_profile_flag {
    RIZZ_PROFILE_FLAG_AGGREGATE = 1, // Search parent for same-named samples and merge timing instead of adding a new sample
    RIZZ_PROFILE_FLAG_RECURSIVE = 2, // Merge sample with parent if it's the same sample
} rizz_profile_flag;
typedef uint32_t rizz_profile_flags;

typedef struct rizz_api_core {
    // Thread-safe tracking allocator, this is the recommended allocator to use outside of core
    const sx_alloc* (*alloc)(void);
    // heap allocator: thread-safe, lowest-level allocator. allocates dynamically from heap (libc->malloc)
    const sx_alloc* (*heap_alloc)(void);

    // temp stack allocator: fast and thread-safe (per job thread only).
    // NOTE: do not keep tmp_alloc memory between multiple frames, cuz at the end of each frame, the tmp_allocs are reset
    // stack-mode API: Temp allocators behave like stack, so they can push() and pop() memory offsets
    //                 useful for allocating big chunks and also save memory between frames
    //          Note:  Not recommended to use another allocator with arbitary source within push/pop block
    const sx_alloc* (*tmp_alloc_push)(void);
    void (*tmp_alloc_pop)(void);
    const sx_alloc* (*tmp_alloc_push_trace)(const char* file, uint32_t line);

    // TLS functions are used for setting TLS variables to worker threads by an external source
    // register: use name to identify the variable (Id). (not thread-safe)
    // tls_var: gets pointer to variable, (thread-safe)
    //          `init_cb` will be called on the thread if it's the first time the variable is
    //          is fetched. you should initialize the variable and return it's pointer
    //          destroying tls variables are up to the user, after variable is destroyed, the return
    //          value of tls_var may be invalid
    void (*tls_register)(const char* name, void* user,
                         void* (*init_cb)(int thread_idx, uint32_t thread_id, void* user));
    void* (*tls_var)(const char* name);

    sx_alloc* (*trace_alloc_create)(const char* name, rizz_mem_options mem_opts, const char* parent, const sx_alloc* alloc);
    void (*trace_alloc_destroy)(sx_alloc* alloc);
    void (*trace_alloc_clear)(sx_alloc* alloc);
    void (*trace_alloc_capture_frame)(void);

    rizz_version (*version)(void);

    uint64_t (*delta_tick)(void);
    uint64_t (*elapsed_tick)(void);
    float (*delta_time)(void);
    float (*fps)(void);
    float (*fps_mean)(void);
    int64_t (*frame_index)(void);
    void (*pause)(void);
    void (*resume)(void);
    bool (*is_paused)(void);

    void (*set_cache_dir)(const char* path);
    const char* (*cache_dir)(void);
    const char* (*data_dir)(void);

    const char* (*str_alloc)(uint32_t* phandle, const char* fmt, ...);
    void        (*str_free)(uint32_t handle);
    const char* (*str_cstr)(uint32_t handle);

    // threads
    rizz_thread* (*thread_create)(int (*thread_fn)(void* user_data), void* user_data, const char* debug_name);
    int (*thread_destroy)(rizz_thread* thrd);

    // jobs
    sx_job_t (*job_dispatch)(int count,
                             void (*callback)(int start, int end, int thrd_index, void* user),
                             void* user, sx_job_priority priority, uint32_t tags);
    void (*job_wait_and_del)(sx_job_t job);
    bool (*job_test_and_del)(sx_job_t job);
    int (*job_num_threads)(void);
    int (*job_thread_index)(void);

    void (*coro_invoke)(void (*coro_cb)(sx_fiber_transfer), void* user);
    void (*coro_end)(void* pfrom);
    void (*coro_wait)(void* pfrom, int msecs);
    void (*coro_yield)(void* pfrom, int nframes);

    void (*register_log_backend)(const char* name,
                                 void (*log_cb)(const rizz_log_entry* entry, void* user), void* user);
    void (*unregister_log_backend)(const char* name);

    // use rizz_log_xxxx macros instead of these
    void (*print_info)(uint32_t channels, const char* source_file, int line, const char* fmt, ...);
    void (*print_debug)(uint32_t channels, const char* source_file, int line, const char* fmt, ...);
    void (*print_verbose)(uint32_t channels, const char* source_file, int line, const char* fmt, ...);
    void (*print_error)(uint32_t channels, const char* source_file, int line, const char* fmt, ...);
    void (*print_warning)(uint32_t channels, const char* source_file, int line, const char* fmt, ...);
    void (*set_log_level)(rizz_log_level level);

    void (*begin_profile_sample)(const char* name, rizz_profile_flags flags, uint32_t* hash_cache);
    void (*end_profile_sample)(void);

    rizz_profile_capture (*profile_capture_create)(const char* filename);
    void (*profile_capture_end)(rizz_profile_capture tp);
    void (*profile_capture_sample_begin)(rizz_profile_capture tp, const char* name, const char* file, uint32_t line);
    void (*profile_capture_sample_end)(rizz_profile_capture tp);
    rizz_profile_capture (*profile_capture_startup)(void);

    void (*register_console_command)(const char* cmd, rizz_core_cmd_cb* callback, const char* shortcut, void* user);
    void (*execute_console_command)(const char* cmd_and_args);

    // debugging
    void (*show_graphics_debugger)(bool* p_open);
    void (*show_memory_debugger)(bool* p_open);
    void (*show_log)(bool* p_open);
} rizz_api_core;

#define rizz_log_info(_text, ...)     (RIZZ_CORE_API_VARNAME)->print_info(0, __FILE__, __LINE__, _text, ##__VA_ARGS__)
#define rizz_log_debug(_text, ...)    (RIZZ_CORE_API_VARNAME)->print_debug(0, __FILE__, __LINE__, _text, ##__VA_ARGS__)
#define rizz_log_verbose(_text, ...)  (RIZZ_CORE_API_VARNAME)->print_verbose(0, __FILE__, __LINE__, _text, ##__VA_ARGS__)
#define rizz_log_error(_text, ...)    (RIZZ_CORE_API_VARNAME)->print_error(0, __FILE__, __LINE__, _text, ##__VA_ARGS__)
#define rizz_log_warn(_text, ...)     (RIZZ_CORE_API_VARNAME)->print_warning(0, __FILE__, __LINE__, _text, ##__VA_ARGS__)

#define rizz_log_info_channels(_channels, _text, ...)     (RIZZ_CORE_API_VARNAME)->print_info((_channels), __FILE__, __LINE__, _text, ##__VA_ARGS__)
#define rizz_log_debug_channels(_channels, _text, ...)    (RIZZ_CORE_API_VARNAME)->print_debug((_channels), __FILE__, __LINE__, _text, ##__VA_ARGS__)
#define rizz_log_verbose_channels(_channels, _text, ...)  (RIZZ_CORE_API_VARNAME)->print_verbose((_channels), __FILE__, __LINE__, _text, ##__VA_ARGS__)
#define rizz_log_error_channels(_channels, _text, ...)    (RIZZ_CORE_API_VARNAME)->print_error((_channels), __FILE__, __LINE__, _text, ##__VA_ARGS__)
#define rizz_log_warn_channels(_channels, _text, ...)     (RIZZ_CORE_API_VARNAME)->print_warning((_channels), __FILE__, __LINE__, _text, ##__VA_ARGS__)

// coroutines
#ifdef __cplusplus
#    define rizz_coro_declare(_name)     \
            RIZZ_POINTER static auto coro__##_name = [](sx_fiber_transfer __transfer)
#else
#    define rizz_coro_declare(_name)     \
            static void coro__##_name_(sx_fiber_transfer __transfer);   \
            RIZZ_POINTER static void (*coro__##_name)(sx_fiber_transfer __transfer) = coro__##_name_; \
            static void coro__##_name_(sx_fiber_transfer __transfer)
#endif

#define rizz_coro_userdata()             __transfer.user
#define rizz_coro_wait(_msecs)           (RIZZ_CORE_API_VARNAME)->coro_wait(&__transfer.from, (_msecs))
#define rizz_coro_yield()                (RIZZ_CORE_API_VARNAME)->coro_yield(&__transfer.from, 1)
#define rizz_coro_yieldn(_n)             (RIZZ_CORE_API_VARNAME)->coro_yield(&__transfer.from, (_n))
#define rizz_coro_end()                  (RIZZ_CORE_API_VARNAME)->coro_end(&__transfer.from)
#define rizz_coro_invoke(_name, _user)   (RIZZ_CORE_API_VARNAME)->coro_invoke(coro__##_name, (_user))

// using these macros are preferred to begin_profile_sample() and end_profile_sample()
// because They provide cache variables for name hashing and also somewhat emulates C++ RAII
#define rizz_profile_begin(_name, _flags) \
        static uint32_t rmt_sample_hash_##_name = 0; \
        uint32_t rmt_sample_raii_##_name; \
        (RIZZ_CORE_API_VARNAME)->begin_profile_sample(#_name, _flags, &rmt_sample_hash_##_name)
#define rizz_profile_end(_name)               \
        (void)rmt_sample_raii_##_name;   \
        (RIZZ_CORE_API_VARNAME)->end_profile_sample();

// using these macros are preferred over profile_capture_sample_begin() and profile_capture_sample_end()
// becuase they shorten the syntax, and pass __FILE__, __LINE__ by defult
#define rizz_profile_capture_begin(_cid, _name) \
        (RIZZ_CORE_API_VARNAME)->profile_capture_sample_begin(_cid, sx_stringize(_name), __FILE__, __LINE__)
#define rizz_profile_capture_end(_cid, _name) (RIZZ_CORE_API_VARNAME)->profile_capture_sample_end(_cid)            

// usage pattern:
// rizz_profile(name) {
//  ...
// } // automatically ends profile sample
#define rizz_profile(_name) static uint32_t sx_concat(rmt_sample_hash_, _name) = 0; \
        sx_defer((RIZZ_CORE_API_VARNAME)->begin_profile_sample(sx_stringize(_name), 0, &sx_concat(rmt_sample_hash_, _name)), \
                 (RIZZ_CORE_API_VARNAME)->end_profile_sample())

#define rizz_profile_capture_sample(_cid, _name) \
            sx_defer((RIZZ_CORE_API_VARNAME)->profile_capture_sample_begin(_cid, _name, __FILE__, __LINE__), \
                     (RIZZ_CORE_API_VARNAME)->profile_capture_sample_end(_cid))

// use these to profile startup times and inits 
#define rizz_profile_startup_begin(_name)   (RIZZ_CORE_API_VARNAME)->profile_capture_sample_begin((RIZZ_CORE_API_VARNAME)->profile_capture_startup(), _name, __FILE__, __LINE__)
#define rizz_profile_startup_end()          (RIZZ_CORE_API_VARNAME)->profile_capture_sample_end((RIZZ_CORE_API_VARNAME)->profile_capture_startup())

#define rizz_with_temp_alloc(_name) sx_with(const sx_alloc* _name = (RIZZ_CORE_API_VARNAME)->tmp_alloc_push(), \
                                            (RIZZ_CORE_API_VARNAME)->tmp_alloc_pop()) 

////////////////////////////////////////////////////////////////////////////////////////////////////
// @graphics
// _sg-types.h is copy/pasted from sokol_gfx.h, all the types are equal between these two files

#define _rizz_concat_path_3(s1, s2, s3) sx_stringize(s1/s2/s3)
#define _rizz_shader_path_lang(_basepath, _lang, _filename) _rizz_concat_path_3(_basepath, _lang, _filename)

// HELP: this macro can be used as a helper in including cross-platform shaders from source
//       appends _api_ (hlsl/glsl/gles/msl) between `_basepath` and `_filename`
//       only use this in including header shaders. example:
//       #include rizz_shader_path(shaders/include, myshader.h)
#define rizz_shader_path(_basepath, _filename) _rizz_shader_path_lang(_basepath, RIZZ_GRAPHICS_SHADER_LANG, _filename)

typedef enum rizz_gfx_backend {
    RIZZ_GFX_BACKEND_GLCORE33,
    RIZZ_GFX_BACKEND_GLES2,
    RIZZ_GFX_BACKEND_GLES3,
    RIZZ_GFX_BACKEND_D3D11,
    RIZZ_GFX_BACKEND_METAL_IOS,
    RIZZ_GFX_BACKEND_METAL_MACOS,
    RIZZ_GFX_BACKEND_METAL_SIMULATOR,
    RIZZ_GFX_BACKEND_DUMMY,
} rizz_gfx_backend;

typedef enum rizz_shader_lang {
    RIZZ_SHADER_LANG_GLES,
    RIZZ_SHADER_LANG_HLSL,
    RIZZ_SHADER_LANG_MSL,
    RIZZ_SHADER_LANG_GLSL,
    _RIZZ_SHADER_LANG_COUNT
} rizz_shader_lang;

typedef enum rizz_shader_stage {
    RIZZ_SHADER_STAGE_VS,
    RIZZ_SHADER_STAGE_FS,
    RIZZ_SHADER_STAGE_CS,
    _RIZZ_SHADER_STAGE_COUNT
} rizz_shader_stage;

typedef enum rizz_shader_code_type {
    RIZZ_SHADER_CODE_SOURCE,
    RIZZ_SHADER_CODE_BYTECODE
} rizz_shader_code_type;

typedef struct rizz_shader_refl_input {
    char name[32];
    char semantic[32];
    int semantic_index;
    sg_vertex_format type;
} rizz_shader_refl_input;

typedef struct rizz_shader_refl_uniform_buffer {
    char name[32];
    int size_bytes;
    int binding;
    int array_size;    // for flattened ubos, we must provide array_size to the api with the type
                       // FLOAT4
} rizz_shader_refl_uniform_buffer;

typedef struct rizz_shader_refl_buffer {
    char name[32];
    int size_bytes;
    int binding;
    int array_stride;
} rizz_shader_refl_buffer;

typedef struct rizz_shader_refl_texture {
    char name[32];
    int binding;
    sg_image_type type;
} rizz_shader_refl_texture;

typedef struct rizz_shader_refl {
    rizz_shader_lang lang;
    rizz_shader_stage stage;
    int profile_version;
    char source_file[32];
    rizz_shader_refl_input* inputs;
    int num_inputs;
    rizz_shader_refl_texture* textures;
    int num_textures;
    rizz_shader_refl_texture* storage_images;
    int num_storage_images;
    rizz_shader_refl_buffer* storage_buffers;
    int num_storage_buffers;
    rizz_shader_refl_uniform_buffer* uniform_buffers;
    int num_uniform_buffers;
    rizz_shader_code_type code_type;
    bool flatten_ubos;
} rizz_shader_refl;

// shader metadata
typedef struct rizz_shader_info {
    rizz_shader_refl_input inputs[SG_MAX_VERTEX_ATTRIBUTES];
    int num_inputs;
    uint32_t name_hdl;
} rizz_shader_info;

typedef struct rizz_vertex_attr {
    const char* semantic;
    int semantic_idx;
    int offset;
    sg_vertex_format format;
    int buffer_index;
} rizz_vertex_attr;

typedef struct rizz_vertex_layout {
    rizz_vertex_attr attrs[SG_MAX_VERTEX_ATTRIBUTES];
} rizz_vertex_layout;

typedef struct rizz_shader {
    sg_shader shd;
    rizz_shader_info info;
} rizz_shader;

typedef struct rizz_texture_load_params {
    int first_mip;
    sg_filter min_filter;
    sg_filter mag_filter;
    sg_wrap wrap_u;
    sg_wrap wrap_v;
    sg_wrap wrap_w;
    sg_pixel_format fmt;    // request image format. only valid for basis files
    int aniso;
    bool srgb;
} rizz_texture_load_params;

// texture metadata
typedef struct rizz_texture_info {
    uint32_t name_hdl;      // get name with the_core->str_cstr();
    sg_image_type type;
    sg_pixel_format format;
    int mem_size_bytes;
    int width;
    int height;
    union {
        int depth;
        int layers;
    };
    int mips;
    int bpp;
} rizz_texture_info;

typedef struct rizz_texture {
    sg_image img;
    rizz_texture_info info;
} rizz_texture;

// per-frame stats
typedef enum rizz_gfx_perframe_trace_zone {
    RIZZ_GFX_TRACE_COMMON = 0,
    RIZZ_GFX_TRACE_IMGUI,
    _RIZZ_GFX_TRACE_COUNT
} rizz_gfx_perframe_trace_zone;

typedef struct rizz_gfx_perframe_trace_info {
    int num_draws;
    int num_instances;
    int num_apply_pipelines;
    int num_apply_passes;
    int num_elements;
} rizz_gfx_perframe_trace_info;

typedef struct rizz_gfx_trace_info {
    // per-frame stats
    rizz_gfx_perframe_trace_info pf[_RIZZ_GFX_TRACE_COUNT];

    // persistent stats
    int num_pipelines;
    int num_shaders;
    int num_passes;
    int num_images;
    int num_buffers;

    int64_t texture_size;
    int64_t texture_peak;

    int64_t buffer_size;
    int64_t buffer_peak;

    int64_t render_target_size;
    int64_t render_target_peak;
    // end: persistent stats
} rizz_gfx_trace_info;

typedef struct sjson_context sjson_context;    // shader_parse_reflection

// There are two kinds of drawing APIs:
//
// Immediate API: access directly to GPU graphics API. this is actually a thin wrapper over backend
//                Calls are executed immediately and sequentially, unlike _staged_ API (see below)
//                Note: this API is NOT multi-threaded
//                Immediate mode is more of a direct and immediate graphics API
//                It is recommended to use staged API mentioned below,
//
// Staged API:   staged (deferred calls), multi-threaded API.
//               Contains only a selection of drawing functions
//               Can be called within worker threads spawned by `job_dispatch`, but with some rules
//               and restrictions
//
// Usage: always call begin_stage first, execute commands, then call end_stage
//        At the end of the frame step, all commands buffers will be merged and executed by the
//        rendering stages. Also, stages must be registered and setup before using staged
//        functions. (see below)
//
// Rule #1: in a worker threads, always end_stage before spawning and waiting for
//          another job because the command-buffer may change on thread-switch and drawing will be
//          messed up
//          Example of misuse:
//              { // dispatched job (worker-thread)
//                  job_dispatch(..);
//                  begin_stage(..)
//                  begin_pass(..);
//                  apply_viewport(..);
//                  job_wait_and_del(..)    // DON'T DO THIS: after wait, cmd-buffer will be changed
//                                          // instead, wait after end_stage call
//                  draw(..)
//                  end_stage(..)
//              }
//
// Rule #2: Do not destroy graphics objects (buffers/shaders/textures) during rendering work
//          This is actually like the multi-threaded asset usage pattern (see asset.h)
//          You should only destroy graphics objects when they are not being rendered or used
//
// Rule #3: The commands will be submitted to GPU at the end of the frame update automatically
//          But, you can use `presend_commands` and `commit_commands` to submit commands early
//          and prevent the GPU driver from doing too much work at the end. see below:
//
// Common multi-threaded usage pattern is like as follows:
//
// [thread #1]   ->             |---draw---|                       |---draw---|
// [thread #2]   ->             |---draw---|                       |---draw---|
// [thread #3]   ->             |---draw---|                       |---draw---|
// [game update] -> ----dispatch+---draw---|wait+present---dispatch+commit----|wait---draw--- <- end
//                                                  |                    |                         |
//  present called when no drawing is being done <-/                     |                         |
//       commit called during drawing (main thread) but after present <-/                          |
//    when frame is done, the framework will automatically execute and flush remaining commands <-/
//
typedef struct rizz_api_gfx_draw {
    bool (*begin)(rizz_gfx_stage stage);
    void (*end)(void);

    void (*begin_default_pass)(const sg_pass_action* pass_action, int width, int height);
    void (*begin_pass)(sg_pass pass, const sg_pass_action* pass_action);
    void (*apply_viewport)(int x, int y, int width, int height, bool origin_top_left);
    void (*apply_scissor_rect)(int x, int y, int width, int height, bool origin_top_left);
    void (*apply_pipeline)(sg_pipeline pip);
    void (*apply_bindings)(const sg_bindings* bind);
    void (*apply_uniforms)(sg_shader_stage stage, int ub_index, const void* data, int num_bytes);
    void (*draw)(int base_element, int num_elements, int num_instances);
    void (*dispatch)(int thread_group_x, int thread_group_y, int thread_group_z);
    void (*end_pass)(void);
    void (*update_buffer)(sg_buffer buf, const void* data_ptr, int data_size);
    int (*append_buffer)(sg_buffer buf, const void* data_ptr, int data_size);
    void (*update_image)(sg_image img, const sg_image_content* data);

    // profile
    void (*begin_profile_sample)(const char* name, uint32_t* hash_cache);
    void (*end_profile_sample)(void);

    // debug version of calls (see macros below)
    void (*begin_default_pass_d)(const sg_pass_action* pass_action, int width, int height, const char* file, uint32_t line);
    void (*begin_pass_d)(sg_pass pass, const sg_pass_action* pass_action, const char* file, uint32_t line);
    void (*apply_viewport_d)(int x, int y, int width, int height, bool origin_top_left, const char* file, uint32_t line);
    void (*apply_scissor_rect_d)(int x, int y, int width, int height, bool origin_top_left, const char* file, uint32_t line);
    void (*apply_pipeline_d)(sg_pipeline pip, const char* file, uint32_t line);
    void (*apply_bindings_d)(const sg_bindings* bind, const char* file, uint32_t line);
    void (*apply_uniforms_d)(sg_shader_stage stage, int ub_index, const void* data, int num_bytes, const char* file, uint32_t line);
    void (*draw_d)(int base_element, int num_elements, int num_instances, const char* file, uint32_t line);
    void (*dispatch_d)(int thread_group_x, int thread_group_y, int thread_group_z, const char* file, uint32_t line);
    void (*end_pass_d)(const char* file, uint32_t line);
    void (*update_buffer_d)(sg_buffer buf, const void* data_ptr, int data_size, const char* file, uint32_t line);
    int (*append_buffer_d)(sg_buffer buf, const void* data_ptr, int data_size, const char* file, uint32_t line);
    void (*update_image_d)(sg_image img, const sg_image_content* data, const char* file, uint32_t line);
} rizz_api_gfx_draw;

#define rizz_gfx_begin(_stage) \
    (RIZZ_GRAPHICS_API_VARNAME)->staged.begin(_stage)
#define rizz_gfx_end() \
    (RIZZ_GRAPHICS_API_VARNAME)->staged.end()
#define rizz_gfx_begin_default_pass(_pass_action, _width, _height) \
    (RIZZ_GRAPHICS_API_VARNAME)->staged.begin_default_pass_d(_pass_action, _width, _height, __FILE__, __LINE__)
#define rizz_gfx_begin_pass(_pass, _pass_action) \
    (RIZZ_GRAPHICS_API_VARNAME)->staged.begin_pass_d(_pass, _pass_action, __FILE__, __LINE__)
#define rizz_gfx_apply_viewport(_x, _y, _width, _height, _origin_top_left) \
    (RIZZ_GRAPHICS_API_VARNAME)->staged.apply_viewport_d(_x, _y, _width, _height, _origin_top_left, __FILE__, __LINE__)
#define rizz_gfx_apply_scissor_rect(_x, _y, _width, _height, _origin_top_left) \
    (RIZZ_GRAPHICS_API_VARNAME)->staged.apply_scissor_rect_d(_x, _y, _width, _height, _origin_top_left, __FILE__, __LINE__)
#define rizz_gfx_apply_pipeline(_pip) \
    (RIZZ_GRAPHICS_API_VARNAME)->staged.apply_pipeline_d(_pip, __FILE__, __LINE__)
#define rizz_gfx_apply_bindings(_bindings) \
    (RIZZ_GRAPHICS_API_VARNAME)->staged.apply_bindings_d(_bindings, __FILE__, __LINE__)
#define rizz_gfx_apply_uniforms(_stage, _ub_index, _data, _bytes) \
    (RIZZ_GRAPHICS_API_VARNAME)->staged.apply_uniforms_d(_stage, _ub_index, _data, _bytes, __FILE__, __LINE__)
#define rizz_gfx_draw(_base_element, _num_elements, _num_instances) \
    (RIZZ_GRAPHICS_API_VARNAME)->staged.draw_d(_base_element, _num_elements, _num_instances, __FILE__, __LINE__)
#define rizz_gfx_dispatch(_thread_group_x, _thread_group_y, _thread_group_z) \
    (RIZZ_GRAPHICS_API_VARNAME)->staged.dispatch_d(_thread_group_x, _thread_group_y, _thread_group_z, __FILE__, __LINE__)
#define rizz_gfx_end_pass() \
    (RIZZ_GRAPHICS_API_VARNAME)->staged.end_pass_d(__FILE__, __LINE__)
#define rizz_gfx_update_buffer(_buf, _data, _size) \
    (RIZZ_GRAPHICS_API_VARNAME)->staged.update_buffer_d(_buf, _data, _size, __FILE__, __LINE__)
#define rizz_gfx_append_buffer(_buf, _data, _size) \
    (RIZZ_GRAPHICS_API_VARNAME)->staged.append_buffer_d(_buf, _data, _size, __FILE__, __LINE__)
#define rizz_gfx_update_image(_img, _data, _size) \
    (RIZZ_GRAPHICS_API_VARNAME)->staged.update_image_d(_img, _data, _size, __FILE__, __LINE__)

// using these macros are preferred to draw_api->begin_profile_sample() and draw_api->end_profile_sample()
// because They provide cache variables for name hashing and also somewhat emulates C++ RAII
#define rizz_gfx_profile_begin(_name)                \
    static uint32_t rmt_gpu_sample_hash_##_name = 0; \
    uint32_t rmt_gpu_sample_raii_##_name;            \
    (RIZZ_GRAPHICS_API_VARNAME)->staged.begin_profile_sample(#_name, &rmt_gpu_sample_hash_##_name)
#define rizz_gfx_profile_end(_name)    \
    (void)rmt_gpu_sample_raii_##_name; \
    (RIZZ_GRAPHICS_API_VARNAME)->staged.end_profile_sample();

typedef struct rizz_api_gfx {
    rizz_api_gfx_draw imm;       // immediate draw API
    rizz_api_gfx_draw staged;    // staged (deferred calls) draw API

    const sx_alloc* (*alloc)(void);
    rizz_gfx_backend (*backend)(void);
    bool (*GL_family)(void);
    bool (*GLES_family)(void);
    void (*reset_state_cache)(void);

    // multi-threading
    // swaps the command buffers, makes previously submitted commands visible to `commit_commands`
    // NOTE: care must be taken when calling this function, first of all, it should be called on
    //       the main thread. And should never be called when rendering jobs are running or
    //       undefined behviour will occur. see documentation for more info.
    void (*present_commands)(void);
    // call this function to submit queued commands to the gpu. it will be also called automatically
    // at the end of the frame. User must call `present_commands` before making this call
    // NOTE: should call this function only on the main thread. see documentation for more info.
    void (*commit_commands)(void);

    // resource creation, destruction and updating
    sg_buffer (*make_buffer)(const sg_buffer_desc* desc);
    sg_image (*make_image)(const sg_image_desc* desc);
    sg_shader (*make_shader)(const sg_shader_desc* desc);
    sg_pipeline (*make_pipeline)(const sg_pipeline_desc* desc);
    sg_pass (*make_pass)(const sg_pass_desc* desc);

    // destroys (destroys are deferred calls, they execute after 1 frame, if object is not used)
    void (*destroy_buffer)(sg_buffer buf);
    void (*destroy_image)(sg_image img);
    void (*destroy_shader)(sg_shader shd);
    void (*destroy_pipeline)(sg_pipeline pip);
    void (*destroy_pass)(sg_pass pass);

    // get resource state (initial, alloc, valid, failed)
    bool (*query_buffer_overflow)(sg_buffer buf);
    sg_resource_state (*query_buffer_state)(sg_buffer buf);
    sg_resource_state (*query_image_state)(sg_image img);
    sg_resource_state (*query_shader_state)(sg_shader shd);
    sg_resource_state (*query_pipeline_state)(sg_pipeline pip);
    sg_resource_state (*query_pass_state)(sg_pass pass);

    // separate resource allocation and initialization (for async setup)
    sg_buffer (*alloc_buffer)(void);
    sg_image (*alloc_image)(void);
    sg_shader (*alloc_shader)(void);
    sg_pipeline (*alloc_pipeline)(void);
    sg_pass (*alloc_pass)(void);

    const char* (*str_alloc)(uint32_t* phandle, const char* fmt, ...);
    void (*rizz__str_free)(uint32_t handle);
    const char* (*rizz__str_cstr)(uint32_t handle);

    // init/fail objects
    void (*init_buffer)(sg_buffer buf_id, const sg_buffer_desc* desc);
    void (*init_image)(sg_image img_id, const sg_image_desc* desc);
    void (*init_shader)(sg_shader shd_id, const sg_shader_desc* desc);
    void (*init_pipeline)(sg_pipeline pip_id, const sg_pipeline_desc* desc);
    void (*init_pass)(sg_pass pass_id, const sg_pass_desc* desc);
    void (*fail_buffer)(sg_buffer buf_id);
    void (*fail_image)(sg_image img_id);
    void (*fail_shader)(sg_shader shd_id);
    void (*fail_pipeline)(sg_pipeline pip_id);
    void (*fail_pass)(sg_pass pass_id);

    // rendering contexts for multi-window rendering (optional)
    sg_context (*setup_context)(void);
    void (*activate_context)(sg_context ctx_id);
    void (*discard_context)(sg_context ctx_id);

    sg_trace_hooks (*install_trace_hooks)(const sg_trace_hooks* trace_hooks);
    sg_desc (*query_desc)(void);
    sg_buffer_info (*query_buffer_info)(sg_buffer buf);
    sg_image_info (*query_image_info)(sg_image img);
    sg_shader_info (*query_shader_info)(sg_shader shd);
    sg_pipeline_info (*query_pipeline_info)(sg_pipeline pip);
    sg_pass_info (*query_pass_info)(sg_pass pass);
    sg_features (*query_features)(void);
    sg_limits (*query_limits)(void);
    sg_pixelformat_info (*query_pixelformat)(sg_pixel_format fmt);
    sg_buffer_desc (*query_buffer_defaults)(const sg_buffer_desc* desc);
    sg_image_desc (*query_image_defaults)(const sg_image_desc* desc);
    sg_shader_desc (*query_shader_defaults)(const sg_shader_desc* desc);
    sg_pipeline_desc (*query_pipeline_defaults)(const sg_pipeline_desc* desc);
    sg_pass_desc (*query_pass_defaults)(const sg_pass_desc* desc);

    // internal use (imgui plugin)
    void (*internal_state)(void** make_cmdbuff, int* make_cmdbuff_sz);

    // Stage:
    //     To performed deferred drawing calls, you should setup rendering stages on application
    //     init: `register_stage` Stages can be any group of drawing calls. For example, drawing a
    //     shadow-map for a light can be a stage Stages can also depend on each other. Multiple
    //     stages can depend on one stage to be finished, like a tree graph When the parent stage is
    //     disabled, all child stages are disabled
    rizz_gfx_stage (*stage_register)(const char* name, rizz_gfx_stage parent_stage);
    void (*stage_enable)(rizz_gfx_stage stage);
    void (*stage_disable)(rizz_gfx_stage stage);
    bool (*stage_isenabled)(rizz_gfx_stage stage);
    rizz_gfx_stage (*stage_find)(const char* name);

    // Shader
    rizz_shader_refl* (*shader_parse_reflection)(const sx_alloc* alloc, const char* stage_refl_json,
                                                 int stage_refl_len);
    void (*shader_free_reflection)(rizz_shader_refl* refl, const sx_alloc* alloc);
    sg_shader_desc* (*shader_setup_desc)(sg_shader_desc* desc, const rizz_shader_refl* vs_refl,
                                         const void* vs, int vs_size,
                                         const rizz_shader_refl* fs_refl, const void* fs,
                                         int fs_size, uint32_t* name_hdl);
    rizz_shader (*shader_make_with_data)(const sx_alloc* alloc, uint32_t vs_data_size,
                                         const uint32_t* vs_data, uint32_t vs_refl_size,
                                         const uint32_t* vs_refl_json, uint32_t fs_data_size,
                                         const uint32_t* fs_data, uint32_t fs_refl_size,
                                         const uint32_t* fs_refl_json);
    sg_pipeline_desc* (*shader_bindto_pipeline)(const rizz_shader* shd, sg_pipeline_desc* pip_desc,
                                                const rizz_vertex_layout* vl);
    sg_pipeline_desc* (*shader_bindto_pipeline_sg)(sg_shader shd,
                                                   const rizz_shader_refl_input* inputs,
                                                   int num_inputs, sg_pipeline_desc* pip_desc,
                                                   const rizz_vertex_layout* vl);
    const rizz_shader* (*shader_get)(rizz_asset shader_asset);

    // texture
    sg_image (*texture_white)(void);
    sg_image (*texture_black)(void);
    sg_image (*texture_checker)(void);
    rizz_texture (*texture_create_checker)(int checker_size, int size, const sx_color colors[2]);
    const rizz_texture* (*texture_get)(rizz_asset texture_asset);
    void (*texture_set_default_quality)(sg_filter min_filter, sg_filter mag_filter, int aniso, int first_mip);
    void (*texture_default_quality)(sg_filter* min_filter, sg_filter* mag_filter, int* aniso, int* first_mip);
    uint32_t (*texture_surface_pitch)(sg_pixel_format fmt, uint32_t width, uint32_t height, uint32_t row_align);

    // info
    const rizz_gfx_trace_info* (*trace_info)(void);
} rizz_api_gfx;

////////////////////////////////////////////////////////////////////////////////////////////////////
// @plugin

#ifndef RIZZ_BUNDLE
// RIZZ_STATE: must put before any static variable that needs to be persistant in plugin reloads
// RIZZ_POINTER: must put before any function pointer that is passed to the engine like coroutines
// Reformatted from 'cr.h'
#    if SX_COMPILER_MSVC
#        pragma section(".state", read, write)
#        define RIZZ_STATE __declspec(allocate(".state"))
#        pragma section(".ptrs", read, write)
#        define RIZZ_POINTER __declspec(allocate(".ptrs"))
#    elif SX_PLATFORM_APPLE
#        define RIZZ_STATE __attribute__((used, section("__DATA,__state")))
//#        define RIZZ_POINTER __attribute__((used, section("__DATA,__ptrs")))  // skipped this because of Regression bug
#        define RIZZ_POINTER 
#    elif SX_COMPILER_GCC || SX_COMPILER_CLANG
#        pragma section(".state", read, write)
#        define RIZZ_STATE __attribute__((section(".state")))
//#        pragma section(".ptrs", read, write)
//#        define RIZZ_POINTER __attribute__((section(".ptrs"))) // may not work on some platforms, see: https://github.com/fungos/cr/issues/32
#        define RIZZ_POINTER
#    endif

// RIZZ_PLUGIN_EXPORT
#    if SX_COMPILER_MSVC
#        ifdef __cplusplus
#            define RIZZ_PLUGIN_EXPORT extern "C" __declspec(dllexport)
#        else
#            define RIZZ_PLUGIN_EXPORT __declspec(dllexport)
#        endif
#    else
#        ifdef __cplusplus
#            define RIZZ_PLUGIN_EXPORT extern "C" __attribute__((visibility("default")))
#        else
#            define RIZZ_PLUGIN_EXPORT __attribute__((visibility("default")))
#        endif
#    endif    // else: SX_COMPILER_MSVC
#else
#    ifdef __cplusplus
#        define RIZZ_PLUGIN_EXPORT extern "C"
#    else
#        define RIZZ_PLUGIN_EXPORT
#    endif
#    define RIZZ_STATE
#    define RIZZ_POINTER
#endif    // RIZZ_BUNDLE

typedef enum rizz_api_type {
    RIZZ_API_CORE = 0,
    RIZZ_API_PLUGIN,
    RIZZ_API_APP,
    RIZZ_API_GFX,
    RIZZ_API_REFLECT,
    RIZZ_API_VFS,
    RIZZ_API_ASSET,
    RIZZ_API_CAMERA,
    RIZZ_API_HTTP,
    _RIZZ_API_COUNT
} rizz_api_type;

typedef enum rizz_plugin_event {
    RIZZ_PLUGIN_EVENT_LOAD = 0,
    RIZZ_PLUGIN_EVENT_STEP = 1,
    RIZZ_PLUGIN_EVENT_UNLOAD = 2,
    RIZZ_PLUGIN_EVENT_SHUTDOWN = 3,
    RIZZ_PLUGIN_EVENT_INIT = 4
} rizz_plugin_event;

typedef enum rizz_plugin_crash {
    RIZZ_PLUGIN_CRASH_NONE = 0,    // No error
    RIZZ_PLUGIN_CRASH_SEGFAULT,    // SIGSEGV / EXCEPTION_ACCESS_VIOLATION
    RIZZ_PLUGIN_CRASH_ILLEGAL,     // illegal instruction (SIGILL) / EXCEPTION_ILLEGAL_INSTRUCTION
    RIZZ_PLUGIN_CRASH_ABORT,       // abort (SIGBRT)
    RIZZ_PLUGIN_CRASH_MISALIGN,    // bus error (SIGBUS) / EXCEPTION_DATATYPE_MISALIGNMENT
    RIZZ_PLUGIN_CRASH_BOUNDS,      // EXCEPTION_ARRAY_BOUNDS_EXCEEDED
    RIZZ_PLUGIN_CRASH_STACKOVERFLOW,        // EXCEPTION_STACK_OVERFLOW
    RIZZ_PLUGIN_CRASH_STATE_INVALIDATED,    // one or more global data sectio changed and does
                                            // not safely match basically a failure of
                                            // cr_plugin_validate_sections
    RIZZ_PLUGIN_CRASH_BAD_IMAGE,            // Binary is not valid
    RIZZ_PLUGIN_CRASH_OTHER,                // Unknown or other signal,
    RIZZ_PLUGIN_CRASH_USER = 0x100,
} rizz_plugin_crash;

enum rizz_plugin_info_flags_ { RIZZ_PLUGIN_INFO_EVENT_HANDLER = 0x1 };
typedef uint32_t rizz_plugin_info_flags;

// Plugins should implement these functions (names should be the same without the _cb)
// use rizz_plugin_decl macros to declare the proper function signatures based on build type
// (bundle/dynamic)
typedef struct rizz_plugin rizz_plugin;
typedef struct rizz_plugin_info rizz_plugin_info;
typedef int(rizz_plugin_main_cb)(rizz_plugin* ctx, rizz_plugin_event e);
typedef void(rizz_plugin_get_info_cb)(rizz_plugin_info* out_info);
typedef void(rizz_plugin_event_handler_cb)(const rizz_app_event* ev);

typedef struct rizz_plugin_info {
    uint32_t version;
    const char** deps;    // array: name of dependency plugins
    int num_deps;
    char name[32];
    char desc[256];

#ifdef RIZZ_BUNDLE
    // These callback functions are automatically assigned by auto-generated script (see
    // bundle.cmake)
    rizz_plugin_main_cb* main_cb;
    rizz_plugin_event_handler_cb* event_cb;
#endif
} rizz_plugin_info;

typedef struct rizz_api_plugin {
    bool (*load)(const char* name);
    void (*inject_api)(const char* name, uint32_t version, void* api);
    void (*remove_api)(const char* name, uint32_t version);
    void* (*get_api)(rizz_api_type api, uint32_t version);
    void* (*get_api_byname)(const char* name, uint32_t version);
    const char* (*crash_reason)(rizz_plugin_crash crash);
} rizz_api_plugin;

// Data layout is same as 'cr_plugin' but with different variable names to make it more
// user-friendly
typedef struct rizz_plugin {
    void* _p;
    rizz_api_plugin* api;
    uint32_t iteration;    // What reload we are on, first load is 1
    rizz_plugin_crash crash_reason;
} rizz_plugin;

#ifndef RIZZ_BUNDLE
#    define rizz_plugin_decl_main(_name, _plugin_param_name, _event_param_name)  \
        RIZZ_PLUGIN_EXPORT int rizz_plugin_main(rizz_plugin* _plugin_param_name, \
                                                rizz_plugin_event _event_param_name)

#    define rizz_plugin_decl_event_handler(_name, __event_param_name) \
        RIZZ_PLUGIN_EXPORT void rizz_plugin_event_handler(const rizz_app_event* __event_param_name)

#    define rizz_plugin_implement_info(_name, _version, _desc, _deps, _num_deps) \
        RIZZ_PLUGIN_EXPORT void rizz_plugin_get_info(rizz_plugin_info* out_info) \
        {                                                                        \
            out_info->version = (_version);                                      \
            out_info->deps = (_deps);                                            \
            out_info->num_deps = (_num_deps);                                    \
            sx_strcpy(out_info->name, sizeof(out_info->name), #_name);           \
            sx_strcpy(out_info->desc, sizeof(out_info->desc), (_desc));          \
        }
#else
#    define rizz_plugin_decl_main(_name, _plugin_param_name, _event_param_name)          \
        RIZZ_PLUGIN_EXPORT int rizz_plugin_main_##_name(rizz_plugin* _plugin_param_name, \
                                                        rizz_plugin_event _event_param_name)
#    define rizz_plugin_decl_event_handler(_name, __event_param_name) \
        RIZZ_PLUGIN_EXPORT void rizz_plugin_event_handler_##_name(    \
            const rizz_app_event* __event_param_name)

#    define rizz_plugin_implement_info(_name, _version, _desc, _deps, _num_deps)         \
        RIZZ_PLUGIN_EXPORT void rizz_plugin_get_info_##_name(rizz_plugin_info* out_info) \
        {                                                                                \
            out_info->version = (_version);                                              \
            out_info->deps = (_deps);                                                    \
            out_info->num_deps = (_num_deps);                                            \
            sx_strcpy(out_info->name, sizeof(out_info->name), #_name);                   \
            sx_strcpy(out_info->desc, sizeof(out_info->desc), (_desc));                  \
        }
#endif    // RIZZ_BUNDLE


////////////////////////////////////////////////////////////////////////////////////////////////////
// @http
typedef enum rizz_http_status {
    RIZZ_HTTP_PENDING = 0,
    RIZZ_HTTP_COMPLETED,
    RIZZ_HTTP_FAILED
} rizz_http_status;

typedef struct rizz_http_state {
    rizz_http_status status;
    int status_code;
    char const* reason_phrase;
    char const* content_type;
    size_t response_size;
    void* response_data;
} rizz_http_state;

typedef void(rizz_http_cb)(const rizz_http_state* http, void* user);

typedef struct rizz_api_http {
    const sx_alloc* (*alloc)(void);
    
    // normal requests: returns immediately (async sockets)
    // check `status_code` for retrieved http object to determine if it's finished or failed
    // call `free` if http data is not needed any more. if not freed by the user, it will be freed
    // when engine exits and throws a warning
    rizz_http (*get)(const char* url);
    rizz_http (*post)(const char* url, const void* data, size_t size);
    void (*free)(rizz_http handle);

    // return can be NULL (if it is not even started)
    const rizz_http_state* (*state)(rizz_http handle);

    // callback requests: triggers the callback when get/post is complete
    // NOTE: do not call `free` in the callback functions, the request will be freed automatically
    void (*get_cb)(const char* url, rizz_http_cb* callback, void* user);
    void (*post_cb)(const char* url, const void* data, size_t size, rizz_http_cb* callback, void* user);
} rizz_api_http;

////////////////////////////////////////////////////////////////////////////////////////////////////
// @reflection
// 
// A simple reflection/serialization system that supports POD types and tested structs
// Before using the reflection data for any of your types, you should first register them with rizz_refl_reg_XXX 
// family of macros (see below). There are 3 types of reflection types: 1)enums 2)functions 3)struct fields
// Enums are registered by their Type, Enumerator and enum value
// Functions are registered by function name and pointer
// Fields are registered by their parent struct type, their own type and field name, along with their offset and other info
// You can register already registered struct types as fields for other structs. 
// In the example below, our goal is to regiter `rizz_shader_info` struct, which has an array of rizz_shader_refl_input struct:
//
//    rizz_refl_reg_enum(ctx, sg_vertex_format, SG_VERTEXFORMAT_FLOAT, NULL);
//    rizz_refl_reg_enum(ctx, sg_vertex_format, SG_VERTEXFORMAT_FLOAT2, NULL);
//    rizz_refl_reg_enum(ctx, sg_vertex_format, SG_VERTEXFORMAT_FLOAT3, NULL);
//    rizz_refl_reg_enum(ctx, sg_vertex_format, SG_VERTEXFORMAT_FLOAT4, NULL);
//    rizz_refl_reg_enum(ctx, sg_vertex_format, SG_VERTEXFORMAT_BYTE4, NULL);
//    rizz_refl_reg_enum(ctx, sg_vertex_format, SG_VERTEXFORMAT_BYTE4N, NULL);
//    rizz_refl_reg_enum(ctx, sg_vertex_format, SG_VERTEXFORMAT_UBYTE4, NULL);
//    rizz_refl_reg_enum(ctx, sg_vertex_format, SG_VERTEXFORMAT_UBYTE4N, NULL);
//    rizz_refl_reg_enum(ctx, sg_vertex_format, SG_VERTEXFORMAT_SHORT2, NULL);
//    rizz_refl_reg_enum(ctx, sg_vertex_format, SG_VERTEXFORMAT_SHORT2N, NULL);
//    rizz_refl_reg_enum(ctx, sg_vertex_format, SG_VERTEXFORMAT_SHORT4, NULL);
//    rizz_refl_reg_enum(ctx, sg_vertex_format, SG_VERTEXFORMAT_SHORT4N, NULL);
//    rizz_refl_reg_enum(ctx, sg_vertex_format, SG_VERTEXFORMAT_UINT10_N2, NULL);
//
//    rizz_refl_reg_field(ctx, rizz_shader_refl_input, char[32], name, "shader input name", NULL);
//    rizz_refl_reg_field(ctx, rizz_shader_refl_input, char[32], semantic, "shader semantic name", NULL);
//    rizz_refl_reg_field(ctx, rizz_shader_refl_input, int, semantic_index, "shader semantic index", NULL);
//    rizz_refl_reg_field(ctx, rizz_shader_refl_input, sg_vertex_format, type, "shader input type", NULL);
//
//    rizz_refl_reg_field(ctx, rizz_shader_info, rizz_shader_refl_input[SG_MAX_VERTEX_ATTRIBUTES], inputs, "shader inputs", NULL);
//    rizz_refl_reg_field(ctx, rizz_shader_info, int, num_inputs, "shader input count", NULL);
//
// After you have registered the fields and enums, you can use in-engine json serializer to serialize data back and forth
// Or make a custom parser yourself. See basic-usage document for more detailed info on that topic.
// Now in the example above, we want to write the data in rizz_shader_info instance to a json file:
//
//        sx_mem_block* jmem = the_refl->serialize_json(ctx, "rizz_shader_info", &shader->info, the_core->heap_alloc(), true);
//        sx_assert(jmem);
//        FILE* f = fopen("test.json", "wb");
//        fwrite(jmem->data, jmem->size - 1, 1, f);
//        fclose(f); 
//
// And now, deserialize the rizz_shader_info data from the same json file:
//
//        rizz_shader_info info;
//        rizz_asset a = the_asset->load("json", "test.json", &(rizz_json_load_params){0}, 
//                        RIZZ_ASSET_LOAD_FLAG_ABSOLUTE_PATH|RIZZ_ASSET_LOAD_FLAG_WAIT_ON_LOAD, NULL, 0);
//        the_refl->deserialize_json(ctx, "rizz_shader_info", &info, the_asset->obj(a).ptr, 0);
//
typedef enum rizz_refl_type {
    RIZZ_REFL_ENUM,    //
    RIZZ_REFL_FUNC,    //
    RIZZ_REFL_FIELD    //
} rizz_refl_type;

enum rizz_refl_flags_ {
    RIZZ_REFL_FLAG_IS_PTR = 0x1,       // field is pointer
    RIZZ_REFL_FLAG_IS_STRUCT = 0x2,    // field is struct type
    RIZZ_REFL_FLAG_IS_ARRAY = 0x4,     // field is array
    RIZZ_REFL_FLAG_IS_ENUM = 0x8       // field is enum
};
typedef uint32_t rizz_refl_flags;

typedef struct rizz_refl_info {
    union {
        void* any;
        intptr_t offset;
    };

    const char* type;
    const char* name;
    const char* base;
    const char* desc;
    int size;
    int array_size;
    int stride;
    uint32_t flags;
    rizz_refl_type internal_type;
    const void* meta;
} rizz_refl_info;

typedef struct rizz_refl_context rizz_refl_context;
typedef struct rizz_json rizz_json;    // rizz/json.h

typedef enum rizz_refl_variant_type {
    RIZZ_REFL_VARIANTTYPE_UNKNOWN = 0,
    RIZZ_REFL_VARIANTTYPE_CSTRING,
    RIZZ_REFL_VARIANTTYPE_CHAR,
    RIZZ_REFL_VARIANTTYPE_FLOAT,
    RIZZ_REFL_VARIANTTYPE_DOUBLE,
    RIZZ_REFL_VARIANTTYPE_INT32,
    RIZZ_REFL_VARIANTTYPE_INT8,
    RIZZ_REFL_VARIANTTYPE_INT16,
    RIZZ_REFL_VARIANTTYPE_INT64,
    RIZZ_REFL_VARIANTTYPE_UINT8,
    RIZZ_REFL_VARIANTTYPE_UINT16,
    RIZZ_REFL_VARIANTTYPE_UINT32,
    RIZZ_REFL_VARIANTTYPE_UINT64,
    RIZZ_REFL_VARIANTTYPE_BOOL,
    RIZZ_REFL_VARIANTTYPE_MAT4,
    RIZZ_REFL_VARIANTTYPE_MAT3,
    RIZZ_REFL_VARIANTTYPE_VEC4,
    RIZZ_REFL_VARIANTTYPE_VEC3,
    RIZZ_REFL_VARIANTTYPE_VEC2,
    RIZZ_REFL_VARIANTTYPE_IVEC2,
    RIZZ_REFL_VARIANTTYPE_COLOR,
    RIZZ_REFL_VARIANTTYPE_AABB,
    RIZZ_REFL_VARIANTTYPE_RECT,
    _RIZZ_REFL_VARIANTTYPE_COUNT
} rizz_refl_variant_type;

typedef struct rizz_refl_variant {
    rizz_refl_variant_type type;
    union {
        const char* str;
        bool b;
        
        uint8_t u8;
        uint16_t u16;
        uint32_t u32;
        uint64_t u64;

        int8_t i8;
        int16_t i16;
        int32_t i32;
        int64_t i64;

        float f;
        double d;

        sx_mat4 mat4;
        sx_mat3 mat3;
        sx_vec4 v4;
        sx_vec3 v3;
        sx_vec2 v2;
        sx_ivec2 iv2;
        sx_color color;
        sx_aabb aabb;
        sx_rect rect;
    };
} rizz_refl_variant;

typedef struct rizz_refl_serialize_callbacks {
    bool (*on_begin)(const char* type_name, void* user);
    void (*on_end)(void* user);
    void (*on_builtin)(const char* name, rizz_refl_variant value, void* user, const void* meta, bool last_in_parent);
    void (*on_builtin_array)(const char* name, const rizz_refl_variant* var, int count, void* user, 
                             const void* meta, bool last_in_parent);
    void (*on_struct_begin)(const char* name, const char* type_name, int size, int count, void* user, const void* meta);
    void (*on_struct_array_element)(int index, void* user, const void* meta);
    void (*on_struct_end)(void* user, const void* meta, bool last_in_parent);
    void (*on_enum)(const char* name, int value, const char* value_name, void* user, const void* meta, bool last_in_parent);
} rizz_refl_serialize_callbacks;

typedef struct rizz_refl_deserialize_callbacks {
    bool (*on_begin)(const char* type_name, void* user);
    void (*on_end)(void* user);
    void (*on_builtin)(const char* name, void* data, rizz_refl_variant_type type, int size, void* user, 
                       const void* meta, bool last_in_parent);
    void (*on_builtin_array)(const char* name, void* data, rizz_refl_variant_type type, int count, int stride, 
                             void* user, const void* meta, bool last_in_parent);
    void (*on_struct_begin)(const char* name, const char* type_name, int size, int count, void* user, const void* meta);
    void (*on_struct_array_element)(int index, void* user, const void* meta);
    void (*on_struct_end)(void* user, const void* meta, bool last_in_parent);
    void (*on_enum)(const char* name, int* out_value, void* user, const void* meta, bool last_in_parent);
} rizz_refl_deserialize_callbacks;

typedef struct rizz_refl_field {
    rizz_refl_info info;
    void* value;    // pointer than contains arbitary field value(s) based on info.type/array/etc.
} rizz_refl_field;

typedef struct rizz_api_refl {
    rizz_refl_context* (*create_context)(const sx_alloc* alloc);
    void (*destroy_context)(rizz_refl_context* ctx);

    // use provided macros instead of this function
    int (*_reg_private)(rizz_refl_context* ctx, 
                        rizz_refl_type internal_type,  // core type (enum/func/field)
                        void* any,                     // offset_in_struct for fields, integer value for enums, func pointer for functions
                        const char* type,              // enums: enum name string, fields: type of field (int/float/struct_type), func: typedef of func
                        const char* name,              // enums: enum value string, fields: name of the field, func: function name
                        const char* base,              // only valid for fields, which is owner struct name
                        const char* desc,              // description
                        int size,                      // enums: size of parent enum type, fields: size of the field value, func: size of function pointer
                        int base_size,                 // only valid for fields, size of owner struct
                        const void* meta               // meta data per type, will be passed to callbacks (mostly extra UI stuff)
                        );                

    int (*size_of)(rizz_refl_context* ctx, const char* base_type);
    void* (*get_func)(rizz_refl_context* ctx,const char* name);
    int (*get_enum)(rizz_refl_context* ctx,const char* name, int not_found);
    void* (*get_field)(rizz_refl_context* ctx,const char* base_type, void* obj, const char* name);
    const char* (*get_enum_name)(rizz_refl_context* ctx,const char* type, int val);
    int (*reg_count)(rizz_refl_context* ctx);

    bool (*serialize)(rizz_refl_context* ctx, const char* type_name, const void* data, void* user, 
                      const rizz_refl_serialize_callbacks* callbacks);
    bool (*deserialize)(rizz_refl_context* ctx, const char* type_name, void* data, void* user,
                        const rizz_refl_deserialize_callbacks* callbacks);
    int (*get_fields)(rizz_refl_context* ctx, const char* base_type, void* obj, rizz_refl_field* fields, 
                      int max_fields);

    bool (*deserialize_json)(rizz_refl_context* ctx, 
                             const char* type_name, 
                             void* data, 
                             rizz_json* json, 
                             int root_token_id);
    sx_mem_block* (*serialize_json)(rizz_refl_context* ctx, 
                                    const char* type_name, 
                                    const void* data, 
                                    const sx_alloc* alloc,
                                    bool prettify);
    
} rizz_api_refl;

// reflection info registration macros
#define rizz_refl_reg_enum(_ctx, _type, _name, _meta) \
    (RIZZ_REFLECT_API_VARNAME)->_reg_private(_ctx, RIZZ_REFL_ENUM, (void*)(intptr_t)_name, #_type, #_name, NULL, "", sizeof(_type), 0, _meta)
#define rizz_refl_reg_func(_ctx, _type, _name, _desc, _meta) \
    (RIZZ_REFLECT_API_VARNAME)->_reg_private(_ctx, RIZZ_REFL_FUNC, &_name, #_type, #_name, NULL, _desc, sizeof(void*), 0, _meta)
#define rizz_refl_reg_field(_ctx, _struct, _type, _name, _desc, _meta)   \
    (RIZZ_REFLECT_API_VARNAME)->_reg_private(_ctx, RIZZ_REFL_FIELD, &(((_struct*)0)->_name), #_type, #_name, #_struct, _desc, sizeof(_type), sizeof(_struct), _meta)

////////////////////////////////////////////////////////////////////////////////////////////////////
// @json
// json files can be loaded by asset manager with the type_name="json"
// the underlying type for rizz_asset_obj is a pointer to rizz_json. remember to include "rizz/json.h"
// load parameters:
// load_cb/reload_cb are optional (can be NULL) and can be given to asset loader, to automatically trigger when 
// json data is loaded or reloaded. They will always run in the main thread.
// NOTE: keeping callback functions in framework will likely cause trouble when the guest program reloads
// TODO: fix this

typedef void (rizz_json_reload_cb)(rizz_json* new_json, rizz_json* prev_json);
typedef void (rizz_json_load_cb)(rizz_json* json);
typedef void (rizz_json_unload_cb)(rizz_json* json);

typedef struct rizz_json_load_params {
    rizz_json_load_cb* load_fn;
    rizz_json_reload_cb* reload_fn;
    rizz_json_unload_cb* unload_fn;
    void* user;
} rizz_json_load_params;

////////////////////////////////////////////////////////////////////////////////////////////////////
// @misc

// rizz_event_queue: small _circular_ stack-based event queue.
// useful for pushing and polling gameplay events to components/objects
// NOTE: This is a small on-stack event queue implementation, and it's not thread-safe
//       If you want to use a more general thread-safe queue, use sx/thread.h: sx_queue_spsc
// NOTE: This is a circular queue, so it only holds RIZZ_EVENTQUEUE_MAX_EVENTS events and overwrites
//       previous ones if it's overflowed. So use it with this limitation in mind
//       most gameplay events for a component shouldn't require many events per-frame. If this
//       happens to not be the case, then make your own queue or increase the
//       `RIZZ_EVENTQUEUE_MAX_EVENTS` value
//
#ifndef RIZZ_EVENTQUEUE_MAX_EVENTS
#    define RIZZ_EVENTQUEUE_MAX_EVENTS 4
#endif

typedef struct rizz_event {
    int e;
    void* user;
} rizz_event;

typedef struct rizz_event_queue {
    rizz_event events[RIZZ_EVENTQUEUE_MAX_EVENTS];
    int first;
    int count;
} rizz_event_queue;

SX_INLINE void rizz_event_push(rizz_event_queue* eq, int event, void* user)
{
    if (eq->count < RIZZ_EVENTQUEUE_MAX_EVENTS) {
        int index = (eq->first + eq->count) % RIZZ_EVENTQUEUE_MAX_EVENTS;
        eq->events[index].e = event;
        eq->events[index].user = user;
        ++eq->count;
    } else {
        // overwrite previous events !!!
        int first = eq->first;
        first = ((first + 1) < RIZZ_EVENTQUEUE_MAX_EVENTS) ? (first + 1) : 0;
        int index = (first + eq->count) % RIZZ_EVENTQUEUE_MAX_EVENTS;
        eq->events[index].e = event;
        eq->events[index].user = user;
        eq->first = first;
    }
}

SX_INLINE bool rizz_event_poll(rizz_event_queue* eq, rizz_event* e)
{
    if (eq->count > 0) {
        int first = eq->first;
        *e = eq->events[first];
        eq->first = ((first + 1) < RIZZ_EVENTQUEUE_MAX_EVENTS) ? (first + 1) : 0;
        --eq->count;
        return true;
    } else {
        return false;
    }
}

SX_INLINE rizz_event rizz_event_peek(const rizz_event_queue* eq)
{
    sx_assert(eq->count > 0);
    return eq->events[eq->first];
}

SX_INLINE bool rizz_event_empty(const rizz_event_queue* eq)
{
    return eq->count == 0;
}

// tween helper: useful for small animations and transitions
//
// usage:
//      frame_update() {
//          static rizz_tween t;
//          float val = rizz_tween_update(&t, delta_time, )
//          // val: [0, 1]. you can ease it or do whatever you like
//          if (rizz_tween_done(&t)) {
//              // tweening is finished. trigger something
//          }
//      }
//
typedef struct rizz_tween {
    float tm;
} rizz_tween;

SX_INLINE float rizz_tween_update(rizz_tween* tween, float dt, float max_tm)
{
    sx_assert(max_tm > 0.0);
    float t = sx_min(1.0f, tween->tm / max_tm);
    tween->tm += dt;
    return t;
}
