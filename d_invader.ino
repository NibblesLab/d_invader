#include <M5Stack.h>

#define KEYBOARD_I2C_ADDR     0X88
#define KEYBOARD_INT          5

#define YPOS 75

/*
 *  Valiables
 */
static xQueueHandle keyboard_queue = NULL;
long acc_b;     // Accumrator
int acc_e;
int inp_b;      // Input buffer
int inp_e;
int inp_d;      // Inputed digit remain
int inp_s;      // Inputting value's sign
long pmem_b;    // Permanent memory
int pmem_e;
long mem_b;     // Memory
int mem_e;
int mem_s;
int g_hiscore;        // Hi-score
int g_score;          // Score
uint8_t opr;          // Operation to calcurate
bool inp_mode;        // Input mode false:integer true:under decimal point
bool g_mode;          // Game mode false:calc true:game
bool g_fire;          // Fire flag false:not fire true:fire
bool ufo_flag;        // UFO appear flag false:next enemy is normal true:next enemy is UFO
bool go_flag;         // Game Over flag false:in play true:end of game
bool en_keys;         // Key input enable false:not key-in true:key-in
bool err_flag;        // Error flag false:normal true:in error mode
bool acc_empty_flag;  // Empty accumrator flag false:not empty true:empty
uint8_t g_phase;      // Game proceeding
uint8_t g_aim;        // Aiming number
uint8_t g_remain;     // Remain my deffence line
uint8_t g_enc;        // Playing encounter
uint8_t g_stage;      // Playing stage
uint8_t g_sum;        // Sum for UFO appear flag
uint8_t g_enemy[6];   // Displayed enemy
uint8_t g_enum;       // Appeared enemy
uint8_t g_shot;       // Shot(both hit and miss) counter in encounter
uint8_t g_shoot;      // Shoot counter in encounter
unsigned long g_elapse; // Next action time

/*
 * Waves
 */
uint8_t w_hit[1080];    // Hit enemy (44.1k)
uint8_t w_miss[1080];   // Miss (44.1k)
uint8_t w_ufo[14300];   // Hit UFO (44.1k)
uint8_t w_win[16000];   // Clear encounter (8k)
uint8_t w_lose[16000];  // Lose encounter (8k)
uint8_t w_gover[16000]; // Game over (8k)

/*
 * Graphics
 */
extern unsigned short digit8[15][2450]; // Digits
extern unsigned short dot[25];          // Decimal-point
extern unsigned short minus[2][144];    // Minus for most-left indicator
extern unsigned short i_add[2][400];    // Add indicator
extern unsigned short i_sub[2][400];    // Subtract indicator
extern unsigned short i_mul[2][400];    // Multiple indicator
extern unsigned short i_div[2][400];    // Divide indicator
extern unsigned short memory[2][486];   // Memory indicator
extern unsigned short err[2][486];      // Error indicator

/*
 * Keyboard interrupt handler
 */
static void IRAM_ATTR keyboard_cb() {
  uint8_t key_val;
  Wire.requestFrom(KEYBOARD_I2C_ADDR, 1);  // request 1 byte from keyboard
  key_val = Wire.read();                  // receive a byte as character
  if(key_val != 0) {
    xQueueSendFromISR(keyboard_queue, &key_val, NULL);
  }
}

/*
 * Prepare sound effect(UFO) sub
 */
void set_wave_ufo_sub(int pos, int h1, int l1, int h2, int l2, int hi, int lo)
{
  int p = 0, rep;

  while(1)
  {
    rep = p + h1 >= 1100 ? 1100 - p : h1;
    memset(&w_ufo[pos+p], hi, rep);
    p += rep;
    if(p == 1100) return;
    rep = p + l1 >= 1100 ? 1100 - p : l1;
    memset(&w_ufo[pos+p], lo, rep);
    p += rep;
    if(p == 1100) return;
    rep = p + h2 >= 1100 ? 1100 - p : h2;
    memset(&w_ufo[pos+p], hi, rep);
    p += rep;
    if(p == 1100) return;
    rep = p + l2 >= 1100 ? 1100 - p : l2;
    memset(&w_ufo[pos+p], lo, rep);
    p += rep;
    if(p == 1100) return;
  }
}

/*
 * Prepare sound effect
 */
void set_wave()
{
  int i, j;

  // Hit
  for(i = 0; i < 1080; i+=36)
  {
    memset(&w_hit[i], 21, 18);
    memset(&w_hit[i+18], 1, 18);
  }
  w_hit[1079] = 0;

  // Miss
  for(i = 0; i < 1040; i+=80)
  {
    memset(&w_miss[i], 21, 40);
    memset(&w_miss[i+40], 1, 40);
  }
  memset(&w_miss[1040], 1, 40);
  w_miss[1079] = 0;

  // Shoot UFO
  set_wave_ufo_sub(0, 10, 10, 10, 9, 138, 118);
  set_wave_ufo_sub(1100, 10, 10, 10, 10, 138, 118);
  set_wave_ufo_sub(2200, 11, 10, 10, 10, 138, 118);
  set_wave_ufo_sub(3300, 11, 10, 11, 10, 138, 118);
  set_wave_ufo_sub(4400, 11, 11, 11, 10, 138, 118);
  set_wave_ufo_sub(5500, 11, 11, 11, 11, 138, 118);
  set_wave_ufo_sub(6600, 12, 11, 11, 11, 138, 118);
  set_wave_ufo_sub(7700, 12, 11, 12, 11, 138, 118);
  set_wave_ufo_sub(8800, 12, 12, 12, 11, 138, 118);
  set_wave_ufo_sub(9900, 12, 12, 12, 12, 138, 118);
  set_wave_ufo_sub(11000, 13, 12, 12, 12, 138, 118);
  set_wave_ufo_sub(12100, 13, 12, 13, 12, 138, 118);
  set_wave_ufo_sub(13200, 13, 13, 13, 12, 138, 118);
  w_ufo[13198] = 1;
  w_ufo[13199] = 0;

  // Encount clear
  for(i = 0; i < 199; i+=6)
  {
    w_win[i] = w_win[i+1] = w_win[i+2] = 138;
    w_win[i+3] = w_win[i+4] = w_win[i+5] = 118;
  }
  for(i = 199; i < 797; i++)
  {
    w_win[i] = 128;
  }
  for(i = 797; i < 2345; i+=6)
  {
    w_win[i] = w_win[i+1] = w_win[i+2] = 138;
    w_win[i+3] = w_win[i+4] = w_win[i+5] = 118;
  }
  for(i = 2345; i < 3912; i++)
  {
    w_win[i] = 128;
  }
  for(i = 3912; i < 7020; i+=6)
  {
    w_win[i] = w_win[i+1] = w_win[i+2] = 138;
    w_win[i+3] = w_win[i+4] = w_win[i+5] = 118;
  }
  for(i = 7020; i < 8587; i++)
  {
    w_win[i] = 128;
  }
  for(i = 8587; i < 11695; i+=6)
  {
    w_win[i] = w_win[i+1] = w_win[i+2] = 138;
    w_win[i+3] = w_win[i+4] = w_win[i+5] = 118;
  }
  for(i = 11695; i < 13262; i++)
  {
    w_win[i] = 128;
  }
  for(i = 13262; i < 14810; i+=6)
  {
    w_win[i] = w_win[i+1] = w_win[i+2] = 138;
    w_win[i+3] = w_win[i+4] = w_win[i+5] = 118;
  }
  for(i = 14810; i < 15998; i++)
  {
    w_win[i] = 128;
  }
  w_win[15998] = 1;
  w_win[15999] = 0;

  // Encount lose
  for(i = 0; i < 14024; i+=10)
  {
    w_lose[i] = w_lose[i+1] = w_lose[i+2] = w_lose[i+3] = w_lose[i+4] = 138;
    w_lose[i+5] = w_lose[i+6] = w_lose[i+7] = w_lose[i+8] = w_lose[i+9] = 118;
  }
  w_lose[14020] = w_lose[14021] = w_lose[14022] = w_lose[14023] = 138;
  for(i = 14024; i < 15998; i++)
  {
    w_lose[i] = 128;
  }
  w_lose[15998] = 1;
  w_lose[15999] = 0;

  // Game over
  for(i = 0; i < 1680; i+=6)
  {
    w_gover[i] = w_gover[i+1] = 138;
    w_gover[i+2] = 118;
    w_gover[i+3] = 138;
    w_gover[i+4] = w_gover[i+5] = 118;
  }
  for(i = 1680; i < 3253; i++)
  {
    w_gover[i] = 128;
  }
  for(i = 3253; i < 4807; i+=6)
  {
    w_gover[i] = w_gover[i+1] = w_gover[i+2] = 138;
    w_gover[i+3] = w_gover[i+4] = w_gover[i+5] = 118;
  }
  for(i = 4807; i < 4821; i++)
  {
    w_gover[i] = 128;
  }
  for(i = 4821; i < 6357; i+=6)
  {
    w_gover[i] = w_gover[i+1] = 138;
    w_gover[i+2] = 118;
    w_gover[i+3] = 138;
    w_gover[i+4] = w_gover[i+5] = 118;
  }
  for(i = 6357; i < 7931; i++)
  {
    w_gover[i] = 128;
  }
  for(i = 7931; i < 9485; i+=6)
  {
    w_gover[i] = w_gover[i+1] = w_gover[i+2] = 138;
    w_gover[i+3] = w_gover[i+4] = w_gover[i+5] = 118;
  }
  for(i = 9485; i < 9499; i++)
  {
    w_gover[i] = 128;
  }
  for(i = 9499; i < 11035; i+=6)
  {
    w_gover[i] = w_gover[i+1] = 138;
    w_gover[i+2] = 118;
    w_gover[i+3] = 138;
    w_gover[i+4] = w_gover[i+5] = 118;
  }
  for(i = 11035; i < 12609; i++)
  {
    w_gover[i] = 128;
  }
  for(i = 12609; i < 14163; i+=6)
  {
    w_gover[i] = w_gover[i+1] = w_gover[i+2] = 138;
    w_gover[i+3] = w_gover[i+4] = w_gover[i+5] = 118;
  }
  for(i = 14163; i < 15998; i++)
  {
    w_gover[i] = 128;
  }
  w_gover[15998] = 1;
  w_gover[15999] = 0;
}

/*
 * Initialize
 */
void setup()
{
  M5.begin();
  Wire.begin();
  M5.Lcd.setTextFont(0);
  M5.Lcd.setTextSize(2);
  M5.Speaker.setVolume(1);

  //create a queue to handle gpio event from isr
  keyboard_queue = xQueueCreate(10, sizeof(uint8_t));
  pinMode(KEYBOARD_INT, INPUT_PULLUP);
  attachInterrupt(KEYBOARD_INT, keyboard_cb, FALLING);

  set_wave();
  inp_b = 0;
  inp_e = 0;
  inp_d = 8;
  opr = 0;
  inp_s = 1;
  inp_mode = false;
  mem_b = 0;
  mem_e = 0;
  mem_s = 1;
  err_flag = false;
  acc_empty_flag = true;
  g_mode = false;
  g_hiscore = 0;
  g_fire = false;
  M5.Lcd.fillScreen(0x0000);
  M5.Lcd.fillRoundRect(0, YPOS-10, 320, 110, 5, 0xC6D4);
  M5.Lcd.setTextColor(TFT_DARKCYAN, TFT_BLACK);
  M5.Lcd.setCursor(44, 223);
  M5.Lcd.printf("GAME");
  M5.Lcd.setCursor(143, 223);
  M5.Lcd.printf("AIM");
  M5.Lcd.setCursor(232, 223);
  M5.Lcd.printf("FIRE");
  disp_num(0,0);
}

/*
 * Display with string
 */
void disp(char *str, int val_e)
{
  uint8_t c;
  int i;

  for(i = 1; i <= 8; i++)
  {
    if(str[10-i] == ' ')
    {
      c = 0;
    }
    else if(str[10-i] == '-')
    {
      c = 12;
    }
    else
    {
      c = str[10-i] - 0x30;
      c++;
    }
    M5.Lcd.drawBitmap((9-i)*35+3, YPOS+20, 35, 70, (uint16_t *)digit8[c]);
  }
  M5.Lcd.drawBitmap(9, YPOS+52, 24, 6, (uint16_t *)minus[str[1]=='-'?0:1]);
  if(!g_mode) // normal mode
  {
    M5.Lcd.drawBitmap((8-val_e)*35+32, YPOS+82, 5, 5, (uint16_t *)dot);
    M5.Lcd.drawBitmap(10, YPOS+29, 27, 18, (uint16_t *)memory[(mem_b!=0||mem_e!=0)?0:1]);
    M5.Lcd.drawBitmap(9, YPOS+62, 27, 18, (uint16_t *)err[err_flag?0:1]);
    M5.Lcd.drawBitmap(239, YPOS, 20, 20, (uint16_t *)i_add[opr == '+'?0:1]);
    M5.Lcd.drawBitmap(259, YPOS, 20, 20, (uint16_t *)i_sub[opr == '-'?0:1]);
    M5.Lcd.drawBitmap(279, YPOS, 20, 20, (uint16_t *)i_mul[opr == '*'?0:1]);
    M5.Lcd.drawBitmap(299, YPOS, 20, 20, (uint16_t *)i_div[opr == '/'?0:1]);
    M5.Lcd.setTextColor(0xDE92, 0xC6D4);
    M5.Lcd.setCursor(33, YPOS+4);
    M5.Lcd.printf("GAME");
    M5.Lcd.setCursor(85, YPOS+4);
    M5.Lcd.printf("OVER");
  }
  else  // game mode
  {
    M5.Lcd.drawBitmap(10, YPOS+29, 27, 18, (uint16_t *)memory[1]);
    M5.Lcd.drawBitmap(9, YPOS+62, 27, 18, (uint16_t *)err[1]);
    M5.Lcd.drawBitmap(239, YPOS, 20, 20, (uint16_t *)i_add[1]);
    M5.Lcd.drawBitmap(259, YPOS, 20, 20, (uint16_t *)i_sub[1]);
    M5.Lcd.drawBitmap(279, YPOS, 20, 20, (uint16_t *)i_mul[1]);
    M5.Lcd.drawBitmap(299, YPOS, 20, 20, (uint16_t *)i_div[1]);
    if(go_flag)
    {
      M5.Lcd.setTextColor(0x0000, 0xC6D4);
    }
    else
    {
      M5.Lcd.setTextColor(0xDE92, 0xC6D4);
    }
    M5.Lcd.setCursor(33, YPOS+4);
    M5.Lcd.printf("GAME");
    M5.Lcd.setCursor(85, YPOS+4);
    M5.Lcd.printf("OVER");
  }
}

/*
 * Prepare strings for display
 */
void disp_num(int val_b, int val_e)
{
  char str[12];
  uint8_t i;

  sprintf(str, "%10d", val_b);
  if(val_e != 0)
  {
    for(i = 9; i > 9-val_e; i--)
    {
      if(str[i-1] == ' ')
      {
        str[i-1] = '0';
      }
      else if(str[i-1] == '-')
      {
        str[i-1] = '0';
        str[i-2] = '-';
      }
    }
  }
  disp(str, val_e);
}

/*
 * Count digits for accmulator
 */
uint8_t numDigitacc()
{
  char str[18];
  uint8_t i, num = 0;

  sprintf(str, "%17d", acc_b);
  for(i = 16; i > 0; i--)
  {
    if(isDigit(str[i]))
    {
      num++;
    }
    else
    {
      return num;
    }
  }
}

/*
 * Adjustment for accumlator
 */
void adjust_acc()
{
  char str[18];
  int i, k;

  if(acc_e > 0)
  {
    sprintf(str, "%17d", acc_b);
    for(i = 17; i > 0; i--)
    {
      if(str[i] == '0')
      {
        str[i] = 0;
        acc_e--;
        if(acc_e == 0)
        {
          break;
        }
      }
      else if(str[i] > '0')
      {
        break;
      }
    }
    acc_b = atoi(str);
  }
  k = numDigitacc();
  if(k-acc_e > 8) // overflow
  {
    err_flag = true;
    return;
  }
  if(acc_e == 8)
  {
    sprintf(str, "%17d", acc_b);
    str[16] = 0;
    acc_e--;
    k--;
    acc_b = atoi(str);
  }
  if(k > 8)
  {
    sprintf(str, "%17d", acc_b);
    str[17-(k-8)] = 0;
    acc_e -= (k-8);
    acc_b = atoi(str);
  }
  return;
}

/*
 * Calcuration accumlator and input buffer
 */
void calc(uint8_t lopr)
{
  uint8_t t;

  switch(lopr)
  {
    case '+':
    case '-':
      if(opr == '%')
      {
        inp_b = acc_b;
        inp_e = acc_e;
        acc_b = pmem_b;
        acc_e = pmem_e;
      }
      if(acc_e > inp_e)
      {
        inp_b *= pow(10, acc_e-inp_e);
        inp_e = acc_e;
      }
      else if(acc_e < inp_e)
      {
        acc_b *= pow(10, inp_e-acc_e);
        acc_e = inp_e;
      }
      if(lopr == '-')
      {
        inp_b = -inp_b;
      }
      acc_b += inp_b;
      adjust_acc();
      break;
    case '*':
      acc_b *= inp_b;
      acc_e += inp_e;
      adjust_acc();
      break;
    case '/':
      if(inp_b == 0)
      {
        err_flag = true;
        acc_b = 0;
        acc_e = 0;
      }
      else
      {
        t = numDigitacc()-1;
        acc_b *= pow(10, 8-t);
        acc_e += 8-t;
        acc_b /= inp_b;
        acc_e -= inp_e;
        adjust_acc();
      }
      break;
    default:
      return;
  }
}

/*
 * Loop
 */
void loop()
{
  uint8_t key_val, i, p;
  long qmem_b;
  int qmem_e;
  char str[12];
  unsigned long tt;

  if(!g_mode) // normal mode
  {
    if(xQueueReceive(keyboard_queue, &key_val, 0)) // Non-blocking
    {
      if(key_val > 0x2F && key_val < 0x3A && !err_flag) // num keys
      {
        if(!err_flag)
        {
        
        }
        if(opr == '=')
        {
          acc_b = 0;
          acc_e = 0;
          opr = 0;
        }
        if(inp_d > 0)
        {
          inp_b = inp_b * 10 + key_val - 0x30;
          if(inp_b != 0 || inp_mode)
          {
            inp_d--;
          }
          if(inp_mode)
          {
            inp_e++;
          }
        }
        disp_num(inp_b*inp_s, inp_e);
      }
      else if(key_val == '.' && !err_flag)
      {
        inp_mode = true;
        if(inp_b == 0)
        {
          inp_d--;
        }
        disp_num(inp_b*inp_s, inp_e);
      }
      else if(key_val == 96)
      {
        if(inp_b != 0)
        {
          inp_s = -1;
          disp_num(inp_b*inp_s, inp_e);
        }
      }
      else if(key_val == 'A')
      {
        if((inp_b == 0 && inp_e == 0 && inp_d == 8) || err_flag)  // all clear
        {
          inp_b = 0;
          inp_e = 0;
          inp_d = 8;
          inp_s = 1;
          inp_mode = false;
          opr = 0;
          acc_b = 0;
          acc_e = 0;
          mem_b = 0;
          mem_e = 0;
          mem_s = 1;
          err_flag = false;
          acc_empty_flag = true;
          disp_num(inp_b, inp_e);
        }
        else  // clear
        {
          inp_b = 0;
          inp_e = 0;
          inp_d = 8;
          disp_num(inp_b, inp_e);
        }
      }
      else if(key_val == '%' && !err_flag)
      {
        pmem_b = acc_b;
        pmem_e = acc_e;
        inp_b *= inp_s;
        switch(opr)
        {
          case '*':
            acc_b *= inp_b;
            acc_e += (2+inp_e);
            adjust_acc();
            disp_num(acc_b, acc_e);
            opr = key_val;
            break;
          case '/':
            calc(opr);
            acc_b *= 100;
            adjust_acc();
            disp_num(acc_b, acc_e);
            opr = 0;
            break;
          case '-':
            calc('/');
            acc_b *= 100;
            inp_b = 100;
            inp_e = 0;
            calc('-');
            adjust_acc();
            disp_num(acc_b, acc_e);
            opr = 0;
            break;
          case '+':
            qmem_b = acc_b;
            qmem_e = acc_e;
            acc_b = 100;
            acc_e = 0;
            calc('-');
            inp_b = acc_b;
            inp_e = acc_e;
            acc_b = qmem_b;
            acc_e = qmem_e;
            calc('/');
            acc_b *= 100;
            adjust_acc();
            disp_num(acc_b, acc_e);
            opr = key_val;
            qmem_b = pmem_b;
            qmem_e = pmem_e;
            pmem_b = acc_b;
            pmem_e = acc_e;
            acc_b = qmem_b;
            acc_e = qmem_e;
            break;
          default:
            break;
        }
      }
      else if(key_val == 'M' && !err_flag)
      {
        if(inp_b == 0 && inp_e == 0)
        {
          if(opr != 0||(acc_b == 0 && acc_e == 0))  // memory recall
          {
            inp_b = mem_b;
            inp_e = mem_e;
            inp_s = mem_s;
            inp_d = 0;
            disp_num(inp_b*inp_s, inp_e);
          }
          else  // integrate by accumlator
          {
            qmem_b = acc_b;
            qmem_e = acc_e;
            inp_b = mem_b*mem_s;
            inp_e = mem_e;
            calc('+');
            if(acc_b < 0)
            {
              mem_s = -1;
              acc_b *= -1;
            }
            mem_b = acc_b;
            mem_e = acc_e;
            acc_b = qmem_b;
            acc_e = qmem_e;
            disp_num(acc_b, acc_e);
          }
        }
        else if(opr == 0) // integrate by input buffer
        {
          acc_b = mem_b*mem_s;
          acc_e = mem_e;
          inp_b *= inp_s;
          calc('+');
          if(acc_b < 0)
          {
            mem_s = -1;
            acc_b *= -1;
          }
          inp_b *= inp_s;
          mem_b = acc_b;
          mem_e = acc_e;
          acc_b = 0;
          acc_e = 0;
          disp_num(inp_b*inp_s, inp_e);
        }
        else  // integrate by result of calcuration
        {
          inp_b *= inp_s;
          calc(opr);
          opr = 0;
          if(acc_b < 0)
          {
            mem_s = -1;
            acc_b *= -1;
          }
          mem_b = acc_b;
          mem_e = acc_e;
          acc_b *= mem_s;
          disp_num(acc_b, acc_e);
          inp_b = 0;
          inp_e = 0;
          inp_d = 8;
          inp_s = 1;
          inp_mode = false;
        }
      }
      else if(!err_flag)
      {
        if(acc_empty_flag)  // store value from input buffer to accumlator
        {
          acc_b = inp_b*inp_s;
          acc_e = inp_e;
          inp_b = 0;
          inp_e = 0;
          inp_d = 8;
          inp_s = 1;
          inp_mode = false;
          opr = key_val;
          acc_empty_flag = false;
          disp_num(acc_b, acc_e);
        }
        else
        {
          if(inp_b == 0 && inp_e == 0 && opr != 0 && key_val != '=')  // correct operation keys
          {
            opr = key_val;
            disp_num(acc_b, acc_e);
          }
          else  // calcurate
          {
            inp_b *= inp_s;
            if(opr == '%')
            {
              calc(key_val);
              opr = 0;
            }
            else
            {
              calc(opr);
              opr = key_val;
            }
            disp_num(acc_b, acc_e);
            inp_b = 0;
            inp_e = 0;
            inp_d = 8;
            inp_s = 1;
            inp_mode = false;
          }
        }
      }
    }
    if(M5.BtnA.wasPressed())  // start game
    {
      g_mode = true;
      g_phase = 0;
      g_score = 0;
      g_sum = 0;
      go_flag = false;
      g_elapse = millis() + 2000;
      sprintf(str, "    %06d", g_hiscore);
      disp(str, 0);
    }
    M5.update();
  }
  else  // game mode
  {
    switch(g_phase)
    {
      case 0: // display hi-score
        en_keys = false;
        tt = millis();
        if(g_elapse < tt)
        {
          g_phase = 1;
          g_elapse = tt + 2000;
          disp("     16-30", 0);
        }
        break;
      case 1: // display restriction
        en_keys = false;
        tt = millis();
        if(g_elapse < tt)
        {
          g_phase = 2;
          g_aim = '0';
          g_remain = 3;
          g_enc = 1;
          g_stage = 0;
          g_shot = 0;
          g_shoot = 0;
          g_sum = 0;
          g_enum = 1;
          g_fire = false;
          ufo_flag = false;
          g_enemy[0] = g_enemy[1] = g_enemy[2] = g_enemy[3] = g_enemy[4] = ' ';
          g_enemy[5] = random(0, 10) + 0x30;
          g_elapse = tt + 1200;
        }
        break;
      case 2: // playing
        en_keys = true;
        tt = millis();
        if(g_elapse < tt) // step of move enemies
        {
          if(g_enemy[0] != ' ') // lose this encounter
          {
            if(g_remain > 1)  // not game over
            {
              g_remain--;
              g_phase = 4;
              g_elapse = tt + 2000;
              sprintf(str, "    %06d", g_score);
              disp(str, 0);
            }
            else  // game over
            {
              g_phase = 5;
              str[0] = str[1] = ' ';
              str[2] = 0x30 + g_enc;
              str[3] = ';';
              sprintf(&str[4], "%06d", g_score);
              go_flag = true;
              disp(str, 0);
            }
            break;
          }
          else  // move enemies
          {
            for(i = 0; i < 5-g_stage; i++)
            {
              g_enemy[i] = g_enemy[i+1];
            }
            if(g_enum < 16) // not appeared last enemy
            {
              g_enum++;
              if(ufo_flag)
              {
                g_enemy[5-g_stage] = ':';
                ufo_flag = false;
              }
              else
              {
                g_enemy[5-g_stage] = random(0, 10) + 0x30;
              }
            }
            else  // no more enemies
            {
              g_enemy[5-g_stage] = ' ';
            }
            g_elapse = tt + (13-g_enc)*100;
          }
        }
        if(g_fire)  // fire
        {
          p = 99;
          for(i = 0; i < 6-g_stage; i++)  // search for appeared enemies
          {
            if(g_enemy[i] == g_aim) // HIT
            {
              p = i;
              g_score += (i + 1) * 10 * (g_stage+1);
              if(g_hiscore < g_score)
              {
                g_hiscore = g_score;
              }
              g_shoot++;
              if(g_aim == ':')  // hit UFO
              {
                g_score += 300;
                disp("          ", 0);
                M5.Speaker.playMusic(w_ufo, 44100);
                g_elapse += 325;
              }
              else  // hit enemy
              {
                g_sum += g_aim - 0x30;
                if(g_sum >= 10) // adjust sum
                {
                  g_sum -= 10;
                  if(g_sum == 0)  // UFO flag
                  {
                    ufo_flag = true;
                  }
                }
                M5.Speaker.playMusic(w_hit, 44100);
                g_elapse += 25;
              }
              break;
            }
          }
          if(g_shoot == 16) // clear this encounter
          {
            g_phase = 3;
            str[0] = str[1] = ' ';
            str[2] = 0x30 + g_enc;
            str[3] = ';';
            sprintf(&str[4], "%06d", g_score);
            disp(str, 0);
            g_elapse = tt + 2000;
            break;
          }
          if(p != 99) // hit enemy or UFO
          {
            for(i = p; i > 0; i--)  // delete hit enemy
            {
              g_enemy[i] = g_enemy[i-1];
            }
            g_enemy[0] = ' ';
          }
          else  // MISS
          {
            M5.Speaker.playMusic(w_miss, 44100);
            g_elapse += 25;
          }
          g_fire = false;
        }
        str[0] = str[1] = ' ';
        str[2] = g_aim;
        str[3] = ':' + g_remain;
        for(i = 0; i < 6; i++)
        {
          str[4+i] = g_enemy[i];
        }
        disp(str, 0);
        if(g_shot == 30)  // reach limit of shot?
        {
          g_phase = 5;
          str[0] = str[1] = ' ';
          str[2] = 0x30 + g_enc;
          str[3] = ';';
          sprintf(&str[4], "%06d", g_score);
          go_flag = true;
          disp(str, 0);
        }
        break;
      case 3: // win this encounter
        en_keys = false;
        M5.Speaker.playMusic(w_win, 8000);
        g_phase = 2;
        g_aim = '0';
        g_shot = 0;
        g_shoot = 0;
        g_sum = 0;
        g_remain = 3;
        g_enum = 1;
        g_fire = false;
        ufo_flag = false;
        g_enc++;
        if(g_enc > 9)
        {
          g_enc = 1;
          g_stage = g_stage == 0 ? 1 : 0 ;
        }
        g_enemy[0] = g_enemy[1] = g_enemy[2] = g_enemy[3] = g_enemy[4] = g_enemy[5] = ' ';
        g_enemy[5-g_stage] = random(0, 10) + 0x30;
        g_elapse = millis() + (13-g_enc)*100;
        break;
      case 4: // lose this encounter
        en_keys = false;
        M5.Speaker.playMusic(w_lose, 8000);
        g_phase = 2;
        g_aim = '0';
        for(i = 0; i < 6; i++)
        {
          if(g_enemy[i] != ' ')
          {
            g_enum--;
          }
          g_enemy[i] = ' ';
        }
        if(ufo_flag)  // set UFO
        {
          g_enemy[5-g_stage] = ':';
          ufo_flag = false;
        }
        else  // set enemy
        {
          g_enemy[5-g_stage] = random(0, 10) + 0x30;
        }
        g_enum++;
        g_elapse = millis() + (13-g_enc)*100;
        if(g_fire)  // clear not proccessed shot
        {
          g_shot--;
        }
        g_fire = false;
        break;
      case 5: // game over
        en_keys = false;
        M5.Speaker.playMusic(w_gover, 8000);
        g_phase = 6;
        break;
      default:
        en_keys = false;
        break;
    }
    if(xQueueReceive(keyboard_queue, &key_val, 0)) // Non-blocking
    {
      if(key_val == 'A')  // return to normal mode
      {
        inp_b = 0;
        inp_e = 0;
        inp_d = 8;
        inp_s = 1;
        inp_mode = false;
        acc_b = 0;
        acc_e = 0;
        g_mode = false;
        err_flag = false;
        disp_num(inp_b, inp_e);
      }
      else if(key_val == '0' && en_keys)  // aiming
      {
        if(g_aim != ':')
        {
          g_aim++;
        }
        else
        {
          g_aim = '0';
        }
      }
      else if(key_val == '=' && en_keys)  // shot
      {
        g_fire = true;
        g_shot++;
      }
    }
    if(M5.BtnA.wasPressed())  // reset game
    {
      g_phase = 0;
      g_score = 0;
      go_flag = false;
      sprintf(str, "    %06d", g_hiscore);
      disp(str, 0);
      g_elapse = millis() + 2000;
    }
    if(M5.BtnB.wasPressed() && en_keys) // aiming
    {
      if(g_aim != ':')
      {
        g_aim++;
      }
      else
      {
        g_aim = '0';
      }
    }
    if(M5.BtnC.wasPressed() && en_keys) // shot
    {
      g_fire = true;
      g_shot++;
    }
    M5.update();
  }
}

