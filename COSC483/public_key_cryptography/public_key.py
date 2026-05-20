#Connor Riley 
#10/3/24
import random
import hashlib
from Crypto.Cipher import AES
from Crypto.Util.Padding import unpad
#need pip install pycrypto, pycryptodome, hashlib
#solves b^e (mod n)
def modular_exponentiation(b : int, e : int, n : int): #c = (b^e) (mod n)
  binary_e = list(bin(e)[2:])
  product = 1
  for i in range(len(binary_e)-1, -1, -1):
    if binary_e[i] == '1':
      product = (product * b) % n
    b = b * b % n
  return product

#wrapper to run miller rabin certain amount of times
def run_miller_rabin_k_times(p : int, k : int):
  if p % 2 == 0 or p < 2:
    return False #evens arent prime
  for i in range(k):
    #print("yes")
    if(miller_rabin(p) == False):
      #print(f'{p} is NOT prime')
      return False
  #print(f'{p} is prime with confidence {1-pow(.25, k)}')
  return True

#miller rabin test
def miller_rabin(p : int): #p = 2^s * d + 1, k is number of rounds
  q = p-1
  s = 0
  while q % 2 == 0:
    d = q // 2
    q = d
    s = s+1
  a = random.randint(1, p-1)
  #print(a)
  x = modular_exponentiation(a, d, p)
  if x == 1 or x == -1 % p:
    return True
  for i in range(s):
    x = pow(x, 2) % p
    if x == -1 % p:
      return True
  return False

#returns the gcd, d and  t of (s*a)+(t*b)=gcd(a,b)
def extended_euclidean_algoritm(a : int, b : int):
  r_old, r = a, b
  s_old, s = 1, 0
  t_old, t = 0, 1
  while r != 0:
    q = r_old // r
    r_old, r = r, r_old-r*q
    s_old, s = s, s_old-s*q
    t_old, t = t, t_old-t*q   
  return r_old, s_old, t_old #r, s, gcd

#generates prime q then checks if p=q*2+1 is also prime, returns p and q
def get_strong_prime_p(num_bits : int):
  q_set = set()
  while True:
    q = random.randrange(2**(num_bits-1), 2**(num_bits)-1, 1)#will get desired bit digit
    if q in q_set or q % 2 == 0:
      continue
    else:
      q_set.add(q)
                                                      
    if run_miller_rabin_k_times(q, 10) == True:
      p = (2*q) + 1
      if run_miller_rabin_k_times(p, 10) == True and p.bit_length() >= 1024:
        return p
        
#gets the shared key of from B^a mod p or (bob public) ^ (alice private) (mod(shared p))
def get_shared_key(a : int, B : int, p : int):
  return modular_exponentiation(B, a, p)

def decrypt_ciphertext(ciphertext, symmetric_key, IV):
  aes = AES.new(symmetric_key, AES.MODE_CBC, IV)
  decrypt_text = aes.decrypt(ciphertext)
  try:
    plain_text_message = unpad(decrypt_text, AES.block_size)
    plain_text = plain_text_message.decode('ASCII')
    print(f'Diffie Hellman Decoded Message: {plain_text}')
  except ValueError as e:
    print('padding error:', e)
  
#rsa part 3
def get_rsa_p_q(num_bits : int):
  while True:
    q = random.randrange(2**(num_bits-1), 2**(num_bits)-1, 1)
    if q % 2 == 0:
      continue
    if run_miller_rabin_k_times(q, 50) == True:
      break
  while True:
    p = random.randrange(2**(num_bits-1), 2**(num_bits)-1, 1)
    if p % 2 == 0:
      continue
    if run_miller_rabin_k_times(p, 50) == True:
      break
  #print(f'p:{p}\nq:{q}')
  return p, q

def get_rsa_n(p : int, q : int):
  return p*q

def get_gcd(a : int, b : int):
  while b != 0:
    a, b = b, a % b
  return a

def get_totient(p : int, q : int):
  return (p-1) * (q-1)
#This works
def get_rsa_p_q_n_totient_d(num_bits : int, e : int):
  while True:
    p, q = get_rsa_p_q(num_bits)
    n = get_rsa_n(p, q)
    t = get_totient(p,q)
    
    if get_gcd(t, e) == 1:
      gcd, d, x = extended_euclidean_algoritm(e, t)
      d = d % t
      return p,q,n,t,d
       
#4 using rsa parameters encrypt message c = m^e (mod n) this works
def encrpyt_rsa(m : str, e : int, n : int):
  int_message = int.from_bytes(m.encode('utf-8'), byteorder='big')
  return modular_exponentiation(int_message, e, n)
  
#5 using rsa parameters decrypt message m = c^d (mod n). when transforming from m to bytes make sure its big endian
def decrypt_rsa(c : int, d : int, n : int):
  int_message = modular_exponentiation(c,d,n)
  print('decoded RSA message: ',(int_message.to_bytes((int_message.bit_length()+7) // 8, byteorder='big')).decode('ASCII'))
  return (int_message.to_bytes((int_message.bit_length()+7) // 8, byteorder='big')).decode()




#shows modular exponentitation working
mod_exp_test = modular_exponentiation(4235880211405804673, 131, 12855544647099734480)
#print(f'{mod_exp_test} - test result\n1442355666387997457 - control value to compare to\n' )
x,s,t = extended_euclidean_algoritm(40, 17)
print(f'EXTENDED EUCLIDEAN ALGORITHM:\ns:{s}\nt:{t}\nx:{x}')


#Beginning of diffie hellman, the p, q, and a are all hardcoded currently but 
#the lines to generate them can be uncommented
#hard coding in as they are primes and it takes time

#p = 217791752603491183165503828914589883704183503010199892791694809567417031499595133428448112402409376691055380393278562779700215966226346259232377432477107683600248074228876519405905531576269855801738479409641519234553043686353971206969803194441285517717908280958487820648838162030537927961052712543820461865347
#q = 108895876301745591582751914457294941852091751505099946395847404783708515749797566714224056201204688345527690196639281389850107983113173129616188716238553841800124037114438259702952765788134927900869239704820759617276521843176985603484901597220642758858954140479243910324419081015268963980526356271910230932673
p = 233000556327543348946447470779219175150430130236907257523476085501968599658761371268535640963004707302492862642690597042148035540759198167263992070601617519279204228564031769469422146187139698860509698350226540759311033166697559129871348428777658832731699421786638279199926610332604408923157248859637890960407
#p = (get_strong_prime_p(1024))
#this was a random 'a' just using it to keep consistent outputs
#a = 73518129915492888034515076571075610260744591509999568634461865865078277847393630798258062082436898833943265492042531491422672614777547881706751998525417588523356168172593209492141330304250163482698273289739960858743596371820958282530164581799086408979556895775583921175669342349043508112584548874126092194627
a = 78795639292132296511708798032446299007837685385671241387432263796817773445630732086063432590798300635775663855293663710203879868446282892467432110860875982721329379794446451113429170880133182704009352412565362603042313855073734149581894947839799407491237768750644216399880258320241698528030710016076130368741
#a = random.randint(1,p)
#print(a)
#gets the public key using g=5 a and p
print('DIFFIE HELLMAN:\npublic key:', modular_exponentiation(5, a, p))
print('')
#this B value changes each time you access the site so will need to be changed if needed to check
B = 100141148161301720830741721537682738300227409918515499843110711256365129877034509486013524925777707107033775214538027240976323033664984867600564151853500509211232205536228363289276217220518549799508032041881640039431556708135177351871126477154734381106944497738482976107951901819308414684736371849837880520435
shared_key = (get_shared_key(a, B, p))
print(f'shared key: {shared_key}\n')
#comvert the shared key to bytes then run through sha-256. take the first 16 bytes for the symmetric key
shared_key_hex = shared_key.to_bytes((shared_key.bit_length()+7) // 8, byteorder='big')
sym_key = hashlib.sha256(shared_key_hex).hexdigest()[:32]
print(f'symmetric key 16 bytes: {sym_key}\n')
#these values change and can aquired from the pass off server
ciphertext = bytes.fromhex('ff686fc8f5b58499dc19a49eb1d32a6c4a33ae2f00156b6999d03b546cee58e5def1d9e65ef384618c7c37dc2722312a66c19b38fdc22e79f4dc9f47fc9fbf8ab0a4a252e4776c6b8c3d6df6bc17127576695caca2b1ec15acb8a7c1a745e6be674729e94bc68feb1bf90173237023f8cc9d00c9016502837f1a06184f5dd093c5b37f1581f0e9eb79924929a00d46b8e9abd120c17e1cd497d72c572e744744')
key = bytes.fromhex(sym_key)
iv = bytes.fromhex('72f4f5d6d3e81757a4dd27789fd8b9bb')
decrypt_ciphertext(ciphertext, key, iv)



#beginning of RSA portion, have p and q hard coded but can generate them via get_rsa_p_q method
print('\n\nRSA:')
rsa_public_exponent_e = 65537
rp, rq, rn, rt, rd = get_rsa_p_q_n_totient_d(1024, rsa_public_exponent_e)
print(f'\n\nRSA p value: {rp}\n\nRSA q value : {rq}\n\nRSA n value: {rn}\n\nRSA totient value: {rt}\n\nRSA d value: {rd}\n')
#first set of pass of server values
#edit the encrypt_rsa with word from pass off server and your n value
rsa_encrypt = encrpyt_rsa('osteostraci', rsa_public_exponent_e, 21104642681090413265095815372627642527362403318788571725580713740037909587044751317631126928816300185692728811313403792606625254682410559687350912404666187748166339015813533967460466019016362060922499348253996952579899486214475010475342992022175111817745509524573671121544720027209998688802706392095276730861110252880803427686442919868412600127466811261845176826218400957944317940770084392587243631470090895530831802015465208066322624921243100809291482808714287557811261843675911847336678850721612472782465075405879127543190005698741099815441864697578337241393658955580711480431648121699390623370572928969720539035841)
print(f'rsa_encrypt: {rsa_encrypt}\n\n') #this will go to pass off server
#decrypt_rsa(c, d, n) get c from pass of server d and n were calculated before
decrypt_rsa(20913775564811085260876899693615902998758065821737608448920496732511293035106143351050796540431762428289446309925415219035759631568460910472174742855640865212933600075482097106108861608885219616005384711487421936880351936873578181672250671765826496212362046073952702351413594671856198963282335602713669294621639943002200588149739058543279831443446976957452469649531184698276490791088077366782108149192159052772675817331808895637459040316932032003726274872527739942224407219945527316483122711720381538410277337275628680383308420106475997435376418414882874903115883260666158791279112833248489244582991355879433425348587, 188063404271736596835618905009605920868816752951348488452921812475123048031404165120414088628236252932611404638708329872930850492615282464217357108874758588963930939549187540427497629661192234059824825966710930013681756869390625236394713022276733224004201864021102948486841272806058245483631849687712919584703572231559141165637364552901094645400523105794288510128103565226219046635709366094089427558002450962558545778052518513010858631850523521541411197102962100957516759524337441366399328357367559211773291705800696603959288941679701044443510848481275335135180778814150403178443854513025013093340016248453879389473, 21104642681090413265095815372627642527362403318788571725580713740037909587044751317631126928816300185692728811313403792606625254682410559687350912404666187748166339015813533967460466019016362060922499348253996952579899486214475010475342992022175111817745509524573671121544720027209998688802706392095276730861110252880803427686442919868412600127466811261845176826218400957944317940770084392587243631470090895530831802015465208066322624921243100809291482808714287557811261843675911847336678850721612472782465075405879127543190005698741099815441864697578337241393658955580711480431648121699390623370572928969720539035841)