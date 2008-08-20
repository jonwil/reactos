/*
 *  FreeLoader
 *
 *  Copyright (C) 2003  Eric Kohl
 *
 *  This program is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with this program; if not, write to the Free Software
 *  Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
 */

#ifndef __AMD64_AMD64_H_
#define __AMD64_AMD64_H_

#define STACK64ADDR	0x74000	/* The 64-bit stack top will be at 0x74000 */

/* Long mode selectors */
#define LMODE_CS	0x10
#define LMODE_DS	0x18
#define RMODE_CS	0x20	/* RMode code selector, base 0 limit 64k */
#define RMODE_DS	0x28	/* RMode data selector, base 0 limit 64k */

#define VA_MASK 0x0000FFFFFFFFFFFFUL

#define PtrToPfn(p) \
    ((((ULONGLONG)p) >> PAGE_SHIFT) & 0xfffffffULL)

#define VAtoPXI(va) (((va) >> PXI_SHIFT) & 0x1FF)
#define VAtoPPI(va) (((va) >> PPI_SHIFT) & 0x1FF)
#define VAtoPDI(va) (((va) >> PDI_SHIFT) & 0x1FF)
#define VAtoPTI(va) (((va) >> PTI_SHIFT) & 0x1FF)

#define HYPERSPACE_BASE             0xfffff70000000000ULL
#define HAL_BASE                    0xffffffff80000000ULL
#define APIC_BASE                   0xffffffffff000000ULL // FIXME

#define NUM_PAGES_KERNEL 

#ifndef ASM
typedef struct _PAGE_DIRECTORY_AMD64
{
    HARDWARE_PTE Pde[512];
} PAGE_DIRECTORY_AMD64, *PPAGE_DIRECTORY_AMD64;

VOID
FORCEINLINE
__lgdt(void *gdt)
{
    asm volatile ("lgdt %0\n" : : "m"(*(short*)gdt));
}

VOID
FORCEINLINE
__lidt(void *idt)
{
    asm volatile ("lidt %0\n" : : "m"(*(short*)idt));
}

PKGDTENTRY64
FORCEINLINE
KiGetGdtEntry(PVOID pGdt, USHORT Index)
{
    return (PKGDTENTRY64)((ULONG64)pGdt + (Index & ~RPL_MASK));
}

VOID
FORCEINLINE
KiInitGdtEntry(PKGDTENTRY64 Entry, ULONG64 Base, UCHAR Type, UCHAR Dpl)
{
    Entry->Bits.Type = Type;
    Entry->Bits.Present = 1;
    Entry->Bits.Dpl = Dpl;
    Entry->BaseLow = (USHORT)(Base & 0xFFFF);
    Entry->Bytes.BaseMiddle = (UCHAR)(Base >> 16);
    Entry->Bytes.BaseHigh = (UCHAR)(Base >> 24);
    Entry->BaseUpper = (ULONG)(Base >> 32);
}

VOID FrLdrSetupGdtIdt();

#endif

#endif /* __AMD64_AMD64_H_ */

/* EOF */
