#ifndef NITRO_MISC_H_
#define NITRO_MISC_H_

void OSi_ReferSymbol(void *symbol);

#define SDK_REFER_SYMBOL(symbol) OSi_ReferSymbol((void *)(symbol))

#endif // NITRO_MISC_H_
