/*
 *  radiostate.c
 *
 */

#define DEBUG

#include <string.h>

#include "radiostate.h"

#include "debug.h"
#include "md380.h"
#include "syslog.h"
#include "usersdb.h"

#include "stm32f4_discovery.h"
#include "stm32f4xx_conf.h" // again, added because ST didn't put it here ?

#include "codeplug.h"

//#include <arpa/inet.h>

channel_easy current_channel_info_E;

int     rst_voice_active = 0;
int     rst_src          = 0;
int     rst_dst          = 0;
int     rst_grp          = 0;
int     rst_mycall       = 0;
int     g_src            = 0;
uint8_t rst_flco         = 0;

int rst_hdr_sap;
int rst_hdr_src;
int rst_hdr_dst;

// TODO locking. because 1 writer locking no prio. readers only visualize.

void mute_speaker() {
    // bp_send_beep(BEEP_TEST_1);
    // GPIO_SetBits(GPIOB, GPIO_Pin_8); //Mutes speaker (but only for a moment)
    char buffa = 0;
    c5000_spi0_readreg(0x0E, &buffa);
    buffa &= ~(0x08);
    c5000_spi0_writereg(0x0E, buffa);
}

inline int is_tracing() { return (global_addl_config.debug != 0) || (global_addl_config.netmon != 0); }

// Filter out stupid callsign that shows up for dv4mini
void updateSrcDst(int src, int dst) {
    if (src != 3112528) {
        rst_src = src;
    }
    if (dst != 3112528) {
        rst_dst = dst;
    }
}

void rst_voice_lc_header(lc_t* lc) {
    //
    // DMR FULL_LC DATA CALLBCK
    //
    // This callback method will be called when a FULL_LC data is decoded,
    // it LC data will be a DT_VOICE_HEADER_LC DMRData frame,
    // or reassembled FULL_LC data from DMR Voice EmbeddedData.
    //
    // Please see ETSI doc for the all above concepts/details.
    //
    // - BG5HHP
    //
    int flco = get_flco(lc);

    if (flco == 0 || flco == 3) { // FULL_LC for LC Header - BG5HHP
        int src = get_adr(lc->src);
        int dst = get_adr(lc->dst);

        int groupcall = flco == 0;
        if (groupcall)
            g_src = src;

        if ((!rst_voice_active || rst_src != src || rst_dst != dst)) {
            updateSrcDst(src, dst);
            rst_flco = flco;

            PRINT("\n* Call from %d to %s%d started.\n", src, groupcall ? "group " : "", dst);

            PRINT("cs ");

            if (talkerAlias.src == 0) {
                // mark call start.
                // we'd better store the call state in a global LCHeader structure
                // and remove the redundant variables.
                // - BG5HHP
                talkerAlias.src    = src;
                talkerAlias.length = 0;
            }

            char grp_c = 'U';
            if (flco == 0) {
                grp_c   = 'G';
                rst_grp = 1;
            } else {
                rst_grp = 0;
            }

            LOGR("cs %c %d->%d\n", grp_c, src, dst);
            rst_voice_active = 1;
            rx_voice         = 1; // flag for new voice call received
        }

    } else if (flco >= 4 && flco <= 7) { // FULL_LC for TalkerAlias - BG5HHP
        LOGR("ta %d %d->%d\n", flco - 4, rst_src, rst_dst);
        decode_ta(lc);
    } else if (flco == 8) { // FULL_LC for GPS - BG5HHP
        LOGR("gps %c %d->%d\n", rst_grp ? 'G' : 'U', rst_src, rst_dst);
    } else {
        // Unknown type,
        LOGR("unknown %c %d->%d\n", rst_grp ? 'G' : 'U', rst_src, rst_dst);
    }
}

void rst_term_with_lc(lc_t* lc) {
    int src  = get_adr(lc->src);
    int dst  = get_adr(lc->dst);
    int flco = get_flco(lc);

    int groupcall = (flco == 0);
    if (groupcall)
        g_src = src;

    if (rst_voice_active) {
        updateSrcDst(src, dst);
        PRINT("\n* Call from %d to %s%d ended.\n", src, groupcall ? "group " : "", dst);

        // mark call terminate - BG5HHP
        talkerAlias.src = 0;

        PRINT("ce ");
        dump_full_lc(lc);

        char grp_c = 'U';
        if (groupcall) {
            grp_c = 'G';
        }

        LOGR("ce %c %d->%d\n", grp_c, src, dst);

        rst_voice_active = 0;
        rx_voice         = 0; // flag for voice call ended
    }
}

void rst_signal_other_call() { rst_mycall = 0; }

void rst_signal_my_call() { rst_mycall = 1; }

int blocks_outstanding;
int current_dpf;
int pad_octets;
int expected_serial;

#define DATABUF_SZ 1024
uint8_t databuffer[DATABUF_SZ];
int     dataidx;

int get_dpf(uint8_t* data) { return data[0] & 0xF; }

uint8_t get_blocks_to_follow(uint8_t* data) { return data[8] & 0x7F; }

uint8_t get_appended_blocks(uint8_t* data) { return (data[1] & 0xF) + (data[0] & 0x30); }

uint8_t get_pad_octets(uint8_t* data) {
    int r = data[1] & 0xF;
    r += (data[0] & 0x10);
    return r;
}

inline uint8_t get_sap(uint8_t* data) { return (data[1] >> 4) & 0xF; }

inline uint8_t get_grp(uint8_t* data) { return (data[0] & 0x80) != 0; }

inline uint8_t get_answer(uint8_t* data) { return (data[0] & 0x40) != 0; }

uint8_t get_sap_prop(uint8_t* data) { return (data[0] >> 4) & 0xF; }

void rst_data_header(void* data) {
    int dpf = get_dpf(data);
    int grp = get_grp(data); // ignore for proprietary
    int btf = 0;
    int poc = 0;
    int sap = -1;

    switch (dpf) {
        case DPF_UNCONFIRMED:
        case DPF_CONFIRMED:
        case DPF_RESPONSE:
            btf = get_blocks_to_follow(data);
            // not for resp_pkt, but it is defined as 0 there.
            poc = get_pad_octets(data);
            break;
        case DPF_DEFINED_SHORT:
            btf = get_appended_blocks(data);
            break;
    }
    switch (dpf) {
        case DPF_UNCONFIRMED:
        case DPF_CONFIRMED:
        case DPF_RESPONSE:
        case DPF_RAW_STATUS_SHORT:
        case DPF_DEFINED_SHORT:
        case DPF_UDT:
            sap = get_sap(data);
            break;
        case DPF_PROPRIETARY:
            sap = get_sap_prop(data);
            break;
    }
    blocks_outstanding = btf;
    pad_octets         = poc;
    current_dpf        = dpf;
    dataidx            = 0;
    expected_serial    = 0;

    data_hdr_t* datahdr = data;

    rst_hdr_sap = sap;
    rst_hdr_src = get_adr(datahdr->src);
    rst_hdr_dst = get_adr(datahdr->dst);

    char ug = 'U';
    if (grp) {
        ug = 'G';
    }

    LOGR("dh %c (%d) %d->%d %d\n", ug, rst_hdr_sap, rst_hdr_src, rst_hdr_dst, get_answer(data));

    PRINT("sap=%d %s dpf=%d %s src=%d dst=%d btf=%d grp=%d\n", sap, sap_to_str(sap), dpf, dpf_to_str(dpf),
          get_adr(datahdr->src), get_adr(datahdr->dst), btf, grp);

    //    PRINT("data: ");
    //    PRINTHEX(datahdr,12);
    //    PRINT("\n");
}

void rst_add_packet(void* data, int len) {
    if (dataidx + len > DATABUF_SZ) {
        // skip for now.
        return;
    }

    uint8_t* p = &databuffer[dataidx];

    memcpy(p, data, len);

    dataidx += len;
}

void rst_unconf_data_packet(void* data, int len) { rst_add_packet(data, len); }

void rst_conf_data_packet(void* data, int len) {
    uint8_t* datap = data;

    uint8_t serial = datap[0] >> 1;
    PRINT("serial %d\n", serial);

    if (serial != expected_serial) {
        PRINT("out of order\n");
        // TODO signal damaged buffer.
        // fix for now:
        blocks_outstanding++;
        return;
    }
    expected_serial++;

    // TODO implement CRD checking

    len -= 2;
    data += 2;

    rst_add_packet(data, len);
}

void rst_packet_complete(void* data, int len) {
    PRINT("buffer: ");
    PRINTHEX(databuffer, len);
    PRINT("\n");
    PRINT("buffer: ");
    PRINTASC(databuffer, len);
    PRINT("\n");

    uint8_t* b = data;

    //    uint32_t saddr = ntohl( *(uint32_t*)(b+12) );
    //    uint32_t daddr = ntohl( *(uint32_t*)(b+16) );
    uint16_t sport = (b[20] << 8) + b[21];
    uint16_t dport = (b[22] << 8) + b[23];

    LOGR("%d.%d.%d.%d:%d %d.%d.%d.%d:%d\n", b[12], b[13], b[14], b[15], sport, b[16], b[17], b[18], b[19], dport);
    PRINT("%d.%d.%d.%d:%d %d.%d.%d.%d:%d\n", b[12], b[13], b[14], b[15], sport, b[16], b[17], b[18], b[19], dport);
}

void rst_data_block(void* data, int len) {
#if 0
    PRINT("data: ");
    PRINTHEX(data,len);
    PRINT("\n");    
    PRINT("data: ");
    PRINTASC(data,len);
    PRINT("\n");
#endif

    if (blocks_outstanding < 1) {
        PRINT("spurious data block?\n");
        return;
    }
    blocks_outstanding--;

    switch (current_dpf) {
        default:
            // guess.
            rst_conf_data_packet(data, len);
            break;
        case DPF_DEFINED_SHORT:
            // guess.
            rst_conf_data_packet(data, len);
            break;
        case DPF_UNCONFIRMED:
            rst_unconf_data_packet(data, len);
            break;
        case DPF_UDT:
            // TODO
            break;
    }

    if (blocks_outstanding == 0) {
        // packet complete
        int idx = dataidx - pad_octets;

        rst_packet_complete(databuffer, idx);
    }
}
