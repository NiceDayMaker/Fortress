import asciiartx2, asciiartx4
import numpy

path = "Resources/image.png"
x2, x2pri = asciiartx2.image_to_unicode_ascii(path, 100)
x4, x4pri = asciiartx4.image_to_unicode_blocks_sharp(path, 200)

print(x2)
print(x4)

