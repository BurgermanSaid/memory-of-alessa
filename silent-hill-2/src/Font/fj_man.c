#include "common.h"
#include "Font/fj_man.h"
#include "Font/font.h"

INCLUDE_ASM("asm/nonmatchings/Font/fj_man", fjInitAll);

INCLUDE_ASM("asm/nonmatchings/Font/fj_man", fjInitFontTexture);

INCLUDE_ASM("asm/nonmatchings/Font/fj_man", fjMoveEffect);

INCLUDE_ASM("asm/nonmatchings/Font/fj_man", fjDrawExec);

INCLUDE_ASM("asm/nonmatchings/Font/fj_man", fjFontDrawExec);

void fjFontDrawExecVif1(void) {
    void* adr1; void* adr2;

    d1cSend(fontTexLoad(sh2gfw_Get_BaseTBP0for2D(), FONT_TEX_CLUT_ADR));

    
    adr1 = fontFlush();
    adr2 = mfontFlush();
    
    d1cSend(adr1);
    d1cSend(adr2);
    
    d1cSend(fontAfterEnv());
}


#define ABORT() asm("breakc 0")
int fjAssert_(const char *file, int line, const char *str) __attribute__((noreturn))
{
    printf("assertion \"%s\" failed: file \"%s\", line %d\n", str, file, line);
    ABORT();
    return 1;
}
