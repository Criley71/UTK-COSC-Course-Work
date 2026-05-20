#Connor Riley
#hash attack cs 483
import hashlib
import binascii
import random
import matplotlib.pyplot as plt
import numpy as np
import matplotlib.patches as mpatches
def string_to_bin(s): #converts string to binary to be ran through sha
  return ''.join(format(ord(char), '08b') for char in s)

def hash_and_mask(binary_string : str, num_bits: int):
  hex_string = '{0:0>4x}'.format(int(binary_string,2))
  message = binascii.a2b_hex(hex_string)
  hashed_message = hashlib.sha1(message).hexdigest()
  pad_bits = 8
  binary_message = bin(int(hashed_message, 16))[2:].zfill(num_bits)
  bin_mask = int("1" * num_bits, 2)
  return(bin(int(binary_message, 2) & bin_mask))[2:].zfill(num_bits)

def mask(hash : int, num_bits: int):
  bin_mask = int("1" * num_bits, 2)
  print(("hash masked:", hash & bin_mask))
  return ((hash & bin_mask))

def pre_image_attack(bit_size: int):
  sum = 0
  i = 0
  string_to_hash = ''.join(random.choices("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz!@#$%^&*()=-_+/?.><", k=512))
  iteration_list = []
  for j in range(0, 50):
    result = hash_and_mask(string_to_bin(string_to_hash), bit_size)
    i = 0
    while(True):
      i+= 1
      random_string = ''
      random_string = ''.join(random.choices("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz!@#$%^&*()=-_+/?.><", k=512))
      test = hash_and_mask(string_to_bin(random_string), bit_size)
      if test == result:
        iteration_list.append(i)
        print("Found it after ", i, " iterations input:", random_string)
        break
    sum += i
  print("average", sum/50, "bit_size:", bit_size)
  print(test)
  return iteration_list;


 
def collision_attack(bit_size : int):
  sum = 0
  iteration_list = []
  for j in range(0, 50):
    i= 0
    hashed_dict = {}
    while(True):
      i+= 1
      string1 = ''
      string1 = ''.join(random.choices("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz!@#$%^&*()=-_+/?.><", k=512))
      hash1 = hash_and_mask(string_to_bin(string1), bit_size)
      if hash1 in hashed_dict and hashed_dict[hash1] != string1:
        print(f'{string1}: {hash1} = {hashed_dict[hash1]}: {hash1}, i = {i}')
        iteration_list.append(i)
        sum += i
        break
      else:
        hashed_dict[hash1] = string1
        
  print(f'average: {sum/50} for bit_size {bit_size}\n\n')
  return iteration_list    
        
      
      
"""# COLLISION ATTACK EXECUTION AND GRAPH delete the 3 quatoes on this line to uncomment and run the preimage
collision_8 = collision_attack(8)
collision_10 = collision_attack(10)
collision_12 = collision_attack(12)
collision_14 = collision_attack(14)
collision_16 = collision_attack(16)
collision_18 = collision_attack(18)
collision_20 = collision_attack(20)
collision_22 = collision_attack(22)


data = [collision_8, collision_10, collision_12, collision_14, collision_16, collision_18, collision_20, collision_22]
fig, ax = plt.subplots()
ax.set_yscale('log')

ax.set_xticklabels(['8-bits','10-bits','12-bits','14-bits','16-bits','18-bits','20-bits', '22-bits'])
ax.boxplot(data, showmeans=True, meanprops=dict(marker='x'))

mean_handle = mpatches.Patch(color='green', label='Mean')
median_handle = mpatches.Patch(color='orange', label='Median')
expected_handle = mpatches.Patch(color='blue', label='Expected')

x = [1, 2, 3, 4, 5, 6, 7, 8]
y = []
for i in range(8, 23, 2):
  y.append(2**(i/2))
ax.plot(x, y, color='blue', marker='o', linestyle='-', label='Expected Iterations')
for i,d in enumerate(data, start=1):
  mean = np.mean(d)
  median = np.median(d)
  offset = 2
  if i >= 7:
    offset = 20
  ax.text(i, mean+offset*i, f'{mean:.2f}', horizontalalignment='center', verticalalignment='bottom',color='green', label='mean')
  ax.text(i, median-offset, f'{median:.2f}', horizontalalignment='center', verticalalignment='top',color='orange', label='median')
for i, d in enumerate(y, start=1):
  expected = d
  offset = 2
  if i >= 7:
    offset = 20
  ax.text(i, expected-offset*i, f'{expected:.2f}', horizontalalignment='center', verticalalignment='top',color='blue', label='expected')

  
ax.grid(True, axis='y')
ax.set_xlabel('Bit Size')
ax.set_ylabel('Iterations required')
ax.set_title('SHA-1 Collision Attack on Different Hash Sizes')
ax.legend(handles=[mean_handle, median_handle, expected_handle])
plt.show()


"""
string_to_hash = ''.join(random.choices("ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789abcdefghijklmnopqrstuvwxyz!@#$%^&*()=-_+/?.><", k=512))
pre_image_attack_8 = pre_image_attack(8)
pre_image_attack_10 = pre_image_attack(10)
pre_image_attack_12 = pre_image_attack(12)
pre_image_attack_14 = pre_image_attack(14)
pre_image_attack_16 = pre_image_attack(16)
pre_image_attack_18 = pre_image_attack(18)
pre_image_attack_20 = pre_image_attack(20)
pre_image_attack_22 = pre_image_attack(22)
data = [pre_image_attack_8, pre_image_attack_10, pre_image_attack_12, pre_image_attack_14, pre_image_attack_16, pre_image_attack_18, pre_image_attack_20, pre_image_attack_22]
fig, ax = plt.subplots()
ax.set_yscale('log')

ax.set_xticklabels(['8-bits','10-bits','12-bits','14-bits','16-bits','18-bits','20-bits', '22-bits'])
ax.boxplot(data, showmeans=True, meanprops=dict(marker='x'))

mean_handle = mpatches.Patch(color='green', label='Mean')
median_handle = mpatches.Patch(color='orange', label='Median')
expected_handle = mpatches.Patch(color='blue', label='Expected')
x = [1, 2, 3, 4, 5, 6, 7, 8]
y = []
for i in range(8, 23, 2):
  y.append(2**(i))
ax.plot(x, y, color='blue', marker='o', linestyle='-', label='Expected Iterations')
for i,d in enumerate(data, start=1):
  mean = np.mean(d)
  median = np.median(d)
  offset = 2
  if i >= 7:
    offset = 20
  ax.text(i, mean+offset*i, f'{mean:.2f}', horizontalalignment='center', verticalalignment='bottom',color='green', label='mean')
  ax.text(i, median-offset*i, f'{median:.2f}', horizontalalignment='center', verticalalignment='top',color='orange', label='median')
for i, d in enumerate(y, start=1):
  expected = d
  offset = 2
  if i >= 7:
    offset = 20
  ax.text(i, expected-offset*i, f'{expected:.2f}', horizontalalignment='center', verticalalignment='top',color='blue', label='expected')

  
ax.grid(True, axis='y')
ax.set_xlabel('Bit Size')
ax.set_ylabel('Iterations required')
ax.set_title('SHA-1 Pre-Image Attack on Different Hash Sizes')
ax.legend(handles=[mean_handle, median_handle, expected_handle])
plt.savefig("pre_image_512.png")
plt.show()



#"""
