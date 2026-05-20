import hashlib
genesis_block = "311ccd6590c95e60050373ee1011e8329e23691940b6575f524b381cde0f70b4"
quote = str(input("Quote: "))
print()
nonce = 0
block_count = 0;
last_block = genesis_block
while block_count != 10:
  if nonce == 0:
    quote_bytes = quote.encode("ascii")
    last_block_bytes = bytes.fromhex(last_block)
  nonce_bytes = nonce.to_bytes((nonce.bit_length() + 7) // 8, 'big')
  hash_value = last_block_bytes + nonce_bytes + quote_bytes
  h = hashlib.sha256(hash_value)
  digest = h.digest()
  if digest[:3] == b'\x00\x00\x00':
    print(f"Block {block_count+1}: ")
    print("Nonce: ", nonce)
    print("Block Hash: ",digest.hex())
    last_block = digest.hex()
    nonce = -1
    print()
    if block_count != 9:
      quote = str(input("Next Quote: "))
    print()
    block_count += 1
  nonce += 1
