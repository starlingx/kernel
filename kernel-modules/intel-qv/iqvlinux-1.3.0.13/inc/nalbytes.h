
#ifndef INC_NALBYTES_H_
#define INC_NALBYTES_H_ 

#ifndef HIBYTE
    #define HIBYTE(_x) (UINT8)(((_x)>>8)&0xFF)
#endif

#ifndef LOBYTE
    #define LOBYTE(_x) (UINT8)((_x)&0xFF)
#endif

#ifndef HIWORD
    #define HIWORD(_x) ((UINT16)(((_x)>>16)&0xFFFF))
#endif

#ifndef LOWORD
    #define LOWORD(_x) ((UINT16)((_x)&0xFFFF))
#endif

#ifndef HIDWORD
    #define HIDWORD(_x) ((UINT32)(((_x)>>32)&0xFFFFFFFF))
#endif

#ifndef LODWORD
    #define LODWORD(_x) ((UINT32)((_x)&0xFFFFFFFF))
#endif

#ifndef LOW_BYTE
    #define LOW_BYTE(word) LOBYTE(word)
#endif

#ifndef HIGH_BYTE
    #define HIGH_BYTE(word) HIBYTE(word)
#endif

#ifndef LOW_WORD
    #define LOW_WORD(dword) LOWORD(dword)
#endif

#ifndef HIGH_WORD
    #define HIGH_WORD(dword) HIWORD(dword)
#endif

#ifndef MAKE_WORD
    #define MAKE_WORD(hi, low) ((UINT16) ((((UINT16)(hi)) << 8) | (low)))
#endif

#ifndef MAKE_DWORD
    #define MAKE_DWORD(hi, low) ((UINT32) ((((UINT32)(hi)) << 16) | (low)))
#endif

#ifndef MAKE_QWORD
    #define MAKE_QWORD(hi, low) ((UINT64) ((((UINT64)(hi)) << 32) | (low)))
#endif

#ifndef WORD_SWAP_BYTE_ARRAY
#define WORD_SWAP_BYTE_ARRAY(word) (UINT16)( (((UINT16)(word) & 0x00ff) << 8) | \
                                                (((UINT16)(word) & 0xff00) >> 8) )
#endif

#ifndef DWORD_SWAP_BYTE_ARRAY
#define DWORD_SWAP_BYTE_ARRAY(dword) (UINT32)( (((UINT32)(dword) & 0x000000ff) << 24) | \
                                                (((UINT32)(dword) & 0x0000ff00) << 8) | \
                                                (((UINT32)(dword) & 0x00ff0000) >> 8) | \
                                                (((UINT32)(dword) & 0xff000000) >> 24) )
#endif

#if defined NAL_BIG_ENDIAN

#ifndef BYTE_SWAP_WORD
    #define BYTE_SWAP_WORD(value) (UINT16)( (((UINT16)(value) & 0x00ff)) | \
                                                (((UINT16)(value) & 0xff00)) )
#endif

#ifndef BYTE_SWAP_DWORD
    #define BYTE_SWAP_DWORD(dword) (UINT32)( (((UINT32)(dword) & 0x000000ff) << 24) | \
                                                (((UINT32)(dword) & 0x0000ff00) << 8) | \
                                                (((UINT32)(dword) & 0x00ff0000) >> 8) | \
                                                (((UINT32)(dword) & 0xff000000) >> 24) )
#endif

#ifndef WORD_SWAP_DWORD
    #define WORD_SWAP_DWORD(value) (UINT32)( (((UINT32)(value) & 0x0000FFFF) << 16) | \
                                                (((UINT32)(value) & 0xFFFF0000) >> 16) )
#endif

#ifndef BYTE_SWAP_QWORD
    #define BYTE_SWAP_QWORD(_dest, _src) \
        { \
        ((UINT8*)_dest)[0] = ((UINT8*)_src)[7]; \
        ((UINT8*)_dest)[1] = ((UINT8*)_src)[6]; \
        ((UINT8*)_dest)[2] = ((UINT8*)_src)[5]; \
        ((UINT8*)_dest)[3] = ((UINT8*)_src)[4]; \
        ((UINT8*)_dest)[4] = ((UINT8*)_src)[3]; \
        ((UINT8*)_dest)[5] = ((UINT8*)_src)[2]; \
        ((UINT8*)_dest)[6] = ((UINT8*)_src)[1]; \
        ((UINT8*)_dest)[7] = ((UINT8*)_src)[0]; \
        }
#endif

#ifndef WORD_SWAP
    #define WORD_SWAP(dword) WORD_SWAP_DWORD(dword)
#endif

#ifndef BYTE_SWAP
    #define BYTE_SWAP(word) BYTE_SWAP_WORD(word)
#endif

#else

#ifndef BYTE_SWAP_WORD
    #define BYTE_SWAP_WORD(value) (UINT16)( (((UINT16)(value) & 0x00ff) << 8) | \
                                                (((UINT16)(value) & 0xff00) >> 8) )
#endif

#ifndef BYTE_SWAP_DWORD
    #define BYTE_SWAP_DWORD(dword) (UINT32)( (((UINT32)(dword) & 0x000000ff) << 24) | \
                                                (((UINT32)(dword) & 0x0000ff00) << 8) | \
                                                (((UINT32)(dword) & 0x00ff0000) >> 8) | \
                                                (((UINT32)(dword) & 0xff000000) >> 24) )
#endif

#ifndef WORD_SWAP_DWORD
    #define WORD_SWAP_DWORD(value) (UINT32)( (((UINT32)(value) & 0x0000FFFF) << 16) | \
                                                (((UINT32)(value) & 0xFFFF0000) >> 16) )
#endif

#ifndef BYTE_SWAP_QWORD
    #define BYTE_SWAP_QWORD(_dest, _src) \
        { \
        ((UINT8*)_dest)[0] = ((UINT8*)_src)[7]; \
        ((UINT8*)_dest)[1] = ((UINT8*)_src)[6]; \
        ((UINT8*)_dest)[2] = ((UINT8*)_src)[5]; \
        ((UINT8*)_dest)[3] = ((UINT8*)_src)[4]; \
        ((UINT8*)_dest)[4] = ((UINT8*)_src)[3]; \
        ((UINT8*)_dest)[5] = ((UINT8*)_src)[2]; \
        ((UINT8*)_dest)[6] = ((UINT8*)_src)[1]; \
        ((UINT8*)_dest)[7] = ((UINT8*)_src)[0]; \
        }
#endif

#ifndef WORD_SWAP
    #define WORD_SWAP(dword) WORD_SWAP_DWORD(dword)
#endif

#ifndef BYTE_SWAP
    #define BYTE_SWAP(word) BYTE_SWAP_WORD(word)
#endif

#endif

#define MAKE_DATA_WORD_TCP_CHECKSUM(hi,low) \
    ((UINT16) ((((UINT16)(hi)) << 8) | (low)))

#define MAKE_DATA_WORD_UDP_CHECKSUM(hi,low) \
    ((UINT16) ((((UINT16)(hi)) << 8) | (low)))

#ifndef ARE_ALL_FLAGS_SET
    #define ARE_ALL_FLAGS_SET(VALUE,FLAGS) ( ((VALUE) & (FLAGS) ) == (FLAGS) )
#endif

#ifndef ARE_ALL_FLAGS_CLEAR
    #define ARE_ALL_FLAGS_CLEAR(VALUE,FLAGS) ( ((VALUE) & ~(FLAGS) ) == 0 )
#endif

#ifndef IS_AT_LEAST_ONE_FLAG_SET
    #define IS_AT_LEAST_ONE_FLAG_SET(VALUE,FLAGS) ( ((VALUE) & (FLAGS)) != 0 )
#endif

#ifndef IS_AT_LEAST_ONE_FLAG_CLEAR
    #define IS_AT_LEAST_ONE_FLAG_CLEAR(VALUE,FLAGS) ( ((VALUE) & ~(FLAGS)) != 0 )
#endif

#define ROUNDUP(i,size) ((i) = (((i) + (size) - 1) & ~((size) - 1)))

#endif
