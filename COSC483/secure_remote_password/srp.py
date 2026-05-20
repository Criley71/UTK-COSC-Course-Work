import random
import hashlib
import sys

#from the pub key cryptography project
#solves b^e (mod n)

def modular_exponentiation(b : int, e : int, n : int): #c = (b^e) (mod n)
  binary_e = list(bin(e)[2:])
  product = 1
  for i in range(len(binary_e)-1, -1, -1):
    if binary_e[i] == '1':
      product = (product * b) % n
    b = b * b % n
  return product

def get_a(p: int):#p is given by passoff server
  a = random.randint(1, p-1)
  print(f'a = {a}\n')
  return a



def get_public_key(g : int, a : int, p : int):
  pk = modular_exponentiation(g, a, p)
  print(f'DH public key (g^a) (first input on passoff website): {pk}\n')
  return pk

def string_to_bytes(string):
    encoded = string.encode('ascii')
    return encoded

def hash_salted_password(password : str, salt : str, itr : int):#x = H(salt || password)^itr
  password_bytes = password.encode("ascii")
  salt_bytes = bytes.fromhex(salt)
  salted_password = salt_bytes + password_bytes
  hash_result = salted_password
  for i in range(itr):
    hash_result = hashlib.sha256(hash_result).digest()
  result = hash_result.hex()
  print(f'Password hash x (1st input on 2nd page)= {int(result, 16)}')
  return int(result, 16)


def get_k(p : int, g : int):
  p_bytes = p.to_bytes((p.bit_length() + 7) // 8, 'big')
  g_bytes = g.to_bytes((g.bit_length() + 7) // 8, 'big')
  p_g_concat = p_bytes + g_bytes
  k = hashlib.sha256(p_g_concat)
  k = k.digest()
  k = k.hex()
  print(f'k (2nd input on 2nd page) = {int(k, 16)}\n')
  return int(k, 16)
  
#g^b = B - k * (g^x)(mod p)
def SRP_public_key(B : int, k : int, g : int, x : int, p : int):
  print(f'g^b (3rd input on 2nd page)= {modular_exponentiation(B - k * modular_exponentiation(g,x,p), 1, p)}\n')
  return modular_exponentiation(B - k * modular_exponentiation(g,x,p), 1, p)
  #this was returning the negative value for some reason, so i added the outer modular exponention 
  #its just the result to the power of 1 mod p so it results back in the appropiate value


def get_u(ga : int, gb : int):
  ga_bytes = ga.to_bytes((ga.bit_length() + 7) // 8, 'big')
  gb_bytes = gb.to_bytes((gb.bit_length() + 7) // 8, 'big')
  combined = ga_bytes + gb_bytes
  u = hashlib.sha256(combined)
  u = u.digest()
  u = u.hex()
  print(f'u (4th input on 2nd page) = {int(u, 16)}\n')
  return int(u, 16)
  
def get_shared_key(gb: int, a : int, u : int, x : int, p : int):
  #print(f'\n\ngb check: {gb}\n\na check: {a}\n\nu check: {u}\n\nx check: {x}\n\np check: {p}\n')
  n = modular_exponentiation(gb, (a + (u * x)), p)
  print(f'shared key (5th input on 2nd page) = {n}\n') 
  return(n)


#H(H(p) ^ H(g) || H(username) || salt || ga || gb || shared_key)
def m1_zero_knowledge_proof(p : int, g : int, username : str, salt : str, ga : int, gb : int, shared_key : int):
  p_bytes = p.to_bytes((p.bit_length() + 7) // 8, 'big')
  g_bytes = g.to_bytes((g.bit_length() + 7) // 8, 'big')
  username_bytes = username.encode('ascii')
  salt_bytes = bytes.fromhex(salt)
  ga_bytes = ga.to_bytes((ga.bit_length() + 7) // 8, 'big')
  gb_bytes = gb.to_bytes((gb.bit_length() + 7) // 8, 'big')
  shared_key_bytes = shared_key.to_bytes((shared_key.bit_length() + 7) // 8, 'big')
  
  Hp = hashlib.sha256(p_bytes).digest()
  Hg = hashlib.sha256(g_bytes).digest()
  Hp_xor_Hg = bytes(a ^ b for a,b in zip(Hp, Hg))
  
  H_username = hashlib.sha256(username_bytes).digest()
  
  inner_hash_value = Hp_xor_Hg + H_username + salt_bytes + ga_bytes + gb_bytes + shared_key_bytes
  result = hashlib.sha256(inner_hash_value).digest()
  print(f'M1 (1st input on 3rd page) = {result.hex()}')
  return result.hex()

# H(ga || m1 || shared_key)
def m2_zero_knowledge_proof(ga : int, M1 : str, shared_key : int):
  ga_bytes = ga.to_bytes((ga.bit_length() + 7) // 8, 'big')
  m1_bytes = bytes.fromhex(M1)
  shared_key_bytes = shared_key.to_bytes((shared_key.bit_length() + 7) // 8, 'big')
  inner_hash_value = ga_bytes + m1_bytes + shared_key_bytes
  result = hashlib.sha256(inner_hash_value).digest()
  print(f'M2 (2nd input on 3rd page) = {result.hex()}')
  return result.hex()

  
  

 
#given by passoff server
p = 233000556327543348946447470779219175150430130236907257523476085501968599658761371268535640963004707302492862642690597042148035540759198167263992070601617519279204228564031769469422146187139698860509698350226540759311033166697559129871348428777658832731699421786638279199926610332604408923157248859637890960407
g = 5
a = get_a(p)

a = 138006408390535488254855268661683523435522036686011775636858624119407805899752181935342055098221925779806206763069729961758339832477186496684822519811699574385963571407582789078947464744027900469733133349294383399175878689827839291415605375082050917727532695444456828584638784288497839281393744739420284105878
ga = 1883679938559350215892708263706670796568162113017940948229715882484187063013124295564733204371708301703737948394913919717853009210221001211721072727471489775105865533313489721802830324661774327276234243746495155039478102568107208157443191967338458360887114437213723603329258011783615090607732663886246359009
ga = get_public_key(g, a, p) #this is also given by passoff server

#first step of passoff server should now be complete, make sure you update the 2 values above
#second part given values: 
username = "criley16"
password = "encharge"
salt_hex = "f397f088"
pass_hash_itr = 1000
B = 149967260558012222409835358674618153808156359358647215625510783398213320863462299200201661761435354536756704475477456816194249333624457673425026075519065482034262191334535086487377645869532354790694376103456569089686523326918141896245921643573355612711223482214237701958152292194110416327794612206924044941756
#g^a, g and p on the passoff server are ga, g and p respctivly
x = hash_salted_password(password, salt_hex, pass_hash_itr)
K = get_k(p, g)
gb = SRP_public_key(B, K, g, x, p)
u = get_u(ga, gb)
shared_key = get_shared_key(gb, a, u, x, p)
#second step should now be complete
#third step requires username, salt_hex, ga, gb, shared_key, p, g
#all the same values as before
m1 = m1_zero_knowledge_proof(p, g, username, salt_hex, ga, gb, shared_key)
m2 = m2_zero_knowledge_proof(ga, m1, shared_key)

print("ABOVE IS SET VALUES THAT I HAD", end='')
arg_count = len(sys.argv)
if(arg_count == 2 and sys.argv[1] == '-i'):
  p = int(input("\nEnter pass off website value P >"))
  print('\n')
  ga = get_public_key(g, a, p) #this is also given by passoff server
  username = str(input("\nInput Username from pass off website >"))
  password = str(input("\nInput Password from pass off website >"))
  salt_hex = str(input("\nInput Salt (in hex) from pass off website >"))
  pass_hash_itr = 1000
  B = int(input("\nEnter B from pass off website >"))
  print('\n\nINPUTS FOR 2nd PAGE:\n')
  x = hash_salted_password(password, salt_hex, pass_hash_itr)
  print('\n')
  K = get_k(p, g)
  gb = SRP_public_key(B, K, g, x, p)
  u = get_u(ga, gb)
  shared_key = get_shared_key(gb, a, u, x, p)
  print('INPUTS FOR 3rd PAGE:\n')
  m1 = m1_zero_knowledge_proof(p, g, username, salt_hex, ga, gb, shared_key)
  print('')
  m2 = m2_zero_knowledge_proof(ga, m1, shared_key)
else:
  print(', USE THE ARGUMENT "-i" FOR AN INTERACTIVE VERSION')


#p = 233000556327543348946447470779219175150430130236907257523476085501968599658761371268535640963004707302492862642690597042148035540759198167263992070601617519279204228564031769469422146187139698860509698350226540759311033166697559129871348428777658832731699421786638279199926610332604408923157248859637890960407
#ga = 1883679938559350215892708263706670796568162113017940948229715882484187063013124295564733204371708301703737948394913919717853009210221001211721072727471489775105865533313489721802830324661774327276234243746495155039478102568107208157443191967338458360887114437213723603329258011783615090607732663886246359009