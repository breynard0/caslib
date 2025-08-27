#include "letters.h"
#include "draw.h"
#include "dutils.h"

struct DrawData
{
    short x;
    short y;
    short size;
    short buf_width;
};

// clang-format off
// const char LET[8] = { 
//     0b
// };

const char LET0[8] = { 
    0b00111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00111100
};

const char LET1[8] = { 
    0b00011000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
};

const char LET2[8] = { 
    0b01111110,
    0b00000010,
    0b00000010,
    0b01111110,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01111110,
};

const char LET3[8] = { 
    0b01111110,
    0b00000010,
    0b00000010,
    0b01111110,
    0b00000010,
    0b00000010,
    0b00000010,
    0b01111110,
};

const char LET4[8] = { 
    0b00000110,
    0b00001010,
    0b00010010,
    0b00100010,
    0b00111110,
    0b00000010,
    0b00000010,
    0b00000010,
};

const char LET5[8] = { 
    0b01111110,
    0b01000000,
    0b01000000,
    0b01111110,
    0b00000010,
    0b00000010,
    0b00000010,
    0b01111110,
};

const char LET6[8] = { 
    0b01111110,
    0b01000000,
    0b01000000,
    0b01111110,
    0b01000010,
    0b01000010,
    0b01000010,
    0b01111110,
};

const char LET7[8] = { 
    0b01111110,
    0b00000010,
    0b00000010,
    0b00000100,
    0b00001000,
    0b00010000,
    0b00100000,
    0b00100000,
};

const char LET8[8] = { 
    0b00111100,
    0b01000010,
    0b01000010,
    0b00111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00111100,
};

const char LET9[8] = { 
    0b00111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00111110,
    0b00000010,
    0b00000100,
    0b00111000,
};

const char LETPI[8] = { 
  0b01111110,
  0b00100100,
  0b00100100,
  0b00100100,
  0b00100100,
  0b00100100,
  0b00100100,
  0b00100010,
};

const char LETDEG[8] = { 
  0b00111100,
  0b00100100,
  0b00100100,
  0b00111100,
  0b00000000,
  0b00000000,
  0b00000000,
  0b00000000,
};

const char LETROOT[8] = { 
    0b00111111,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b00100000,
    0b10100000,
    0b01100000,
};

const char LETS[8] = { 
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111110,
    0b01000000,
    0b00111100,
    0b00000010,
    0b01111100
};

const char LETI[8] = { 
    0b00000000,
    0b00000000,
    0b00010000,
    0b00000000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
};

const char LETN[8] = { 
    0b00000000,
    0b00000000,
    0b00000000,
    0b01111000,
    0b01000100,
    0b01000100,
    0b01000100,
    0b01000100,
};

const char LETC[8] = { 
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111110,
    0b01000000,
    0b01000000,
    0b01000000,
    0b00111110,
};

const char LETO[8] = { 
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111100,
    0b01000010,
    0b01000010,
    0b01000010,
    0b00111100,
};

const char LETT[8] = { 
    0b00000000,
    0b00000000,
    0b00010000,
    0b01111100,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00001100,
};

const char LETA[8] = { 
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111100,
    0b01000010,
    0b01000010,
    0b00111110,
    0b00000010,
};

const char LETL[8] = { 
    0b00000000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b000010000,
};

const char LETG[8] = { 
    0b00000000,
    0b00111110,
    0b01000010,
    0b01000010,
    0b00111110,
    0b00000010,
    0b00000100,
    0b00111000,
};

const char LETB[8] = { 
    0b00000000,
    0b01000000,
    0b01000000,
    0b01000000,
    0b01111100,
    0b01000010,
    0b01000010,
    0b01111100,
};

const char LETD[8] = { 
    0b00000000,
    0b00000010,
    0b00000010,
    0b00000010,
    0b00111110,
    0b01000010,
    0b01000010,
    0b00111110,
};

const char LETE[8] = { 
    0b00000000,
    0b00000000,
    0b00000000,
    0b00111100,
    0b01000010,
    0b01111100,
    0b01000000,
    0b00111100,
};

const char LETF[8] = { 
    0b00000000,
    0b00000000,
    0b00111110,
    0b01000000,
    0b01111100,
    0b01000000,
    0b01000000,
    0b01000000,
};

const char LETADD[8] = { 
    0b00000000,
    0b00000000,
    0b00001000,
    0b00001000,
    0b00111110,
    0b00001000,
    0b00001000,
    0b00000000,
};

const char LETSUB[8] = { 
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b01111110,
    0b00000000,
    0b00000000,
    0b00000000,
};

const char LETMUL[8] = { 
    0b00000000,
    0b00000000,
    0b00100100,
    0b00011000,
    0b00011000,
    0b00100100,
    0b00000000,
    0b00000000,
};

const char LETDIV[8] = { 
    0b00000010,
    0b00000100,
    0b00000100,
    0b00001000,
    0b00001000,
    0b00010000,
    0b00010000,
    0b00100000,
};

const char LETEXP[8] = { 
    0b00011000,
    0b00100100,
    0b01000010,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
};

const char LETBS[8] = { 
    0b00000100,
    0b00001000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00010000,
    0b00001000,
    0b00000100,
};

const char LETBE[8] = { 
    0b00100000,
    0b00010000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00001000,
    0b00010000,
    0b00100000,
};

const char LETEQ[8] = { 
    0b00000000,
    0b00000000,
    0b00000000,
    0b01111110,
    0b00000000,
    0b01111110,
    0b00000000,
    0b00000000,
};

const char LETDOT[8] = { 
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00000000,
    0b00011000,
    0b00011000,
};

const char LETQUES[8] = { 
  0b00111100,
  0b01000010,
  0b00000010,
  0b00000100,
  0b00001000,
  0b00001000,
  0b00000000,
  0b00001000,
};

const char LETX[8] = { 
    0b00000000,
    0b00000000,
    0b01000100,
    0b00101000,
    0b00010000,
    0b00101000,
    0b01000100,
};

const char LETY[8] = { 
    0b00000000,
    0b00100010,
    0b00100010,
    0b00011110,
    0b00000010,
    0b00000100,
    0b00111000,
};

const char LETW[8] = { 
    0b00000000,
    0b00000000,
    0b00000000,
    0b01010100,
    0b01010100,
    0b01010100,
    0b01010100,
    0b00101000,
};

const char LETU[8] = { 
  0b00000000,
  0b00000000,
  0b00000000,
  0b01000010,
  0b01000010,
  0b01000010,
  0b00111110,
  0b00000010,
};

const char LETM[8] = { 
  0b00000000,
  0b00000000,
  0b00000000,
  0b01000000,
  0b01111110,
  0b01010010,
  0b01010010,
  0b01010010,
};

const char LETR[8] = {
  0b00000000,
  0b00000000,
  0b00000000,
  0b00100000,
  0b00111100,
  0b00100010,
  0b00100000,
  0b00100000,
};

// clang-format on

void draw_bitmap(const char bitmap[8], struct DrawData data, char* buffer)
{
    short x = data.x;
    short y = data.y;
    short size = data.size;
    short res = size / 8;
    short buf_width = data.buf_width;

    for (int i = 0; i < 8; i++)
    {
        for (int j = 0; j < 8; j++)
        {
            short pix_state = bitmap[i] & (0b10000000 >> j);
            for (int k = 0; k < res; k++)
            {
                for (int l = 0; l < res; l++)
                {
                    if (pix_state)
                    {
                        set_pixel_on(x + (j * res) + k, y + (i * res) + l, buffer,
                                     buf_width);
                    }
                }
            }
        }
    }
}

void draw_letter(char letter, short x, short y, short size, char* buffer,
                 short buf_width)
{
    struct DrawData data = {};
    data.x = x;
    data.y = y;
    data.size = size;
    data.buf_width = buf_width;

    switch (letter)
    {
    case '0':
        draw_bitmap(LET0, data, buffer);
        break;
    case '1':
        draw_bitmap(LET1, data, buffer);
        break;
    case '2':
        draw_bitmap(LET2, data, buffer);
        break;
    case '3':
        draw_bitmap(LET3, data, buffer);
        break;
    case '4':
        draw_bitmap(LET4, data, buffer);
        break;
    case '5':
        draw_bitmap(LET5, data, buffer);
        break;
    case '6':
        draw_bitmap(LET6, data, buffer);
        break;
    case '7':
        draw_bitmap(LET7, data, buffer);
        break;
    case '8':
        draw_bitmap(LET8, data, buffer);
        break;
    case '9':
        draw_bitmap(LET9, data, buffer);
        break;
    case '+':
        draw_bitmap(LETADD, data, buffer);
        break;
    case '-':
        draw_bitmap(LETSUB, data, buffer);
        break;
    case '*':
        draw_bitmap(LETMUL, data, buffer);
        break;
    case '/':
        draw_bitmap(LETDIV, data, buffer);
        break;
    case '^':
        draw_bitmap(LETEXP, data, buffer);
        break;
    case '(':
        draw_bitmap(LETBS, data, buffer);
        break;
    case ')':
        draw_bitmap(LETBE, data, buffer);
        break;
    case '=':
        draw_bitmap(LETEQ, data, buffer);
        break;
    case '.':
        draw_bitmap(LETDOT, data, buffer);
        break;
    case '?':
        draw_bitmap(LETQUES, data, buffer);
        break;
    case '@':
        draw_bitmap(LETPI, data, buffer);
        break;
    case '%':
        draw_bitmap(LETDEG, data, buffer);
        break;
    case '#':
        draw_bitmap(LETROOT, data, buffer);
        break;
    case 's':
        draw_bitmap(LETS, data, buffer);
        break;
    case 'i':
        draw_bitmap(LETI, data, buffer);
        break;
    case 'n':
        draw_bitmap(LETN, data, buffer);
        break;
    case 'c':
        draw_bitmap(LETC, data, buffer);
        break;
    case 'o':
        draw_bitmap(LETO, data, buffer);
        break;
    case 't':
        draw_bitmap(LETT, data, buffer);
        break;
    case 'a':
        draw_bitmap(LETA, data, buffer);
        break;
    case 'l':
        draw_bitmap(LETL, data, buffer);
        break;
    case 'g':
        data.y += 3;
        draw_bitmap(LETG, data, buffer);
        break;
    case 'b':
        draw_bitmap(LETB, data, buffer);
        break;
    case 'd':
        draw_bitmap(LETD, data, buffer);
        break;
    case 'e':
        draw_bitmap(LETE, data, buffer);
        break;
    case 'f':
        draw_bitmap(LETF, data, buffer);
        break;
    case 'x':
        draw_bitmap(LETX, data, buffer);
        break;
    case 'y':
        data.y += 3;
        draw_bitmap(LETY, data, buffer);
        break;
    case 'w':
        draw_bitmap(LETW, data, buffer);
        break;
    case 'u':
        draw_bitmap(LETU, data, buffer);
        break;
    case 'm':
        draw_bitmap(LETM, data, buffer);
        break;
    case 'r':
        draw_bitmap(LETR, data, buffer);
        break;
    default:
        break;
    }
}