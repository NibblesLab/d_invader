const unsigned short i_div[2][400] = {
{ 0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,
  0xD4C6,0xD4C6,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xD4C6,0xD4C6,0xD4C6,
  0xD4C6,0x0000,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x0000,0xD4C6,0xD4C6,
  0xD4C6,0x0000,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x0000,0xD4C6,
  0xD4C6,0x0000,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x0000,0x0000,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x0000,0xD4C6,
  0xD4C6,0x0000,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x0000,0x0000,0x0000,0x0000,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x0000,0xD4C6,
  0xD4C6,0x0000,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x0000,0x0000,0x0000,0x0000,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x0000,0xD4C6,
  0xD4C6,0x0000,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x0000,0x0000,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x0000,0xD4C6,
  0xD4C6,0x0000,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x0000,0xD4C6,
  0xD4C6,0x0000,0xD4C6,0xD4C6,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xD4C6,0xD4C6,0x0000,0xD4C6,
  0xD4C6,0x0000,0xD4C6,0xD4C6,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xD4C6,0xD4C6,0x0000,0xD4C6,
  0xD4C6,0x0000,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x0000,0xD4C6,
  0xD4C6,0x0000,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x0000,0x0000,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x0000,0xD4C6,
  0xD4C6,0x0000,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x0000,0x0000,0x0000,0x0000,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x0000,0xD4C6,
  0xD4C6,0x0000,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x0000,0x0000,0x0000,0x0000,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x0000,0xD4C6,
  0xD4C6,0x0000,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x0000,0x0000,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x0000,0xD4C6,
  0xD4C6,0x0000,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x0000,0xD4C6,
  0xD4C6,0xD4C6,0x0000,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x0000,0xD4C6,0xD4C6,
  0xD4C6,0xD4C6,0xD4C6,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0x0000,0xD4C6,0xD4C6,0xD4C6,
  0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6},

{ 0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,
  0xD4C6,0xD4C6,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0xD4C6,0xD4C6,0xD4C6,
  0xD4C6,0x92DE,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x92DE,0xD4C6,0xD4C6,
  0xD4C6,0x92DE,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x92DE,0xD4C6,
  0xD4C6,0x92DE,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x92DE,0x92DE,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x92DE,0xD4C6,
  0xD4C6,0x92DE,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x92DE,0x92DE,0x92DE,0x92DE,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x92DE,0xD4C6,
  0xD4C6,0x92DE,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x92DE,0x92DE,0x92DE,0x92DE,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x92DE,0xD4C6,
  0xD4C6,0x92DE,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x92DE,0x92DE,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x92DE,0xD4C6,
  0xD4C6,0x92DE,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x92DE,0xD4C6,
  0xD4C6,0x92DE,0xD4C6,0xD4C6,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0xD4C6,0xD4C6,0x92DE,0xD4C6,
  0xD4C6,0x92DE,0xD4C6,0xD4C6,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0xD4C6,0xD4C6,0x92DE,0xD4C6,
  0xD4C6,0x92DE,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x92DE,0xD4C6,
  0xD4C6,0x92DE,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x92DE,0x92DE,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x92DE,0xD4C6,
  0xD4C6,0x92DE,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x92DE,0x92DE,0x92DE,0x92DE,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x92DE,0xD4C6,
  0xD4C6,0x92DE,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x92DE,0x92DE,0x92DE,0x92DE,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x92DE,0xD4C6,
  0xD4C6,0x92DE,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x92DE,0x92DE,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x92DE,0xD4C6,
  0xD4C6,0x92DE,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x92DE,0xD4C6,
  0xD4C6,0xD4C6,0x92DE,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0x92DE,0xD4C6,0xD4C6,
  0xD4C6,0xD4C6,0xD4C6,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0x92DE,0xD4C6,0xD4C6,0xD4C6,
  0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6,0xD4C6}
};
