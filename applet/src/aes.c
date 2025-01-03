
#include <string.h>

#include "config.h"
#include "dmesg.h"
#include "gfx.h"
#include "md380.h"
#include "printf.h"
#include "tooldfu.h"
#include "version.h"

/* These Motorola Basic Privacy keys are sampled manually from silent
   frames in the air, so they are imperfect and likely contain flipped
   bits.  A better method would be to extract the complete sequence from
   either motorola firmware or automatically fetch them from the first
   frame of a transmission.
 */
const char* getmotorolabasickey(int i) {
    switch (i & 0xFF) {
        case 1:
            return "\x1F\x00\x1F\x00\x1F\x00\x00";
        case 2:
            return "\xE3\x00\xE3\x00\xE3\x00\x01";
        case 3:
            return "\xFC\x00\xFC\x00\xFC\x00\x01";
        case 4:
            return "\x25\x03\x25\x03\x25\x03\x00";
        case 5:
            return "\x3A\x03\x3A\x03\x3A\x03\x00";
        case 6:
            return "\xC6\x03\xC6\x03\xC6\x03\x01";
        case 7:
            return "\xD9\x03\xD9\x03\xD9\x03\x01";
        case 8:
            return "\x4A\x05\x4A\x05\x4A\x05\x00";
        case 9:
            return "\x55\x05\x55\x05\x55\x05\x00";
        case 10:
            return "\xA9\x05\xA9\x05\xA9\x05\x01";
        case 11:
            return "\xB6\x05\xB6\x05\xB6\x05\x01";
        case 12:
            return "\x6F\x06\x6F\x06\x6F\x06\x00";
        case 13:
            return "\x70\x06\x70\x06\x70\x06\x00";
        case 14:
            return "\x8C\x06\x8C\x06\x8C\x06\x01";
        case 15:
            return "\x93\x06\x93\x06\x93\x06\x01";
        case 16:
            return "\x26\x08\x26\x18\x26\x18\x00";
        case 17:
            return "\x39\x08\x39\x18\x39\x18\x00";
            // List previously got sparse after after key 17, keys 18 to 255 have now been included by
            // iScottybotty@gmail.com
        case 18:
            return "\xC5\x08\xC5\x18\xC5\x18\x01";
        case 19:
            return "\xDA\x08\xDA\x18\xDA\x18\x01";
        case 20:
            return "\x03\x0B\x03\x1B\x03\x1B\x00";
        case 21:
            return "\x1C\x0B\x1C\x1B\x1C\x1B\x00";
        case 22:
            return "\xE0\x0B\xE0\x1B\xE0\x1B\x01";
        case 23:
            return "\xFF\x0B\xFF\x1B\xFF\x1B\x01";
        case 24:
            return "\x6C\x0D\x6C\x1D\x6C\x1D\x00";
        case 25:
            return "\x73\x0D\x73\x1D\x73\x1D\x00";
        case 26:
            return "\x8F\x0D\x8F\x1D\x8F\x1D\x01";
        case 27:
            return "\x90\x0D\x90\x1D\x90\x1D\x01";
        case 28:
            return "\x49\x0E\x49\x1E\x49\x1E\x00";
        case 29:
            return "\x56\x0E\x56\x1E\x56\x1E\x00";
        case 30:
            return "\xAA\x0E\xAA\x1E\xAA\x1E\x01";
        case 31:
            return "\xB5\x0E\xB5\x1E\xB5\x1E\x01";
        case 32:
            return "\x4B\x08\x4B\x28\x4B\x28\x00";
        case 33:
            return "\x54\x08\x54\x28\x54\x28\x00";
        case 34:
            return "\xA8\x08\xA8\x28\xA8\x28\x01";
        case 35:
            return "\xB7\x08\xB7\x28\xB7\x28\x01";
        case 36:
            return "\x6E\x0B\x6E\x2B\x6E\x2B\x00";
        case 37:
            return "\x71\x0B\x71\x2B\x71\x2B\x00";
        case 38:
            return "\x8D\x0B\x8D\x2B\x8D\x2B\x01";
        case 39:
            return "\x92\x0B\x92\x2B\x92\x2B\x01";
        case 40:
            return "\x01\x0D\x01\x2D\x01\x2D\x00";
        case 41:
            return "\x1E\x0D\x1E\x2D\x1E\x2D\x00";
        case 42:
            return "\xE2\x0D\xE2\x2D\xE2\x2D\x01";
        case 43:
            return "\xFD\x0D\xFD\x2D\xFD\x2D\x01";
        case 44:
            return "\x24\x0E\x24\x2E\x24\x2E\x00";
        case 45:
            return "\x3B\x0E\x3B\x2E\x3B\x2E\x00";
        case 46:
            return "\xC7\x0E\xC7\x2E\xC7\x2E\x01";
        case 47:
            return "\xD8\x0E\xD8\x2E\xD8\x2E\x01";
        case 48:
            return "\x6D\x00\x6D\x30\x6D\x30\x00";
        case 49:
            return "\x72\x00\x72\x30\x72\x30\x00";
        case 50:
            return "\x8E\x00\x8E\x30\x8E\x30\x01";
        case 51:
            return "\x91\x00\x91\x30\x91\x30\x01";
        case 52:
            return "\x48\x03\x48\x33\x48\x33\x00";
        case 53:
            return "\x57\x03\x57\x33\x57\x33\x00";
        case 54:
            return "\xAB\x03\xAB\x33\xAB\x33\x01";
        case 55:
            return "\xB4\x03\xB4\x33\xB4\x33\x01";
        case 56:
            return "\x27\x05\x27\x35\x27\x35\x00";
        case 57:
            return "\x38\x05\x38\x35\x38\x35\x00";
        case 58:
            return "\xC4\x05\xC4\x35\xC4\x35\x01";
        case 59:
            return "\xDB\x05\xDB\x35\xDB\x35\x01";
        case 60:
            return "\x02\x06\x02\x36\x02\x36\x00";
        case 61:
            return "\x1D\x06\x1D\x36\x1D\x36\x00";
        case 62:
            return "\xE1\x06\xE1\x36\xE1\x36\x01";
        case 63:
            return "\xFE\x06\xFE\x36\xFE\x36\x01";
        case 64:
            return "\x2B\x09\x2B\x49\x2B\x49\x00";
        case 65:
            return "\x34\x09\x34\x49\x34\x49\x00";
        case 66:
            return "\xC8\x09\xC8\x49\xC8\x49\x01";
        case 67:
            return "\xD7\x09\xD7\x49\xD7\x49\x01";
        case 68:
            return "\x0E\x0A\x0E\x4A\x0E\x4A\x00";
        case 69:
            return "\x11\x0A\x11\x4A\x11\x4A\x00";
        case 70:
            return "\xED\x0A\xED\x4A\xED\x4A\x01";
        case 71:
            return "\xF2\x0A\xF2\x4A\xF2\x4A\x01";
        case 72:
            return "\x61\x0C\x61\x4C\x61\x4C\x00";
        case 73:
            return "\xAE\x0C\xAE\x4C\xAE\x4C\x01";
        case 74:
            return "\x82\x0C\x82\x4C\x82\x4C\x01";
        case 75:
            return "\x9D\x0C\x9D\x4C\x9D\x4C\x01";
        case 76:
            return "\x44\x0F\x44\x4F\x44\x4F\x00";
        case 77:
            return "\x5B\x0F\x5B\x4F\x5B\x4F\x00";
        case 78:
            return "\xA7\x0F\xA7\x4F\xA7\x4F\x01";
        case 79:
            return "\xB8\x0F\xB8\x4F\xB8\x4F\x01";
        case 80:
            return "\x0D\x01\x0D\x51\x0D\x51\x00";
        case 81:
            return "\x12\x01\x12\x51\x12\x51\x00";
        case 82:
            return "\xEE\x01\xEE\x51\xEE\x51\x01";
        case 83:
            return "\xF1\x01\xF1\x51\xF1\x51\x01";
        case 84:
            return "\x28\x02\x28\x52\x28\x52\x00";
        case 85:
            return "\x37\x02\x37\x52\x37\x52\x00";
        case 86:
            return "\xCB\x02\xCB\x52\xCB\x52\x01";
        case 87:
            return "\xD4\x02\xD4\x52\xD4\x52\x01";
        case 88:
            return "\x47\x04\x47\x54\x47\x54\x00";
        case 89:
            return "\x58\x04\x58\x54\x58\x54\x00";
        case 90:
            return "\xA4\x04\xA4\x54\xA4\x54\x01";
        case 91:
            return "\xBB\x04\xBB\x54\xBB\x54\x01";
        case 92:
            return "\x62\x07\x62\x57\x62\x57\x00";
        case 93:
            return "\x7D\x07\x7D\x57\x7D\x57\x00";
        case 94:
            return "\x81\x07\x81\x57\x81\x57\x01";
        case 95:
            return "\x9E\x07\x9E\x57\x9E\x57\x01";
        case 96:
            return "\x60\x01\x60\x61\x60\x61\x00";
        case 97:
            return "\x7F\x01\x7F\x61\x7F\x61\x00";
        case 98:
            return "\x83\x01\x83\x61\x83\x61\x01";
        case 99:
            return "\x9C\x01\x9C\x61\x9C\x61\x01";
        case 100:
            return "\x45\x02\x45\x62\x45\x62\x00";
        case 101:
            return "\x5A\x02\x5A\x62\x5A\x62\x00";
        case 102:
            return "\xA6\x02\xA6\x62\xA6\x62\x01";
        case 103:
            return "\xB9\x02\xB9\x62\xB9\x62\x01";
        case 104:
            return "\x2A\x04\x2A\x64\x2A\x64\x00";
        case 105:
            return "\x35\x04\x35\x64\x35\x64\x00";
        case 106:
            return "\xC9\x04\xC9\x64\xC9\x64\x01";
        case 107:
            return "\xD6\x04\xD6\x64\xD6\x64\x01";
        case 108:
            return "\x0F\x07\x0F\x67\x0F\x67\x00";
        case 109:
            return "\x10\x07\x10\x67\x10\x67\x00";
        case 110:
            return "\xEC\x07\xEC\x67\xEC\x67\x01";
        case 111:
            return "\xF3\x07\xF3\x67\xF3\x67\x01";
        case 112:
            return "\x46\x09\x46\x79\x46\x79\x00";
        case 113:
            return "\x59\x09\x59\x79\x59\x79\x00";
        case 114:
            return "\xA5\x09\xA5\x79\xA5\x79\x01";
        case 115:
            return "\xBA\x09\xBA\x79\xBA\x79\x01";
        case 116:
            return "\x63\x0A\x63\x7A\x63\x7A\x00";
        case 117:
            return "\x7C\x0A\x7C\x7A\x7C\x7A\x00";
        case 118:
            return "\x80\x0A\x80\x7A\x80\x7A\x01";
        case 119:
            return "\x9F\x0A\x9F\x7A\x9F\x7A\x01";
        case 120:
            return "\x0C\x0C\x0C\x7C\x0C\x7C\x00";
        case 121:
            return "\x13\x0C\x13\x7C\x13\x7C\x00";
        case 122:
            return "\xEF\x0C\xEF\x7C\xEF\x7C\x01";
        case 123:
            return "\xF0\x0C\xF0\x7C\xF0\x7C\x01";
        case 124:
            return "\x29\x0F\x29\x7F\x29\x7F\x00";
        case 125:
            return "\x36\x0F\x36\x7F\x36\x7F\x00";
        case 126:
            return "\xCA\x0F\xCA\x7F\xCA\x7F\x01";
        case 127:
            return "\xD5\x0F\xD5\x7F\xD5\x7F\x01";
        case 128:
            return "\x4D\x09\x4D\x89\x4D\x89\x00";
        case 129:
            return "\x52\x09\x52\x89\x52\x89\x00";
        case 130:
            return "\xAE\x09\xAE\x89\xAE\x89\x01";
        case 131:
            return "\xB1\x09\xB1\x89\xB1\x89\x01";
        case 132:
            return "\x68\x0A\x68\x8A\x68\x8A\x00";
        case 133:
            return "\x77\x0A\x77\x8A\x77\x8A\x00";
        case 134:
            return "\x8B\x0A\x8B\x8A\x8B\x8A\x01";
        case 135:
            return "\x94\x0A\x94\x8A\x94\x8A\x01";
        case 136:
            return "\x07\x0C\x07\x8C\x07\x8C\x00";
        case 137:
            return "\x18\x0C\x18\x8C\x18\x8C\x00";
        case 138:
            return "\xE4\x0C\xE4\x8C\xE4\x8C\x01";
        case 139:
            return "\xFB\x0C\xFB\x8C\xFB\x8C\x01";
        case 140:
            return "\x22\x0F\x22\x8F\x22\x8F\x00";
        case 141:
            return "\x3D\x0F\x3D\x8F\x3D\x8F\x00";
        case 142:
            return "\xC1\x0F\xC1\x8F\xC1\x8F\x01";
        case 143:
            return "\xDE\x0F\xDE\x8F\xDE\x8F\x01";
        case 144:
            return "\x6B\x01\x6B\x91\x6B\x91\x00";
        case 145:
            return "\x74\x01\x74\x91\x74\x91\x00";
        case 146:
            return "\x88\x01\x88\x91\x88\x91\x01";
        case 147:
            return "\x97\x01\x97\x91\x97\x91\x01";
        case 148:
            return "\x4E\x02\x4E\x92\x4E\x92\x00";
        case 149:
            return "\x51\x02\x51\x92\x51\x92\x00";
        case 150:
            return "\xAD\x02\xAD\x92\xAD\x92\x01";
        case 151:
            return "\xB2\x02\xB2\x92\xB2\x92\x01";
        case 152:
            return "\x21\x04\x21\x94\x21\x94\x00";
        case 153:
            return "\x3E\x04\x3E\x94\x3E\x94\x00";
        case 154:
            return "\xC2\x04\xC2\x94\xC2\x94\x01";
        case 155:
            return "\xDD\x04\xDD\x94\xDD\x94\x01";
        case 156:
            return "\x04\x07\x04\x97\x04\x97\x00";
        case 157:
            return "\x1B\x07\x1B\x97\x1B\x97\x00";
        case 158:
            return "\xE7\x07\xE7\x97\xE7\x97\x01";
        case 159:
            return "\xF8\x07\xF8\x97\xF8\x97\x01";
        case 160:
            return "\x06\x01\x06\xA1\x06\xA1\x00";
        case 161:
            return "\x19\x01\x19\xA1\x19\xA1\x00";
        case 162:
            return "\xE5\x01\xE5\xA1\xE5\xA1\x01";
        case 163:
            return "\xFA\x01\xFA\xA1\xFA\xA1\x01";
        case 164:
            return "\x23\x02\x23\xA2\x23\xA2\x00";
        case 165:
            return "\x3C\x02\x3C\xA2\x3C\xA2\x00";
        case 166:
            return "\xC0\x02\xC0\xA2\xC0\xA2\x01";
        case 167:
            return "\xDF\x02\xDF\xA2\xDF\xA2\x01";
        case 168:
            return "\x4C\x04\x4C\xA4\x4C\xA4\x00";
        case 169:
            return "\x53\x04\x53\xA4\x53\xA4\x00";
        case 170:
            return "\xAF\x04\xAF\xA4\xAF\xA4\x01";
        case 171:
            return "\xB0\x04\xB0\xA4\xB0\xA4\x01";
        case 172:
            return "\xB0\x04\xB0\xA4\xB0\xA4\x01";
        case 173:
            return "\x76\x07\x76\xA7\x76\xA7\x00";
        case 174:
            return "\x8A\x07\x8A\xA7\x8A\xA7\x01";
        case 175:
            return "\x95\x07\x95\xA7\x95\xA7\x01";
        case 176:
            return "\x20\x09\x20\xB9\x20\xB9\x00";
        case 177:
            return "\x3F\x09\x3F\xB9\x3F\xB9\x00";
        case 178:
            return "\xC3\x09\xC3\xB9\xC3\xB9\x01";
        case 179:
            return "\xDC\x09\xDC\xB9\xDC\xB9\x01";
        case 180:
            return "\x05\x0A\x05\xBA\x05\xBA\x00";
        case 181:
            return "\x1A\x0A\x1A\xBA\x1A\xBA\x00";
        case 182:
            return "\xE6\x0A\xE6\xBA\xE6\xBA\x01";
        case 183:
            return "\xF9\x0A\xF9\xBA\xF9\xBA\x01";
        case 184:
            return "\x6A\x0C\x6A\xBC\x6A\xBC\x00";
        case 185:
            return "\x75\x0C\x75\xBC\x75\xBC\x00";
        case 186:
            return "\x89\x0C\x89\xBC\x89\xBC\x01";
        case 187:
            return "\x96\x0C\x96\xBC\x96\xBC\x01";
        case 188:
            return "\x4F\x0F\x4F\xBF\x4F\xBF\x00";
        case 189:
            return "\x50\x0F\x50\xBF\x50\xBF\x00";
        case 190:
            return "\xAC\x0F\xAC\xBF\xAC\xBF\x01";
        case 191:
            return "\xB3\x0F\xB3\xBF\xB3\xBF\x01";
        case 192:
            return "\x66\x00\x66\xC0\x66\xC0\x00";
        case 193:
            return "\x79\x00\x79\xC0\x79\xC0\x00";
        case 194:
            return "\x85\x00\x85\xC0\x85\xC0\x01";
        case 195:
            return "\x9A\x00\x9A\xC0\x9A\xC0\x01";
        case 196:
            return "\x43\x03\x43\xC3\x43\xC3\x00";
        case 197:
            return "\x5C\x03\x5C\xC3\x5C\xC3\x00";
        case 198:
            return "\xA0\x03\xA0\xC3\xA0\xC3\x01";
        case 199:
            return "\xBF\x03\xBF\xC3\xBF\xC3\x01";
        case 200:
            return "\x2C\x05\x2C\xC5\x2C\xC5\x00";
        case 201:
            return "\x33\x05\x33\xC5\x33\xC5\x00";
        case 202:
            return "\xCF\x05\xCF\xC5\xCF\xC5\x01";
        case 203:
            return "\xD0\x05\xD0\xC5\xD0\xC5\x01";
        case 204:
            return "\x09\x06\x09\xC6\x09\xC6\x00";
        case 205:
            return "\x16\x06\x16\xC6\x16\xC6\x00";
        case 206:
            return "\xEA\x06\xEA\xC6\xEA\xC6\x01";
        case 207:
            return "\xF5\x06\xF5\xC6\xF5\xC6\x01";
        case 208:
            return "\x84\x00\x84\xD0\x84\xD0\x01";
        case 209:
            return "\x85\x0F\x85\xDF\x85\xDF\x01";
        case 210:
            return "\x8A\x03\x8A\xD3\x8A\xD3\x01";
        case 211:
            return "\x8B\x0C\x8B\xDC\x8B\xDC\x01";
        case 212:
            return "\xB6\x05\xB6\xD5\xB6\xD5\x01";
        case 213:
            return "\xB7\x0A\xB7\xDA\xB7\xDA\x01";
        case 214:
            return "\xB8\x06\xB8\xD6\xB8\xD6\x01";
        case 215:
            return "\xB9\x09\xB9\xD9\xB9\xD9\x01";
        case 216:
            return "\xD0\x0A\xD0\xDA\xD0\xDA\x01";
        case 217:
            return "\xD1\x05\xD1\xD5\xD1\xD5\x01";
        case 218:
            return "\xDE\x09\xDE\xD9\xDE\xD9\x01";
        case 219:
            return "\xDF\x06\xDF\xD6\xDF\xD6\x01";
        case 220:
            return "\xE2\x0F\xE2\xDF\xE2\xDF\x01";
        case 221:
            return "\xE3\x00\xE3\xD0\xE3\xD0\x01";
        case 222:
            return "\xEC\x0C\xEC\xDC\xEC\xDC\x01";
        case 223:
            return "\xED\x03\xED\xD3\xED\xD3\x01";
        case 224:
            return "\x2D\x08\x2D\xE8\x2D\xE8\x00";
        case 225:
            return "\x32\x08\x32\xE8\x32\xE8\x00";
        case 226:
            return "\xCE\x08\xCE\xE8\xCE\xE8\x01";
        case 227:
            return "\xD1\x08\xD1\xE8\xD1\xE8\x01";
        case 228:
            return "\x08\x0B\x08\xEB\x08\xEB\x00";
        case 229:
            return "\x17\x0B\x17\xEB\x17\xEB\x00";
        case 230:
            return "\xEB\x0B\xEB\xEB\xEB\xEB\x01";
        case 231:
            return "\xF4\x0B\xF4\xEB\xF4\xEB\x01";
        case 232:
            return "\x67\x0D\x67\xED\x67\xED\x00";
        case 233:
            return "\x78\x0D\x78\xED\x78\xED\x00";
        case 234:
            return "\x84\x0D\x84\xED\x84\xED\x01";
        case 235:
            return "\x9B\x0D\x9B\xED\x9B\xED\x01";
        case 236:
            return "\x42\x0E\x42\xEE\x42\xEE\x00";
        case 237:
            return "\x5D\x0E\x5D\xEE\x5D\xEE\x00";
        case 238:
            return "\xA1\x0E\xA1\xEE\xA1\xEE\x01";
        case 239:
            return "\xBE\x0E\xBE\xEE\xBE\xEE\x01";
        case 240:
            return "\x0B\x00\x0B\xF0\x0B\xF0\x00";
        case 241:
            return "\x14\x00\x14\xF0\x14\xF0\x00";
        case 242:
            return "\xE8\x00\xE8\xF0\xE8\xF0\x01";
        case 243:
            return "\xF7\x00\xF7\xF0\xF7\xF0\x01";
        case 244:
            return "\x2E\x03\x2E\xF3\x2E\xF3\x00";
        case 245:
            return "\x31\x03\x31\xF3\x31\xF3\x00";
        case 246:
            return "\xCD\x03\xCD\xF3\xCD\xF3\x01";
        case 247:
            return "\xD2\x03\xD2\xF3\xD2\xF3\x01";
        case 248:
            return "\x41\x05\x41\xF5\x41\xF5\x00";
        case 249:
            return "\x5E\x05\x5E\xF5\x5E\xF5\x00";
        case 250:
            return "\xA2\x05\xA2\xF5\xA2\xF5\x01";
        case 251:
            return "\xBD\x05\xBD\xF5\xBD\xF5\x01";
        case 252:
            return "\x64\x06\x64\xF6\x64\xF6\x00";
        case 253:
            return "\x7B\x06\x7B\xF6\x7B\xF6\x00";
        case 254:
            return "\x87\x06\x87\xF6\x87\xF6\x01";
        case 255:
            return "\x98\x06\x98\xF6\x98\xF6\x01";
        default:
            printf("\nERROR: Motorola Basic Key %d is unknown.\n", i);
            return "ERROR MESSAGE";
    }
}

/* This hook intercepts calls to an initial AES check at startup that
   might (or might not) be related to the ALPU-MP chip.

   Is not related to the ALPU-MP chip (20160720ae)
   ALPU-MP "lib" has his own ?-Crypto
*/
void aes_startup_check_hook() {
#ifdef CONFIG_AES
    printf("Performing AES startup check.\n");
    // Call back to the original function.
    /* int *toret= */ aes_startup_check();

    /* aes_startup_check() will set the byte at 0x2001d39b to 0x42,
       which is then checked repeatedly elsewhere in the code, causing
       mysterious things to fail.  If we find that this check has failed,
       let's force it back the other way and hope for the best.
    */
    if (*((char*)0x2001d39b) != 0x42) {
        printf("Startup AES check failed.  Attempting to forge the results.\n");
        printf("*0x2001d39b = 0x%02x\n", *((char*)0x2001d39b));

        // Force the correct value.
        *((char*)0x2001d39b) = 0x42;
    }
#endif // CONFIG_AES
}

/* This hook intercepts calls to aes_loadkey(), so that AES keys can
   be printed to the dmesg log.
*/
char* aes_loadkey_hook(char* key) {
    // Print the key that we are to load.
    printf("aes_loadkey (0x%08x): ", key);
    printhex(key, 16);
    printf("\n");

    key = aes_loadkey(key);
    return key;
}

/* This hook intercepts calls to aes_cipher(), which is used to turn
   the 128-bit Enhanced Privacy Key into a 49-bit sequence that gets
   XORed with the audio before transmission and after reception.

   By changing the output to match Motorola's Basic Privacy, we can
   patch the MD380 to be compatible with a Motorola network.

   The function is also used for two startup checks, presumably
   related to the ALPU-MP copy protection chip.  If those checks are
   interfered with, the radio will boot to a blank white screen.
 */
char* aes_cipher_hook(char* pkt) {
    char* res;
    int   i, sum = 0;
    printf("aes_cipher(0x%08x);\nIN  :", pkt);
    printhex(pkt, 16); // Print the Enhanced Privacy Key

    // Sum all the the first byte, looking for near-empty keys.
    for (i = 1; i < 16; i++)
        sum |= pkt[i];
    if (!sum) {
        printf("\nHooking keystream for Motorola Basic Privacy compatibility.\n");
        memcpy(pkt, getmotorolabasickey(pkt[0]), 7);
        printhex(pkt, 16); // Print the keystream it produces. (First 49 bits are XOR'ed with the audio.)
        printf("\n");
        return pkt;
    }

    /* The key has more than its least-significant byte set, so we'll
       use the original Tytera algorithm.  At some point, it might make
       sense to replace this with proper crypto, rather than XOR.
    */
    printf("\nOUT :");
    res = aes_cipher(pkt);
    printhex(res, 16); // Print the keystream it produces. (First 49 bits are XOR'ed with the audio.)
    printf("\n");
    return res;
}
