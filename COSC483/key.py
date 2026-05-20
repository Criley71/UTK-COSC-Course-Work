import math
    
phex = "F7E75FDC469067FFDC4E847C51F452DF"
qhex = "E85CED54AF57E53E092113E62F436F4F"
ehex = "0D88C3"

p = int(phex, 16)
q = int(qhex, 16)
e = int(ehex, 16)
#p = 329520679814142392965336341297134588639
#q = 308863399973593539130925275387286220623
#e = 886979

#take p and q and multiply them to find n, the modulus for encryption
n = p * q
#calculate phi with p-1 * q-1 this will be used with e to find the private key
phi = (p - 1) * (q-1)
#use pow to calculate the modular inverse
d = pow(e, -1, phi)
print ("is it that easy ", d)
test = 24212225287904763939160097464943268930139828978795606022583874367720623008491 % phi
hex_private_key = hex(d)
print("                ", hex_private_key)
print("huh             ", test)