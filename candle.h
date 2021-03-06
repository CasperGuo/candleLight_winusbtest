#pragma once
#include <stdint.h>
#include <stdbool.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void* candle_list_handle;
typedef void* candle_handle;

typedef enum {
    CANDLE_DEVSTATE_AVAIL,
    CANDLE_DEVSTATE_INUSE
} candle_devstate_t;

typedef enum {
    CANDLE_FRAMETYPE_UNKNOWN,
    CANDLE_FRAMETYPE_RECEIVE,
    CANDLE_FRAMETYPE_ECHO,
    CANDLE_FRAMETYPE_ERROR,
    CANDLE_FRAMETYPE_TIMESTAMP_OVFL
} candle_frametype_t;

typedef enum {
    CANDLE_MODE_NORMAL        = 0x00,
    CANDLE_MODE_LISTEN_ONLY   = 0x01,
    CANDLE_MODE_LOOP_BACK     = 0x02,
    CANDLE_MODE_TRIPLE_SAMPLE = 0x04,
    CANDLE_MODE_ONE_SHOT      = 0x08
} candle_mode_t;

typedef enum {
    CANDLE_ERR_OK                  =  0,
    CANDLE_ERR_CREATE_FILE         =  1,
    CANDLE_ERR_WINUSB_INITIALIZE   =  2,
    CANDLE_ERR_QUERY_INTERFACE     =  3,
    CANDLE_ERR_QUERY_PIPE          =  4,
    CANDLE_ERR_PARSE_IF_DESCR      =  5,
    CANDLE_ERR_SET_HOST_FORMAT     =  6,
    CANDLE_ERR_GET_DEVICE_INFO     =  7,
    CANDLE_ERR_GET_BITTIMING_CONST =  8,
    CANDLE_ERR_PREPARE_READ        =  9,
    CANDLE_ERR_SET_DEVICE_MODE     = 10,
    CANDLE_ERR_SET_BITTIMING       = 11,
    CANDLE_ERR_BITRATE_FCLK        = 12,
    CANDLE_ERR_BITRATE_UNSUPPORTED = 13,
    CANDLE_ERR_SEND_FRAME          = 14,
    CANDLE_ERR_READ_TIMEOUT        = 15,
    CANDLE_ERR_READ_WAIT           = 16,
    CANDLE_ERR_READ_RESULT         = 17,
    CANDLE_ERR_READ_SIZE           = 18,
    CANDLE_ERR_SETUPDI_IF_DETAILS  = 19,
    CANDLE_ERR_SETUPDI_IF_DETAILS2 = 20,
    CANDLE_ERR_MALLOC              = 21,
    CANDLE_ERR_PATH_LEN            = 22,
    CANDLE_ERR_CLSID               = 23,
    CANDLE_ERR_GET_DEVICES         = 24,
    CANDLE_ERR_SETUPDI_IF_ENUM     = 25,
    CANDLE_ERR_SET_TIMESTAMP_MODE  = 26,
    CANDLE_ERR_DEV_OUT_OF_RANGE    = 27,
} candle_err_t;

#pragma pack(push,1)

typedef struct {
    uint32_t echo_id;
    uint32_t can_id;
    uint8_t can_dlc;
    uint8_t channel;
    uint8_t flags;
    uint8_t reserved;
    uint8_t data[8];
    uint32_t timestamp_us;
} candle_frame_t;

typedef struct {
    uint32_t feature;
    uint32_t fclk_can;
    uint32_t tseg1_min;
    uint32_t tseg1_max;
    uint32_t tseg2_min;
    uint32_t tseg2_max;
    uint32_t sjw_max;
    uint32_t brp_min;
    uint32_t brp_max;
    uint32_t brp_inc;
} candle_capability_t;

typedef struct {
    uint32_t prop_seg;
    uint32_t phase_seg1;
    uint32_t phase_seg2;
    uint32_t sjw;
    uint32_t brp;
} candle_bittiming_t;

#pragma pack(pop)

bool candle_list_scan(candle_list_handle *list);
bool candle_list_free(candle_list_handle list);
bool candle_list_length(candle_list_handle list, uint8_t *len);

bool candle_dev_get(candle_list_handle list, uint8_t dev_num, candle_handle *hdev);
bool candle_dev_get_state(candle_handle hdev, candle_devstate_t *state);
wchar_t *candle_dev_get_path(candle_handle hdev);
bool candle_dev_open(candle_handle hdev);
bool candle_dev_close(candle_handle hdev);
bool candle_dev_free(candle_handle hdev);

bool candle_channel_count(candle_handle hdev, uint8_t *num_channels);
bool candle_channel_get_capabilities(candle_handle hdev, uint8_t ch, candle_capability_t *cap);
bool candle_channel_set_timing(candle_handle hdev, uint8_t ch, candle_bittiming_t *data);
bool candle_channel_set_bitrate(candle_handle hdev, uint8_t ch, uint32_t bitrate);
bool candle_channel_start(candle_handle hdev, uint8_t ch, uint32_t flags);
bool candle_channel_stop(candle_handle hdev, uint8_t ch);

bool candle_frame_send(candle_handle hdev, uint8_t ch, candle_frame_t *frame);
bool candle_frame_read(candle_handle hdev, candle_frame_t *frame, uint32_t timeout_ms);

candle_frametype_t candle_frame_type(candle_frame_t *frame);
uint32_t candle_frame_id(candle_frame_t *frame);
bool candle_frame_is_extended_id(candle_frame_t *frame);
uint8_t candle_frame_dlc(candle_frame_t *frame);
uint8_t *candle_frame_data(candle_frame_t *frame);
uint32_t candle_frame_timestamp_us(candle_frame_t *frame);

candle_err_t candle_dev_last_error(candle_handle hdev);

#ifdef __cplusplus
}
#endif
