# G2

reads in a monochrome BMP font file to generate a bitmap file

### BMP
- 128*128 px
- 8*8 char blocks
- 256 blocks / glyphs
- monochrome
- white = 1, black = 0
- compatible format generated by Paint.NET by saving image as BMP with 1-bit bit depth

### BITMAP
- 8 bytes per glyph
- 1st byte is top row... 8th byte is bottom row
- most significant bit is left-most pixel... least significant bit is right-most pixel
