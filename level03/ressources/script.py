s = 'Q}|u`sfg~sf{}|a3'
o = 'Congratulations!'

xor_values = []

for i in range(len(s)):
    xor_value = ord(s[i]) ^ ord(o[i])
    xor_values.append(xor_value)

print(xor_values)

if all(x == xor_values[0] for x in xor_values):
    password = 322424845 - xor_values[0]
    print(f"The key used for XOR encryption is: {xor_values[0]}")
    print(f"The password is : {password}")