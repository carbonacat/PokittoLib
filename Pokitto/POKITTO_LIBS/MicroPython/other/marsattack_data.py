import upygame as pygame

def ConvStrTo4bitPixelArray(w, h, strArray, numArray):
    #print("strArray=", strArray)
    for y in range(h):
        for x in range(w//2):
            #print("y=",y," x=", x*2)
            pixStr = "0x" + strArray[y*w + (x*2)] + strArray[y*w + (x*2) + 1]
            pix = int(pixStr, 16)
            numArray.append(pix)

def ConvStrTo2bitPixelArray(w, h, strArray, numArray):
    #print("strArray=", strArray)
    for y in range(h):
        for x in range(w//4):
            # make a byte
            #print("y=",y," x=", x*4)
            c0 = int(strArray[y*w + (x*4)])
            c1 = int(strArray[y*w + (x*4) + 1])
            c2 = int(strArray[y*w + (x*4) + 2])
            c3 = int(strArray[y*w + (x*4) + 3])
            #print("c0=", bin(c0),"c1=", bin(c1),"c2=", bin(c2),"c3=", bin(c3))
            bytepixels = (c0 << 6) + (c1 << 4) + (c2 << 2) + c3
            #print("bytepixels=",bin(bytepixels))
            numArray.append(bytepixels)

# Sine data
#create sine array
fpFactor = 256;
pseudoPi = 128;
#for pseudoRad in range(3*2*pseudoPi/2):  # store array of 3/2*pi elements to contain full sin and cos circle
#    rad = pseudoRad * math.pi / pseudoPi
#    fpSin = int(round(math.sin(rad)*fpFactor))
#    fpSinCosArray.append(fpSin)
#    #print("pseudoRad=",pseudoRad,"rad=",rad,"fpSin=",fpSin)
#    print(",",fpSin,end='')

fpSinCosArray = [
    0, 6, 13, 19, 25, 31, 38, 44, 50, 56, 62, 68, 74, 80, 86, 92, 98, 104, 109, 115, 121, 126, 132, 137, 142,
    147, 152, 157, 162, 167, 172, 177, 181, 185, 190, 194, 198, 202, 206, 209, 213, 216, 220, 223, 226, 229,
    231, 234, 237, 239, 241, 243, 245, 247, 248, 250, 251, 252, 253, 254, 255, 255, 256, 256, 256, 256, 256,
    255, 255, 254, 253, 252, 251, 250, 248, 247, 245, 243, 241, 239, 237, 234, 231, 229, 226, 223, 220, 216,
    213, 209, 206, 202, 198, 194, 190, 185, 181, 177, 172, 167, 162, 157, 152, 147, 142, 137, 132, 126, 121,
    115, 109, 104, 98, 92, 86, 80, 74, 68, 62, 56, 50, 44, 38, 31, 25, 19, 13, 6, 0, -6, -13, -19, -25, -31,
    -38, -44, -50, -56, -62, -68, -74, -80, -86, -92, -98, -104, -109, -115, -121, -126, -132, -137, -142,
    -147, -152, -157, -162, -167, -172, -177, -181, -185, -190, -194, -198, -202, -206, -209, -213, -216,
    -220, -223, -226, -229, -231, -234, -237, -239, -241, -243, -245, -247, -248, -250, -251, -252, -253,
    -254, -255, -255, -256, -256, -256, -256, -256, -255, -255, -254, -253, -252, -251, -250, -248, -247,
    -245, -243, -241, -239, -237, -234, -231, -229, -226, -223, -220, -216, -213, -209, -206, -202, -198,
    -194, -190, -185, -181, -177, -172, -167, -162, -157, -152, -147, -142, -137, -132, -126, -121, -115,
    -109, -104, -98, -92, -86, -80, -74, -68, -62, -56, -50, -44, -38, -31, -25, -19, -13, -6, 0, 6, 13, 19,
    25, 31, 38, 44, 50, 56, 62, 68, 74, 80, 86, 92, 98, 104, 109, 115, 121, 126, 132, 137, 142, 147, 152, 157,
    162, 167, 172, 177, 181, 185, 190, 194, 198, 202, 206, 209, 213, 216, 220, 223, 226, 229, 231, 234, 237,
    239, 241, 243, 245, 247, 248, 250, 251, 252, 253, 254, 255, 255, 256, 256, 256, 256, 256, 255, 255, 254,
    253, 252, 251, 250, 248, 247, 245, 243, 241, 239, 237, 234, 231, 229, 226, 223, 220, 216, 213, 209, 206,
    202, 198, 194, 190, 185, 181, 177, 172, 167, 162, 157, 152, 147, 142, 137, 132, 126, 121, 115, 109, 104,
    98, 92, 86, 80, 74, 68, 62, 56, 50, 44, 38, 31, 25, 19, 13, 6
]

# Rock picture
w = 16
h = 14
pixelsStr = \
"3300000000003300" \
"3300000000003300" \
"3330000000033300" \
"3303000000303300" \
"3300300003003300" \
"3300030030003300" \
"3300003300003300" \
"3300003300003300" \
"3300030030003300" \
"3300300003003300" \
"3303000000303300" \
"3330000000033300" \
"3300000000003300" \
"3300000000003300"
rockPixels_f0 = []
ConvStrTo2bitPixelArray(w, h, pixelsStr, rockPixels_f0)
rockSurf_f0 = pygame.surface.Surface(w, h, bytearray(rockPixels_f0))

# Ship picture frames
w = 32
h = 16
pixelsStr = \
"11100000000000000000000000000000" \
"11110000000000000000000000000000" \
"11111000000000000000000000000000" \
"01111100000000000000000000000000" \
"01111110000000000000000000000000" \
"01111111000000000000000000000000" \
"00111111100000000000000000000000" \
"00111111110000000000000000000000" \
"00011111111111111111111111000000" \
"00011111111111111111111111100000" \
"00101111111111111111111111111000" \
"00111111111111111111111111111110" \
"00111111111113333333331111111331" \
"00111111113333333333333111111311" \
"00100111111111111111111111111110" \
"00000000011000000000000000110000"
shipPixels = []
ConvStrTo2bitPixelArray(w, h, pixelsStr, shipPixels)
shipSurf = pygame.surface.Surface(w, h, bytearray(shipPixels))




