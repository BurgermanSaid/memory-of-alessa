#ifndef OTN_ITEMMAIN_H
#define OTN_ITEMMAIN_H

int itemmain(void);

void itemmainmain(void);

void cur_step(void);

void command_main(int command_step);

void weapon_command_main(int command_step);

void item_main_setup(void);

void set_position(int step);

void item_turn(void);

void item_position(float* f0, float* f1, int count, float* r0);

int move_near(float f0, float f1, float f2, float f3);

int command_kind(int kind);

void del_check(void);

int kind_no(int kind);

void event_item_use(int kind);

void combine_item_use(int kind);

void examine_main(void);

void examine_file_load(void);

void examine2_main(void);

void font_print(void);

void kage_font(u_short* str, u_short num, int x, int y);

void lookline(float (*box)[4]);

void item_allay(void);

void sprite(void);

void look_command(int gosa);

void look_volume(void);

void look_hp(void);

void stock_line(int leftx, int hoge);

void look_combine(void);

void look_zanzo(float* za0, float* za1, float* za2, float* za3);

void look_blackscr(int num);

void item_fade_in(void);

void item_fade_out(void);

void item_examine_fade_out(void);

extern struct /* @anon1 */ {
    // total size: 0x14
    unsigned char kind;  // offset 0x0, size 0x1
    unsigned char count; // offset 0x1, size 0x1
    unsigned char del;   // offset 0x2, size 0x1
    float pos[2];        // offset 0x4, size 0x8
    float rot;           // offset 0xC, size 0x4
    float item_scale;    // offset 0x10, size 0x4
};

#endif // OTN_ITEMMAIN_H
